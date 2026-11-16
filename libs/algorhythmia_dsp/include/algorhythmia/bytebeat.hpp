#pragma once

#include "algorhythmia/dsp_math.hpp"
#include "algorhythmia/scene.hpp"

#include <array>
#include <cstdint>

namespace algorhythmia {
// unsigned modulo arithmetic makes every overflow and shift well-defined.
class Bytebeat {
public:
    void init(float sampleRate) noexcept;
    [[nodiscard]] StereoFrame process(StereoFrame input, SceneControls controls) noexcept;
    [[nodiscard]] std::uint8_t telemetry() const noexcept {
        return word_;
    }
    [[nodiscard]] std::uint32_t tick() const noexcept {
        return tick_;
    }
    [[nodiscard]] float velocity() const noexcept {
        return velocity_;
    }
    [[nodiscard]] static std::uint32_t expression(std::uint32_t tick, unsigned equation) noexcept;

private:
    [[nodiscard]] static float bank(std::uint32_t tick, float form, bool alternate) noexcept;
    std::uint32_t tick_ = 0;
    float phase_ = 0.0F;
    float inverseSampleRate_ = 1.0F / 48'000.0F;
    float velocity_ = 1.0F;
    float inertia_ = 0.0F;
    std::uint8_t word_ = 0;
    std::array<TapeHead, 2> heads_{};
};
static_assert(Scene<Bytebeat>);
} // namespace algorhythmia
