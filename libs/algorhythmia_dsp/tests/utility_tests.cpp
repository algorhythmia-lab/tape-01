#include <algorhythmia/delay_buffer.hpp>
#include <algorhythmia/smoother.hpp>
#include <cmath>
#include <cstddef>
#include <doctest/doctest.h>
#include <limits>

TEST_CASE("DelayBuffer starts at zero, wraps and clamps ages") {
    algorhythmia::DelayBuffer<float, 4> buffer;
    static_assert(decltype(buffer)::capacity() == 4);
    for (std::size_t age = 0; age < 8; ++age) {
        CHECK(buffer.read(age) == 0.0F);
    }
    for (std::size_t value = 1; value <= 20; ++value) {
        buffer.push(static_cast<float>(value));
        for (std::size_t age = 0; age < 4; ++age) {
            CHECK(buffer.read(age) == (value > age ? static_cast<float>(value - age) : 0.0F));
        }
        CHECK(buffer.read(4) == buffer.read(3));
        CHECK(buffer.read(std::numeric_limits<std::size_t>::max()) == buffer.read(3));
    }
    buffer.clear();
    for (std::size_t age = 0; age < 8; ++age) {
        CHECK(buffer.read(age) == 0.0F);
    }
    buffer.push(42.0F);
    CHECK(buffer.read(0) == 42.0F);
    CHECK(buffer.read(1) == 0.0F);
}

TEST_CASE("A one-sample delay never indexes outside its storage") {
    algorhythmia::DelayBuffer<float, 1> buffer;
    buffer.push(1.0F);
    CHECK(buffer.read(0) == 1.0F);
    buffer.push(2.0F);
    CHECK(buffer.read(0) == 2.0F);
    CHECK(buffer.read(1) == 2.0F);
    CHECK(buffer.read(std::numeric_limits<std::size_t>::max()) == 2.0F);
}

TEST_CASE("Smoothing reaches an exact endpoint within its fixed sample budget") {
    algorhythmia::LinearSmoother smoother;
    smoother.reset(0.0F, 480);
    smoother.setTarget(1.0F);
    float previous = 0.0F;
    for (std::size_t frame = 1; frame < 480; ++frame) {
        // Repeated block snapshots must not restart the ramp.
        smoother.setTarget(1.0F);
        const float current = smoother.next();
        CHECK(current > previous);
        CHECK(current < 1.0F);
        CHECK(current == doctest::Approx(static_cast<float>(frame) / 480.0F).epsilon(0.00001));
        previous = current;
    }
    CHECK(smoother.next() == 1.0F);
    CHECK(smoother.next() == 1.0F);
    smoother.setTarget(0.0F);
    for (std::size_t frame = 1; frame < 480; ++frame) {
        CHECK(smoother.next() > 0.0F);
    }
    CHECK(smoother.next() == 0.0F);
    CHECK(smoother.next() == 0.0F);
}

TEST_CASE("Retargeted smoothing begins at the current value") {
    algorhythmia::LinearSmoother smoother;
    smoother.reset(0.0F, 4);
    smoother.setTarget(1.0F);
    CHECK(smoother.next() == 0.25F);
    CHECK(smoother.next() == 0.5F);
    smoother.setTarget(0.0F);
    CHECK(smoother.next() == 0.375F);
    CHECK(smoother.next() == 0.25F);
    CHECK(smoother.next() == 0.125F);
    CHECK(smoother.next() == 0.0F);
    smoother.reset(0.0F, 0);
    smoother.setTarget(1.0F);
    CHECK(smoother.next() == 1.0F);
}

TEST_CASE("Tiny smoothing targets do not leak denormal output") {
    algorhythmia::LinearSmoother smoother;
    smoother.reset(0.0F, 480);
    smoother.setTarget(std::numeric_limits<float>::denorm_min());
    for (std::size_t frame = 0; frame < 960; ++frame) {
        const float sample = smoother.next();
        CHECK(std::isfinite(sample));
        CHECK(std::fpclassify(sample) != FP_SUBNORMAL);
    }
}

TEST_CASE("cubic tape reads exact integer samples and interpolates a linear interior") {
    algorhythmia::DelayBuffer<float, 8> tape;
    for (unsigned i = 0; i < 8; ++i) {
        tape.push(static_cast<float>(i));
    }
    for (unsigned i = 0; i < 8; ++i) {
        CHECK(tape.readCubic(static_cast<float>(i)) == static_cast<float>(i));
    }
    CHECK(tape.readCubic(3.25F) == doctest::Approx(3.25F));
    CHECK(tape.readCubic(-0.25F) == tape.readCubic(7.75F));
    CHECK(tape.readCubic(8.25F) == tape.readCubic(0.25F));
    CHECK(tape.readCubic(std::numeric_limits<float>::quiet_NaN()) == 0.0F);
    CHECK(std::isfinite(tape.readCubic(std::numeric_limits<float>::max())));
    CHECK(std::isfinite(tape.readCubic(-std::numeric_limits<float>::max())));
    algorhythmia::DelayBuffer<float, 1> single;
    single.push(3.0F);
    CHECK(single.readCubic(0.7F) == 3.0F);
}
