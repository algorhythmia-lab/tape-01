#include "algorhythmia/host/processor.hpp"
#include "algorhythmia/host/state.hpp"

#include <doctest/doctest.h>
#include <limits>
#include <ostream>
#include <vector>
using namespace algorhythmia::host;

TEST_CASE("state schema is bounded, portable and transactional on every failure") {
    for (const auto& preset : presets) {
        State actual;
        const auto bytes = encode(preset.state);
        CHECK(decode(bytes, actual) == DecodeResult::Ok);
        CHECK(actual == preset.state);
        CHECK(bytes[0] == 'S');
        CHECK(bytes[1] == 'Y');
        CHECK(bytes[2] == 'S');
        CHECK(bytes[3] == '1');
        for (std::size_t i = 0; i < bytes.size(); ++i) {
            auto broken = bytes;
            broken[i] ^= 0x80;
            State destination = presets[1].state;
            CHECK(decode(broken, destination) != DecodeResult::Ok);
            CHECK(destination == presets[1].state);
        }
        for (std::size_t n = 0; n < bytes.size(); ++n)
            CHECK(decode(std::span(bytes).first(n), actual) == DecodeResult::Size);
    }
    auto future = encode(State{});
    future[4] = 2;
    State destination;
    CHECK(decode(future, destination) == DecodeResult::Version);
    future[4] = 0;
    CHECK(decode(future, destination) == DecodeResult::Version);
    State invalid;
    invalid.values[index(Id::Flux)] = std::numeric_limits<float>::quiet_NaN();
    CHECK(decode(encode(invalid), destination) == DecodeResult::Values);
    invalid = {};
    invalid.values[index(Id::Domain)] = 0.25F;
    CHECK_FALSE(valid(invalid));
    // Reproducible malformed-input fuzz; no decoder path reads outside bounds.
    std::uint32_t random = 19;
    for (unsigned trial = 0; trial < 10000; ++trial) {
        StateBytes bytes;
        for (auto& byte : bytes) {
            random = random * 1664525U + 1013904223U;
            byte = static_cast<std::uint8_t>(random >> 24);
        }
        CHECK(decode(bytes, destination) != DecodeResult::Ok);
    }
}
TEST_CASE("event offsets match independent renders split exactly at those samples") {
    Processor scheduled, split;
    scheduled.prepare(48000);
    split.prepare(48000);
    constexpr std::array events{Event{7, EventKind::Parameter, Id::Drive, 1},
                                Event{73, EventKind::Parameter, Id::Flux, 0.1F},
                                Event{73, EventKind::Parameter, Id::Bias, 0.8F},
                                Event{255, EventKind::Parameter, Id::Domain, 0.5F},
                                Event{801, EventKind::Brake, Id::Flux, 1},
                                Event{811, EventKind::Brake, Id::Flux, 0},
                                Event{877, EventKind::Parameter, Id::Output, 0.65F}};
    std::array<float, 1024> l{}, r{}, expectedL{}, expectedR{};
    REQUIRE(scheduled.process(nullptr, nullptr, l.data(), r.data(), l.size(), events));
    std::size_t cursor = 0;
    for (std::size_t i = 0; i < events.size();) {
        const auto offset = events[i].offset;
        if (offset > cursor)
            REQUIRE(split.process(nullptr, nullptr, expectedL.data() + cursor,
                                  expectedR.data() + cursor, offset - cursor));
        std::array<Event, 2> group{};
        std::size_t n = 0;
        while (i < events.size() && events[i].offset == offset) {
            group[n] = events[i++];
            group[n++].offset = 0;
        }
        const auto end = i < events.size() ? events[i].offset : l.size();
        REQUIRE(split.process(nullptr, nullptr, expectedL.data() + offset,
                              expectedR.data() + offset, end - offset, std::span(group).first(n)));
        cursor = end;
    }
    CHECK(l == expectedL);
    CHECK(r == expectedR);
    CHECK(std::all_of(l.begin(), l.begin() + 7, [](float x) { return x == 0; }));
}
TEST_CASE("malformed and overflowing event batches have no partial side effects") {
    Processor actual, expected;
    actual.prepare(48000);
    expected.prepare(48000);
    std::array<float, 1024> l{}, r{}, el{}, er{};
    std::array<Event, eventCapacity + 1> overflow{};
    overflow[0] = {0, EventKind::Parameter, Id::Drive, 1};
    CHECK_FALSE(actual.process(nullptr, nullptr, l.data(), r.data(), l.size(), overflow));
    CHECK(actual.state() == State{});
    CHECK(actual.status().rejected);
    std::array<Event, 2> invalid{
        {{50, EventKind::Parameter, Id::Drive, 1}, {20, EventKind::Parameter, Id::Bias, 0.2F}}};
    CHECK_FALSE(actual.process(nullptr, nullptr, l.data(), r.data(), l.size(), invalid));
    CHECK(actual.state() == State{});
    invalid[1].offset = 1024;
    CHECK_FALSE(actual.process(nullptr, nullptr, l.data(), r.data(), l.size(), invalid));
    CHECK(expected.process(nullptr, nullptr, el.data(), er.data(), el.size()));
    CHECK(l == el);
    CHECK(r == er);
    CHECK(actual.process(nullptr, nullptr, nullptr, nullptr, 0));
    CHECK_FALSE(actual.process(nullptr, nullptr, nullptr, nullptr, 0, std::span(invalid).first(1)));
}
TEST_CASE("preset recall and prepare reset audio deterministically while preserving controls") {
    Processor p;
    p.prepare(48000);
    std::array<float, 4096> firstL{}, firstR{}, secondL{}, secondR{};
    for (const auto& preset : presets) {
        REQUIRE(p.restore(preset.state));
        REQUIRE(p.process(nullptr, nullptr, firstL.data(), firstR.data(), firstL.size()));
        REQUIRE(p.restore(preset.state));
        REQUIRE(p.process(nullptr, nullptr, secondL.data(), secondR.data(), secondL.size()));
        CHECK(firstL == secondL);
        CHECK(firstR == secondR);
        p.prepare(48000);
        CHECK(p.state() == preset.state);
        REQUIRE(p.process(nullptr, nullptr, secondL.data(), secondR.data(), secondL.size()));
        CHECK(firstL == secondL);
        CHECK(firstR == secondR);
    }
}
TEST_CASE("bypass reaches exact dry stereo with in-place aliasing at all desktop rates") {
    for (float rate : {44100.F, 48000.F, 88200.F, 96000.F, 176400.F, 192000.F}) {
        Processor p;
        p.prepare(rate);
        auto state = presets[1].state;
        state.values[index(Id::Bypass)] = 1;
        REQUIRE(p.restore(state));
        std::array<float, 513> l{}, r{};
        for (std::size_t i = 0; i < l.size(); ++i) {
            l[i] = static_cast<float>(i) / 1024;
            r[i] = -l[i];
        }
        const auto originalL = l, originalR = r;
        REQUIRE(p.process(l.data(), r.data(), l.data(), r.data(), l.size()));
        CHECK(l == originalL);
        CHECK(r == originalR);
    }
}
TEST_CASE("transform needs input and direct brake has no tap side effect") {
    Processor p;
    p.prepare(48000);
    REQUIRE(p.restore(presets[3].state));
    std::array<float, 4096> l{}, r{}, input{};
    REQUIRE(p.process(nullptr, nullptr, l.data(), r.data(), l.size()));
    CHECK(std::all_of(l.begin(), l.end(), [](float x) { return x == 0; }));
    input[0] = 0.5F;
    REQUIRE(p.process(input.data(), input.data(), l.data(), r.data(), l.size()));
    CHECK(std::any_of(l.begin(), l.end(), [](float x) { return x != 0; }));
    constexpr std::array events{Event{0, EventKind::Brake, Id::Flux, 1},
                                Event{1, EventKind::Brake, Id::Flux, 0}};
    REQUIRE(p.process(nullptr, nullptr, l.data(), r.data(), l.size(), events));
    CHECK(p.status().engine.running);
    CHECK_FALSE(p.status().engine.brake);
}

TEST_CASE("schema 1 has a checked wire fixture independent of the serializer") {
    static_assert(stateBytes == 40);
    constexpr StateBytes fixture{0x53, 0x59, 0x53, 0x31, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x40, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x59, 0x4b, 0xd7, 0x6c};
    CHECK(encode(State{}) == fixture);
    State restored;
    REQUIRE(decode(fixture, restored) == DecodeResult::Ok);
    CHECK(restored == State{});
}

TEST_CASE("four factory study names order and schema-one values stay stable") {
    // Literal compatibility fixture, independent of the catalogue initializer.
    constexpr std::array<std::string_view, 4> names{"initial / paused", "01 / magnetic pulse",
                                                    "02 / cellular walls", "03 / forward tape"};
    constexpr std::array<std::array<float, 7>, 4> values{{{.5F, .5F, 0, 0, 0, .75F, 0},
                                                          {.5F, .22F, 1, 0, 0, .75F, 0},
                                                          {.5F, .38F, 1, 0, .5F, .75F, 0},
                                                          {.75F, .2F, 1, 0, 1, .75F, 0}}};
    REQUIRE(presets.size() == 4);
    for (std::size_t i = 0; i < 4; ++i) {
        CHECK(presets[i].name == names[i]);
        CHECK(presets[i].state.values == values[i]);
    }
}
