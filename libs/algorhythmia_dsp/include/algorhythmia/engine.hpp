#pragma once

#include "algorhythmia/bytebeat.hpp"
#include "algorhythmia/cellular.hpp"
#include "algorhythmia/parameters.hpp"
#include "algorhythmia/smoother.hpp"
#include "algorhythmia/transform.hpp"

#include <array>
#include <atomic>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <tuple>

namespace algorhythmia {
static_assert(std::atomic<std::uint32_t>::is_always_lock_free,
              "parameters require native lock-free 32-bit atomics");

struct Telemetry {
    SceneId domain = SceneId::Bytebeat;
    std::uint8_t word = 0;
    bool preview = false;
    bool running = false;
    bool brake = false;
    [[nodiscard]] constexpr std::uint32_t color() const noexcept {
        constexpr std::array<std::uint32_t, 3> colors{0xffb000, 0x00ff66, 0x54e5ff};
        return colors[static_cast<std::size_t>(domain)];
    }
};

struct CommitSnapshot {
    float flux = 0.5F;
    float bias = 0.5F;
    SceneId domain = SceneId::Bytebeat;
    bool state = false;
    bool running = false;
};

class Engine {
public:
    Engine() noexcept = default;
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    // exclusive lifecycle operation; clears tape and restores defaults.
    void init(float sampleRate) noexcept;
    // Audio-owner reset transaction: empty history and exact initial controls.
    [[nodiscard]] bool restore(const CommitSnapshot&, float sampleRate) noexcept;
    // one audio owner. optional inputs; disjoint outputs. corresponding-channel
    // in-place supported. zero frames touches nothing. controls sampled per call.
    void process(const float* inputL, const float* inputR, float* outputL, float* outputR,
                 std::size_t numFrames) noexcept;
    // concurrent independent latest-value mailboxes. flux/bias are continuous;
    // drive is latched run/pause, state is the binary per-domain sub-register.
    void setParameter(ParameterId id, float normalizedValue) noexcept;
    void setScene(SceneId id) noexcept;
    void setMidiCc(std::uint8_t cc, std::uint8_t value) noexcept;
    // raw physical buttons are separate from the latched host parameters.
    // deliver edges at process boundaries; split host blocks for sample offsets.
    void setButton(ParameterId id, bool pressed) noexcept;
    void setBrake(bool engaged) noexcept; // direct desktop equivalent, no tap side effect
    [[nodiscard]] Telemetry telemetry() const noexcept;
    // one non-audio consumer. latest snapshot, quantized to 10-bit dials. flash
    // writing and platform-specific ram residency belong to that worker.
    [[nodiscard]] bool takeCommitRequest(CommitSnapshot& snapshot) noexcept;

private:
    static constexpr std::size_t parameterCount = 4;
    using Scenes = std::tuple<Bytebeat, Cellular, Transform>;
    std::array<std::atomic<std::uint32_t>, parameterCount> parameters_{
        std::bit_cast<std::uint32_t>(0.5F), std::bit_cast<std::uint32_t>(0.5F),
        std::bit_cast<std::uint32_t>(0.0F), std::bit_cast<std::uint32_t>(0.0F)};
    std::atomic<std::uint32_t> scene_{0};
    std::array<std::atomic<std::uint32_t>, 2> buttons_{0, 0};
    std::atomic<std::uint32_t> brake_{0};
    std::atomic<std::uint32_t> telemetry_{0};
    std::atomic<std::uint32_t> commit_{0};
    std::array<LinearSmoother, parameterCount> smoothed_{};
    std::array<LinearSmoother, 3> gains_{};
    Scenes scenes_{};
    float sampleRate_ = config::sampleRate;
    std::uint32_t tapFrames_ = 14400;
    std::uint32_t commitFrames_ = 96000;
    float heldBias_ = 0.5F;
    float gestureBias_ = 0.5F;
    std::array<float, 4> targets_{};
    std::array<std::uint32_t, 2> heldSamples_{};
    std::array<bool, 2> pressed_{};
    SceneId activeScene_ = SceneId::Bytebeat;
    SceneId previewScene_ = SceneId::Bytebeat;
    bool selecting_ = false;
    bool committed_ = false;
    bool initialized_ = false;
    std::uint8_t commitSequence_ = 0;
    std::uint32_t consumedCommit_ = 0; // exclusively owned by the commit consumer.

    void gestures(bool drive, bool state) noexcept;
    void publishCommit() noexcept;
    template <std::size_t... I>
    StereoFrame render(StereoFrame input, SceneControls controls,
                       std::index_sequence<I...>) noexcept {
        StereoFrame sum{};
        const auto mix = [&]<std::size_t N>() noexcept {
            const float gain = gains_[N].next();
            if (gain == 0.0F) {
                return;
            }
            const auto sample = std::get<N>(scenes_).process(input, controls);
            sum.left += gain * sample.left;
            sum.right += gain * sample.right;
        };
        (mix.template operator()<I>(), ...);
        return sum;
    }
};
} // namespace algorhythmia
