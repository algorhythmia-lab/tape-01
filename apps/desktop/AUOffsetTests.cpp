// Independent native AU timing characterization. Strict mode enforces ADR 0002.
#include "algorhythmia/host/processor.hpp"
#include "algorhythmia/host/state.hpp"

#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <string>
#include <thread>
namespace {
void require(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "%s\n", message);
        std::exit(1);
    }
}
struct InputFixture {
    bool enabled = false;
};
float inputSample(unsigned channel, unsigned frame) {
    return channel == 0 ? float(int(frame % 97) - 48) / 64 : float(int(frame % 71) - 35) / 48;
}
OSStatus input(void* context, AudioUnitRenderActionFlags*, const AudioTimeStamp*, UInt32, UInt32,
               AudioBufferList* buffers) {
    for (UInt32 i = 0; i < buffers->mNumberBuffers; ++i) {
        auto* data = static_cast<float*>(buffers->mBuffers[i].mData);
        for (unsigned frame = 0; frame < buffers->mBuffers[i].mDataByteSize / 4; ++frame)
            data[frame] = context && static_cast<InputFixture*>(context)->enabled
                              ? inputSample(i, frame)
                              : 0.F;
    }
    return noErr;
}
} // namespace
int main(int argc, char** argv) {
    require(argc == 2 || argc == 3, "supply AU bundle [--require-positioned]");
    const bool ownership = argc == 3 && std::strcmp(argv[2], "--require-ownership") == 0;
    const bool ownershipV3 = argc == 3 && std::strcmp(argv[2], "--require-ownership-v3") == 0;
    const bool strict = argc == 3 && !ownership && !ownershipV3;
    require(!strict || std::strcmp(argv[2], "--require-positioned") == 0, "unknown option");
    auto url = CFURLCreateFromFileSystemRepresentation(
        nullptr, reinterpret_cast<const UInt8*>(argv[1]), std::strlen(argv[1]), true);
    auto bundle = CFBundleCreate(nullptr, url);
    CFRelease(url);
    require(bundle && CFBundleLoadExecutable(bundle), "AU bundle load failed");
    const auto factory = reinterpret_cast<AudioComponentFactoryFunction>(
        CFBundleGetFunctionPointerForName(bundle, CFSTR("JuceAUFactory")));
    AudioComponentDescription desc{kAudioUnitType_MusicEffect, 'Sys1', 'Algr', 0, 0};
    const auto component =
        AudioComponentRegister(&desc, CFSTR("SYS 01 offset fixture"), 0x10000, factory);
    require(component != nullptr, "AU factory registration failed");
    AudioUnit unit = nullptr;
    require(AudioComponentInstanceNew(component, &unit) == noErr, "AU instantiate failed");
    constexpr unsigned frames = 4096;
    AudioStreamBasicDescription format{48000,
                                       kAudioFormatLinearPCM,
                                       static_cast<UInt32>(kAudioFormatFlagsNativeFloatPacked) |
                                           kAudioFormatFlagIsNonInterleaved,
                                       4,
                                       1,
                                       4,
                                       2,
                                       32,
                                       0};
    for (auto scope : {kAudioUnitScope_Input, kAudioUnitScope_Output})
        require(AudioUnitSetProperty(unit, kAudioUnitProperty_StreamFormat, scope, 0, &format,
                                     sizeof(format)) == noErr,
                "AU stereo format failed");
    UInt32 maxFrames = frames;
    require(AudioUnitSetProperty(unit, kAudioUnitProperty_MaximumFramesPerSlice,
                                 kAudioUnitScope_Global, 0, &maxFrames, sizeof(maxFrames)) == noErr,
            "AU max frames failed");
    InputFixture inputFixture;
    AURenderCallbackStruct callback{input, &inputFixture};
    require(AudioUnitSetProperty(unit, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Input,
                                 0, &callback, sizeof(callback)) == noErr,
            "AU input failed");
    using namespace algorhythmia::host;
    // Literal stable IDs; Both wrappers use the Studio One compatible masked JUCE hash.
    constexpr std::array<AudioUnitParameterID, 7> ids{
        452784731U, 452662059U, 1763717364U, 834166325U, 1194780756U, 1323076768U, 773352680U};
    for (std::size_t i = 0; i < ids.size(); ++i) {
        AudioUnitParameterInfo info{};
        UInt32 bytes = sizeof(info);
        require(AudioUnitGetProperty(unit, kAudioUnitProperty_ParameterInfo, kAudioUnitScope_Global,
                                     ids[i], &info, &bytes) == noErr,
                "stable AU ID missing");
        require(std::strcmp(info.name, std::string(catalogue[i].label).c_str()) == 0,
                "AU ID/name changed");
        std::printf("AU ID %s = %u; max=%g\n", info.name, ids[i], info.maxValue);
        if ((info.flags & kAudioUnitParameterFlag_HasCFNameString) &&
            (info.flags & kAudioUnitParameterFlag_CFNameRelease))
            CFRelease(info.cfNameString);
    }
    std::array<float, frames> inputL{}, inputR{};
    for (unsigned i = 0; i < frames; ++i) {
        inputL[i] = inputSample(0, i);
        inputR[i] = inputSample(1, i);
    }
    std::array<float, frames> left{}, right{}, expectedL{}, expectedR{}, boundaryL{}, boundaryR{};
    struct Stereo {
        UInt32 count;
        AudioBuffer buffers[2];
    } buffers{2, {{1, sizeof(left), left.data()}, {1, sizeof(right), right.data()}}};
    AudioTimeStamp timestamp{};
    timestamp.mFlags = kAudioTimeStampSampleTimeValid;
    if (ownershipV3) {
        using Hook = void (*)(void (*)(void*, void*), void*);
        using Write = void (*)(void*, int, float);
        using Read = void (*)(void*, float*, float*);
        using Listener = void (*)(void*, void (*)(void*, void*, int, float), void*);
        const auto before = reinterpret_cast<Hook>(
            CFBundleGetFunctionPointerForName(bundle, CFSTR("Sys01AUFixtureBeforeCaptureHook")));
        const auto capture = reinterpret_cast<Hook>(
            CFBundleGetFunctionPointerForName(bundle, CFSTR("Sys01AUFixtureCaptureHook")));
        const auto write = reinterpret_cast<Write>(
            CFBundleGetFunctionPointerForName(bundle, CFSTR("Sys01AUFixtureEditorWrite")));
        const auto read = reinterpret_cast<Read>(
            CFBundleGetFunctionPointerForName(bundle, CFSTR("Sys01AUFixtureRead")));
        const auto listener = reinterpret_cast<Listener>(
            CFBundleGetFunctionPointerForName(bundle, CFSTR("Sys01AUFixtureListener")));
        require(before && capture && write && read && listener, "session03 fixture hooks absent");
        struct Context {
            Write write;
            Read read;
            Listener listener;
            AudioUnit unit;
            void* processor = nullptr;
            bool fired = false, coherent = true;
            bool reentrant = false;
            State expected;
            unsigned observations = 0;
            void observe(void* p) {
                constexpr std::array<AudioUnitParameterID, 7> ids{
                    452784731U,  452662059U,  1763717364U, 834166325U,
                    1194780756U, 1323076768U, 773352680U};
                std::array<float, 7> values{}, mirrors{};
                read(p, values.data(), mirrors.data());
                bool authoritative = true, raw = true, host = true;
                for (std::size_t i = 0; i < ids.size(); ++i) {
                    authoritative &= values[i] == expected.values[i];
                    const float max = catalogue[i].steps > 0 ? float(catalogue[i].steps - 1) : 1.F;
                    raw &= mirrors[i] == expected.values[i] * max;
                    float value = -1;
                    require(AudioUnitGetParameter(unit, ids[i], kAudioUnitScope_Global, 0,
                                                  &value) == noErr,
                            "v3 host readback");
                    host &= value == expected.values[i] * max;
                }
                CFPropertyListRef saved = nullptr;
                UInt32 bytes = sizeof(saved);
                require(AudioUnitGetProperty(unit, kAudioUnitProperty_ClassInfo,
                                             kAudioUnitScope_Global, 0, &saved, &bytes) == noErr,
                        "v3 save");
                const auto data = static_cast<CFDataRef>(CFDictionaryGetValue(
                    static_cast<CFDictionaryRef>(saved), CFSTR("jucePluginState")));
                State state;
                require(data && CFDataGetLength(data) == 40 &&
                            decode({CFDataGetBytePtr(data), 40}, state) == DecodeResult::Ok,
                        "v3 schema1 exactly40");
                const bool save = state.values == expected.values;
                std::printf(
                    "S09-OWN-01 %s observation=%u authoritative=%s raw=%s host=%s schema40=%s\n",
                    reentrant ? "reentrant" : "before-capture", observations++,
                    authoritative ? "PASS" : "FAIL", raw ? "PASS" : "FAIL", host ? "PASS" : "FAIL",
                    save ? "PASS" : "FAIL");
                coherent &= authoritative && raw && host && save;
                CFRelease(saved);
            }
        };
        bool all = true;
        for (bool reentrant : {false, true}) {
            State initial;
            initial.values[index(Id::Drive)] = 1;
            for (std::size_t i = 0; i < ids.size(); ++i) {
                const float max = catalogue[i].steps > 0 ? float(catalogue[i].steps - 1) : 1.F;
                require(AudioUnitSetParameter(unit, ids[i], kAudioUnitScope_Global, 0,
                                              initial.values[i] * max, 0) == noErr,
                        "v3 initial setter");
            }
            require(AudioUnitInitialize(unit) == noErr, "v3 initialize");
            Context context{write, read, listener, unit, nullptr, false, true, false, State{}, 0};
            context.reentrant = reentrant;
            context.expected = initial;
            context.expected.values[0] = 0.8F;
            context.expected.values[1] = reentrant ? 0.7F : initial.values[1];
            require(AudioUnitSetParameter(unit, ids[0], kAudioUnitScope_Global, 0, 0.2F, 7) ==
                        noErr,
                    "v3 old flux acceptance");
            if (reentrant)
                require(AudioUnitSetParameter(unit, ids[1], kAudioUnitScope_Global, 0, 0.2F, 9) ==
                            noErr,
                        "v3 old bias acceptance");
            const auto edit = +[](void* c, void* p) {
                auto& context = *static_cast<Context*>(c);
                context.processor = p;
                context.fired = true;
                context.write(p, 0, 0.8F);
                context.observe(p);
            };
            const auto install = +[](void* c, void* p) {
                auto& context = *static_cast<Context*>(c);
                context.processor = p;
                context.listener(
                    p,
                    +[](void* c, void* p, int, float) {
                        auto& context = *static_cast<Context*>(c);
                        if (context.fired)
                            return;
                        context.fired = true; // guard nested notification
                        context.write(p, 0, 0.8F);
                        context.write(p, 1, 0.7F); // invalidate later per-target CAS too
                        context.observe(p);
                    },
                    c);
            };
            if (reentrant)
                capture(install, &context);
            else
                before(edit, &context);
            AudioUnitRenderActionFlags flags = 0;
            require(AudioUnitRender(unit, &flags, &timestamp, 0, frames,
                                    reinterpret_cast<AudioBufferList*>(&buffers)) == noErr,
                    "v3 captured render");
            before(nullptr, nullptr);
            capture(nullptr, nullptr);
            if (reentrant)
                listener(context.processor, nullptr, nullptr);
            require(context.fired, "v3 callback did not fire");
            context.observe(context.processor);
            Processor oracle;
            oracle.prepare(48000);
            require(oracle.restore(initial), "v3 oracle initial");
            const std::array old{Event{7, EventKind::Parameter, Id::Flux, 0.2F},
                                 Event{9, EventKind::Parameter, Id::Bias, 0.2F}};
            require(oracle.process(nullptr, nullptr, expectedL.data(), expectedR.data(), frames,
                                   std::span(old).first(reentrant ? 2 : 1)),
                    "v3 captured oracle");
            bool audio = left == expectedL && right == expectedR;
            for (unsigned block = 0; block < 2; ++block) {
                timestamp.mSampleTime += frames;
                require(AudioUnitRender(unit, &flags, &timestamp, 0, frames,
                                        reinterpret_cast<AudioBufferList*>(&buffers)) == noErr,
                        "v3 following render");
                const std::array newer{Event{0, EventKind::Parameter, Id::Flux, 0.8F},
                                       Event{0, EventKind::Parameter, Id::Bias, 0.7F}};
                require(oracle.process(nullptr, nullptr, expectedL.data(), expectedR.data(), frames,
                                       block == 0
                                           ? std::span<const Event>(newer).first(reentrant ? 2 : 1)
                                           : std::span<const Event>{}),
                        "v3 following oracle");
                audio &= left == expectedL && right == expectedR;
                context.observe(context.processor);
            }
            std::printf("S09-OWN-01 %s captured/following-audio=%s\n",
                        reentrant ? "reentrant" : "before-capture", audio ? "PASS" : "FAIL");
            // Reset owns lifecycle here: no Render or listener is active.
            require(AudioUnitSetParameter(unit, ids[0], kAudioUnitScope_Global, 0, 0.2F, 7) ==
                        noErr,
                    "v3 lifecycle old acceptance");
            context.write(context.processor, 0, 0.8F);
            context.observe(context.processor);
            require(AudioUnitReset(unit, kAudioUnitScope_Global, 0) == noErr, "v3 lifecycle reset");
            context.observe(context.processor);
            timestamp.mSampleTime += frames;
            require(AudioUnitRender(unit, &flags, &timestamp, 0, frames,
                                    reinterpret_cast<AudioBufferList*>(&buffers)) == noErr,
                    "v3 lifecycle following render");
            require(
                oracle.process(nullptr, nullptr, expectedL.data(), expectedR.data(), frames, {}),
                "v3 lifecycle following oracle");
            const bool lifecycleAudio = left == expectedL && right == expectedR;
            std::printf("S09-OWN-01 %s lifecycle-audio=%s\n",
                        reentrant ? "reentrant" : "before-capture",
                        lifecycleAudio ? "PASS" : "FAIL");
            all &= context.coherent && audio && lifecycleAudio;
            require(AudioUnitUninitialize(unit) == noErr, "v3 cleanup");
            timestamp.mSampleTime += frames;
        }
        require(AudioComponentInstanceDispose(unit) == noErr, "v3 dispose");
        CFRelease(bundle);
        return all ? 0 : 1;
    }
    if (ownership) {
        using Hook = void (*)(void (*)(void*, void*), void*);
        using EditorWrite = void (*)(void*, int, float);
        const auto hook = reinterpret_cast<Hook>(
            CFBundleGetFunctionPointerForName(bundle, CFSTR("Sys01AUFixtureCaptureHook")));
        const auto editorWrite = reinterpret_cast<EditorWrite>(
            CFBundleGetFunctionPointerForName(bundle, CFSTR("Sys01AUFixtureEditorWrite")));
        require(hook && editorWrite,
                "ownership fixture hooks absent (default production behavior)");
        struct Barrier {
            std::atomic<int> phase{0};
            void* processor = nullptr;
        } barrier;
        const auto pause = +[](void* context, void* processor) {
            auto& b = *static_cast<Barrier*>(context);
            b.processor = processor;
            b.phase.store(1, std::memory_order_release);
            b.phase.notify_one();
            b.phase.wait(1, std::memory_order_acquire);
        };
        bool all = true;
        for (bool editor : {false, true}) {
            State initial;
            initial.values[index(Id::Drive)] = 1;
            for (std::size_t i = 0; i < ids.size(); ++i) {
                const float max = catalogue[i].steps > 0 ? float(catalogue[i].steps - 1) : 1.F;
                require(AudioUnitSetParameter(unit, ids[i], kAudioUnitScope_Global, 0,
                                              initial.values[i] * max, 0) == noErr,
                        "ownership initial setter");
            }
            require(AudioUnitInitialize(unit) == noErr, "ownership initialize");
            require(AudioUnitSetParameter(unit, ids[0], kAudioUnitScope_Global, 0, 0.8F, 0) ==
                        noErr,
                    "ownership newer target");
            CFPropertyListRef newer = nullptr;
            UInt32 bytes = sizeof(newer);
            require(AudioUnitGetProperty(unit, kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global,
                                         0, &newer, &bytes) == noErr,
                    "ownership newer save");
            require(AudioUnitSetParameter(unit, ids[0], kAudioUnitScope_Global, 0, 0.2F, 7) ==
                        noErr,
                    "ownership old target");
            barrier.phase.store(0);
            hook(pause, &barrier);
            AudioUnitRenderActionFlags flags = 0;
            std::thread audio([&] {
                require(AudioUnitRender(unit, &flags, &timestamp, 0, frames,
                                        reinterpret_cast<AudioBufferList*>(&buffers)) == noErr,
                        "ownership captured render");
            });
            barrier.phase.wait(0, std::memory_order_acquire);
            if (editor)
                editorWrite(barrier.processor, 0, 0.8F);
            else
                require(AudioUnitSetProperty(unit, kAudioUnitProperty_ClassInfo,
                                             kAudioUnitScope_Global, 0, &newer,
                                             sizeof(newer)) == noErr,
                        "ownership concurrent restore");
            barrier.phase.store(2, std::memory_order_release);
            barrier.phase.notify_one();
            audio.join();
            hook(nullptr, nullptr);
            AudioUnitParameterValue value = -1;
            require(AudioUnitGetParameter(unit, ids[0], kAudioUnitScope_Global, 0, &value) == noErr,
                    "ownership readback");
            const bool readback = value == 0.8F;
            CFPropertyListRef saved = nullptr;
            bytes = sizeof(saved);
            require(AudioUnitGetProperty(unit, kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global,
                                         0, &saved, &bytes) == noErr,
                    "ownership save");
            const auto data = static_cast<CFDataRef>(CFDictionaryGetValue(
                static_cast<CFDictionaryRef>(saved), CFSTR("jucePluginState")));
            State state;
            require(data && CFDataGetLength(data) == 40 &&
                        decode({CFDataGetBytePtr(data), 40}, state) == DecodeResult::Ok,
                    "ownership schema changed");
            const bool save = state.values[0] == 0.8F;
            Processor oracle;
            oracle.prepare(48000);
            if (!editor) {
                initial.values[0] = 0.8F;
                require(oracle.restore(initial), "ownership restore oracle");
            } else {
                require(oracle.restore(initial), "ownership editor oracle");
                const std::array old{Event{0, EventKind::Parameter, Id::Flux, 0.8F},
                                     Event{7, EventKind::Parameter, Id::Flux, 0.2F}};
                require(oracle.process(nullptr, nullptr, expectedL.data(), expectedR.data(), frames,
                                       old),
                        "ownership captured oracle");
                require(left == expectedL && right == expectedR,
                        "ownership editor captured epoch differs");
            }
            bool parity = true;
            for (unsigned block = 0; block < 2; ++block) {
                timestamp.mSampleTime += frames;
                require(AudioUnitRender(unit, &flags, &timestamp, 0, frames,
                                        reinterpret_cast<AudioBufferList*>(&buffers)) == noErr,
                        "ownership following render");
                const std::array edit{Event{0, EventKind::Parameter, Id::Flux, 0.8F}};
                require(oracle.process(nullptr, nullptr, expectedL.data(), expectedR.data(), frames,
                                       editor && block == 0 ? std::span<const Event>(edit)
                                                            : std::span<const Event>{}),
                        "ownership following oracle");
                parity &= left == expectedL && right == expectedR;
                if (left != expectedL || right != expectedR)
                    for (unsigned i = 0; i < frames; ++i)
                        if (left[i] != expectedL[i] || right[i] != expectedR[i]) {
                            std::printf(
                                "ownership %s block=%u mismatch=%u actual=%g,%g expected=%g,%g\n",
                                editor ? "editor" : "restore", block, i, left[i], right[i],
                                expectedL[i], expectedR[i]);
                            break;
                        }
            }
            std::printf(
                "S09-OWN-01 %s: newer readback=%s (%g) schema40=%s (%g) following-audio=%s\n",
                editor ? "editor" : "restore", readback ? "PASS" : "FAIL", value,
                save ? "PASS" : "FAIL", state.values[0], parity ? "PASS" : "FAIL");
            all &= readback && save && parity;
            CFRelease(saved);
            CFRelease(newer);
            require(AudioUnitUninitialize(unit) == noErr, "ownership cleanup");
            timestamp.mSampleTime += frames;
        }
        require(AudioComponentInstanceDispose(unit) == noErr, "ownership dispose");
        CFRelease(bundle);
        return all ? 0 : 1;
    }
    unsigned mismatches = 0;
    for (int path = 0; path < 3; ++path) {
        // New lifecycle resets DSP; parameter defaults are explicitly restored.
        for (std::size_t i = 0; i < ids.size(); ++i) {
            const float maximum =
                catalogue[i].steps > 0 ? static_cast<float>(catalogue[i].steps - 1) : 1.F;
            require(AudioUnitSetParameter(unit, ids[i], kAudioUnitScope_Global, 0,
                                          catalogue[i].initial * maximum, 0) == noErr,
                    "AU defaults failed");
        }
        require(AudioUnitInitialize(unit) == noErr, "AU initialize failed");
        Processor positioned, boundary;
        positioned.prepare(48000);
        boundary.prepare(48000);
        std::array<Event, 3> points{Event{9, EventKind::Parameter, Id::Drive, 1},
                                    Event{61, EventKind::Parameter, Id::Drive, 0},
                                    Event{91, EventKind::Parameter, Id::Drive, 1}};
        if (path == 0) {
            for (const auto& point : points)
                require(AudioUnitSetParameter(unit, ids[index(Id::Drive)], kAudioUnitScope_Global,
                                              0, point.value,
                                              static_cast<UInt32>(point.offset)) == noErr,
                        "AU offset setter failed");
        } else if (path == 1) {
            std::array<AudioUnitParameterEvent, 3> native{};
            for (std::size_t i = 0; i < native.size(); ++i) {
                auto& event = native[i];
                event.scope = kAudioUnitScope_Global;
                event.parameter = ids[index(Id::Drive)];
                event.eventType = kParameterEvent_Immediate;
                event.eventValues.immediate.bufferOffset = static_cast<UInt32>(points[i].offset);
                event.eventValues.immediate.value = points[i].value;
            }
            require(AudioUnitScheduleParameters(unit, native.data(), native.size()) == noErr,
                    "AU scheduled immediate dispatch failed");
        } else {
            for (const auto& point : points)
                require(MusicDeviceMIDIEvent(unit, 0xb0, 22, point.value > 0 ? 127 : 0,
                                             static_cast<UInt32>(point.offset)) == noErr,
                        "AU CC22 failed");
        }
        AudioUnitRenderActionFlags flags = 0;
        require(AudioUnitRender(unit, &flags, &timestamp, 0, frames,
                                reinterpret_cast<AudioBufferList*>(&buffers)) == noErr,
                "AU render failed");
        require(positioned.process(nullptr, nullptr, expectedL.data(), expectedR.data(), frames,
                                   points),
                "positioned oracle failed");
        constexpr std::array finalTarget{Event{0, EventKind::Parameter, Id::Drive, 1}};
        require(boundary.process(nullptr, nullptr, boundaryL.data(), boundaryR.data(), frames,
                                 finalTarget),
                "boundary oracle failed");
        const bool parity = left == expectedL && right == expectedR;
        const bool collapsed = left == boundaryL && right == boundaryR;
        std::printf("AU %s: positioned=%s collapsed-final-at-zero=%s\n",
                    path == 0   ? "SetParameter(offset)"
                    : path == 1 ? "ScheduleParameters(immediate)"
                                : "MIDI CC22",
                    parity ? "PASS" : "FAIL", collapsed ? "yes" : "no");
        if (path < 2) {
            require(parity || collapsed, "AU result matches neither oracle");
            mismatches += !parity;
        } else
            require(parity, "AU MIDI offsets differ");
        timestamp.mSampleTime += frames;
        flags = 0;
        require(AudioUnitRender(unit, &flags, &timestamp, 0, frames,
                                reinterpret_cast<AudioBufferList*>(&buffers)) == noErr,
                "AU following render failed");
        auto& next = parity ? positioned : boundary;
        require(next.process(nullptr, nullptr, expectedL.data(), expectedR.data(), frames),
                "AU next oracle failed");
        require(left == expectedL && right == expectedR, "AU replayed points on following block");
        timestamp.mSampleTime += frames;
        require(AudioUnitUninitialize(unit) == noErr, "AU uninitialize failed");
    }
    if (strict && mismatches == 0) {
        unsigned cases = 0;
        Processor oracle;
        const auto start = [&](double rate, const State& state) {
            format.mSampleRate = rate;
            for (auto scope : {kAudioUnitScope_Input, kAudioUnitScope_Output})
                require(AudioUnitSetProperty(unit, kAudioUnitProperty_StreamFormat, scope, 0,
                                             &format, sizeof(format)) == noErr,
                        "strict AU rate failed");
            for (std::size_t i = 0; i < ids.size(); ++i) {
                const float maximum = catalogue[i].steps > 0 ? float(catalogue[i].steps - 1) : 1.F;
                require(AudioUnitSetParameter(unit, ids[i], kAudioUnitScope_Global, 0,
                                              state.values[i] * maximum, 0) == noErr,
                        "strict AU inactive target failed");
            }
            require(AudioUnitInitialize(unit) == noErr, "strict AU initialize failed");
            oracle.prepare(static_cast<float>(rate));
            require(oracle.restore(state), "strict initial oracle failed");
            timestamp.mSampleTime += frames;
        };
        const auto stop = [&] {
            require(AudioUnitUninitialize(unit) == noErr, "strict AU cleanup failed");
        };
        const auto render = [&](unsigned length, std::span<const Event> events = {}) {
            buffers.buffers[0].mDataByteSize = buffers.buffers[1].mDataByteSize = length * 4;
            AudioUnitRenderActionFlags flags = 0;
            require(AudioUnitRender(unit, &flags, &timestamp, 0, length,
                                    reinterpret_cast<AudioBufferList*>(&buffers)) == noErr,
                    "strict AU render failed");
            require(oracle.process(inputFixture.enabled ? inputL.data() : nullptr,
                                   inputFixture.enabled ? inputR.data() : nullptr, expectedL.data(),
                                   expectedR.data(), length, events),
                    "strict oracle failed");
            if (!std::equal(left.begin(), left.begin() + length, expectedL.begin()) ||
                !std::equal(right.begin(), right.begin() + length, expectedR.begin())) {
                std::fprintf(stderr, "strict sample mismatch: case=%u length=%u time=%.0f\n", cases,
                             length, timestamp.mSampleTime);
                for (unsigned i = 0; i < length; ++i)
                    if (left[i] != expectedL[i] || right[i] != expectedR[i]) {
                        std::fprintf(stderr,
                                     "first mismatch frame=%u actual=%g,%g expected=%g,%g\n", i,
                                     left[i], right[i], expectedL[i], expectedR[i]);
                        break;
                    }
                std::exit(1);
            }
            timestamp.mSampleTime += std::max(length, 1U);
            ++cases;
        };
        const auto dispatch = [&](std::span<const Event> events, bool scheduled) {
            std::array<AudioUnitParameterEvent, 256> native{};
            require(events.size() <= native.size(), "fixture capacity");
            for (std::size_t i = 0; i < events.size(); ++i) {
                const auto& e = events[i];
                const auto p = index(e.id);
                const float maximum = catalogue[p].steps > 0 ? float(catalogue[p].steps - 1) : 1.F;
                auto& n = native[i];
                n.scope = kAudioUnitScope_Global;
                n.parameter = ids[p];
                n.eventType = kParameterEvent_Immediate;
                n.eventValues.immediate.bufferOffset = static_cast<UInt32>(e.offset);
                n.eventValues.immediate.value = e.value * maximum;
                if (!scheduled)
                    require(AudioUnitSetParameter(unit, n.parameter, n.scope, 0,
                                                  n.eventValues.immediate.value,
                                                  n.eventValues.immediate.bufferOffset) == noErr,
                            "strict AU setter failed");
            }
            if (scheduled)
                require(AudioUnitScheduleParameters(unit, native.data(),
                                                    static_cast<UInt32>(events.size())) == noErr,
                        "strict AU schedule failed");
        };
        // Every parameter/path/rate/block edge. Equal-offset ties retain caller
        // order; independent DSP supplies the step target + 10 ms smoothing.
        for (double rate : {44100., 48000., 88200., 96000., 176400., 192000.})
            for (bool scheduled : {false, true})
                for (const auto& parameter : catalogue)
                    for (unsigned length : {1U, 7U, 63U, 64U, 65U, 511U, 4096U}) {
                        State initial;
                        initial.values[index(Id::Drive)] = parameter.id == Id::Drive ? 0.F : 1.F;
                        start(rate, initial);
                        const float high = parameter.steps > 0 ? 1.F : 0.83F;
                        const float low = parameter.steps > 0 ? 0.F : 0.17F;
                        std::array<Event, 4> points{
                            {{0, EventKind::Parameter, parameter.id, high},
                             {length / 2, EventKind::Parameter, parameter.id, low},
                             {length / 2, EventKind::Parameter, parameter.id, high},
                             {length - 1, EventKind::Parameter, parameter.id, low}}};
                        dispatch(points, scheduled);
                        render(length, points);
                        render(511); // final target, no replay
                        stop();
                    }
        State running;
        running.values[index(Id::Drive)] = 1;
        for (bool scheduled : {false, true}) {
            start(48000, running);
            std::array<Event, 256> dense{};
            for (unsigned i = 0; i < dense.size(); ++i)
                dense[i] = {i / 4, EventKind::Parameter, Id::Flux, float(i % 17) / 16};
            dispatch(dense, scheduled);
            render(64, dense); // exact 256, no implicit duplicate scalar slot
            render(64);
            dispatch(dense, scheduled);
            constexpr std::array extra{Event{0, EventKind::Parameter, Id::Bypass, 1}};
            dispatch(extra, scheduled);
            render(64); // 257 rejects whole epoch
            render(64); // rejected host targets cannot replay
            dispatch(extra, scheduled);
            render(64, extra); // valid edit recovers even if readback unchanged
            stop();
            start(48000, running);
            dispatch(dense, scheduled);
            require(MusicDeviceMIDIEvent(unit, 0xb0, 21, 90, 0) == noErr, "merged CC failed");
            render(64); // parameters + MIDI share cap
            render(64);
            stop();
        }
        for (float invalid : {std::numeric_limits<float>::quiet_NaN(),
                              std::numeric_limits<float>::infinity(), -0.1F, 1.1F}) {
            start(48000, running);
            std::printf("invalid value %g case %u\n", invalid, cases);
            constexpr std::array good{Event{9, EventKind::Parameter, Id::Flux, 0.3F}};
            dispatch(good, false);
            require(AudioUnitSetParameter(unit, ids[0], kAudioUnitScope_Global, 0, invalid, 2) !=
                        noErr,
                    "invalid AU value accepted");
            render(64);
            render(64);
            stop();
        }
        for (UInt32 offset : {64U, std::numeric_limits<UInt32>::max()}) {
            start(48000, running);
            const std::array bad{Event{offset, EventKind::Parameter, Id::Flux, 0.2F}};
            dispatch(bad, false);
            render(64);
            render(64);
            stop();
        }
        for (Id id : {Id::Drive, Id::State, Id::Domain, Id::Bypass})
            for (float raw : {0.49999F, 0.5F, 0.50001F}) {
                start(48000, running);
                require(AudioUnitSetParameter(unit, ids[index(id)], kAudioUnitScope_Global, 0, raw,
                                              7) == noErr,
                        "quantization setter failed");
                const float normalized = std::round(raw) / float(catalogue[index(id)].steps - 1);
                AudioUnitParameterValue readback = -1;
                require(AudioUnitGetParameter(unit, ids[index(id)], kAudioUnitScope_Global, 0,
                                              &readback) == noErr &&
                            readback == std::round(raw),
                        "pending quantized AU readback failed");
                const std::array quantized{Event{7, EventKind::Parameter, id, normalized}};
                render(64, quantized);
                render(64);
                stop();
            }
        start(48000, running);
        constexpr std::array zeroPending{Event{7, EventKind::Parameter, Id::Flux, 0.2F}};
        dispatch(zeroPending, true);
        const auto zeroTime = timestamp.mSampleTime;
        render(0);
        timestamp.mSampleTime = zeroTime;
        render(64, zeroPending); // same timestamp after zero must reach Render
        const auto cachedL = left, cachedR = right;
        timestamp.mSampleTime -= 64;
        constexpr std::array nextPending{Event{9, EventKind::Parameter, Id::Bias, 0.7F}};
        dispatch(nextPending, false);
        buffers.buffers[0].mDataByteSize = buffers.buffers[1].mDataByteSize = 64 * 4;
        AudioUnitRenderActionFlags flags = 0;
        require(AudioUnitRender(unit, &flags, &timestamp, 0, 64,
                                reinterpret_cast<AudioBufferList*>(&buffers)) == noErr &&
                    left == cachedL && right == cachedR,
                "AU same-time cache changed");
        buffers.buffers[0].mDataByteSize = buffers.buffers[1].mDataByteSize = 7 * 4;
        require(AudioUnitRender(unit, &flags, &timestamp, 0, 7,
                                reinterpret_cast<AudioBufferList*>(&buffers)) != noErr,
                "AU different-length same-time cache silently accepted");
        timestamp.mSampleTime += 64;
        render(64, nextPending);
        render(64);
        stop();
        start(48000, running);
        AudioUnitParameterEvent ramp{};
        ramp.scope = kAudioUnitScope_Global;
        ramp.parameter = ids[0];
        ramp.eventType = kParameterEvent_Ramped;
        require(AudioUnitScheduleParameters(unit, &ramp, 1) != noErr, "AU ramp silently accepted");
        render(64);
        render(64);
        stop();
        // Pending native targets must survive an off-audio ClassInfo save, and
        // eight coherent restores must clear old native ingress before render.
        start(48000, running);
        dispatch(zeroPending, false);
        CFPropertyListRef saved = nullptr;
        UInt32 savedSize = sizeof(saved);
        require(AudioUnitGetProperty(unit, kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global, 0,
                                     &saved, &savedSize) == noErr &&
                    saved,
                "strict pending AU save failed");
        const auto data = static_cast<CFDataRef>(
            CFDictionaryGetValue(static_cast<CFDictionaryRef>(saved), CFSTR("jucePluginState")));
        require(data != nullptr && CFDataGetLength(data) == 40, "AU schema 1 size/key changed");
        State savedState;
        require(decode({CFDataGetBytePtr(data), 40}, savedState) == DecodeResult::Ok &&
                    savedState.values[index(Id::Flux)] == 0.2F,
                "pending AU target absent from saved state");
        for (unsigned i = 0; i < 8; ++i)
            require(AudioUnitSetProperty(unit, kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global,
                                         0, &saved, sizeof(saved)) == noErr,
                    "AU eight restore slots failed");
        require(oracle.restore(savedState), "restore oracle failed");
        render(64);
        render(64);
        CFRelease(saved);
        stop();
        // Two actual setter producers plus MIDI share one serialized epoch.
        start(48000, running);
        std::thread first([&] {
            for (unsigned i = 0; i < 64; ++i)
                require(AudioUnitSetParameter(unit, ids[0], kAudioUnitScope_Global, 0,
                                              float(i % 17) / 16, i * 2) == noErr,
                        "AU producer one failed");
        });
        std::thread second([&] {
            for (unsigned i = 0; i < 64; ++i)
                require(AudioUnitSetParameter(unit, ids[1], kAudioUnitScope_Global, 0,
                                              float(i % 13) / 12, i * 2 + 1) == noErr,
                        "AU producer two failed");
        });
        for (unsigned i = 0; i < 64; ++i)
            require(MusicDeviceMIDIEvent(unit, 0xb0, 22, 127, i) == noErr, "MPSC MIDI failed");
        first.join();
        second.join();
        std::array<Event, 192> concurrent{};
        for (unsigned i = 0; i < 64; ++i) {
            concurrent[i * 2] = {i * 2, EventKind::Parameter, Id::Flux, float(i % 17) / 16};
            concurrent[i * 2 + 1] = {i * 2 + 1, EventKind::Parameter, Id::Bias, float(i % 13) / 12};
            concurrent[128 + i] = {i, EventKind::Parameter, Id::Drive, 1};
        }
        std::stable_sort(concurrent.begin(), concurrent.end(),
                         [](const Event& a, const Event& b) { return a.offset < b.offset; });
        render(128, concurrent);
        render(128);
        stop();
        start(48000, running);
        dispatch(zeroPending, false);
        UInt32 bypassValue = 1;
        require(AudioUnitSetProperty(unit, kAudioUnitProperty_BypassEffect, kAudioUnitScope_Global,
                                     0, &bypassValue, sizeof(bypassValue)) == noErr,
                "AU property bypass failed");
        bypassValue = 0;
        UInt32 bytes = sizeof(bypassValue);
        require(AudioUnitGetProperty(unit, kAudioUnitProperty_BypassEffect, kAudioUnitScope_Global,
                                     0, &bypassValue, &bytes) == noErr &&
                    bypassValue == 1,
                "AU pending bypass property readback failed");
        constexpr std::array propertyPoints{Event{0, EventKind::Parameter, Id::Bypass, 1},
                                            Event{7, EventKind::Parameter, Id::Flux, 0.2F}};
        render(64, propertyPoints);
        render(64);
        stop();
        // Unsupported IDs/scopes/elements/event types and huge event counts
        // must reject a pending valid batch before any host-sized traversal.
        for (unsigned kind = 0; kind < 6; ++kind) {
            start(48000, running);
            dispatch(zeroPending, false);
            AudioUnitParameterEvent invalid{};
            invalid.scope = kAudioUnitScope_Global;
            invalid.parameter = ids[0];
            invalid.eventType = kParameterEvent_Immediate;
            invalid.eventValues.immediate.value = 0.4F;
            if (kind == 0)
                invalid.parameter = 123456U;
            if (kind == 1)
                invalid.scope = kAudioUnitScope_Input;
            if (kind == 2)
                invalid.element = 1;
            if (kind == 3)
                invalid.eventType = static_cast<AUParameterEventType>(99);
            if (kind == 4)
                invalid.eventValues.immediate.value = std::numeric_limits<float>::quiet_NaN();
            require(AudioUnitScheduleParameters(unit, &invalid, kind == 5 ? 0xffffffffU : 1) !=
                        noErr,
                    "invalid scheduled AU event accepted");
            render(64);
            render(64);
            stop();
        }
        // Accepted host targets survive graph teardown before the next render;
        // old positioned offsets do not. This remains a strict exit requirement.
        start(48000, running);
        dispatch(zeroPending, false);
        stop();
        require(AudioUnitInitialize(unit) == noErr, "AU pending-target reinitialize failed");
        State rebuilt = running;
        rebuilt.values[index(Id::Flux)] = 0.2F;
        oracle.prepare(48000);
        require(oracle.restore(rebuilt), "AU rebuild oracle failed");
        AudioUnitParameterValue rebuiltReadback = -1;
        require(AudioUnitGetParameter(unit, ids[0], kAudioUnitScope_Global, 0, &rebuiltReadback) ==
                        noErr &&
                    rebuiltReadback == 0.2F,
                "S09-LIFE-01: accepted pending FLUX lost across Cleanup/Initialize");
        render(64);
        render(64);
        stop();
        // Reset discards old offsets but preserves accepted pending targets.
        start(48000, running);
        dispatch(zeroPending, false);
        require(AudioUnitReset(unit, kAudioUnitScope_Global, 0) == noErr, "AU Reset failed");
        AudioUnitParameterValue resetReadback = -1;
        require(AudioUnitGetParameter(unit, ids[0], kAudioUnitScope_Global, 0, &resetReadback) ==
                        noErr &&
                    resetReadback == 0.2F,
                "AU Reset lost target");
        const std::array resetPoint{Event{0, EventKind::Parameter, Id::Flux, 0.2F}};
        render(64, resetPoint);
        render(64);
        stop();
        // Full restore queue rejects a ninth transaction without changing
        // controls or introducing pending/native replay. AU property returns
        // SDK success; schema/readback/audio establish the processor result.
        start(48000, running);
        dispatch(zeroPending, false);
        CFPropertyListRef fullSaved = nullptr;
        UInt32 fullBytes = sizeof(fullSaved);
        require(AudioUnitGetProperty(unit, kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global, 0,
                                     &fullSaved, &fullBytes) == noErr,
                "restore overflow save");
        for (unsigned i = 0; i < 8; ++i)
            require(AudioUnitSetProperty(unit, kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global,
                                         0, &fullSaved, sizeof(fullSaved)) == noErr,
                    "restore overflow fill");
        auto rejectedState = running;
        rejectedState.values[0] = 0.9F;
        const auto rejectedBytes = encode(rejectedState);
        auto rejectedData = CFDataCreate(nullptr, rejectedBytes.data(), rejectedBytes.size());
        auto ninth =
            CFDictionaryCreateMutableCopy(nullptr, 0, static_cast<CFDictionaryRef>(fullSaved));
        CFDictionarySetValue(ninth, CFSTR("jucePluginState"), rejectedData);
        CFPropertyListRef ninthState = ninth;
        require(AudioUnitSetProperty(unit, kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global, 0,
                                     &ninthState, sizeof(ninthState)) == noErr,
                "restore ninth dispatch");
        AudioUnitParameterValue fullReadback = -1;
        require(AudioUnitGetParameter(unit, ids[0], kAudioUnitScope_Global, 0, &fullReadback) ==
                        noErr &&
                    fullReadback == 0.2F,
                "restore overflow changed readback");
        auto acceptedState = running;
        acceptedState.values[0] = 0.2F;
        require(oracle.restore(acceptedState), "restore overflow oracle");
        render(64);
        render(64);
        CFRelease(ninth);
        CFRelease(rejectedData);
        CFRelease(fullSaved);
        stop();
        // Loaded stereo transform with different channel waveforms and exact
        // independent reference samples at every supported rate.
        for (double rate : {44100., 48000., 88200., 96000., 176400., 192000.}) {
            State transform = running;
            transform.values[index(Id::Domain)] = 1;
            inputFixture.enabled = true;
            start(rate, transform);
            constexpr std::array transformPoints{Event{7, EventKind::Parameter, Id::Flux, 0.75F},
                                                 Event{63, EventKind::Parameter, Id::State, 1}};
            dispatch(transformPoints, true);
            render(511, transformPoints);
            render(4096);
            stop();
        }
        inputFixture.enabled = false;
        // Zero AU input elements select the disabled processor layout. Both
        // generators remain exact and transform receives deterministic silence.
        for (float domain : {0.F, 0.5F, 1.F}) {
            UInt32 inputCount = 0;
            const auto disableResult =
                AudioUnitSetProperty(unit, kAudioUnitProperty_ElementCount, kAudioUnitScope_Input,
                                     0, &inputCount, sizeof(inputCount));
            std::printf("AU disabled-input ElementCount status=%d\n", int(disableResult));
            require(disableResult == noErr, "S09-BUS-01: AU disabled-input element count rejected");
            State disabled = running;
            disabled.values[index(Id::Domain)] = domain;
            // Output stays stereo; initialize directly with literal setters.
            for (std::size_t i = 0; i < ids.size(); ++i) {
                const float max = catalogue[i].steps > 0 ? float(catalogue[i].steps - 1) : 1.F;
                require(AudioUnitSetParameter(unit, ids[i], kAudioUnitScope_Global, 0,
                                              disabled.values[i] * max, 0) == noErr,
                        "disabled input setter");
            }
            require(AudioUnitInitialize(unit) == noErr, "AU disabled initialize");
            oracle.prepare(static_cast<float>(format.mSampleRate));
            require(oracle.restore(disabled), "disabled oracle");
            timestamp.mSampleTime += frames;
            render(511);
            render(4096);
            stop();
            inputCount = 1;
            require(AudioUnitSetProperty(unit, kAudioUnitProperty_ElementCount,
                                         kAudioUnitScope_Input, 0, &inputCount,
                                         sizeof(inputCount)) == noErr,
                    "AU reenable input");
            require(AudioUnitSetProperty(unit, kAudioUnitProperty_StreamFormat,
                                         kAudioUnitScope_Input, 0, &format,
                                         sizeof(format)) == noErr,
                    "AU reenable stereo format");
        }
        std::printf("strict expanded AU: %u sample-comparison cases PASS\n", cases);
    }
    require(AudioComponentInstanceDispose(unit) == noErr, "AU dispose failed");
    CFRelease(bundle);
    std::printf("AU characterization complete; accepted parameter timing contract: %s\n",
                mismatches ? "FAIL / Sprint 09 repair required" : "PASS");
    return strict && mismatches ? 1 : 0;
}
