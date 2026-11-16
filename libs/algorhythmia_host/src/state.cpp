#include "algorhythmia/host/state.hpp"

#include <bit>

namespace algorhythmia::host {
namespace {
void put(StateBytes& b, std::size_t offset, std::uint32_t value) noexcept {
    for (unsigned i = 0; i < 4; ++i)
        b[offset + i] = static_cast<std::uint8_t>(value >> (8 * i));
}
std::uint32_t get(std::span<const std::uint8_t> b, std::size_t offset) noexcept {
    std::uint32_t result = 0;
    for (unsigned i = 0; i < 4; ++i)
        result |= static_cast<std::uint32_t>(b[offset + i]) << (8 * i);
    return result;
}
std::uint32_t crc(std::span<const std::uint8_t> bytes) noexcept {
    std::uint32_t value = 0xffffffffU;
    for (auto byte : bytes) {
        value ^= byte;
        for (unsigned i = 0; i < 8; ++i)
            value = (value >> 1) ^ ((value & 1) ? 0xedb88320U : 0U);
    }
    return ~value;
}
} // namespace
StateBytes encode(const State& state) noexcept {
    StateBytes b{};
    put(b, 0, 0x31535953U); // SYS1
    put(b, 4, 1U);
    for (std::size_t i = 0; i < count; ++i)
        put(b, 8 + 4 * i, std::bit_cast<std::uint32_t>(state.values[i]));
    put(b, stateBytes - 4, crc(std::span(b).first(stateBytes - 4)));
    return b;
}
DecodeResult decode(std::span<const std::uint8_t> b, State& destination) noexcept {
    // There is no released earlier schema. v0 fixtures must be rejected, never guessed.
    if (b.size() != stateBytes)
        return DecodeResult::Size;
    if (get(b, 0) != 0x31535953U)
        return DecodeResult::Magic;
    if (get(b, 4) != 1U)
        return DecodeResult::Version;
    if (get(b, stateBytes - 4) != crc(b.first(stateBytes - 4)))
        return DecodeResult::Checksum;
    State candidate;
    for (std::size_t i = 0; i < count; ++i)
        candidate.values[i] = std::bit_cast<float>(get(b, 8 + 4 * i));
    if (!valid(candidate))
        return DecodeResult::Values;
    destination = candidate;
    return DecodeResult::Ok;
}
} // namespace algorhythmia::host
