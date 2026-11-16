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
    require(argc == 2, "supply exact hook-ON AU bundle");
    auto url =
        CFURLCreateFromFileSystemRepresentation(nullptr, reinterpret_cast<const UInt8*>(argv[1]),
                                                static_cast<CFIndex>(std::strlen(argv[1])), true);
    auto bundle = CFBundleCreate(nullptr, url);
    CFRelease(url);
    require(bundle && CFBundleLoadExecutable(bundle), "load AU");
    const auto symbol = [&](const char* name) {
        auto key = CFStringCreateWithCString(nullptr, name, kCFStringEncodingUTF8);
        auto result = CFBundleGetFunctionPointerForName(bundle, key);
        CFRelease(key);
        require(result != nullptr, name);
        return result;
    };
    using Hook = void (*)(void (*)(void*, void*), void*);
    using Write = void (*)(void*, int, float);
    using Listener = void (*)(void*, void (*)(void*, void*, int, float), void*);
    using Observer = void (*)(void (*)(void*, void*, int, bool), void*);
    auto before = reinterpret_cast<Hook>(symbol("Sys01AUFixtureBeforeCaptureHook"));
    auto write = reinterpret_cast<Write>(symbol("Sys01AUFixtureEditorWrite"));
    auto listener = reinterpret_cast<Listener>(symbol("Sys01AUFixtureListener"));
    auto observer = reinterpret_cast<Observer>(symbol("Sys01AUFixtureSuppressionHook"));
    auto factory = reinterpret_cast<AudioComponentFactoryFunction>(symbol("JuceAUFactory"));
    AudioComponentDescription description{kAudioUnitType_MusicEffect, 'Sys1', 'Algr', 0, 0};
    auto component =
        AudioComponentRegister(&description, CFSTR("SYS 01 suppression fixture"), 0x10000, factory);
    require(component != nullptr, "register AU");
    std::array<AudioUnit, 2> units{};
    std::array<void*, 2> processors{};
    struct Buffers {
        UInt32 count = 2;
        AudioBuffer buffers[2]{};
    };
    std::array<float, 64> left{}, right{};
    Buffers buffers{2, {{1, sizeof(left), left.data()}, {1, sizeof(right), right.data()}}};
    AudioTimeStamp stamp{};
    stamp.mFlags = kAudioTimeStampSampleTimeValid;
    auto render = [&](AudioUnit unit) {
        AudioUnitRenderActionFlags flags = 0;
        require(AudioUnitRender(unit, &flags, &stamp, 0, 64,
                                reinterpret_cast<AudioBufferList*>(&buffers)) == noErr,
                "render");
        stamp.mSampleTime += 64;
    };
    bool allocationFree = true;
#ifdef SYS01_NATIVE_RT_PROBE
    sys01::probe::selfTest();
#endif
    for (unsigned i = 0; i < 2; ++i) {
        require(AudioComponentInstanceNew(component, &units[i]) == noErr, "instantiate");
        AURenderCallbackStruct callback{input, nullptr};
        require(AudioUnitSetProperty(units[i], kAudioUnitProperty_SetRenderCallback,
                                     kAudioUnitScope_Input, 0, &callback,
                                     sizeof(callback)) == noErr,
                "input callback");
        require(AudioUnitInitialize(units[i]) == noErr, "initialize");
        before(+[](void* c, void* p) { *static_cast<void**>(c) = p; }, &processors[i]);
        require(AudioUnitSetParameter(units[i], 452784731U, kAudioUnitScope_Global, 0, 0.2F, 0) ==
                    noErr,
                "accept native target");
        std::thread first([&] {
#ifdef SYS01_NATIVE_RT_PROBE
            sys01::probe::begin();
#endif
            render(units[i]);
#ifdef SYS01_NATIVE_RT_PROBE
            auto counts = sys01::probe::end();
            allocationFree &= counts.allocation == 0 && counts.deallocation == 0;
            std::printf("fresh AU instance %u alloc=%llu free=%llu mutex=%llu unfair=%llu\n", i,
                        counts.allocation, counts.deallocation, counts.mutex, counts.unfair);
            sys01::probe::printAllocation(counts);
#endif
        });
        first.join();
        before(nullptr, nullptr);
        require(processors[i] != nullptr, "capture processor");
    }
    struct Context {
        Write write;
        std::array<void*, 2> processors;
        std::atomic<unsigned> suppressed{0}, independent{0}, wrong{0};
        bool entered = false;
    } context{write, processors};
    observer(
        +[](void* c, void* p, int, bool suppressed) {
            auto& x = *static_cast<Context*>(c);
            if (suppressed)
                ++x.suppressed;
            else
                ++x.independent;
            if (p != x.processors[0] && suppressed)
                ++x.wrong;
        },
        &context);
    listener(
        processors[0],
        +[](void* c, void* p, int, float) {
            auto& x = *static_cast<Context*>(c);
            if (x.entered)
                return;
            x.entered = true;
            x.write(p, 0, 0.8F);
            x.write(p, 1, 0.7F);
            x.write(x.processors[1], 0, 0.6F); // same thread, different instance
            std::thread independent([&] { x.write(p, 1, 0.9F); });
            independent.join(); // fixture scheduling, outside allocation measurement
        },
        &context);
    require(AudioUnitSetParameter(units[0], 452784731U, kAudioUnitScope_Global, 0, 0.3F, 0) ==
                noErr,
            "nested native target");
    std::thread nested([&] { render(units[0]); });
    nested.join();
    listener(processors[0], nullptr, nullptr);
    const bool nestedOK = context.suppressed == 3 && context.independent == 2 && context.wrong == 0;
    std::printf("nested final suppressed=%u expected=3 independent=%u expected=2 wrong=%u %s\n",
                context.suppressed.load(), context.independent.load(), context.wrong.load(),
                nestedOK ? "PASS" : "FAIL");
    // A failed conditional publication must leave no residual suppression.
    context.suppressed = 0;
    context.independent = 0;
    before(+[](void* c, void* p) { static_cast<Context*>(c)->write(p, 0, 0.85F); }, &context);
    require(AudioUnitSetParameter(units[0], 452784731U, kAudioUnitScope_Global, 0, 0.1F, 0) ==
                noErr,
            "invalidated target");
    std::thread invalidated([&] {
        render(units[0]);
        write(processors[0], 1, 0.45F);
    });
    invalidated.join();
    before(nullptr, nullptr);
    const bool invalidatedOK = context.suppressed == 0 && context.independent == 2;
    std::printf("invalidated CAS independent=%u suppressed=%u %s\n", context.independent.load(),
                context.suppressed.load(), invalidatedOK ? "PASS" : "FAIL");
    // Legal Reset and inactive setters publish synchronously on lifecycle owner.
    context.suppressed = 0;
    context.independent = 0;
    require(AudioUnitSetParameter(units[0], 452784731U, kAudioUnitScope_Global, 0, 0.12F, 0) ==
                noErr,
            "reset target");
    require(AudioUnitReset(units[0], kAudioUnitScope_Global, 0) == noErr, "reset");
    require(AudioUnitUninitialize(units[0]) == noErr, "uninitialize");
    require(AudioUnitSetParameter(units[0], 452784731U, kAudioUnitScope_Global, 0, 0.14F, 0) ==
                noErr,
            "inactive setter");
    write(processors[0], 1, 0.46F);
    const bool lifecycleOK = context.suppressed == 2 && context.independent == 1;
    std::printf("lifecycle suppressed=%u independent=%u %s\n", context.suppressed.load(),
                context.independent.load(), lifecycleOK ? "PASS" : "FAIL");
    // Native reentry on the legal inactive lifecycle owner exercises scoped
    // predecessor restoration, including a second native publication of FLUX.
    struct NativeContext {
        AudioUnit unit;
        bool entered = false;
    } nativeContext{units[0]};
    context.suppressed = 0;
    context.independent = 0;
    listener(
        processors[0],
        +[](void* c, void*, int, float) {
            auto& x = *static_cast<NativeContext*>(c);
            if (x.entered)
                return;
            x.entered = true;
            require(AudioUnitSetParameter(x.unit, 452662059U, kAudioUnitScope_Global, 0, 0.15F,
                                          0) == noErr,
                    "nested inactive BIAS");
            require(AudioUnitSetParameter(x.unit, 452784731U, kAudioUnitScope_Global, 0, 0.16F,
                                          0) == noErr,
                    "nested inactive FLUX");
        },
        &nativeContext);
    require(AudioUnitSetParameter(units[0], 452784731U, kAudioUnitScope_Global, 0, 0.17F, 0) ==
                noErr,
            "outer inactive publication");
    listener(processors[0], nullptr, nullptr);
    write(processors[0], 1, 0.18F);
    const bool nativeNestingOK = context.suppressed == 3 && context.independent == 1;
    std::printf("native nesting suppressed=%u independent=%u expected=3/1 %s\n",
                context.suppressed.load(), context.independent.load(),
                nativeNestingOK ? "PASS" : "FAIL");
    // Concurrent instances render independently while their native scopes are
    // active. Each instance has its own buffers, timestamp and audio owner.
    context.suppressed = 0;
    context.independent = 0;
    context.wrong = 0;
    observer(
        +[](void* c, void*, int, bool suppressed) {
            auto& x = *static_cast<Context*>(c);
            if (suppressed)
                ++x.suppressed;
            else
                ++x.independent;
        },
        &context);
    require(AudioUnitInitialize(units[0]) == noErr, "reinitialize");
    std::array<std::thread, 2> workers;
    for (unsigned i = 0; i < workers.size(); ++i) {
        workers[i] = std::thread([&, i] {
            std::array<float, 64> l{}, r{};
            Buffers b{2, {{1, sizeof(l), l.data()}, {1, sizeof(r), r.data()}}};
            AudioTimeStamp t{};
            t.mFlags = kAudioTimeStampSampleTimeValid;
            t.mSampleTime = 100000;
            for (unsigned n = 0; n < 64; ++n) {
                require(AudioUnitSetParameter(units[i], 452784731U, kAudioUnitScope_Global, 0,
                                              n % 2 ? 0.2F : 0.3F, 0) == noErr,
                        "concurrent target");
                AudioUnitRenderActionFlags flags = 0;
                require(AudioUnitRender(units[i], &flags, &t, 0, 64,
                                        reinterpret_cast<AudioBufferList*>(&b)) == noErr,
                        "concurrent render");
                write(processors[i], 1, n % 2 ? 0.4F : 0.5F);
                t.mSampleTime += 64;
            }
        });
    }
    for (auto& worker : workers)
        worker.join();
    const bool concurrentOK = context.suppressed == 128 && context.independent == 128;
    std::printf("concurrent instances suppressed=%u independent=%u expected=128/128 %s\n",
                context.suppressed.load(), context.independent.load(),
                concurrentOK ? "PASS" : "FAIL");
    observer(nullptr, nullptr);
    for (auto unit : units)
        require(AudioComponentInstanceDispose(unit) == noErr, "dispose");
    CFRelease(bundle);
    return allocationFree && nestedOK && invalidatedOK && lifecycleOK && nativeNestingOK &&
                   concurrentOK
               ? 0
               : 1;
}
