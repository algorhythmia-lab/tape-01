#pragma once

#include "algorhythmia/dsp_math.hpp"
#include "algorhythmia/scene.hpp"
#include "algorhythmia/voice.hpp"

#include <array>
#include <cstdint>

namespace algorhythmia {
struct CellularState {
    std::uint32_t cells = 1U;
    std::uint8_t rule = 30U;
};

class Cellular {
public:
    static constexpr std::size_t voiceCount = 8;
    void init(float sampleRate) noexcept;
    [[nodiscard]] StereoFrame process(StereoFrame input, SceneControls controls) noexcept;
    [[nodiscard]] std::uint8_t telemetry() const noexcept {
        return state_.rule;
    }
    [[nodiscard]] std::uint32_t cells() const noexcept {
        return state_.cells;
    }
    [[nodiscard]] std::uint32_t generation() const noexcept {
        return generation_;
    }
    [[nodiscard]] static std::uint32_t evolve(std::uint32_t cells, std::uint8_t rule) noexcept;
    [[nodiscard]] static std::uint8_t ruleForBias(float bias) noexcept;

private:
    void excite(std::uint32_t flips, float bias, float state) noexcept;
    CellularState state_{};
    std::array<VoiceState, voiceCount> voices_{};
    std::array<DcBlocker, 2> dc_{};
    float inverseRate_ = 1.0F / 48000.0F;
    float generationPhase_ = 0.0F;
    float decay_ = 0.0F;
    std::uint32_t generation_ = 0;
    bool pendingSeed_ = true;
};
static_assert(Scene<Cellular>);
} // namespace algorhythmia
