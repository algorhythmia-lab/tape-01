#include <algorhythmia/engine.hpp>
#include <algorhythmia/parameters.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <doctest/doctest.h>
#include <limits>
#include <vector>

namespace {

using algorhythmia::Engine;
using algorhythmia::ParameterId;
using algorhythmia::SceneId;

constexpr float kSampleRate = 48'000.0F;
constexpr std::size_t kFadeFrames = 480;
constexpr std::size_t kRenderFrames = 12'000;

struct StereoRender {
    std::vector<float> left;
    std::vector<float> right;
};

[[nodiscard]] StereoRender render(Engine& engine, std::size_t frames, std::size_t blockSize = 127) {
    StereoRender result{std::vector<float>(frames), std::vector<float>(frames)};
    for (std::size_t offset = 0; offset < frames; offset += blockSize) {
        const auto count = std::min(blockSize, frames - offset);
        engine.process(nullptr, nullptr, result.left.data() + offset, result.right.data() + offset,
                       count);
    }
    return result;
}

void checkValidAudio(const StereoRender& audio) {
    for (const auto* channel : {&audio.left, &audio.right}) {
        CHECK(std::all_of(channel->begin(), channel->end(), [](float sample) {
            return std::isfinite(sample) && std::fpclassify(sample) != FP_SUBNORMAL;
        }));
    }
}

void enableBytebeat(Engine& engine, float rate = kSampleRate) {
    engine.init(rate);
    engine.setScene(SceneId::Bytebeat);
    engine.setParameter(ParameterId::Drive, 1.0F);
}

} // namespace

TEST_CASE("A freshly initialized engine stays exactly silent until DRIVE") {
    Engine engine;
    engine.init(kSampleRate);
    const auto audio = render(engine, kRenderFrames);
    CHECK(
        std::all_of(audio.left.begin(), audio.left.end(), [](float value) { return value == 0; }));
    CHECK(std::all_of(audio.right.begin(), audio.right.end(),
                      [](float value) { return value == 0; }));
}

TEST_CASE("A callback before initialization is silent and missing output pointers are tolerated") {
    Engine engine;
    const auto uninitialized = render(engine, 64);
    CHECK(std::all_of(uninitialized.left.begin(), uninitialized.left.end(),
                      [](float value) { return value == 0.0F; }));
    CHECK(std::all_of(uninitialized.right.begin(), uninitialized.right.end(),
                      [](float value) { return value == 0.0F; }));
    enableBytebeat(engine);
    std::array<float, 8> output{};
    output.fill(1234.5F);
    engine.process(nullptr, nullptr, output.data(), nullptr, output.size());
    engine.process(nullptr, nullptr, nullptr, output.data(), output.size());
    CHECK(std::all_of(output.begin(), output.end(), [](float value) { return value == 1234.5F; }));
}

TEST_CASE(
    "Bytebeat produces finite, normal-or-zero audio at supported rates and control extremes") {
    for (const float rate : {8'000.0F, 44'100.0F, 48'000.0F, 96'000.0F, 192'000.0F}) {
        for (const float time : {0.0F, 0.5F, 1.0F}) {
            for (const float form : {0.0F, 0.5F, 1.0F}) {
                for (const float mode : {0.0F, 1.0F}) {
                    CAPTURE(rate);
                    CAPTURE(time);
                    CAPTURE(form);
                    CAPTURE(mode);
                    Engine engine;
                    enableBytebeat(engine, rate);
                    engine.setParameter(ParameterId::Flux, time);
                    engine.setParameter(ParameterId::Bias, form);
                    engine.setParameter(ParameterId::State, mode);
                    checkValidAudio(render(engine, kRenderFrames));
                }
            }
        }
    }

    Engine engine;
    enableBytebeat(engine);
    const auto audio = render(engine, kRenderFrames);
    CHECK(
        std::any_of(audio.left.begin(), audio.left.end(), [](float value) { return value != 0; }));
    CHECK(std::any_of(audio.right.begin(), audio.right.end(),
                      [](float value) { return value != 0; }));
}

TEST_CASE("Processing touches exactly the requested output range") {
    constexpr std::size_t capacity = 1024;
    constexpr float canary = 1234.5F;
    std::array<float, capacity + 2> left{};
    std::array<float, capacity + 2> right{};
    std::array<float, capacity> inputLeft{};
    std::array<float, capacity> inputRight{};
    Engine engine;
    enableBytebeat(engine);

    for (const std::size_t frames : {0U, 1U, 2U, 3U, 7U, 31U, 64U, 127U, 256U, 511U, 1024U}) {
        CAPTURE(frames);
        left.fill(canary);
        right.fill(canary);
        engine.process(inputLeft.data(), inputRight.data(), left.data() + 1, right.data() + 1,
                       frames);
        CHECK(left.front() == canary);
        CHECK(right.front() == canary);
        CHECK(std::all_of(left.begin() + static_cast<std::ptrdiff_t>(frames + 1), left.end(),
                          [](float value) { return value == canary; }));
        CHECK(std::all_of(right.begin() + static_cast<std::ptrdiff_t>(frames + 1), right.end(),
                          [](float value) { return value == canary; }));
        for (std::size_t index = 1; index <= frames; ++index) {
            CHECK(std::isfinite(left[index]));
            CHECK(std::isfinite(right[index]));
            CHECK(std::fpclassify(left[index]) != FP_SUBNORMAL);
            CHECK(std::fpclassify(right[index]) != FP_SUBNORMAL);
        }
    }
    // A zero-length callback may contain no valid channel pointers at all.
    engine.process(nullptr, nullptr, nullptr, nullptr, 0);
}

TEST_CASE("Reset restores the exact same initial rendering") {
    Engine engine;
    enableBytebeat(engine);
    const auto first = render(engine, kRenderFrames);
    engine.setParameter(ParameterId::Flux, 0.91F);
    engine.setParameter(ParameterId::Bias, 0.72F);
    engine.setParameter(ParameterId::State, 1.0F);
    static_cast<void>(render(engine, 1337));
    enableBytebeat(engine);
    const auto second = render(engine, kRenderFrames);
    CHECK(first.left == second.left);
    CHECK(first.right == second.right);
}

TEST_CASE("A fixed control sequence does not depend on audio block size") {
    Engine whole;
    Engine fragmented;
    enableBytebeat(whole);
    enableBytebeat(fragmented);
    for (std::size_t stage = 0; stage < 3; ++stage) {
        CAPTURE(stage);
        const auto expected = render(whole, kRenderFrames, kRenderFrames);
        const auto actual = render(fragmented, kRenderFrames, 37);
        CHECK(expected.left == actual.left);
        CHECK(expected.right == actual.right);
        const float value = static_cast<float>(stage + 1) / 3.0F;
        for (auto* engine : {&whole, &fragmented}) {
            engine->setParameter(ParameterId::Flux, value);
            engine->setParameter(ParameterId::Bias, value);
            engine->setParameter(ParameterId::State, value);
        }
    }
}

TEST_CASE("DRIVE fades to an exact zero endpoint") {
    Engine engine;
    enableBytebeat(engine);
    static_cast<void>(render(engine, 4096));
    engine.setParameter(ParameterId::Drive, 0.0F);
    const auto fade = render(engine, kFadeFrames + 512, 1);
    checkValidAudio(fade);
    CHECK(std::any_of(fade.left.begin(), fade.left.begin() + 32,
                      [](float sample) { return sample != 0.0F; }));
    CHECK(std::all_of(fade.left.begin() + static_cast<std::ptrdiff_t>(kFadeFrames), fade.left.end(),
                      [](float sample) { return sample == 0.0F; }));
    CHECK(std::all_of(fade.right.begin() + static_cast<std::ptrdiff_t>(kFadeFrames),
                      fade.right.end(), [](float sample) { return sample == 0.0F; }));
}

TEST_CASE("transport ramps down and computational tape pauses deterministically") {
    Engine engine;
    enableBytebeat(engine);
    static_cast<void>(render(engine, 4096));
    engine.setParameter(ParameterId::Drive, 0.0F);
    const auto fade = render(engine, kFadeFrames + 100);
    checkValidAudio(fade);
    CHECK(fade.left.back() == 0.0F);
    engine.setParameter(ParameterId::Drive, 1.0F);
    const auto resumed = render(engine, 4096);
    CHECK(std::any_of(resumed.left.begin(), resumed.left.end(), [](float x) { return x != 0; }));
}

TEST_CASE("Nonfinite controls and unknown identifiers leave the current target intact") {
    Engine expected;
    Engine actual;
    enableBytebeat(expected);
    enableBytebeat(actual);
    for (const auto id :
         {ParameterId::Flux, ParameterId::Bias, ParameterId::Drive, ParameterId::State}) {
        actual.setParameter(id, std::numeric_limits<float>::quiet_NaN());
        actual.setParameter(id, std::numeric_limits<float>::infinity());
        actual.setParameter(id, -std::numeric_limits<float>::infinity());
    }
    // Deliberately exercise unknown IDs representable by the fixed uint8_t base.
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    actual.setParameter(static_cast<ParameterId>(255), 1.0F);
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    actual.setScene(static_cast<SceneId>(255));
    const auto reference = render(expected, kRenderFrames);
    const auto received = render(actual, kRenderFrames);
    CHECK(reference.left == received.left);
    CHECK(reference.right == received.right);
}

TEST_CASE("Finite controls clamp and the latest pending control value wins") {
    for (const float endpoint : {0.0F, 1.0F}) {
        Engine expected;
        Engine actual;
        enableBytebeat(expected);
        enableBytebeat(actual);
        for (const auto id :
             {ParameterId::Flux, ParameterId::Bias, ParameterId::Drive, ParameterId::State}) {
            expected.setParameter(id, endpoint);
            actual.setParameter(id, 0.37F);
            actual.setParameter(id, endpoint == 0.0F ? -100.0F : 100.0F);
        }
        const auto reference = render(expected, kRenderFrames);
        const auto received = render(actual, kRenderFrames);
        CHECK(reference.left == received.left);
        CHECK(reference.right == received.right);
    }
}

TEST_CASE("Invalid sample rates use the documented 48 kHz fallback") {
    for (const float rate :
         {0.0F, -1.0F, 7999.0F, 192'001.0F, std::numeric_limits<float>::quiet_NaN(),
          std::numeric_limits<float>::infinity()}) {
        CAPTURE(rate);
        Engine expected;
        Engine actual;
        enableBytebeat(expected);
        enableBytebeat(actual, rate);
        const auto reference = render(expected, kRenderFrames);
        const auto received = render(actual, kRenderFrames);
        CHECK(reference.left == received.left);
        CHECK(reference.right == received.right);
    }
}

TEST_CASE("cellular produces stereo and unrecorded live tape remains silent") {
    Engine engine;
    enableBytebeat(engine);
    engine.setScene(SceneId::CellularFm);
    const auto fm = render(engine, 48000);
    checkValidAudio(fm);
    CHECK(std::any_of(fm.left.begin() + 480, fm.left.end(), [](float x) { return x != 0; }));
    CHECK(fm.left != fm.right);
    engine.setScene(SceneId::LiveTransform);
    const auto live = render(engine, 4096);
    CHECK(live.left.back() == 0.0F);
    CHECK(live.right.back() == 0.0F);
}
