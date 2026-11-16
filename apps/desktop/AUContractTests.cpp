// In-process registration loads this exact bundle; no personal AU installation.
#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreMIDI/CoreMIDI.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
#ifdef SYS01_NATIVE_RT_PROBE
#include "RealtimeProbe.hpp"
#endif
namespace {
void require(bool value, const char* message) {
    if (!value) {
        std::fprintf(stderr, "%s\n", message);
        std::exit(1);
    }
}
OSStatus input(void*, AudioUnitRenderActionFlags*, const AudioTimeStamp*, UInt32, UInt32,
               AudioBufferList* buffers) {
    for (UInt32 i = 0; i < buffers->mNumberBuffers; ++i)
        if (buffers->mBuffers[i].mData)
            std::memset(buffers->mBuffers[i].mData, 0, buffers->mBuffers[i].mDataByteSize);
    return noErr;
}
} // namespace
int main(int argc, char** argv) {
    require(argc == 2, "supply exact AU bundle path");
    auto url =
        CFURLCreateFromFileSystemRepresentation(nullptr, reinterpret_cast<const UInt8*>(argv[1]),
                                                static_cast<CFIndex>(std::strlen(argv[1])), true);
    auto bundle = CFBundleCreate(nullptr, url);
    CFRelease(url);
    require(bundle && CFBundleLoadExecutable(bundle), "AU bundle load failed");
    const auto factory = reinterpret_cast<AudioComponentFactoryFunction>(
        CFBundleGetFunctionPointerForName(bundle, CFSTR("JuceAUFactory")));
    require(factory != nullptr, "AU factory missing");
    AudioComponentDescription description{kAudioUnitType_MusicEffect, 'Sys1', 'Algr', 0, 0};
    const auto component =
        AudioComponentRegister(&description, CFSTR("SYS 01 local fixture"), 0x10000, factory);
    require(component != nullptr, "AU in-process registration failed");
    AudioUnit unit = nullptr;
    require(AudioComponentInstanceNew(component, &unit) == noErr, "AU instantiate failed");
    UInt32 maxFrames = 4096;
    require(AudioUnitSetProperty(unit, kAudioUnitProperty_MaximumFramesPerSlice,
                                 kAudioUnitScope_Global, 0, &maxFrames, sizeof(maxFrames)) == noErr,
            "AU max frames failed");
    AURenderCallbackStruct callback{input, nullptr};
    require(AudioUnitSetProperty(unit, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Input,
                                 0, &callback, sizeof(callback)) == noErr,
            "AU input callback failed");
    UInt32 size = 0;
    require(AudioUnitGetPropertyInfo(unit, kAudioUnitProperty_ParameterList, kAudioUnitScope_Global,
                                     0, &size, nullptr) == noErr,
            "AU parameter list size failed");
    std::array<AudioUnitParameterID, 32> ids{};
    require(size <= sizeof(ids) &&
                AudioUnitGetProperty(unit, kAudioUnitProperty_ParameterList, kAudioUnitScope_Global,
                                     0, ids.data(), &size) == noErr,
            "AU parameter list failed");
    AudioUnitParameterID drive = 0, flux = 0;
    bool foundDrive = false, foundFlux = false;
    for (std::size_t i = 0; i < size / sizeof(ids[0]); ++i) {
        AudioUnitParameterInfo info{};
        UInt32 infoSize = sizeof(info);
        require(AudioUnitGetProperty(unit, kAudioUnitProperty_ParameterInfo, kAudioUnitScope_Global,
                                     ids[i], &info, &infoSize) == noErr,
                "AU parameter info failed");
        if (std::strcmp(info.name, "DRIVE") == 0) {
            drive = ids[i];
            foundDrive = true;
        }
        if (std::strcmp(info.name, "FLUX") == 0) {
            flux = ids[i];
            foundFlux = true;
        }
        if ((info.flags & kAudioUnitParameterFlag_HasCFNameString) &&
            (info.flags & kAudioUnitParameterFlag_CFNameRelease))
            CFRelease(info.cfNameString);
    }
    require(foundDrive && foundFlux, "AU catalogue names missing");
    std::array<float, 4096> left{}, right{};
    struct StereoList {
        UInt32 count;
        AudioBuffer buffers[2];
    } buffers{2, {{1, sizeof(left), left.data()}, {1, sizeof(right), right.data()}}};
    AudioTimeStamp timestamp{};
    timestamp.mFlags = kAudioTimeStampSampleTimeValid;
#ifdef SYS01_NATIVE_RT_PROBE
    sys01::probe::selfTest();
#endif
    unsigned blocks = 0;
    for (double rate : {44100., 48000., 88200., 96000., 176400., 192000.}) {
        AudioStreamBasicDescription format{
            rate,
            kAudioFormatLinearPCM,
            static_cast<UInt32>(kAudioFormatFlagsNativeFloatPacked) |
                static_cast<UInt32>(kAudioFormatFlagIsNonInterleaved),
            sizeof(float),
            1,
            sizeof(float),
            2,
            32,
            0};
        for (auto scope : {kAudioUnitScope_Input, kAudioUnitScope_Output})
            require(AudioUnitSetProperty(unit, kAudioUnitProperty_StreamFormat, scope, 0, &format,
                                         sizeof(format)) == noErr,
                    "AU stream format failed");
        require(AudioUnitInitialize(unit) == noErr, "AU initialize failed");
        require(AudioUnitSetParameter(unit, drive, kAudioUnitScope_Global, 0, 1, 0) == noErr,
                "AU DRIVE failed");
        for (UInt32 frames : {0U, 1U, 7U, 63U, 64U, 65U, 511U, 4096U}) {
            OSStatus result = noErr;
            const auto entry = [&] {
            // MIDI entry is part of the diagnostic boundary, not just the DSP call.
#ifdef SYS01_NATIVE_RT_PROBE
                sys01::probe::begin();
#endif
                for (UInt32 i = 0; i < std::min(frames, 256U); ++i)
                    result |= MusicDeviceMIDIEvent(unit, 0xb0, 21, i % 128, i);
                result |= AudioUnitSetParameter(unit, flux, kAudioUnitScope_Global, 0, 0.4F, 0);
                AudioUnitRenderActionFlags flags = 0;
                buffers.buffers[0].mDataByteSize = buffers.buffers[1].mDataByteSize =
                    frames * sizeof(float);
                result |= AudioUnitRender(unit, &flags, &timestamp, 0, frames,
                                          reinterpret_cast<AudioBufferList*>(&buffers));
#ifdef SYS01_NATIVE_RT_PROBE
                const auto counts = sys01::probe::end();
                std::printf("AU entry %u frames %u: alloc=%llu free=%llu mutex=%llu unfair=%llu\n",
                            blocks, frames, counts.allocation, counts.deallocation, counts.mutex,
                            counts.unfair);
                if (counts.allocation != 0)
                    sys01::probe::printAllocation(counts);
#endif
            };
            if (blocks == 1) {
                std::thread firstAudio(entry);
                firstAudio.join();
            } else {
                entry();
            }
            require(result == noErr, "AU MIDI/automation/render failed");
            for (UInt32 i = 0; i < frames; ++i)
                require(std::isfinite(left[i]) && std::isfinite(right[i]), "AU nonfinite sample");
            // AU SDK caches by timestamp. Give the zero-block probe its own
            // timestamp so the next variable-size call actually reaches Render.
            timestamp.mSampleTime += std::max(frames, 1U);
            ++blocks;
        }
        // Modern UMP CC path and SysEx filtering are exercised through the
        // AudioUnit dispatch, not direct processor methods.
        alignas(MIDIEventList) std::array<std::byte, 4096> umpStorage{};
        auto* list = reinterpret_cast<MIDIEventList*>(umpStorage.data());
        auto* packet = MIDIEventListInit(list, kMIDIProtocol_1_0);
        constexpr std::array<UInt32, 3> words{0x20b01540U, 0x30120000U, 0x00000000U};
        require(MIDIEventListAdd(list, umpStorage.size(), packet, 0, words.size(), words.data()) !=
                    nullptr,
                "UMP fixture construction failed");
        std::array<UInt8, 65536> sysex{};
        sysex.front() = 0xf0;
        sysex.back() = 0xf7;
#ifdef SYS01_NATIVE_RT_PROBE
        sys01::probe::begin();
#endif
        OSStatus edgeResult = noErr;
        if (__builtin_available(macOS 12.0, *))
            edgeResult = MusicDeviceMIDIEventList(unit, 0, list);
        else
            std::printf(
                "AU UMP dispatch unavailable before macOS 12; legacy MIDI remains tested\n");
        edgeResult |= MusicDeviceSysEx(unit, sysex.data(), static_cast<UInt32>(sysex.size()));
        // 257 legacy events reject the entire batch; negative and out-of-block
        // offsets are handled without changing controls or consuming UI edges.
        for (unsigned i = 0; i < 257; ++i)
            edgeResult |= MusicDeviceMIDIEvent(unit, 0xb0, 21, i % 128, 0);
        AudioUnitRenderActionFlags edgeFlags = 0;
        edgeResult |= AudioUnitRender(unit, &edgeFlags, &timestamp, 0, 64,
                                      reinterpret_cast<AudioBufferList*>(&buffers));
#ifdef SYS01_NATIVE_RT_PROBE
        const auto edgeCounts = sys01::probe::end();
        std::printf("AU overflow/UMP/SysEx entry: alloc=%llu free=%llu mutex=%llu unfair=%llu\n",
                    edgeCounts.allocation, edgeCounts.deallocation, edgeCounts.mutex,
                    edgeCounts.unfair);
#endif
        require(edgeResult == noErr, "AU UMP/SysEx/overflow failed");
        timestamp.mSampleTime += 64;
        // Property restore runs on the control owner, concurrently with actual
        // native rendering; immutable queue overflow remains explicit.
        CFPropertyListRef saved = nullptr;
        UInt32 savedSize = sizeof(saved);
        require(AudioUnitGetProperty(unit, kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global, 0,
                                     &saved, &savedSize) == noErr &&
                    saved != nullptr,
                "AU native state capture failed");
        std::thread restoreAudio([&] {
            for (int i = 0; i < 200; ++i) {
                AudioUnitRenderActionFlags flags = 0;
                require(AudioUnitRender(unit, &flags, &timestamp, 0, 64,
                                        reinterpret_cast<AudioBufferList*>(&buffers)) == noErr,
                        "AU concurrent restore render failed");
                timestamp.mSampleTime += 64;
            }
        });
        for (int i = 0; i < 20; ++i)
            require(AudioUnitSetProperty(unit, kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global,
                                         0, &saved, sizeof(saved)) == noErr,
                    "AU concurrent restore property failed");
        restoreAudio.join();
        CFRelease(saved);
        require(AudioUnitReset(unit, kAudioUnitScope_Global, 0) == noErr, "AU reset failed");
        require(AudioUnitUninitialize(unit) == noErr, "AU uninitialize failed");
    }
    require(AudioComponentInstanceDispose(unit) == noErr, "AU dispose failed");
    // Registered factory remains valid until process exit; don't unload it.
    CFRelease(bundle);
    std::printf("loaded AU: %u blocks, six rates, bounded MIDI workload, reset/reinitialize "
                "passed; offset parity not claimed\n",
                blocks);
}
