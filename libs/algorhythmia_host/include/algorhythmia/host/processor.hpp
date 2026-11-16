#pragma once
#include "algorhythmia/engine.hpp"
#include "algorhythmia/host/catalogue.hpp"

#include <span>

namespace algorhythmia::host {
enum class EventKind { Parameter, DriveEdge, StateEdge, Brake };
struct Event {
    std::size_t offset = 0;
    EventKind kind = EventKind::Parameter;
    Id id = Id::Flux;
    float value = 0;
};
inline constexpr std::size_t eventCapacity = 256;
struct Status {
    Telemetry engine;
    float inputPeak = 0;
    float outputPeak = 0;
    std::array<float, 2> inputChannelPeak{}, outputChannelPeak{};
    bool clipped = false;
    bool rejected = false;
};
// Exclusively audio-owned. Adapters hand over immutable, prevalidated transactions.
// Offset order is ascending; ties retain caller order; an invalid/overflowed batch
// is rejected atomically and audio continues with the last valid controls.
class Processor {
public:
    void prepare(float sampleRate) noexcept;
    [[nodiscard]] bool restore(const State&) noexcept;
    [[nodiscard]] bool process(const float* inL, const float* inR, float* outL, float* outR,
                               std::size_t frames, std::span<const Event> events = {}) noexcept;
    [[nodiscard]] const State& state() const noexcept {
        return state_;
    }
    [[nodiscard]] Status status() const noexcept {
        return status_;
    }

private:
    Engine engine_;
    State state_;
    LinearSmoother trim_, wet_;
    float sampleRate_ = 48000;
    Status status_{};
    void apply(const Event&) noexcept;
    void render(const float*, const float*, float*, float*, std::size_t) noexcept;
};
} // namespace algorhythmia::host
