#include "audio_contract.hpp"

#include <cmath>
#include <cstdio>

namespace {
// The contract probe also enforces static lifetime, as required on the device.
algorhythmia::firmware::FirmwareState state;
} // namespace

int main() {
    using namespace algorhythmia;
    using namespace algorhythmia::firmware;
    state.engine.init(static_cast<float>(sampleRate));
    state.engine.setScene(SceneId::Bytebeat);
    state.engine.setParameter(ParameterId::Drive, 1.0F);
    state.engine.process(state.io.inputL.data(), state.io.inputR.data(), state.io.outputL.data(),
                         state.io.outputR.data(), blockFrames);

    for (std::size_t frame = 0; frame < blockFrames; ++frame) {
        if (!std::isfinite(state.io.outputL[frame]) || !std::isfinite(state.io.outputR[frame])) {
            return 1;
        }
    }
    std::printf("48 kHz stereo / 64 frames: history=%zu, engine=%zu, I/O=%zu, total=%zu bytes\n",
                audioStorageBudgetBytes, sizeof(Engine), sizeof(AudioIo), sizeof(FirmwareState));
    return 0;
}
