#include "algorhythmia/host/processor.hpp"

#include <algorithm>
#include <cmath>

namespace algorhythmia::host {
static_assert(index(Id::Flux) == static_cast<std::size_t>(ParameterId::Flux));
static_assert(index(Id::Bias) == static_cast<std::size_t>(ParameterId::Bias));
static_assert(index(Id::Drive) == static_cast<std::size_t>(ParameterId::Drive));
static_assert(index(Id::State) == static_cast<std::size_t>(ParameterId::State));
void Processor::prepare(float rate) noexcept {
    sampleRate_ = sanitizeRate(rate);
    static_cast<void>(engine_.restore(
        {state_.values[index(Id::Flux)], state_.values[index(Id::Bias)],
         static_cast<SceneId>(std::lround(state_.values[index(Id::Domain)] * 2)),
         state_.values[index(Id::State)] != 0, state_.values[index(Id::Drive)] != 0},
        sampleRate_));
    status_ = {};
    const auto ramp =
        static_cast<std::uint32_t>(std::lround(sampleRate_ * config::smoothingSeconds));
    trim_.reset(std::pow(10.0F, trimDb(state_.values[index(Id::Output)]) / 20.0F), ramp);
    wet_.reset(1 - state_.values[index(Id::Bypass)], ramp);
    for (const auto& p : catalogue)
        apply({0, EventKind::Parameter, p.id, state_.values[index(p.id)]});
}
bool Processor::restore(const State& state) noexcept {
    if (!valid(state))
        return false;
    state_ = state;
    // Parameter-only recall starts all phases/history deterministically. It is
    // explicit reset, not a claim to resume a captured performance.
    prepare(sampleRate_);
    return true;
}
void Processor::apply(const Event& event) noexcept {
    if (event.kind == EventKind::Brake) {
        engine_.setBrake(event.value != 0);
        return;
    }
    if (event.kind != EventKind::Parameter) {
        engine_.setButton(event.kind == EventKind::DriveEdge ? ParameterId::Drive
                                                             : ParameterId::State,
                          event.value != 0);
        return;
    }
    state_.values[index(event.id)] = event.value;
    switch (event.id) {
    case Id::Flux:
    case Id::Bias:
    case Id::Drive:
    case Id::State:
        engine_.setParameter(static_cast<ParameterId>(index(event.id)), event.value);
        break;
    case Id::Domain:
        engine_.setScene(static_cast<SceneId>(std::lround(event.value * 2)));
        break;
    case Id::Output:
        trim_.setTarget(std::pow(10.0F, trimDb(event.value) / 20.0F));
        break;
    case Id::Bypass:
        wet_.setTarget(1 - event.value);
        break;
    case Id::Count:
        break;
    }
}
void Processor::render(const float* inL, const float* inR, float* outL, float* outR,
                       std::size_t frames) noexcept {
    // Fixed conversion scratch also preserves dry samples for in-place hosts.
    constexpr std::size_t chunk = 64;
    std::array<float, chunk> dryL{}, dryR{}, wetL{}, wetR{};
    for (std::size_t offset = 0; offset < frames;) {
        const auto n = std::min(chunk, frames - offset);
        for (std::size_t i = 0; i < n; ++i) {
            dryL[i] = inL ? sanitizeAudio(inL[offset + i]) : 0;
            dryR[i] = inR ? sanitizeAudio(inR[offset + i]) : 0;
            status_.inputChannelPeak[0] = std::max(status_.inputChannelPeak[0], std::abs(dryL[i]));
            status_.inputChannelPeak[1] = std::max(status_.inputChannelPeak[1], std::abs(dryR[i]));
            status_.inputPeak = std::max({status_.inputPeak, std::abs(dryL[i]), std::abs(dryR[i])});
        }
        engine_.process(dryL.data(), dryR.data(), wetL.data(), wetR.data(), n);
        for (std::size_t i = 0; i < n; ++i) {
            const auto wet = wet_.next();
            const auto gain = trim_.next();
            const auto l = wetL[i] * gain * wet + dryL[i] * (1 - wet);
            const auto r = wetR[i] * gain * wet + dryR[i] * (1 - wet);
            status_.clipped |= std::abs(l) > 1 || std::abs(r) > 1;
            outL[offset + i] = snap(std::clamp(l, -1.0F, 1.0F));
            outR[offset + i] = snap(std::clamp(r, -1.0F, 1.0F));
            status_.outputChannelPeak[0] =
                std::max(status_.outputChannelPeak[0], std::abs(outL[offset + i]));
            status_.outputChannelPeak[1] =
                std::max(status_.outputChannelPeak[1], std::abs(outR[offset + i]));
            status_.outputPeak = std::max(
                {status_.outputPeak, std::abs(outL[offset + i]), std::abs(outR[offset + i])});
        }
        offset += n;
    }
    status_.engine = engine_.telemetry();
}
bool Processor::process(const float* inL, const float* inR, float* outL, float* outR,
                        std::size_t frames, std::span<const Event> events) noexcept {
    // Zero blocks consume nothing, including gestures and state transitions.
    if (frames == 0)
        return events.empty();
    if (!outL || !outR)
        return false;
    bool accepted = events.size() <= eventCapacity;
    std::size_t previous = 0;
    if (accepted)
        for (const auto& e : events) {
            accepted &= e.offset < frames && e.offset >= previous &&
                        (e.kind == EventKind::Parameter
                             ? valid(e.id, e.value)
                             : ((e.kind == EventKind::DriveEdge || e.kind == EventKind::StateEdge ||
                                 e.kind == EventKind::Brake) &&
                                (e.value == 0 || e.value == 1)));
            previous = e.offset;
        }
    status_ = {};
    status_.rejected = !accepted;
    if (!accepted)
        events = {};
    std::size_t cursor = 0;
    for (const auto& e : events) {
        if (e.offset > cursor)
            render(inL ? inL + cursor : nullptr, inR ? inR + cursor : nullptr, outL + cursor,
                   outR + cursor, e.offset - cursor);
        apply(e);
        cursor = e.offset;
    }
    if (cursor < frames)
        render(inL ? inL + cursor : nullptr, inR ? inR + cursor : nullptr, outL + cursor,
               outR + cursor, frames - cursor);
    return accepted;
}
} // namespace algorhythmia::host
