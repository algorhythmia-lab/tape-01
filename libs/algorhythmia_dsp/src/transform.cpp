#include "algorhythmia/transform.hpp"

#include <algorithm>
#include <cmath>

namespace algorhythmia {
void Transform::init(float rate) noexcept {
    rate = sanitizeRate(rate);
    inverseRate_ = 1.0F / rate;
    peakRelease_ = std::exp(-inverseRate_ / 0.15F);
    position_ = static_cast<float>(config::delayFrames - 2048);
    wear_ = peaks_ = {};
    meter_ = 0;
    for (std::size_t i = 0; i < tapes_.size(); ++i) {
        tapes_[i].clear();
        recordDc_[i].init(rate);
        playbackDc_[i].init(rate);
    }
}

float Transform::speedForFlux(float flux) noexcept {
    // center detent is a true zero-hz head; endpoints are +/-2x capstan speed.
    return std::abs(flux - 0.5F) <= 0.005F ? 0.0F : 4.0F * flux - 2.0F;
}

StereoFrame Transform::process(StereoFrame input, SceneControls controls) noexcept {
    const float speed = speedForFlux(controls.flux) * (1.0F - 2.0F * controls.state);
    const std::array incoming{sanitizeAudio(input.left), sanitizeAudio(input.right)};
    std::array<float, 2> output{};
    const float pole = 6.283185307F * (10000.0F - 7500.0F * controls.bias) * inverseRate_;
    const float coefficient = pole / (1.0F + pole);
    const float feedback = 1.2F * controls.bias;
    meter_ = 0;
    for (std::size_t channel = 0; channel < tapes_.size(); ++channel) {
        const float read = tapes_[channel].readCubic(position_);
        wear_[channel] = snap(wear_[channel] + coefficient * (read - wear_[channel]));
        // limiting before storage makes >100% feedback bounded even after cubic
        // overshoot; dc rejection prevents a stationary head becoming dc runaway.
        const float playback = playbackDc_[channel].process(wear_[channel]);
        output[channel] = snap(0.9F * std::tanh(playback * (1.0F + 1.5F * controls.bias)));
        if (controls.running && !controls.brake && speed != 0.0F) {
            const float record =
                recordDc_[channel].process(incoming[channel] + feedback * playback);
            tapes_[channel].push(snap(std::tanh(record)));
        }
        peaks_[channel] = snap(std::max(std::abs(output[channel]), peaks_[channel] * peakRelease_));
        const auto level = std::min(static_cast<unsigned>(std::ceil(peaks_[channel] * 4.0F)), 4U);
        // bit7 is the leftmost led; left grows out from center, right mirrors it.
        const auto ladder = ((1U << level) - 1U);
        const auto left = ladder << 4U;
        unsigned right = 0;
        for (unsigned i = 0; i < level; ++i) {
            right |= 1U << (3U - i);
        }
        meter_ |= static_cast<std::uint8_t>(channel == 0 ? left : right);
    }
    if (controls.running) {
        position_ += speed;
        constexpr float size = static_cast<float>(config::delayFrames);
        // speed is bounded +/-2: no unbounded modulo or catch-up loop.
        if (position_ >= size) {
            position_ -= size;
        }
        if (position_ < 0.0F) {
            position_ += size;
        }
    }
    return {output[0], output[1]};
}
} // namespace algorhythmia
