#include <algorhythmia/bytebeat.hpp>
#include <algorhythmia/cellular.hpp>
#include <algorhythmia/engine.hpp>
#include <algorhythmia/transform.hpp>
#include <array>
#include <cmath>
#include <doctest/doctest.h>
#include <limits>

using namespace algorhythmia;
namespace {
void advance(Engine& engine, std::size_t frames) {
    std::array<float, 128> left{}, right{};
    while (frames != 0) {
        const auto count = std::min(frames, left.size());
        engine.process(nullptr, nullptr, left.data(), right.data(), count);
        frames -= count;
    }
}
std::uint32_t scalarEvolution(std::uint32_t cells, std::uint8_t rule) {
    std::uint32_t result = 0;
    for (unsigned i = 0; i < 32; ++i) {
        const auto neighbor = (((cells >> ((i + 31) % 32)) & 1U) << 2U) |
                              (((cells >> i) & 1U) << 1U) | ((cells >> ((i + 1) % 32)) & 1U);
        result |= ((static_cast<std::uint32_t>(rule) >> neighbor) & 1U) << i;
    }
    return result;
}
} // namespace

TEST_CASE("periodic automaton agrees with independent scalar rule evaluation") {
    for (const auto rule : {30U, 90U, 110U, 150U}) {
        std::uint32_t cells = 1;
        for (unsigned generation = 0; generation < 256; ++generation) {
            const auto next = Cellular::evolve(cells, static_cast<std::uint8_t>(rule));
            CHECK(next == scalarEvolution(cells, static_cast<std::uint8_t>(rule)));
            cells = next;
        }
    }
    CHECK(Cellular::ruleForBias(0.0F) == 30);
    CHECK(Cellular::ruleForBias(0.25F) == 90);
    CHECK(Cellular::ruleForBias(0.5F) == 110);
    CHECK(Cellular::ruleForBias(0.75F) == 150);
    CHECK(Cellular::ruleForBias(1.0F) == 150);
}

TEST_CASE("bytebeat azimuth, modulo ticks, pause and capstan inertia") {
    Bytebeat byte;
    byte.init(48000);
    SceneControls controls{0.5F, 0.75F, 0.0F, true, false};
    bool stereo = false;
    for (unsigned i = 0; i < 48000; ++i) {
        const auto sample = byte.process({}, controls);
        stereo |= sample.left != sample.right;
        CHECK(std::isfinite(sample.left));
        CHECK(std::abs(sample.left) <= 0.9F);
    }
    CHECK(stereo);
    const auto before = byte.tick();
    controls.running = false;
    for (unsigned i = 0; i < 480; ++i) {
        static_cast<void>(byte.process({}, controls));
    }
    CHECK(byte.tick() == before);
    controls.running = controls.brake = true;
    for (unsigned i = 0; i < 96000; ++i) {
        static_cast<void>(byte.process({}, controls));
    }
    CHECK(byte.velocity() < 0.001F);
    controls.brake = false;
    for (unsigned i = 0; i < 48000; ++i) {
        static_cast<void>(byte.process({}, controls));
    }
    CHECK(byte.velocity() > 0.99F);
    CHECK(Bytebeat::expression(0xffffffffU, 0) ==
          0xffffffffU * ((0xffffffffU >> 5U) | (0xffffffffU >> 8U)));
    byte.init(48000);
    controls.bias = 0.0F;
    for (unsigned i = 0; i < 2000; ++i) {
        const auto sample = byte.process({}, controls);
        CHECK(sample.left == sample.right);
    }
}

TEST_CASE("cellular flux sets generation rate and brake stops wall evolution") {
    Cellular cell;
    cell.init(48000);
    SceneControls controls{1.0F, 0.25F, 0.0F, true, false};
    for (unsigned i = 0; i < 48000; ++i) {
        static_cast<void>(cell.process({}, controls));
    }
    CHECK(cell.generation() >= 19);
    CHECK(cell.generation() <= 20);
    const auto generation = cell.generation();
    controls.brake = true;
    for (unsigned i = 0; i < 48000; ++i) {
        static_cast<void>(cell.process({}, controls));
    }
    CHECK(cell.generation() == generation);
    CHECK(cell.telemetry() == 90);
}

TEST_CASE("live tape has independent channels, reverse, freeze, scrub and bounded feedback") {
    Transform tape;
    tape.init(48000);
    SceneControls controls{0.75F, 0.0F, 0.0F, true, false};
    bool audible = false;
    for (unsigned i = 0; i < 36000; ++i) {
        const auto output =
            tape.process({0.4F * std::sin(static_cast<float>(i) * 0.02F), 0.0F}, controls);
        audible |= output.left != 0;
        CHECK(output.right == 0.0F);
    }
    CHECK(audible);
    CHECK((tape.telemetry() & 15U) == 0);
    const auto write = tape.writeIndex();
    controls.brake = true;
    const auto initialPosition = tape.position();
    for (unsigned i = 0; i < 512; ++i) {
        static_cast<void>(tape.process({1, 1}, controls));
    }
    CHECK(tape.writeIndex() == write);
    CHECK(tape.position() != initialPosition);
    controls.flux = 0.5F;
    const auto frozen = tape.position();
    for (unsigned i = 0; i < 1024; ++i) {
        static_cast<void>(tape.process({}, controls));
    }
    CHECK(tape.position() == frozen);
    CHECK(tape.writeIndex() == write);
    controls.flux = 0.25F;
    const auto reverse = tape.position();
    static_cast<void>(tape.process({}, controls));
    CHECK(tape.position() == doctest::Approx(reverse - 1.0F));
    controls.state = 1.0F;
    static_cast<void>(tape.process({}, controls));
    CHECK(tape.position() == reverse);
    controls.brake = false;
    controls.bias = 1.0F;
    controls.flux = 1.0F;
    for (unsigned i = 0; i < 96000; ++i) {
        const auto output = tape.process({8, -8}, controls);
        CHECK(std::isfinite(output.left));
        CHECK(std::fpclassify(output.left) != FP_SUBNORMAL);
        CHECK(std::abs(output.left) <= 0.9F);
        CHECK(std::abs(output.right) <= 0.9F);
    }
}

TEST_CASE("drive taps latch; state previews use locked symmetric words; commits are asynchronous") {
    Engine engine;
    engine.init(48000);
    engine.setButton(ParameterId::Drive, true);
    advance(engine, 4800);
    engine.setButton(ParameterId::Drive, false);
    advance(engine, 1);
    CHECK(engine.telemetry().running);
    engine.setButton(ParameterId::State, true);
    advance(engine, 14400);
    CHECK(engine.telemetry().preview);
    CHECK(engine.telemetry().word == 0xc0);
    for (const auto bias : {0.5F, 0.0F, 1.0F}) {
        engine.setParameter(ParameterId::Bias, bias);
        advance(engine, 1);
        // a rotation away from the held value is required to enter selection.
    }
    CHECK(engine.telemetry().domain == SceneId::LiveTransform);
    CHECK(engine.telemetry().word == 0x03);
    CHECK(engine.telemetry().color() == 0x54e5ff);
    engine.setParameter(ParameterId::Bias, 0.5F);
    advance(engine, 1);
    // after selection starts, returning to the initial dial value must update preview.
    CHECK(engine.telemetry().domain == SceneId::CellularFm);
    CHECK(engine.telemetry().word == 0x18);
    engine.setButton(ParameterId::State, false);
    advance(engine, 480);
    CHECK(engine.telemetry().domain == SceneId::CellularFm);
    CHECK_FALSE(engine.telemetry().preview);
    engine.setButton(ParameterId::State, true);
    advance(engine, 95999);
    CommitSnapshot snapshot;
    CHECK_FALSE(engine.takeCommitRequest(snapshot));
    advance(engine, 1);
    CHECK(engine.takeCommitRequest(snapshot));
    CHECK(snapshot.domain == SceneId::CellularFm);
    CHECK(snapshot.running);
    CHECK(snapshot.bias == doctest::Approx(0.5F).epsilon(0.002));
    advance(engine, 10000);
    CHECK_FALSE(engine.takeCommitRequest(snapshot));
}

TEST_CASE("midi and button hold transport contracts remain separate") {
    Engine engine;
    engine.init(48000);
    engine.setMidiCc(22, 127);
    engine.setMidiCc(23, 64);
    advance(engine, 480);
    CHECK(engine.telemetry().running);
    CHECK(engine.telemetry().domain == SceneId::CellularFm);
    engine.setButton(ParameterId::Drive, true);
    advance(engine, 24000);
    engine.setButton(ParameterId::Drive, false);
    advance(engine, 480);
    CHECK(engine.telemetry().running);
    engine.setMidiCc(22, 0);
    advance(engine, 480);
    CHECK_FALSE(engine.telemetry().running);
}

TEST_CASE("all domains preserve block partition determinism with stereo in-place input") {
    constexpr std::size_t frames = 4096;
    for (const auto scene : {SceneId::Bytebeat, SceneId::CellularFm, SceneId::LiveTransform}) {
        Engine whole, fragmented;
        whole.init(48000);
        fragmented.init(48000);
        for (auto* engine : {&whole, &fragmented}) {
            engine->setParameter(ParameterId::Drive, 1);
            engine->setParameter(ParameterId::Flux, 0.75F);
            engine->setScene(scene);
        }
        std::array<float, frames> left{}, right{}, expectedL{}, expectedR{};
        for (std::size_t i = 0; i < frames; ++i) {
            left[i] = std::sin(static_cast<float>(i) * 0.02F);
            right[i] = std::cos(static_cast<float>(i) * 0.03F);
        }
        whole.process(left.data(), right.data(), expectedL.data(), expectedR.data(), frames);
        for (std::size_t offset = 0; offset < frames; offset += 37) {
            const auto count = std::min(std::size_t{37}, frames - offset);
            fragmented.process(left.data() + offset, right.data() + offset, left.data() + offset,
                               right.data() + offset, count);
        }
        CHECK(left == expectedL);
        CHECK(right == expectedR);
    }
}

TEST_CASE("nonfinite and denormal stereo input is sanitized before tape storage") {
    Engine engine;
    engine.init(8000);
    engine.setScene(SceneId::LiveTransform);
    advance(engine, 80);
    engine.setParameter(ParameterId::Flux, 1);
    engine.setParameter(ParameterId::Bias, 1);
    engine.setParameter(ParameterId::Drive, 1);
    std::array<float, 64> left{}, right{}, outputL{}, outputR{};
    left.fill(std::numeric_limits<float>::infinity());
    right.fill(std::numeric_limits<float>::denorm_min());
    for (unsigned i = 0; i < 1024; ++i) {
        engine.process(left.data(), right.data(), outputL.data(), outputR.data(), left.size());
        for (auto value : outputL) {
            CHECK(value == 0.0F);
        }
        for (auto value : outputR) {
            CHECK(value == 0.0F);
        }
    }
}

TEST_CASE("cellular and live tape remain bounded at supported rates and dial extremes") {
    for (const float rate : {8000.0F, 44100.0F, 48000.0F, 96000.0F, 192000.0F}) {
        Cellular cellular;
        Transform tape;
        cellular.init(rate);
        tape.init(rate);
        for (const float flux : {0.0F, 0.5F, 1.0F}) {
            for (const float bias : {0.0F, 0.5F, 1.0F}) {
                SceneControls controls{flux, bias, 0.0F, true, false};
                bool valid = true;
                for (unsigned i = 0; i < 4000; ++i) {
                    const auto fm = cellular.process({}, controls);
                    const auto live = tape.process({0.8F, -0.8F}, controls);
                    for (const float x : {fm.left, fm.right, live.left, live.right}) {
                        valid &= std::isfinite(x) && std::fpclassify(x) != FP_SUBNORMAL &&
                                 std::abs(x) <= 1.0F;
                    }
                }
                CHECK(valid);
            }
        }
    }
}

TEST_CASE("STATE freezes audible BIAS during a ramp and preview commits the selected domain") {
    Engine expected, actual;
    expected.init(48000);
    actual.init(48000);
    for (auto* e : {&expected, &actual}) {
        e->setParameter(ParameterId::Drive, 1);
        e->setParameter(ParameterId::Bias, 0);
        advance(*e, 480);
        e->setParameter(ParameterId::Bias, 1);
        advance(*e, 120);
    }
    // 120 / 480 ramp samples yields 0.25 audible BIAS; the requested value is 1.
    actual.setButton(ParameterId::State, true);
    float audible = 0;
    for (int i = 0; i < 120; ++i)
        audible += 1.0F / 480.0F;
    expected.setParameter(ParameterId::Bias, audible);
    std::array<float, 32> al{}, ar{}, el{}, er{};
    actual.process(nullptr, nullptr, al.data(), ar.data(), al.size());
    expected.process(nullptr, nullptr, el.data(), er.data(), el.size());
    CHECK(al == el);
    CHECK(ar == er);
    advance(actual, 14400);
    actual.setParameter(ParameterId::Bias, 0.4F);
    advance(actual, 96000);
    CommitSnapshot snapshot;
    REQUIRE(actual.takeCommitRequest(snapshot));
    CHECK(snapshot.domain == SceneId::CellularFm);
    CHECK(snapshot.bias == doctest::Approx(0.25F).epsilon(0.002));
    CHECK(actual.telemetry().preview);
}
