#include "algorhythmia/cellular.hpp"

#include <algorithm>
#include <bit>
#include <cmath>

namespace algorhythmia {
namespace {
constexpr float twoPi = 6.283185307F;
// bounded odd polynomial sine approximation; avoids per-voice library calls.
float sine(float phase) noexcept {
    const float x = (phase < 0.5F ? phase : phase - 1.0F) * twoPi;
    const float y = 1.273239545F * x - 0.405284735F * x * std::abs(x);
    return 0.225F * (y * std::abs(y) - y) + y;
}
float wrap(float phase) noexcept {
    return phase - std::floor(phase);
}
} // namespace

void Cellular::init(float rate) noexcept {
    rate = sanitizeRate(rate);
    state_ = {};
    voices_ = {};
    generation_ = 0;
    generationPhase_ = 0.0F;
    pendingSeed_ = true;
    inverseRate_ = 1.0F / rate;
    decay_ = std::exp(-inverseRate_ / 0.12F);
    for (auto& dc : dc_) {
        dc.init(rate);
    }
}

std::uint8_t Cellular::ruleForBias(float bias) noexcept {
    constexpr std::array<std::uint8_t, 4> rules{30, 90, 110, 150};
    return rules[std::min(static_cast<unsigned>(bias * 4.0F), 3U)];
}

std::uint32_t Cellular::evolve(std::uint32_t cells, std::uint8_t rule) noexcept {
    // bit-parallel elementary automaton with periodic boundary conditions.
    const auto left = std::rotl(cells, 1);
    const auto right = std::rotr(cells, 1);
    std::uint32_t next = 0;
    for (unsigned neighborhood = 0; neighborhood < 8; ++neighborhood) {
        if ((rule & (1U << neighborhood)) != 0) {
            next |= ((neighborhood & 4U) != 0 ? left : ~left) &
                    ((neighborhood & 2U) != 0 ? cells : ~cells) &
                    ((neighborhood & 1U) != 0 ? right : ~right);
        }
    }
    return next;
}

void Cellular::excite(std::uint32_t flips, float bias, float state) noexcept {
    constexpr std::array<float, 8> notes{130.8128F, 146.8324F, 164.8138F, 195.9977F,
                                         220.0F,    261.6256F, 293.6648F, 329.6276F};
    // four cells share one voice; aggregate simultaneous walls without stealing
    // unbounded voices. both rising and falling flips excite the bank.
    for (std::size_t i = 0; i < voiceCount; ++i) {
        const auto events = static_cast<unsigned>((flips >> (i * 4U)) & 15U);
        if (events == 0) {
            continue;
        }
        auto& voice = voices_[i];
        voice.envelope =
            std::min(1.0F, voice.envelope + 0.25F * static_cast<float>(std::popcount(events)));
        const float frequency = notes[i] * (1.0F + state);
        voice.carrierIncrement = frequency * inverseRate_;
        // constrain the modulator and deviation for the lowest render rate.
        voice.modulatorIncrement = std::min(0.12F, voice.carrierIncrement * (1.0F + 2.0F * bias));
        const float pan = static_cast<float>(i) / static_cast<float>(voiceCount - 1);
        voice.panLeft = std::sqrt(1.0F - pan);
        voice.panRight = std::sqrt(pan);
    }
}

StereoFrame Cellular::process(StereoFrame, SceneControls controls) noexcept {
    state_.rule = ruleForBias(controls.bias);
    if (controls.running && !controls.brake) {
        if (pendingSeed_) {
            excite(state_.cells, controls.bias, controls.state);
            pendingSeed_ = false;
        }
        // four generations per beat, 30..300 bpm; bounded below one per frame.
        generationPhase_ += (2.0F + 18.0F * controls.flux) * inverseRate_;
        if (generationPhase_ >= 1.0F) {
            generationPhase_ -= 1.0F;
            const auto next = evolve(state_.cells, state_.rule);
            excite(state_.cells ^ next, controls.bias, controls.state);
            state_.cells = next;
            ++generation_;
            // an extinct periodic lattice gets a deterministic new wall seed.
            if (state_.cells == 0) {
                state_.cells = std::rotl(1U, static_cast<int>(generation_ & 31U));
            }
        }
    }
    StereoFrame output{};
    for (auto& voice : voices_) {
        if (voice.envelope == 0.0F) {
            continue;
        }
        voice.carrierPhase = wrap(voice.carrierPhase + voice.carrierIncrement);
        voice.modulatorPhase = wrap(voice.modulatorPhase + voice.modulatorIncrement);
        const float modulation =
            (0.025F + 0.08F * controls.bias) * voice.envelope * sine(voice.modulatorPhase);
        const float sample = sine(wrap(voice.carrierPhase + modulation)) * voice.envelope;
        output.left += sample * voice.panLeft;
        output.right += sample * voice.panRight;
        voice.envelope = snap(voice.envelope * decay_);
    }
    return {snap(std::tanh(dc_[0].process(output.left * 0.18F))),
            snap(std::tanh(dc_[1].process(output.right * 0.18F)))};
}
} // namespace algorhythmia
