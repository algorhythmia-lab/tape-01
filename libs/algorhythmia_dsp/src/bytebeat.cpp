#include "algorhythmia/bytebeat.hpp"

#include <algorithm>
#include <cmath>

namespace algorhythmia {
namespace {
float decode(std::uint32_t value) noexcept {
    return (static_cast<float>(value & 0xffU) - 127.5F) * (1.0F / 128.0F);
}
float blend(float a, float b, float fraction) noexcept {
    return a + (b - a) * fraction;
}
} // namespace

void Bytebeat::init(float sampleRate) noexcept {
    sampleRate = sanitizeRate(sampleRate);
    tick_ = 0;
    phase_ = 0.0F;
    word_ = 0;
    velocity_ = 1.0F;
    inverseSampleRate_ = 1.0F / sampleRate;
    inertia_ = 1.0F - std::exp(-inverseSampleRate_ / 0.16F);
    for (auto& head : heads_) {
        head.init(sampleRate);
    }
}

std::uint32_t Bytebeat::expression(std::uint32_t tick, unsigned equation) noexcept {
    switch (equation % 6U) {
    case 0:
        return tick * ((tick >> 5U) | (tick >> 8U));
    case 1:
        return (tick * 3U) ^ (tick >> 10U) ^ (tick >> 5U);
    case 2:
        return (tick * ((tick >> 9U) | 1U)) ^ (tick >> 3U);
    case 3:
        return (tick * 5U) ^ (tick >> 7U);
    case 4:
        return (tick * 9U) & (tick >> 4U);
    default:
        return (tick ^ (tick >> 8U)) * ((tick >> 11U) | 3U);
    }
}

float Bytebeat::bank(std::uint32_t tick, float bias, bool alternate) noexcept {
    const unsigned offset = alternate ? 3U : 0U;
    const float position = bias * 2.0F;
    return position < 1.0F ? blend(decode(expression(tick, offset)),
                                   decode(expression(tick, offset + 1U)), position)
                           : blend(decode(expression(tick, offset + 1U)),
                                   decode(expression(tick, offset + 2U)), position - 1.0F);
}

StereoFrame Bytebeat::process(StereoFrame, SceneControls controls) noexcept {
    const float targetVelocity = controls.brake ? 0.0001F : 1.0F;
    velocity_ += inertia_ * (targetVelocity - velocity_);
    // prevent finite-precision inertia from settling short of its target.
    if (std::abs(velocity_ - targetVelocity) < 1.0e-5F) {
        velocity_ = targetVelocity;
    }
    if (controls.running) {
        phase_ += (500.0F + 7500.0F * controls.flux) * velocity_ * inverseSampleRate_;
        if (phase_ >= 1.0F) {
            phase_ -= 1.0F;
            ++tick_;
        }
    }
    // interpolate fractional azimuth rather than quantizing the width dial.
    const float azimuth = 64.0F * controls.bias;
    const auto offset = static_cast<std::uint32_t>(azimuth);
    const float fraction = azimuth - static_cast<float>(offset);
    const auto evaluate = [&](std::uint32_t tick) noexcept {
        return blend(bank(tick, controls.bias, false), bank(tick, controls.bias, true),
                     controls.state);
    };
    const float left = evaluate(tick_);
    const float right = blend(evaluate(tick_ + offset), evaluate(tick_ + offset + 1U), fraction);
    const auto equation = static_cast<unsigned>(std::lround(controls.bias * 2.0F)) +
                          (controls.state >= 0.5F ? 3U : 0U);
    word_ = static_cast<std::uint8_t>(expression(tick_, equation) & 0xffU);
    return {heads_[0].process(left, controls.bias), heads_[1].process(right, controls.bias)};
}
} // namespace algorhythmia
