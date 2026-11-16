#pragma once

#include "algorhythmia/config.hpp"

#include <algorithm>
#include <cmath>

namespace algorhythmia {
inline float snap(float value) noexcept {
    return std::abs(value) < 1.0e-20F ? 0.0F : value;
}

inline float sanitizeRate(float rate) noexcept {
    return std::isfinite(rate) && rate >= config::minimumSampleRate &&
                   rate <= config::maximumSampleRate
               ? rate
               : config::sampleRate;
}

inline float sanitizeAudio(float value) noexcept {
    return std::isfinite(value) ? snap(std::clamp(value, -8.0F, 8.0F)) : 0.0F;
}

// sample-rate invariant dc rejection with recursive state snapping.
class DcBlocker {
public:
    void init(float rate) noexcept {
        coefficient_ = std::exp(-6.283185307F * 20.0F / rate);
        input_ = output_ = 0.0F;
    }
    float process(float input) noexcept {
        output_ = snap(input - input_ + coefficient_ * output_);
        input_ = input;
        return output_;
    }

private:
    float coefficient_ = 0.0F;
    float input_ = 0.0F;
    float output_ = 0.0F;
};

// bounded phenomenological magnetic memory, not a physical material solver.
class TapeHead {
public:
    void init(float rate) noexcept {
        inverseRate_ = 1.0F / rate;
        memory_ = lowpass_ = 0.0F;
        dc_.init(rate);
    }
    float process(float input, float bias) noexcept {
        const float drive = 1.0F + 4.0F * bias;
        const float magnetized = std::tanh(drive * input + 0.12F * bias * memory_);
        memory_ = snap(memory_ + 0.08F * (magnetized - memory_));
        // backward-euler one-pole; positive and stable at all supported rates.
        const float pole = 6.283185307F * (9000.0F - 6000.0F * bias) * inverseRate_;
        lowpass_ = snap(lowpass_ + (pole / (1.0F + pole)) * (magnetized - lowpass_));
        return snap(std::clamp(dc_.process(lowpass_) * 0.45F, -0.9F, 0.9F));
    }

private:
    float inverseRate_ = 1.0F / config::sampleRate;
    float memory_ = 0.0F;
    float lowpass_ = 0.0F;
    DcBlocker dc_{};
};
} // namespace algorhythmia
