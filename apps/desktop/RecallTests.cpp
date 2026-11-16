#include "PluginProcessor.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace {
namespace contract = algorhythmia::host;
void require(bool ok, const char* message) {
    if (!ok)
        throw std::runtime_error(message);
}
void input(juce::AudioBuffer<float>& b, bool signal, unsigned offset = 0) {
    for (int c = 0; c < 2; ++c)
        for (int i = 0; i < b.getNumSamples(); ++i)
            b.setSample(c, i,
                        signal ? .12589254F * std::sin(static_cast<float>(i + offset) *
                                                       (c == 0 ? .031F : .047F))
                               : 0);
}
void render(sys01::PluginProcessor& p, juce::AudioBuffer<float>& b, bool signal) {
    input(b, signal);
    juce::MidiBuffer midi;
    p.processBlock(b, midi);
}
void equal(const juce::AudioBuffer<float>& a, const juce::AudioBuffer<float>& b) {
    for (int c = 0; c < 2; ++c)
        for (int i = 0; i < a.getNumSamples(); ++i)
            require(a.getSample(c, i) == b.getSample(c, i), "recall PCM differs");
}
void silent(const juce::AudioBuffer<float>& b) {
    for (int c = 0; c < 2; ++c)
        for (int i = 0; i < b.getNumSamples(); ++i)
            require(b.getSample(c, i) == 0, "old tape or generator leaked through recall");
}
} // namespace
int main(int argc, char** argv) {
    try {
        juce::ScopedJuceInitialiser_GUI juce;
        const auto folder = argc == 2 ? std::filesystem::path(argv[1]) : std::filesystem::path{};
        if (!folder.empty())
            std::filesystem::create_directories(folder);
        juce::AudioBuffer<float> actual(2, 4096), expected(2, 4096);
        unsigned reopens = 0;
        for (unsigned program = 0; program < contract::presets.size(); ++program) {
            sys01::PluginProcessor original;
            original.prepareToPlay(48000, 4096);
            original.setCurrentProgram(static_cast<int>(program));
            for (unsigned i = 0; i < 24; ++i)
                render(original, actual, true); // save a sounding, populated instance
            juce::MemoryBlock saved;
            original.getStateInformation(saved);
            const auto canonical = contract::encode(contract::presets[program].state);
            require(saved.getSize() == 40 &&
                        std::memcmp(saved.getData(), canonical.data(), 40) == 0,
                    "saved state contains transient audio or changed program parameters");
            if (!folder.empty()) {
                const auto path = folder / ("program-" + std::to_string(program) + ".sys1");
                std::ofstream out(path, std::ios::binary);
                out.write(static_cast<const char*>(saved.getData()), 40);
                out.close();
                std::ifstream in(path, std::ios::binary);
                std::array<char, 40> disk{};
                in.read(disk.data(), disk.size());
                require(in.gcount() == 40 && std::memcmp(disk.data(), saved.getData(), 40) == 0,
                        "disk state round trip failed");
                saved.replaceAll(disk.data(), disk.size());
            }
            for (float rate : {44100.F, 48000.F, 88200.F, 96000.F, 176400.F, 192000.F}) {
                for (unsigned reopen = 0; reopen < 3; ++reopen) {
                    sys01::PluginProcessor p;
                    if (reopen % 2 == 0)
                        p.setStateInformation(saved.getData(), 40);
                    p.prepareToPlay(rate, 4096);
                    if (reopen % 2 != 0)
                        p.setStateInformation(saved.getData(), 40);
                    contract::Processor reference;
                    reference.prepare(rate);
                    require(reference.restore(contract::presets[program].state), "invalid preset");
                    for (unsigned block = 0; block < 3; ++block) {
                        input(actual, program == 3);
                        input(expected, program == 3);
                        require(reference.process(
                                    expected.getReadPointer(0), expected.getReadPointer(1),
                                    expected.getWritePointer(0), expected.getWritePointer(1), 4096),
                                "reference failed");
                        juce::MidiBuffer midi;
                        p.processBlock(actual, midi);
                        equal(actual, expected);
                    }
                    require(p.controls() == contract::presets[program].state,
                            "reopen changed parameters");
                    juce::MemoryBlock again;
                    p.getStateInformation(again);
                    require(again == saved, "repeated save is not canonical");
                    // Same instance changes sample rate: targets survive, tape/phases reset.
                    const auto nextRate = rate == 48000 ? 96000.F : 48000.F;
                    p.prepareToPlay(nextRate, 4096);
                    reference.prepare(nextRate);
                    input(actual, false);
                    input(expected, false);
                    juce::MidiBuffer midi;
                    p.processBlock(actual, midi);
                    require(reference.process(nullptr, nullptr, expected.getWritePointer(0),
                                              expected.getWritePointer(1), 4096),
                            "rate oracle failed");
                    equal(actual, expected);
                    ++reopens;
                }
            }
        }
        for (const int program : {1, 2}) {
            sys01::PluginProcessor disabled;
            auto layout = disabled.getBusesLayout();
            layout.inputBuses.set(0, juce::AudioChannelSet::disabled());
            require(disabled.setBusesLayout(layout), "input-disabled stereo layout rejected");
            disabled.setCurrentProgram(program);
            disabled.prepareToPlay(48000, 4096);
            render(disabled, actual, false);
            require(actual.getMagnitude(0, 4096) > 0, "input-disabled generator failed to sound");
        }
        // Malformed/future blobs must preserve the running history as well as controls.
        sys01::PluginProcessor tested, control;
        for (auto* p : {&tested, &control}) {
            p->prepareToPlay(48000, 4096);
            p->setCurrentProgram(3);
        }
        for (unsigned i = 0; i < 48; ++i) {
            auto bytes = contract::encode(contract::presets[3].state);
            if (i < 40)
                bytes[i] ^= 0x80;
            else if (i < 42)
                bytes[4] = i == 40 ? 0 : 2;
            else {
                auto invalid = contract::presets[3].state;
                invalid.values[0] = std::numeric_limits<float>::quiet_NaN();
                bytes = contract::encode(invalid); // valid CRC, invalid value
            }
            tested.setStateInformation(bytes.data(), i == 47 ? 39 : 40);
            require(tested.controls() == control.controls(), "invalid blob changed parameters");
            render(tested, actual, true);
            render(control, expected, true);
            equal(actual, expected);
        }
        // Performance CC modifies sound, never declared saved host parameters.
        juce::MemoryBlock before, after;
        tested.getStateInformation(before);
        juce::MidiBuffer midi;
        midi.addEvent(juce::MidiMessage::controllerEvent(1, 20, 0), 13);
        midi.addEvent(juce::MidiMessage::controllerEvent(1, 23, 0), 17);
        input(actual, false);
        tested.processBlock(actual, midi);
        tested.getStateInformation(after);
        require(before == after, "performance MIDI contaminated saved parameters");
        // Recall transform with silence after recording: empty tape must stay exactly empty.
        tested.setCurrentProgram(3);
        render(tested, actual, false);
        silent(actual);
        tested.setCurrentProgram(0);
        render(tested, actual, false);
        silent(actual);
        std::cout << "PASS: " << reopens
                  << " native adapter reopens (4 programs x 6 rates x 3 cycles), disk 40-byte "
                     "states, before/after prepare, exact PCM, rate reset, 48 malformed/future "
                     "cases preserve running history, input-disabled generation, MIDI exclusion "
                     "and empty tape/reset. "
                     "This fixture is not a DAW project test.\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
