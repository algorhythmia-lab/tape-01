#pragma once

#include <concepts>
#include <cstdint>

namespace algorhythmia {
struct StereoFrame {
    float left = 0.0F;
    float right = 0.0F;
};

// audio-owned controls; continuous values are finite and normalized.
struct SceneControls {
    float flux = 0.5F;
    float bias = 0.5F;
    float state = 0.0F;
    bool running = false;
    bool brake = false;
};

// static dispatch: no virtual calls, heap ownership, or sdk types.
template <typename T>
concept Scene = requires(T scene, float rate, StereoFrame input, SceneControls controls) {
    { scene.init(rate) } noexcept -> std::same_as<void>;
    { scene.process(input, controls) } noexcept -> std::same_as<StereoFrame>;
    { scene.telemetry() } noexcept -> std::same_as<std::uint8_t>;
};
} // namespace algorhythmia
