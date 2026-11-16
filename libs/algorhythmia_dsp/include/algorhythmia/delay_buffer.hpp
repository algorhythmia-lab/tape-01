#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <type_traits>

namespace algorhythmia {
// Fixed-capacity storage. Instantiate at static scope on a microcontroller;
// do not place a large instance on its audio-task stack. Clear only at init.
template <typename Sample, std::size_t Capacity> class DelayBuffer {
    static_assert(Capacity > 0);

public:
    void clear() noexcept {
        samples_.fill(Sample{});
        writeIndex_ = 0;
    }

    void push(Sample value) noexcept {
        samples_[writeIndex_] = value;
        if (++writeIndex_ == Capacity) {
            writeIndex_ = 0;
        }
    }

    // Read age zero for most recently written sample. Clamp excessive ages.
    [[nodiscard]] Sample read(std::size_t delay) const noexcept {
        delay = delay < Capacity ? delay : Capacity - 1;
        const auto distance = delay + 1;
        const auto location =
            writeIndex_ >= distance ? writeIndex_ - distance : Capacity - (distance - writeIndex_);
        return samples_[location];
    }

    [[nodiscard]] static constexpr std::size_t capacity() noexcept {
        return Capacity;
    }

    [[nodiscard]] std::size_t writeIndex() const noexcept {
        return writeIndex_;
    }

    // absolute circular position, not age; periodic four-point hermite kernel.
    // interpolation reduces imaging; it cannot band-limit arbitrary tape input.
    [[nodiscard]] float readCubic(float position) const noexcept
        requires std::is_arithmetic_v<Sample>
    {
        if (!std::isfinite(position)) {
            return 0.0F;
        }
        const float size = static_cast<float>(Capacity);
        if (position < 0.0F || position >= size) {
            position = std::fmod(position, size);
            if (position < 0.0F) {
                position += size;
            }
        }
        // rounding at the circular seam can produce exactly capacity.
        if (position >= size) {
            position = 0.0F;
        }
        const auto center = static_cast<std::size_t>(position);
        const float fraction = position - static_cast<float>(center);
        const auto at = [&](std::size_t offset) noexcept {
            return static_cast<float>(samples_[offset % Capacity]);
        };
        const float a = at(center == 0 ? Capacity - 1 : center - 1);
        const float b = at(center);
        const float c = at(center + 1);
        const float d = at(center + 2);
        const float c1 = 0.5F * (c - a);
        const float c2 = a - 2.5F * b + 2.0F * c - 0.5F * d;
        const float c3 = 0.5F * (d - a) + 1.5F * (b - c);
        return ((c3 * fraction + c2) * fraction + c1) * fraction + b;
    }

private:
    std::array<Sample, Capacity> samples_{};
    std::size_t writeIndex_ = 0;
};
} // namespace algorhythmia
