#pragma once

#include <cstdint>

namespace algorhythmia {
// Finite linear ramp: endpoint assignment prevents endless exponential tails.
// Audio-thread owned; callers supply finite, normalized targets.
class LinearSmoother {
public:
    void reset(float value, std::uint32_t rampSamples) noexcept {
        current_ = target_ = value;
        increment_ = 0.0F;
        remaining_ = 0;
        rampSamples_ = rampSamples > 0 ? rampSamples : 1;
    }

    void setTarget(float value) noexcept {
        if (value == target_) {
            return;
        }
        target_ = value;
        remaining_ = rampSamples_;
        increment_ = (target_ - current_) / static_cast<float>(rampSamples_);
    }

    [[nodiscard]] float current() const noexcept {
        return current_;
    }

    [[nodiscard]] float next() noexcept {
        if (remaining_ != 0) {
            --remaining_;
            current_ = remaining_ == 0 ? target_ : current_ + increment_;
        }
        // Also handle tiny finite automation values without relying on host FTZ.
        return current_ > -1.0e-20F && current_ < 1.0e-20F ? 0.0F : current_;
    }

private:
    float current_ = 0.0F;
    float target_ = 0.0F;
    float increment_ = 0.0F;
    std::uint32_t remaining_ = 0;
    std::uint32_t rampSamples_ = 1;
};
} // namespace algorhythmia
