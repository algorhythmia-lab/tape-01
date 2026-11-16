#include "algorhythmia/engine.hpp"

#include <algorithm>
#include <cmath>

namespace algorhythmia {
namespace {
constexpr std::array<float, 4> defaults{0.5F, 0.5F, 0.0F, 0.0F};
constexpr std::size_t index(ParameterId id) noexcept {
    return static_cast<std::size_t>(id);
}
constexpr std::uint8_t previewWord(SceneId scene) noexcept {
    constexpr std::array<std::uint8_t, 3> words{0xc0, 0x18, 0x03};
    return words[static_cast<std::size_t>(scene)];
}
} // namespace

void Engine::init(float sampleRate) noexcept {
    sampleRate_ = sanitizeRate(sampleRate);
    tapFrames_ = static_cast<std::uint32_t>(std::lround(0.3F * sampleRate_));
    commitFrames_ = static_cast<std::uint32_t>(std::lround(2.0F * sampleRate_));
    const auto ramp =
        static_cast<std::uint32_t>(std::lround(sampleRate_ * config::smoothingSeconds));
    for (std::size_t i = 0; i < parameterCount; ++i) {
        parameters_[i].store(std::bit_cast<std::uint32_t>(defaults[i]), std::memory_order_relaxed);
        smoothed_[i].reset(defaults[i], ramp);
    }
    targets_ = defaults;
    for (std::size_t i = 0; i < gains_.size(); ++i) {
        gains_[i].reset(i == 0 ? 1.0F : 0.0F, ramp);
    }
    std::apply([&](auto&... scenes) { (scenes.init(sampleRate_), ...); }, scenes_);
    scene_.store(0, std::memory_order_relaxed);
    for (auto& button : buttons_) {
        button.store(0, std::memory_order_relaxed);
    }
    brake_.store(0, std::memory_order_relaxed);
    telemetry_.store(0, std::memory_order_relaxed);
    commit_.store(0, std::memory_order_relaxed);
    commitSequence_ = 0;
    consumedCommit_ = 0;
    heldSamples_ = {};
    pressed_ = {};
    heldBias_ = gestureBias_ = 0.5F;
    activeScene_ = previewScene_ = SceneId::Bytebeat;
    selecting_ = committed_ = false;
    initialized_ = true;
}

bool Engine::restore(const CommitSnapshot& state, float rate) noexcept {
    if (!std::isfinite(state.flux) || !std::isfinite(state.bias) || state.flux < 0 ||
        state.flux > 1 || state.bias < 0 || state.bias > 1 ||
        static_cast<unsigned>(state.domain) >= 3)
        return false;
    init(rate);
    targets_ = {state.flux, state.bias, state.running ? 1.0F : 0.0F, state.state ? 1.0F : 0.0F};
    const auto ramp =
        static_cast<std::uint32_t>(std::lround(sampleRate_ * config::smoothingSeconds));
    for (std::size_t i = 0; i < parameterCount; ++i) {
        parameters_[i].store(std::bit_cast<std::uint32_t>(targets_[i]), std::memory_order_relaxed);
        smoothed_[i].reset(targets_[i], ramp);
    }
    activeScene_ = previewScene_ = state.domain;
    scene_.store(static_cast<std::uint32_t>(state.domain), std::memory_order_relaxed);
    for (std::size_t i = 0; i < gains_.size(); ++i)
        gains_[i].reset(i == static_cast<std::size_t>(state.domain) ? 1.0F : 0.0F, ramp);
    return true;
}

void Engine::setParameter(ParameterId id, float value) noexcept {
    if (index(id) >= parameterCount || !std::isfinite(value)) {
        return;
    }
    value = snap(std::clamp(value, 0.0F, 1.0F));
    if (id == ParameterId::Drive || id == ParameterId::State) {
        value = value >= 0.5F ? 1.0F : 0.0F;
    }
    parameters_[index(id)].store(std::bit_cast<std::uint32_t>(value), std::memory_order_relaxed);
}
void Engine::setScene(SceneId id) noexcept {
    if (static_cast<std::uint32_t>(id) < 3U) {
        scene_.store(static_cast<std::uint32_t>(id), std::memory_order_relaxed);
    }
}
void Engine::setButton(ParameterId id, bool pressed) noexcept {
    if (id == ParameterId::Drive || id == ParameterId::State) {
        buttons_[id == ParameterId::Drive ? 0 : 1].store(pressed ? 1U : 0U,
                                                         std::memory_order_relaxed);
    }
}
void Engine::setBrake(bool engaged) noexcept {
    brake_.store(engaged ? 1U : 0U, std::memory_order_relaxed);
}
void Engine::setMidiCc(std::uint8_t cc, std::uint8_t value) noexcept {
    if (value > 127) {
        return;
    }
    if (cc == fluxMidiCc || cc == biasMidiCc) {
        setParameter(cc == fluxMidiCc ? ParameterId::Flux : ParameterId::Bias,
                     static_cast<float>(value) / 127.0F);
    } else if (cc == driveMidiCc) {
        setParameter(ParameterId::Drive, value >= 64 ? 1.0F : 0.0F);
    } else if (cc == stateMidiCc) {
        setScene(static_cast<SceneId>(std::min(static_cast<unsigned>(value) * 3U / 128U, 2U)));
    }
}

void Engine::publishCommit() noexcept {
    // single atomic word publishes a coherent snapshot without retry loops.
    const auto flux = static_cast<std::uint32_t>(std::lround(targets_[0] * 1023.0F));
    const auto bias =
        static_cast<std::uint32_t>(std::lround((selecting_ ? heldBias_ : targets_[1]) * 1023.0F));
    ++commitSequence_;
    const auto packed =
        flux | (bias << 10U) |
        (static_cast<std::uint32_t>(selecting_ ? previewScene_ : activeScene_) << 20U) |
        (targets_[3] >= 0.5F ? 1U << 22U : 0U) | (targets_[2] >= 0.5F ? 1U << 23U : 0U) |
        (static_cast<std::uint32_t>(commitSequence_) << 24U);
    commit_.store(packed, std::memory_order_relaxed);
}

void Engine::gestures(bool drive, bool state) noexcept {
    const std::array down{drive, state};
    for (std::size_t i = 0; i < down.size(); ++i) {
        if (down[i] && !pressed_[i]) {
            heldSamples_[i] = 0;
            if (i == 1) {
                heldBias_ = smoothed_[1].current();
                gestureBias_ = targets_[1];
                selecting_ = committed_ = false;
                previewScene_ = activeScene_;
            }
        }
        if (!down[i] && pressed_[i]) {
            if (i == 1 && selecting_) {
                activeScene_ = previewScene_;
                setScene(activeScene_);
            } else if (heldSamples_[i] < tapFrames_) {
                const std::size_t parameter = i + 2;
                targets_[parameter] = 1.0F - targets_[parameter];
                setParameter(i == 0 ? ParameterId::Drive : ParameterId::State, targets_[parameter]);
            }
            heldSamples_[i] = 0;
            if (i == 1) {
                selecting_ = false;
            }
        }
        if (down[i]) {
            heldSamples_[i] = std::min(heldSamples_[i] + 1U, commitFrames_);
        }
        pressed_[i] = down[i];
    }
    if (state && heldSamples_[1] >= tapFrames_ &&
        (selecting_ || std::abs(targets_[1] - gestureBias_) >= 0.025F)) {
        selecting_ = true;
        previewScene_ =
            static_cast<SceneId>(std::min(static_cast<unsigned>(targets_[1] * 3.0F), 2U));
    }
    if (state && heldSamples_[1] >= commitFrames_ && !committed_) {
        publishCommit();
        committed_ = true;
    }
}

void Engine::process(const float* inputL, const float* inputR, float* outputL, float* outputR,
                     std::size_t frames) noexcept {
    if (frames == 0 || outputL == nullptr || outputR == nullptr) {
        return;
    }
    if (!initialized_) {
        std::fill_n(outputL, frames, 0.0F);
        std::fill_n(outputR, frames, 0.0F);
        return;
    }
    for (std::size_t i = 0; i < parameterCount; ++i) {
        targets_[i] = std::bit_cast<float>(parameters_[i].load(std::memory_order_relaxed));
    }
    activeScene_ = static_cast<SceneId>(scene_.load(std::memory_order_relaxed));
    const bool driveDown = buttons_[0].load(std::memory_order_relaxed) != 0;
    const bool stateDown = buttons_[1].load(std::memory_order_relaxed) != 0;
    const bool brake = brake_.load(std::memory_order_relaxed) != 0;
    for (std::size_t frame = 0; frame < frames; ++frame) {
        if (frame == 0 || driveDown || stateDown) {
            gestures(driveDown, stateDown);
        }
        if (frame == 0) {
            for (std::size_t i = 0; i < parameterCount; ++i) {
                smoothed_[i].setTarget(i == 1 && stateDown ? heldBias_ : targets_[i]);
            }
            for (std::size_t i = 0; i < gains_.size(); ++i) {
                gains_[i].setTarget(i == static_cast<std::size_t>(activeScene_) ? 1.0F : 0.0F);
            }
        }
        SceneControls controls{};
        controls.flux = smoothed_[0].next();
        controls.bias = smoothed_[1].next();
        const float gain = smoothed_[2].next();
        controls.state = smoothed_[3].next();
        // pause the computational tape after the gain reaches its endpoint.
        controls.running = gain > 0.0F;
        controls.brake = brake || (driveDown && heldSamples_[0] >= tapFrames_);
        const StereoFrame input{inputL == nullptr ? 0.0F : sanitizeAudio(inputL[frame]),
                                inputR == nullptr ? 0.0F : sanitizeAudio(inputR[frame])};
        const auto output = render(input, controls, std::make_index_sequence<3>{});
        outputL[frame] = snap(std::clamp(output.left * gain, -1.0F, 1.0F));
        outputR[frame] = snap(std::clamp(output.right * gain, -1.0F, 1.0F));
    }
    std::uint8_t word = 0;
    std::size_t sceneIndex = 0;
    std::apply(
        [&](const auto&... scenes) {
            ((sceneIndex++ == static_cast<std::size_t>(activeScene_)
                  ? static_cast<void>(word = scenes.telemetry())
                  : static_cast<void>(0)),
             ...);
        },
        scenes_);
    const auto displayScene = stateDown && heldSamples_[1] >= tapFrames_
                                  ? (selecting_ ? previewScene_ : activeScene_)
                                  : activeScene_;
    const bool preview = stateDown && heldSamples_[1] >= tapFrames_;
    if (preview) {
        word = previewWord(displayScene);
    }
    telemetry_.store(static_cast<std::uint32_t>(word) |
                         (static_cast<std::uint32_t>(displayScene) << 8U) |
                         (preview ? 1U << 10U : 0U) | (targets_[2] >= 0.5F ? 1U << 11U : 0U) |
                         ((brake || (driveDown && heldSamples_[0] >= tapFrames_)) ? 1U << 12U : 0U),
                     std::memory_order_relaxed);
}

Telemetry Engine::telemetry() const noexcept {
    const auto packed = telemetry_.load(std::memory_order_relaxed);
    return {static_cast<SceneId>((packed >> 8U) & 3U), static_cast<std::uint8_t>(packed & 0xffU),
            (packed & (1U << 10U)) != 0, (packed & (1U << 11U)) != 0, (packed & (1U << 12U)) != 0};
}

bool Engine::takeCommitRequest(CommitSnapshot& snapshot) noexcept {
    const auto packed = commit_.load(std::memory_order_relaxed);
    if (packed == consumedCommit_) {
        return false;
    }
    consumedCommit_ = packed;
    snapshot = {static_cast<float>(packed & 1023U) / 1023.0F,
                static_cast<float>((packed >> 10U) & 1023U) / 1023.0F,
                static_cast<SceneId>((packed >> 20U) & 3U), (packed & (1U << 22U)) != 0,
                (packed & (1U << 23U)) != 0};
    return true;
}
} // namespace algorhythmia
