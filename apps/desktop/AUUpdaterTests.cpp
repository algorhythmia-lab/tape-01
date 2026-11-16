// Exact loaded AU: real AudioProcessor::updateHostDisplay and wrapper callback.
#include "NativeEditorHost.hpp"

#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>
#include <array>
#include <atomic>
#include <chrono>
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
AudioComponentFactoryFunction realFactory = nullptr;
AudioComponentPlugInInterface* actualInterface = nullptr;
AudioComponentPlugInInterface* trackedFactory(const AudioComponentDescription* description) {
    actualInterface = static_cast<AudioComponentPlugInInterface*>(realFactory(description));
    return actualInterface;
}
using Change = void (*)(void*, unsigned);
struct Context {
    void* processor = nullptr;
    Change change = nullptr;
    std::array<unsigned, 5> calls{};
    bool nested = false;
    unsigned wrong = 0;
    std::thread::id message = std::this_thread::get_id();
};
constexpr std::array<AudioUnitPropertyID, 5> properties{
    kAudioUnitProperty_Latency, kAudioUnitProperty_ParameterList, kAudioUnitProperty_ParameterInfo,
    kAudioUnitProperty_ClassInfo, kAudioUnitProperty_PresentPreset};
void property(void* ref, AudioUnit, AudioUnitPropertyID id, AudioUnitScope scope,
              AudioUnitElement element) {
    auto& x = *static_cast<Context*>(ref);
    if (std::this_thread::get_id() != x.message || scope != kAudioUnitScope_Global || element != 0)
        ++x.wrong;
    for (unsigned i = 0; i < properties.size(); ++i)
        if (id == properties[i])
            ++x.calls[i];
    if (id == kAudioUnitProperty_Latency && x.nested) {
        x.nested = false;
        x.change(x.processor, 2);
    }
}
#ifdef SYS01_NATIVE_RT_PROBE
sys01::probe::Counts measured;
void probe(bool begin) {
    if (begin)
        sys01::probe::begin();
    else
        measured = sys01::probe::end();
}
#endif
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
    using Probe = void (*)(void (*)(bool));
    auto before = reinterpret_cast<Hook>(symbol("Sys01AUFixtureBeforeCaptureHook"));
    auto change = reinterpret_cast<Change>(symbol("Sys01AUFixtureProcessorChange"));
    [[maybe_unused]] auto observe = reinterpret_cast<Probe>(symbol("Sys01AUFixtureUpdaterProbe"));
    realFactory = reinterpret_cast<AudioComponentFactoryFunction>(symbol("JuceAUFactory"));
    AudioComponentDescription description{kAudioUnitType_MusicEffect, 'Sys1', 'Algr', 0, 0};
    auto component = AudioComponentRegister(&description, CFSTR("SYS 01 updater fixture"), 0x10000,
                                            trackedFactory);
    require(component != nullptr, "register AU");
    Context x;
    x.change = change;
    AudioUnit unit{};
    auto create = [&] {
        x.processor = nullptr;
        require(AudioComponentInstanceNew(component, &unit) == noErr, "instantiate");
        AURenderCallbackStruct callback{input, nullptr};
        require(AudioUnitSetProperty(unit, kAudioUnitProperty_SetRenderCallback,
                                     kAudioUnitScope_Input, 0, &callback,
                                     sizeof(callback)) == noErr,
                "input callback");
        require(AudioUnitInitialize(unit) == noErr, "initialize");
        before(+[](void* c, void* p) { static_cast<Context*>(c)->processor = p; }, &x);
        struct Buffers {
            UInt32 count;
            AudioBuffer b[2];
        };
        std::array<float, 64> left{}, right{};
        Buffers b{2, {{1, sizeof(left), left.data()}, {1, sizeof(right), right.data()}}};
        AudioTimeStamp stamp{};
        stamp.mFlags = kAudioTimeStampSampleTimeValid;
        AudioUnitRenderActionFlags flags = 0;
        std::thread first([&] {
            require(AudioUnitRender(unit, &flags, &stamp, 0, 64,
                                    reinterpret_cast<AudioBufferList*>(&b)) == noErr,
                    "first-thread render");
        });
        first.join();
        before(nullptr, nullptr);
        require(x.processor != nullptr, "captured actual processor");
    };
    auto destroy = [&] {
        require(AudioUnitUninitialize(unit) == noErr, "uninitialize");
        require(AudioComponentInstanceDispose(unit) == noErr, "dispose on message owner");
    };
    auto pump = [&](unsigned count) {
        auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);
        while (x.calls[3] < count && std::chrono::steady_clock::now() < deadline) {
            sys01PumpEditor();
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
        require(x.calls[3] >= count, "pending update delivered with editor closed");
    };
    create();
    for (auto id : properties)
        require(AudioUnitAddPropertyListener(unit, id, property, &x) == noErr, "add host listener");
#ifdef SYS01_NATIVE_RT_PROBE
    sys01::probe::selfTest();
    observe(probe);
#endif
    std::thread fresh([&] { change(x.processor, 7); });
    fresh.join();
    bool ok = true;
#ifdef SYS01_NATIVE_RT_PROBE
    observe(nullptr);
    std::printf("fresh complete wrapper updater alloc=%llu free=%llu mutex=%llu unfair=%llu\n",
                measured.allocation, measured.deallocation, measured.mutex, measured.unfair);
    sys01::probe::printAllocation(measured);
    ok &= measured.allocation == 0 && measured.deallocation == 0 && measured.mutex == 0 &&
          measured.unfair == 0;
#endif
    require(x.calls == std::array<unsigned, 5>{},
            "off-message callback only publishes dirty flags");
    pump(1);
    require(x.calls == std::array<unsigned, 5>{1, 1, 1, 1, 1},
            "all flags delivered in property order");
    x.nested = true;
    change(x.processor, 1);
    require(x.calls == std::array<unsigned, 5>{2, 2, 2, 3, 1}, "nested message change delivered");
    for (unsigned i = 0; i < 128; ++i) {
        std::thread publisher([&] { change(x.processor, 3); });
        publisher.join();
        pump(4 + i);
    }
    // Concurrent dirty publishers overlap actual message delivery. No host
    // callback or processor-listener mutation is performed by worker threads.
    std::atomic<unsigned> done{0};
    std::array<std::thread, 4> workers;
    const auto previous = x.calls;
    for (unsigned i = 0; i < workers.size(); ++i)
        workers[i] = std::thread([&, i] {
            for (unsigned j = 0; j < 256; ++j)
                change(x.processor, 1U << (i % 3));
            ++done;
        });
    while (done.load() != workers.size()) {
        sys01PumpEditor();
        std::this_thread::yield();
    }
    for (auto& worker : workers)
        worker.join();
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(80);
    while (std::chrono::steady_clock::now() < deadline) {
        sys01PumpEditor();
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    for (unsigned i = 0; i < properties.size(); ++i)
        require(x.calls[i] > previous[i], "concurrent flags reach message owner");
    std::printf("updater concurrent publishers=4x256/message polling PASS\n");
    require(x.wrong == 0, "message-owner callback metadata");
    std::printf("updater all flags/nested/128 fresh-thread deliveries PASS\n");
    // Both real host removal variants, then destroy pending updater with no
    // concurrent wrapper entry. Pump after each destruction to expose stale work.
    // Legacy selector is still shipped in SDK but the 64-bit public function
    // is unavailable. Invoke the real loaded instance's dispatch selector.
    auto removeAll = reinterpret_cast<AudioUnitRemovePropertyListenerProc>(
        actualInterface->Lookup(kAudioUnitRemovePropertyListenerSelect));
    require(removeAll(actualInterface, properties[0], property) == noErr, "remove without context");
    for (unsigned i = 1; i < properties.size(); ++i)
        require(AudioUnitRemovePropertyListenerWithUserData(unit, properties[i], property, &x) ==
                    noErr,
                "remove with context");
    destroy();
    for (unsigned i = 0; i < 64; ++i) {
        create();
        std::thread pending([&] { change(x.processor, 7); });
        pending.join();
        destroy();
        sys01PumpEditor();
    }
    std::printf("updater pending legal teardown/editor-closed cycles=64 PASS\n");
    CFRelease(bundle);
    return ok ? 0 : 1;
}
