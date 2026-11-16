#include "PluginProcessor.hpp"

#ifdef SYS01_NATIVE_RT_PROBE
#include "RealtimeProbe.hpp"
#endif
#include <cmath>
#include <cstdio>
#include <thread>

namespace {
void require(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "%s\n", message);
        std::exit(1);
    }
}
void render(sys01::PluginProcessor& p, juce::AudioBuffer<float>& b) {
    juce::MidiBuffer midi;
    b.clear();
    p.processBlock(b, midi);
}
} // namespace
int main(int argc, char** argv) {
    juce::ScopedJuceInitialiser_GUI juce;
    sys01::PluginProcessor original, reopened;
    juce::AudioBuffer<float> first(2, 4096), second(2, 4096);
    original.prepareToPlay(48000, 4096);
    original.setCurrentProgram(1);
    juce::MemoryBlock bytes;
    original.getStateInformation(bytes);
    reopened.setStateInformation(bytes.getData(), static_cast<int>(bytes.getSize()));
    reopened.prepareToPlay(48000, 4096);
    render(original, first);
    render(reopened, second);
    for (int c = 0; c < 2; ++c)
        for (int i = 0; i < 4096; ++i)
            require(first.getSample(c, i) == second.getSample(c, i),
                    "session reopen differs from deterministic recall");
    require(original.outputPeak() > 0, "generator must sound with silent input");
    require(original.controls() == reopened.controls(), "session controls differ");
    bytes[0] ^= 1;
    const auto state = reopened.controls();
    reopened.setStateInformation(bytes.getData(), static_cast<int>(bytes.getSize()));
    require(reopened.controls() == state, "malformed session changed controls");
    // Sample-positioned MIDI is compared with a separate shared processor.
    sys01::PluginProcessor midiPlugin;
    midiPlugin.prepareToPlay(48000, 4096);
    juce::MidiBuffer midi;
    midi.addEvent(juce::MidiMessage::controllerEvent(1, 22, 127), 123);
    midi.addEvent(juce::MidiMessage::controllerEvent(1, 21, 101), 700);
    first.clear();
    midiPlugin.processBlock(first, midi);
    algorhythmia::host::Processor reference;
    reference.prepare(48000);
    constexpr std::array events{
        algorhythmia::host::Event{123, algorhythmia::host::EventKind::Parameter,
                                  algorhythmia::host::Id::Drive, 1},
        algorhythmia::host::Event{700, algorhythmia::host::EventKind::Parameter,
                                  algorhythmia::host::Id::Bias, 101.F / 127.F}};
    require(reference.process(nullptr, nullptr, second.getWritePointer(0),
                              second.getWritePointer(1), 4096, events),
            "reference rejected valid events");
    for (int c = 0; c < 2; ++c)
        for (int i = 0; i < 4096; ++i)
            require(first.getSample(c, i) == second.getSample(c, i),
                    "MIDI offsets were not preserved");
    for (float rate : {44100.F, 48000.F, 88200.F, 96000.F, 176400.F, 192000.F}) {
        reopened.prepareToPlay(rate, 4096);
        for (int size : {0, 1, 7, 63, 64, 65, 511, 4096}) {
            first.setSize(2, size);
            render(reopened, first);
            for (int c = 0; c < 2; ++c)
                for (int i = 0; i < size; ++i)
                    require(std::isfinite(first.getSample(c, i)), "invalid callback sample");
        }
    }
    if (argc == 2) {
        std::unique_ptr<juce::AudioProcessorEditor> editor(original.createEditor());
        for (const int percent : {100, 125, 150, 200}) {
            editor->setSize(640 * percent / 100, 440 * percent / 100);
            const auto image = editor->createComponentSnapshot(editor->getLocalBounds());
            juce::File file(juce::String(argv[1]) + "/sys01-" + juce::String(percent) + ".png");
            auto stream = file.createOutputStream();
            require(stream != nullptr, "unable to write editor snapshot");
            stream->setPosition(0);
            stream->truncate();
            require(juce::PNGImageFormat{}.writeImageToStream(image, *stream),
                    "snapshot encoding failed");
        }
    }
    // Eight-slot restore transaction boundary; ninth and zero callbacks must
    // leave controls/queues intact. The newest captured restore resets once.
    sys01::PluginProcessor queued;
    queued.prepareToPlay(48000, 64);
    auto recall = algorhythmia::host::presets[1].state;
    for (unsigned i = 0; i < 8; ++i) {
        recall.values[0] = static_cast<float>(i) / 8;
        require(queued.restore(recall), "eight restore slots did not fit");
    }
    const auto last = queued.controls();
    require(!queued.restore(algorhythmia::host::presets[2].state) && queued.controls() == last,
            "full restore queue mutated controls");
    first.setSize(2, 0);
    render(queued, first);
    require(!queued.restore(algorhythmia::host::presets[2].state), "zero block consumed restore");
    first.setSize(2, 64);
    render(queued, first);
    require(queued.restore(last), "restore boundary did not recover");
    // Per-channel telemetry must reflect asymmetric input and output, including
    // in-place buffers. Full mailbox drops snapshots instead of overwriting.
    sys01::PluginProcessor metered;
    auto dry = algorhythmia::host::State{};
    dry.values[algorhythmia::host::index(algorhythmia::host::Id::Bypass)] = 1;
    require(metered.restore(dry), "dry telemetry setup failed");
    metered.prepareToPlay(48000, 64);
    for (int i = 0; i < 64; ++i) {
        first.setSample(0, i, 0.25F);
        first.setSample(1, i, -0.75F);
    }
    juce::MidiBuffer emptyMidi;
#ifdef SYS01_NATIVE_RT_PROBE
    sys01::probe::selfTest();
    sys01::probe::begin();
#endif
    metered.processBlock(first, emptyMidi);
#ifdef SYS01_NATIVE_RT_PROBE
    const auto leaf = sys01::probe::end();
    require(leaf.allocation == 0 && leaf.deallocation == 0 && leaf.mutex == 0 && leaf.unfair == 0,
            "controlled callback allocated or locked");
#endif
    const auto snapshot = metered.readTelemetry();
    require(snapshot.block == 1 && snapshot.frames == 64 &&
                snapshot.audio.inputChannelPeak == std::array<float, 2>{0.25F, 0.75F} &&
                snapshot.audio.outputChannelPeak == std::array<float, 2>{0.25F, 0.75F},
            "channel telemetry is not actual channel data");
    for (int i = 0; i < 20; ++i)
        render(metered, first);
    require(metered.readTelemetry().block == 9, "full telemetry mailbox overwrote immutable slots");
    render(metered, first);
    require(metered.readTelemetry().droppedSnapshots == 12, "telemetry drops are not reported");
    // UI brake edges remain queued across a rejected merged batch. A later
    // accepted batch releases brake; zero callbacks cannot consume the edges.
    require(metered.button(true, true) && metered.button(true, false), "UI edges did not fit");
    metered.beginNativeBlock(64);
    for (int i = 0; i < 256; ++i)
        require(metered.nativePoint(0, i % 64, 0.7F), "native maximum did not fit");
    render(metered, first);
    require(metered.readTelemetry().audio.rejected, "UI/native overflow was not rejected");
    render(metered, first);
    require(!metered.readTelemetry().audio.engine.brake, "retained brake release was lost");
    // Malformed MIDI bytes reject the entire batch and count once, rather than
    // letting core rejection consume queued controls. Valid edit recovers.
    const juce::uint8 hostile[]{0xb0, 21, 255};
    midi.addEvent(hostile, 3, 0);
    require(metered.button(true, true), "UI recovery setup failed");
    metered.processBlock(first, midi);
    require(metered.readTelemetry().audio.rejected, "malformed MIDI was not rejected");
    render(metered, first);
    require(metered.readTelemetry().audio.engine.brake, "rejected batch consumed UI edge");
    // Exercise framework lifecycle exclusion, editor attach/teardown listeners,
    // telemetry reader and restore producer concurrently with the audio owner.
    std::atomic<bool> audioDone{false};
    std::thread audioOwner([&] {
        juce::AudioBuffer<float> audioBuffer(2, 64);
        for (int i = 0; i < 2000; ++i) {
            const juce::ScopedLock lock(reopened.getCallbackLock());
            if (!reopened.isSuspended())
                render(reopened, audioBuffer);
        }
        audioDone.store(true);
    });
    for (int i = 0; i < 20; ++i) {
        {
            std::unique_ptr<juce::AudioProcessorEditor> editor(reopened.createEditor());
            editor->setSize(640, 440);
            static_cast<void>(reopened.readTelemetry());
        }
        const juce::ScopedLock lock(reopened.getCallbackLock());
        reopened.suspendProcessing(true);
        reopened.releaseResources();
        reopened.prepareToPlay(48000 + (i % 2) * 48000, 64);
        reopened.suspendProcessing(false);
    }
    while (!audioDone.load())
        static_cast<void>(reopened.readTelemetry());
    audioOwner.join();
    // Restore producer races the callback. Queue-full is an explicit rejection;
    // retry outside audio, never overwrite unconsumed transactions.
    std::atomic<bool> running{true};
    std::thread producer([&] {
        while (running.load()) {
            static_cast<void>(reopened.restore(algorhythmia::host::presets[1].state));
            static_cast<void>(reopened.restore(algorhythmia::host::presets[2].state));
        }
    });
    for (int i = 0; i < 200; ++i)
        render(reopened, first);
    running.store(false);
    producer.join();
    std::puts("desktop adapter: session recall, invalid states, positioned MIDI, rates/blocks and "
              "concurrent restore passed");
}
