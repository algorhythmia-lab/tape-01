#pragma once
#include "algorhythmia/host/catalogue.hpp"

#include <array>
#include <cstdint>
#include <span>

namespace algorhythmia::host {
// Little-endian header + IEEE754 values + CRC32. No native struct serialization.
inline constexpr std::size_t stateBytes = 12 + 4 * count;
using StateBytes = std::array<std::uint8_t, stateBytes>;
enum class DecodeResult { Ok, Size, Magic, Version, Checksum, Values };
[[nodiscard]] StateBytes encode(const State&) noexcept;
// Never modifies destination on error; bounded input, no allocation.
[[nodiscard]] DecodeResult decode(std::span<const std::uint8_t>, State&) noexcept;
} // namespace algorhythmia::host
