#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>

namespace algorhythmia::config {
inline constexpr float sampleRate = 48'000.0F;
inline constexpr float minimumSampleRate = 8'000.0F;
inline constexpr float maximumSampleRate = 192'000.0F;
inline constexpr float smoothingSeconds = 0.010F;
inline constexpr std::size_t blockFrames = 64;
inline constexpr std::size_t channels = 2;
inline constexpr std::size_t delayFrames = 32'768;
inline constexpr std::size_t audioBufferBudgetBytes = std::size_t{256} * 1024;
inline constexpr std::size_t maximumAudioWorkingSetBytes = std::size_t{384} * 1024;
inline constexpr std::size_t plannedAudioBufferBytes = delayFrames * channels * sizeof(float);
static_assert(plannedAudioBufferBytes <= audioBufferBudgetBytes);
static_assert(sizeof(float) == 4 && std::numeric_limits<float>::is_iec559,
              "DSP requires IEEE-754 binary32 float");
} // namespace algorhythmia::config
