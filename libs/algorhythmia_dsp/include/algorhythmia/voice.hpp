#pragma once

namespace algorhythmia {
// bounded two-operator fm voice; phase and event envelopes are audio-owned.
struct VoiceState {
    float carrierPhase = 0.0F;
    float modulatorPhase = 0.0F;
    float envelope = 0.0F;
    float carrierIncrement = 0.0F;
    float modulatorIncrement = 0.0F;
    float panLeft = 0.70710678F;
    float panRight = 0.70710678F;
};
} // namespace algorhythmia
