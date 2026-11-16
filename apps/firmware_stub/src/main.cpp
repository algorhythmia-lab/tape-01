#include "audio_contract.hpp"
#include "pico/stdlib.h"

#if !defined(PICO_RP2350) || !defined(__ARM_ARCH_8M_MAIN__)
#error "This scaffold requires the RP2350 Cortex-M33 target"
#endif

namespace {
algorhythmia::firmware::FirmwareState state;
volatile float lastSample = 0.0F;
} // namespace

int main() {
    using namespace algorhythmia;
    using namespace algorhythmia::firmware;
    state.engine.init(static_cast<float>(sampleRate));
    state.engine.setScene(SceneId::Bytebeat);
    state.engine.setParameter(ParameterId::Drive, 1.0F);

    // Compile/link probe only: no codec, PIO, DMA, sample clock or audio output.
    // Hardware integration will invoke this exact process call on owned DMA
    // blocks. Never fabricate pin assignments from the reference Pico 2 board.
    state.engine.process(state.io.inputL.data(), state.io.inputR.data(), state.io.outputL.data(),
                         state.io.outputR.data(), blockFrames);
    lastSample = state.io.outputL.back();
    while (true) {
        tight_loop_contents();
    }
}
