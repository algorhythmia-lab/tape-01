#pragma once
#include "PluginProcessor.hpp"

#include <optional>

namespace sys01::view {
inline unsigned domain(const contract::State& s) {
    return static_cast<unsigned>(std::lround(s.values[contract::index(contract::Id::Domain)] * 2));
}
inline float fluxValue(unsigned d, float f) {
    if (d == 0)
        return 500 + 7500 * f;
    if (d == 1)
        return 30 + 270 * f;
    return algorhythmia::Transform::speedForFlux(f);
}
inline juce::String format(contract::Id id, unsigned d, float f) {
    if (id == contract::Id::Output)
        return juce::String(contract::trimDb(f), 1) + " DB";
    if (id == contract::Id::Bias)
        return juce::String(100 * f, 1) + "%";
    return juce::String(fluxValue(d, f), d == 0   ? 0
                                         : d == 1 ? 1
                                                  : 2) +
           (d == 0   ? " TICKS/S"
            : d == 1 ? " BPM"
                     : " X");
}
struct Parsed {
    std::optional<float> value;
    juce::String error = "INVALID VALUE: use a decimal in range";
};
// Strict decimal grammar, before conversion; no prefix parse, exponent or clamp.
inline Parsed parse(contract::Id id, unsigned d, juce::String text) {
    text = text.trim().toUpperCase();
    int i = 0, digits = 0;
    if (text[i] == '+' || text[i] == '-')
        ++i;
    while ((text[i] >= '0' && text[i] <= '9')) {
        ++i;
        ++digits;
    }
    if (text[i] == '.') {
        ++i;
        while ((text[i] >= '0' && text[i] <= '9')) {
            ++i;
            ++digits;
        }
    }
    if (digits == 0)
        return {};
    const auto suffix = text.substring(i).trim();
    const auto unit = id == contract::Id::Bias     ? "%"
                      : id == contract::Id::Output ? "DB"
                      : d == 0                     ? "TICKS/S"
                      : d == 1                     ? "BPM"
                                                   : "X";
    if (suffix.isNotEmpty() && suffix != unit && !(id == contract::Id::Bias && suffix == "% MACRO"))
        return {};
    const auto v = text.substring(0, i).getDoubleValue();
    const double lo = id == contract::Id::Output ? -60
                      : id == contract::Id::Bias ? 0
                      : d == 0                   ? 500
                      : d == 1                   ? 30
                                                 : -2;
    const double hi = id == contract::Id::Output ? 20
                      : id == contract::Id::Bias ? 100
                      : d == 0                   ? 8000
                      : d == 1                   ? 300
                                                 : 2;
    if (!std::isfinite(v) || v < lo || v > hi)
        return {};
    auto f = static_cast<float>((v - lo) / (hi - lo));
    if (id == contract::Id::Flux && d == 2) {
        if (v == 0)
            f = .5F;
        else if (algorhythmia::Transform::speedForFlux(f) == 0)
            return {{}, "INVALID VALUE: inside zero detent; enter 0 or a larger magnitude"};
    }
    return {f, {}};
}
inline juce::String macro(unsigned d, float bias) {
    if (d == 0)
        return "equation morph / azimuth / drive";
    if (d == 1)
        return "rule " + juce::String(algorhythmia::Cellular::ruleForBias(bias)) + " / FM coupling";
    return "wear / drive / recursion";
}
inline constexpr int canvasWidth = 640, canvasHeight = 416;
inline juce::AffineTransform canvasTransform(int w, int h) {
    const float s = juce::jlimit(
        1.F, 2.F,
        std::min(w / static_cast<float>(canvasWidth), h / static_cast<float>(canvasHeight)));
    return juce::AffineTransform::scale(s).translated((w - canvasWidth * s) / 2,
                                                      (h - canvasHeight * s) / 2);
}
// Message-thread display history only. Never changes processor state or counters.
struct Health {
    std::uint64_t block = 0;
    std::uint32_t rejected = 0, drops = 0;
    double lastAdvance = 0, lastTick = 0, clipUntil = 0, quietSince = -1;
    bool observed = false, live = false, stale = false, unresolved = false, meterValid = false;
    std::array<bool, 4> meterSilent{true, true, true, true};
    std::array<float, 4> meterDb{-60, -60, -60, -60};
    void update(const TelemetrySnapshot& s, double now) {
        const bool reset = observed && s.block < block;
        const bool advance = s.block != 0 && s.block != block;
        const bool lost = observed && s.droppedSnapshots != drops;
        meterValid = advance ? !lost : meterValid;
        const double dt = std::clamp(now - lastTick, 0.0, 250.0) / 1000;
        if (reset) {
            live = false;
            observed = false;
            quietSince = -1;
            unresolved = false;
        }
        if (advance) {
            live = observed;
            observed = true;
            lastAdvance = now;
            if (live && s.audio.clipped)
                clipUntil = now + 1000;
            if (live && s.audio.inputPeak == 0) {
                if (quietSince < 0)
                    quietSince = now;
            } else
                quietSince = -1;
        }
        if (observed && !reset && static_cast<std::uint32_t>(s.rejections - rejected) != 0 &&
            static_cast<std::uint32_t>(s.rejections - rejected) < 0x80000000U)
            unresolved = true;
        // First observation is history, not a newly announced failure.
        if (block == 0 || reset)
            unresolved = false;
        block = s.block;
        rejected = s.rejections;
        drops = s.droppedSnapshots;
        stale = observed && now - lastAdvance >= 250;
        for (int c = 0; c < 4; ++c) {
            const float p = c < 2 ? s.audio.inputChannelPeak[static_cast<std::size_t>(c)]
                                  : s.audio.outputChannelPeak[static_cast<std::size_t>(c - 2)];
            if (advance)
                meterSilent[static_cast<std::size_t>(c)] = p == 0;
            const auto peak = 20 * std::log10(std::max(p, .001F));
            meterDb[static_cast<std::size_t>(c)] =
                live && !stale && meterValid
                    ? std::max(advance ? peak : -60.F,
                               meterDb[static_cast<std::size_t>(c)] - static_cast<float>(20 * dt))
                    : -60;
        }
        lastTick = now;
    }
    juce::String health() const {
        if (!live)
            return "TELEMETRY UNKNOWN";
        if (stale)
            return "TELEMETRY STALE";
        return drops ? "DISPLAY DROPS " + juce::String(drops) : "TELEMETRY LIVE";
    }
    bool available() const {
        return live && !stale;
    }
};
} // namespace sys01::view
