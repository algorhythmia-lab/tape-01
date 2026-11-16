#pragma once

#include "algorhythmia/config.hpp"
#include "algorhythmia/engine.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace algorhythmia::firmware {

// This transport contract does not choose an ADC, clock tree, PIO program or pins.
inline constexpr auto sampleRate = static_cast<std::uint32_t>(config::sampleRate);
inline constexpr std::size_t channels = config::channels;
inline constexpr std::size_t blockFrames = config::blockFrames;
inline constexpr std::size_t audioStorageBudgetBytes = config::audioBufferBudgetBytes;
inline constexpr std::size_t engineBudgetBytes = audioStorageBudgetBytes + std::size_t{64} * 1024;
inline constexpr std::size_t totalAudioBudgetBytes = config::maximumAudioWorkingSetBytes;

// one stereo float tape is owned by engine; freeze reuses its captured history.

using FloatBlock = std::array<float, blockFrames>;
using TransportBlock = std::array<std::int32_t, blockFrames * channels>;

struct AudioIo {
    FloatBlock inputL{};
    FloatBlock inputR{};
    FloatBlock outputL{};
    FloatBlock outputR{};
    // Two buffers per direction. 24-bit serial samples will occupy 32-bit words;
    // bit alignment, conversion and DMA ownership await the real board design.
    std::array<TransportBlock, 2> receive{};
    std::array<TransportBlock, 2> transmit{};
};

struct FirmwareState {
    Engine engine{};
    AudioIo io{};
};

static_assert(sizeof(float) == 4);
static_assert(config::delayFrames * channels * sizeof(float) == audioStorageBudgetBytes);
static_assert(sizeof(AudioIo) == std::size_t{3} * 1024);
static_assert(sizeof(Engine) <= engineBudgetBytes, "Review the Engine RAM reservation");
static_assert(sizeof(FirmwareState) <= totalAudioBudgetBytes,
              "Engine, I/O and history exceed the planned audio RAM budget");
static_assert(sampleRate == 48'000 && channels == 2 && blockFrames == 64);

} // namespace algorhythmia::firmware
