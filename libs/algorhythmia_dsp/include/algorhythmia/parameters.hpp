#pragma once

#include <cstdint>

namespace algorhythmia {
enum class ParameterId : std::uint8_t { Flux, Bias, Drive, State };
enum class SceneId : std::uint8_t { Bytebeat, CellularFm, LiveTransform };

inline constexpr std::uint8_t fluxMidiCc = 20;
inline constexpr std::uint8_t biasMidiCc = 21;
inline constexpr std::uint8_t driveMidiCc = 22;
inline constexpr std::uint8_t stateMidiCc = 23;
} // namespace algorhythmia
