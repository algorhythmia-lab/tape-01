// Loads the exact diagnostic bundle without installing an AU.
#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>
#include <array>
#include <atomic>
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
using Write = void (*)(void*, int, float);
struct Context {
    Write write;
    std::array<void*, 2> processors{};
    std::atomic<unsigned> events{0}, properties{0}, other{0}, wrong{0};
    std::atomic<bool> entered{false}, finished{false};
};
void property(void* ref, AudioUnit, AudioUnitPropertyID id, AudioUnitScope scope,
              AudioUnitElement element) {
    auto& x = *static_cast<Context*>(ref);
    if (id != kAudioUnitProperty_BypassEffect || scope != kAudioUnitScope_Global || element != 0)
        ++x.wrong;
    ++x.properties; // host-owned callback has no resource/message work
}
} // namespace
int main(int argc, char** argv) {
    require(argc == 2, "supply exact hook-ON AU bundle");
    auto url = CFURLCreateFromFileSystemRepresentation(
        nullptr, reinterpret_cast<const UInt8*>(argv[1]), std::strlen(argv[1]), true);
    auto bundle = CFBundleCreate(nullptr, url);
    CFRelease(url);
    require(bundle && CFBundleLoadExecutable(bundle), "load AU");
    auto symbol = [&](const char* name) {
        auto key = CFStringCreateWithCString(nullptr, name, kCFStringEncodingUTF8);
        auto result = CFBundleGetFunctionPointerForName(bundle, key);
        CFRelease(key);
        require(result != nullptr, name);
        return result;
    };
    using Hook = void (*)(void (*)(void*, void*), void*);
    using Listener = void (*)(void*, void (*)(void*, void*, int, float), void*);
    using EventHook = void (*)(void (*)(void*, void*, const AudioUnitEvent*), void*);
    using Gesture = void (*)(void*, int, bool);
    auto before = reinterpret_cast<Hook>(symbol("Sys01AUFixtureBeforeCaptureHook"));
    auto write = reinterpret_cast<Write>(symbol("Sys01AUFixtureEditorWrite"));
    auto listener = reinterpret_cast<Listener>(symbol("Sys01AUFixtureListener"));
    auto events = reinterpret_cast<EventHook>(symbol("Sys01AUFixtureEventHook"));
    auto gesture = reinterpret_cast<Gesture>(symbol("Sys01AUFixtureGesture"));
    auto factory = reinterpret_cast<AudioComponentFactoryFunction>(symbol("JuceAUFactory"));
    AudioComponentDescription description{kAudioUnitType_MusicEffect, 'Sys1', 'Algr', 0, 0};
    auto component = AudioComponentRegister(&description, CFSTR("SYS 01 notification fixture"),
                                            0x10000, factory);
    require(component != nullptr, "register AU");
    std::array<AudioUnit, 2> units{};
    Context context{write};
    struct Buffers {
        UInt32 count = 2;
        AudioBuffer buffers[2]{};
    };
    std::array<float, 64> left{}, right{};
    Buffers b{2, {{1, sizeof(left), left.data()}, {1, sizeof(right), right.data()}}};
    AudioTimeStamp stamp{};
    stamp.mFlags = kAudioTimeStampSampleTimeValid;
    auto render = [&](unsigned i) {
        AudioUnitRenderActionFlags flags = 0;
        require(AudioUnitRender(units[i], &flags, &stamp, 0, 64,
                                reinterpret_cast<AudioBufferList*>(&b)) == noErr,
                "render");
        stamp.mSampleTime += 64;
    };
    for (unsigned i = 0; i < units.size(); ++i) {
        require(AudioComponentInstanceNew(component, &units[i]) == noErr, "instantiate");
        AURenderCallbackStruct callback{input, nullptr};
        require(AudioUnitSetProperty(units[i], kAudioUnitProperty_SetRenderCallback,
                                     kAudioUnitScope_Input, 0, &callback,
                                     sizeof(callback)) == noErr,
                "input callback");
        require(AudioUnitInitialize(units[i]) == noErr, "initialize");
        before(+[](void* c, void* p) { *static_cast<void**>(c) = p; }, &context.processors[i]);
        std::thread first([&] { render(i); });
        first.join();
        before(nullptr, nullptr);
        require(context.processors[i] != nullptr, "processor");
    }
    CFPropertyListRef state = nullptr;
    UInt32 size = sizeof(state);
    require(AudioUnitGetProperty(units[0], kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global, 0,
                                 &state, &size) == noErr &&
                state != nullptr,
            "save ClassInfo");
    require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_BypassEffect, property,
                                         &context) == noErr,
            "register bypass host listener");
    events(
        +[](void* c, void* p, const AudioUnitEvent* e) {
            auto& x = *static_cast<Context*>(c);
            if (e->mEventType != kAudioUnitEvent_ParameterValueChange ||
                e->mArgument.mParameter.mScope != kAudioUnitScope_Global ||
                e->mArgument.mParameter.mElement != 0)
                ++x.wrong;
            if (p == x.processors[1])
                ++x.other;
            else
                ++x.events;
        },
        &context);
    listener(
        context.processors[0],
        +[](void* c, void*, int, float) {
            auto& x = *static_cast<Context*>(c);
            if (x.entered.exchange(true))
                return;
            // Only fixture scheduling waits. Production callbacks have no wait.
            while (!x.finished.load(std::memory_order_acquire))
                std::this_thread::yield();
        },
        &context);
    std::thread external([&] {
        while (!context.entered.load(std::memory_order_acquire))
            std::this_thread::yield();
        write(context.processors[0], 1, 0.73F); // different parameter listener lock
        write(context.processors[0], 6, 1.0F);
        write(context.processors[1], 1, 0.61F);
        context.finished.store(true, std::memory_order_release);
    });
    require(AudioUnitSetProperty(units[0], kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global, 0,
                                 &state, sizeof(state)) == noErr,
            "restore ClassInfo");
    external.join();
    listener(context.processors[0], nullptr, nullptr);
    bool restoreOK =
        context.events == 2 && context.properties == 1 && context.other == 1 && context.wrong == 0;
    std::printf("overlap restore external events=%u expected=2 bypass-properties=%u expected=1 "
                "other-instance=%u expected=1 wrong=%u %s\n",
                context.events.load(), context.properties.load(), context.other.load(),
                context.wrong.load(), restoreOK ? "PASS" : "FAIL");
    context.events = 0;
    context.properties = 0;
    context.other = 0;
    write(context.processors[0], 1, 0.42F);
    write(context.processors[0], 6, 1.0F);
    bool afterOK = context.events == 2 && context.properties == 1 && context.wrong == 0;
    std::printf("after restore events=%u properties=%u expected=2/1 %s\n", context.events.load(),
                context.properties.load(), afterOK ? "PASS" : "FAIL");
    struct Nested {
        Write write;
        void* processor;
        bool entered = false;
        unsigned count = 0;
        bool intact = true;
    } nested{write, context.processors[0]};
    events(
        +[](void* c, void*, const AudioUnitEvent* e) {
            auto& x = *static_cast<Nested*>(c);
            ++x.count;
            if (x.entered)
                return;
            x.entered = true;
            const auto original = *e;
            x.write(x.processor, 1, 0.66F);
            // This pointer is valid for the complete synchronous call. The
            // original event must still be FLUX when AUEventListenerNotify runs.
            x.intact =
                e->mEventType == original.mEventType &&
                e->mArgument.mParameter.mParameterID ==
                    original.mArgument.mParameter.mParameterID &&
                e->mArgument.mParameter.mAudioUnit == original.mArgument.mParameter.mAudioUnit;
        },
        &nested);
    write(context.processors[0], 0, 0.31F);
    bool nestedOK = nested.count == 2 && nested.intact;
    std::printf("nested event payload intact=%u events=%u expected=1/2 %s\n", nested.intact,
                nested.count, nestedOK ? "PASS" : "FAIL");
    struct Concurrent {
        AudioUnit unit;
        std::array<AudioUnitParameterID, 7> ids{452784731U,  452662059U,  1763717364U, 834166325U,
                                                1194780756U, 1323076768U, 773352680U};
        std::array<std::atomic<unsigned>, 7> values{}, begins{}, ends{};
        std::atomic<unsigned> wrong{0};
    } concurrent{units[0]};
    // IDs are verified against the pinned catalogue by the strict AU oracle.
    events(
        +[](void* c, void*, const AudioUnitEvent* e) {
            auto& x = *static_cast<Concurrent*>(c);
            unsigned i = 0;
            while (i < x.ids.size() && x.ids[i] != e->mArgument.mParameter.mParameterID)
                ++i;
            if (i == x.ids.size() || e->mArgument.mParameter.mAudioUnit != x.unit ||
                e->mArgument.mParameter.mScope != kAudioUnitScope_Global ||
                e->mArgument.mParameter.mElement != 0) {
                ++x.wrong;
                return;
            }
            if (e->mEventType == kAudioUnitEvent_ParameterValueChange)
                ++x.values[i];
            else if (e->mEventType == kAudioUnitEvent_BeginParameterChangeGesture)
                ++x.begins[i];
            else if (e->mEventType == kAudioUnitEvent_EndParameterChangeGesture)
                ++x.ends[i];
            else
                ++x.wrong;
        },
        &concurrent);
    std::array<std::thread, 7> workers;
    for (unsigned i = 0; i < workers.size(); ++i)
        workers[i] = std::thread([&, i] {
            for (unsigned n = 0; n < 128; ++n) {
                gesture(context.processors[0], i, true);
                write(context.processors[0], i, n % 2 ? 0.0F : 1.0F);
                gesture(context.processors[0], i, false);
            }
        });
    for (auto& worker : workers)
        worker.join();
    bool concurrentOK = concurrent.wrong == 0;
    for (unsigned i = 0; i < workers.size(); ++i)
        concurrentOK &=
            concurrent.values[i] == 128 && concurrent.begins[i] == 128 && concurrent.ends[i] == 128;
    std::printf("concurrent seven-key value/begin/end 128 each wrong=%u %s\n",
                concurrent.wrong.load(), concurrentOK ? "PASS" : "FAIL");
    events(nullptr, nullptr);
    require(AudioUnitRemovePropertyListenerWithUserData(units[0], kAudioUnitProperty_BypassEffect,
                                                        property, &context) == noErr,
            "remove host listener");
    // Registry mutation is performed only with all callbacks quiescent. This
    // tests the retained legal lifetime boundary, not concurrent reclamation.
    context.properties = 0;
    for (unsigned n = 0; n < 64; ++n) {
        require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_BypassEffect, property,
                                             &context) == noErr,
                "attach host listener");
        std::thread callback([&] { write(context.processors[0], 6, n % 2 ? 0.0F : 1.0F); });
        callback.join();
        require(AudioUnitRemovePropertyListenerWithUserData(
                    units[0], kAudioUnitProperty_BypassEffect, property, &context) == noErr,
                "detach host listener");
        write(context.processors[0], 6, 0.0F);
    }
    bool lifetimeOK = context.properties == 64;
    std::printf("quiescent property listener attach/detach callbacks=%u expected=64 %s\n",
                context.properties.load(), lifetimeOK ? "PASS" : "FAIL");
#ifdef SYS01_NATIVE_RT_PROBE
    sys01::probe::selfTest();
    std::thread measured([&] {
        sys01::probe::begin();
        write(context.processors[0], 6, 1.0F);
        const auto counts = sys01::probe::end();
        std::printf(
            "fresh external bypass notification alloc=%llu free=%llu mutex=%llu unfair=%llu\n",
            counts.allocation, counts.deallocation, counts.mutex, counts.unfair);
        sys01::probe::printAllocation(counts);
    });
    measured.join();
#endif
    // Drain queued restore before legal lifecycle teardown.
    std::thread drain([&] { render(0); });
    drain.join();
    CFRelease(state);
    for (auto unit : units) {
        require(AudioUnitUninitialize(unit) == noErr, "uninitialize");
        require(AudioComponentInstanceDispose(unit) == noErr, "dispose");
    }
    CFRelease(bundle);
    return restoreOK && afterOK && nestedOK && concurrentOK && lifetimeOK ? 0 : 1;
}
