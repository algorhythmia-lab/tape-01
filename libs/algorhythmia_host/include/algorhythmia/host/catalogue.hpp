#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <string_view>

namespace algorhythmia::host {
// Append only. IDs, order and normalized meanings are part of schema 1.
enum class Id : unsigned { Flux, Bias, Drive, State, Domain, Output, Bypass, Count };
struct Parameter {
    Id id;
    std::string_view key;
    std::string_view label;
    float initial;
    unsigned steps; // zero = continuous
};
inline constexpr std::array catalogue{
    Parameter{Id::Flux, "param_flux", "FLUX", 0.5F, 0},
    Parameter{Id::Bias, "param_bias", "BIAS", 0.5F, 0},
    Parameter{Id::Drive, "transport_drive", "DRIVE", 0.0F, 2},
    Parameter{Id::State, "subregister_state", "STATE", 0.0F, 2},
    Parameter{Id::Domain, "engine_state", "DOMAIN", 0.0F, 3},
    Parameter{Id::Output, "output_trim", "OUTPUT", 0.75F, 0}, // -60..+20 dB, default 0 dB
    Parameter{Id::Bypass, "bypass", "BYPASS", 0.0F, 2}};
inline constexpr std::size_t count = catalogue.size();
constexpr std::size_t index(Id id) noexcept {
    return static_cast<std::size_t>(id);
}
static_assert(count == static_cast<std::size_t>(Id::Count));
static_assert([] {
    for (std::size_t i = 0; i < count; ++i) {
        if (index(catalogue[i].id) != i || catalogue[i].key.empty() || catalogue[i].initial < 0 ||
            catalogue[i].initial > 1)
            return false;
        for (std::size_t j = 0; j < i; ++j)
            if (catalogue[i].key == catalogue[j].key)
                return false;
    }
    return true;
}());
constexpr float trimDb(float value) noexcept {
    return -60.0F + 80.0F * value;
}
inline bool valid(Id id, float value) noexcept {
    if (index(id) >= count || !std::isfinite(value) || value < 0 || value > 1)
        return false;
    const auto steps = catalogue[index(id)].steps;
    return steps == 0 || value * static_cast<float>(steps - 1) ==
                             std::round(value * static_cast<float>(steps - 1));
}
struct State {
    std::array<float, count> values{};
    constexpr State() noexcept {
        for (const auto& p : catalogue)
            values[index(p.id)] = p.initial;
    }
    bool operator==(const State&) const = default;
};
inline bool valid(const State& state) noexcept {
    for (const auto& p : catalogue)
        if (!valid(p.id, state.values[index(p.id)]))
            return false;
    return true;
}
struct Preset {
    std::string_view name;
    State state;
};
inline constexpr auto presets = [] {
    std::array<Preset, 4> result{{{"initial / paused", {}},
                                  {"01 / magnetic pulse", {}},
                                  {"02 / cellular walls", {}},
                                  {"03 / forward tape", {}}}};
    for (std::size_t i = 1; i < result.size(); ++i) {
        auto& v = result[i].state.values;
        v[index(Id::Drive)] = 1.0F;
        v[index(Id::Domain)] = static_cast<float>(i - 1) / 2.0F;
    }
    result[1].state.values[index(Id::Bias)] = 0.22F;
    result[2].state.values[index(Id::Bias)] = 0.38F;
    result[3].state.values[index(Id::Flux)] = 0.75F;
    result[3].state.values[index(Id::Bias)] = 0.2F;
    return result;
}();
} // namespace algorhythmia::host
