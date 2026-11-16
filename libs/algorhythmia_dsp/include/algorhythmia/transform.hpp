#pragma once

#include "algorhythmia/delay_buffer.hpp"
#include "algorhythmia/dsp_math.hpp"
#include "algorhythmia/scene.hpp"

#include <array>
#include <cstdint>

namespace algorhythmia {
class Transform {
public:
    void init(float sampleRate) noexcept;
    [[nodiscard]] StereoFrame process(StereoFrame input, SceneControls controls) noexcept;
    [[nodiscard]] std::uint8_t telemetry() const noexcept {
        return meter_;
    }
    [[nodiscard]] float position() const noexcept {
        return position_;
    }
    [[nodiscard]] std::size_t writeIndex() const noexcept {
        return tapes_[0].writeIndex();
    }
    [[nodiscard]] static float speedForFlux(float flux) noexcept;

private:
    std::array<DelayBuffer<float, config::delayFrames>, 2> tapes_{};
    std::array<DcBlocker, 2> recordDc_{};
    std::array<DcBlocker, 2> playbackDc_{};
    std::array<float, 2> wear_{};
    std::array<float, 2> peaks_{};
    float inverseRate_ = 1.0F / config::sampleRate;
    float peakRelease_ = 0.0F;
    float position_ = 0.0F;
    std::uint8_t meter_ = 0;
};
static_assert(Scene<Transform>);
} // namespace algorhythmia
