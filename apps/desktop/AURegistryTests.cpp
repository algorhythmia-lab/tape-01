// Loads exact hook-ON or default bundle without installing an AU.
#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>
#include <array>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <pthread.h>
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
AudioComponentPlugInInterface* firstInterface = nullptr;
AudioComponentPlugInInterface* trackedFactory(const AudioComponentDescription* description) {
    auto* result = static_cast<AudioComponentPlugInInterface*>(realFactory(description));
    if (firstInterface == nullptr)
        firstInterface = result;
    return result;
}
#ifdef SYS01_NATIVE_RT_PROBE
sys01::probe::Counts registryCounts;
void registryProbe(bool begin) {
    if (begin)
        sys01::probe::begin();
    else
        registryCounts = sys01::probe::end();
}
#endif
} // namespace
int main(int argc, char** argv) {
    require(argc == 2, "supply exact AU bundle");
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
    realFactory = reinterpret_cast<AudioComponentFactoryFunction>(symbol("JuceAUFactory"));
    AudioComponentDescription description{kAudioUnitType_MusicEffect, 'Sys1', 'Algr', 0, 0};
    auto component = AudioComponentRegister(&description, CFSTR("SYS 01 notification fixture"),
                                            0x10000, trackedFactory);
    require(component != nullptr, "register AU");
    std::array<AudioUnit, 2> units{};
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
        std::thread first([&] { render(i); });
        first.join();
    }

    auto publish = [&](float value) {
        require(AudioUnitSetParameter(units[0], 773352680U, kAudioUnitScope_Global, 0, value, 0) ==
                    noErr,
                "native bypass publication");
        render(0);
    };
#ifdef SYS01_NATIVE_RT_PROBE
    sys01::probe::selfTest();
    std::thread measured([&] {
        sys01::probe::begin();
        publish(0.0F);
        const auto counts = sys01::probe::end();
        std::printf("fresh whole native bypass alloc=%llu free=%llu mutex=%llu unfair=%llu\n",
                    counts.allocation, counts.deallocation, counts.mutex, counts.unfair);
        sys01::probe::printAllocation(counts);
    });
    measured.join();
#endif
    // A host callback waits for another thread to add a different registration.
    // Watchdog releases the wait after a finite deadline, retaining an actual
    // loaded-bundle failure rather than hanging the session.
    struct CrossThread {
        std::atomic<bool> entered{false}, added{false}, release{false}, returned{false};
        std::atomic<unsigned> calls{0};
    } cross;
    auto crossTail = +[](void* c, AudioUnit, AudioUnitPropertyID, AudioUnitScope,
                         AudioUnitElement) { ++static_cast<CrossThread*>(c)->calls; };
    auto crossWait =
        +[](void* c, AudioUnit, AudioUnitPropertyID, AudioUnitScope, AudioUnitElement) {
            auto& x = *static_cast<CrossThread*>(c);
            x.entered = true;
            while (!x.added.load() && !x.release.load())
                std::this_thread::yield();
            x.returned = x.added.load();
        };
    require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_BypassEffect, crossWait,
                                         &cross) == noErr,
            "add cross-thread callback");
    std::thread waiting([&] { publish(1.0F); });
    while (!cross.entered.load())
        std::this_thread::yield();
    std::thread adding([&] {
        require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_BypassEffect, crossTail,
                                             &cross) == noErr,
                "cross-thread add");
        cross.added = true;
    });
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    while (!cross.added.load() && std::chrono::steady_clock::now() < deadline)
        std::this_thread::yield();
    cross.release = true;
    waiting.join();
    adding.join();
    bool crossOK = cross.returned && cross.calls == 0;
    require(AudioUnitRemovePropertyListenerWithUserData(units[0], kAudioUnitProperty_BypassEffect,
                                                        crossWait, &cross) == noErr,
            "remove cross-thread callback");
    publish(0.0F);
    crossOK &= cross.calls == 1;
    require(AudioUnitRemovePropertyListenerWithUserData(units[0], kAudioUnitProperty_BypassEffect,
                                                        crossTail, &cross) == noErr,
            "remove cross-thread tail");
    std::printf("cross-thread callback/add completed-before-return=%u calls=%u expected=1/1 %s\n",
                cross.returned.load(), cross.calls.load(), crossOK ? "PASS" : "FAIL");

    struct Registry {
        AudioUnit unit;
        std::atomic<unsigned> first{0}, removed{0}, tail{0}, late{0};
        std::atomic<bool> retired{false};
    } registry{units[0]};
    auto victim = +[](void* c, AudioUnit, AudioUnitPropertyID, AudioUnitScope, AudioUnitElement) {
        auto& x = *static_cast<Registry*>(c);
        ++x.removed;
        if (x.retired.load())
            ++x.late;
    };
    struct Remover {
        Registry* registry;
        AudioUnitPropertyListenerProc victim;
    } remover{&registry, victim};
    auto first =
        +[](void* c, AudioUnit unit, AudioUnitPropertyID id, AudioUnitScope, AudioUnitElement) {
            auto& r = *static_cast<Remover*>(c);
            ++r.registry->first;
            require(AudioUnitRemovePropertyListenerWithUserData(unit, id, r.victim, r.registry) ==
                        noErr,
                    "reentrant removal");
            r.registry->retired = true;
        };
    auto tail = +[](void* c, AudioUnit, AudioUnitPropertyID, AudioUnitScope, AudioUnitElement) {
        ++static_cast<Registry*>(c)->tail;
    };
    require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_BypassEffect, first,
                                         &remover) == noErr,
            "add remover");
    require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_BypassEffect, victim,
                                         &registry) == noErr,
            "add victim");
    require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_BypassEffect, tail,
                                         &registry) == noErr,
            "add tail");
    // Real native setter publication inside Render, not a synthetic PropertyChanged call.
    require(AudioUnitSetParameter(units[0], 773352680U, kAudioUnitScope_Global, 0, 1.0F, 0) ==
                noErr,
            "native bypass");
    std::thread audio([&] { render(0); });
    audio.join();
    bool ok =
        registry.first == 1 && registry.removed == 0 && registry.tail == 1 && registry.late == 0;
    std::printf("reentrant registry first=%u removed=%u tail=%u late=%u expected=1/0/1/0 %s\n",
                registry.first.load(), registry.removed.load(), registry.tail.load(),
                registry.late.load(), ok ? "PASS" : "FAIL");
    require(AudioUnitRemovePropertyListenerWithUserData(units[0], kAudioUnitProperty_BypassEffect,
                                                        first, &remover) == noErr,
            "remove remover");
    require(AudioUnitRemovePropertyListenerWithUserData(units[0], kAudioUnitProperty_BypassEffect,
                                                        tail, &registry) == noErr,
            "remove tail");

    struct Added {
        AudioUnit unit;
        unsigned first = 0, added = 0;
        bool registered = false;
    } added{units[0]};
    auto newListener = +[](void* c, AudioUnit, AudioUnitPropertyID, AudioUnitScope,
                           AudioUnitElement) { ++static_cast<Added*>(c)->added; };
    struct Adder {
        Added* x;
        AudioUnitPropertyListenerProc proc;
    } adder{&added, newListener};
    auto addListener =
        +[](void* c, AudioUnit unit, AudioUnitPropertyID id, AudioUnitScope, AudioUnitElement) {
            auto& x = *static_cast<Adder*>(c);
            ++x.x->first;
            if (!x.x->registered) {
                x.x->registered = true;
                // Force original-vector capacity growth. No fixed listener cap.
                for (unsigned i = 0; i < 257; ++i)
                    require(AudioUnitAddPropertyListener(unit, id, x.proc, x.x) == noErr,
                            "reentrant add");
            }
        };
    require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_BypassEffect, addListener,
                                         &adder) == noErr,
            "add adder");
    publish(0.0F);
    bool addOK = added.first == 1 && added.added == 0;
    publish(1.0F);
    addOK &= added.first == 2 && added.added == 257;
    require(AudioUnitRemovePropertyListenerWithUserData(units[0], kAudioUnitProperty_BypassEffect,
                                                        addListener, &adder) == noErr,
            "remove adder");
    require(AudioUnitRemovePropertyListenerWithUserData(units[0], kAudioUnitProperty_BypassEffect,
                                                        newListener, &added) == noErr,
            "remove all duplicates");
    publish(0.0F);
    addOK &= added.added == 257;
    std::printf("reentrant add snapshot first=%u added=%u expected=2/257 %s\n", added.first,
                added.added, addOK ? "PASS" : "FAIL");

    struct Held {
        std::atomic<bool> entered{false}, attempting{false}, release{false}, returned{false};
        std::atomic<bool> retired{false};
        std::atomic<unsigned> calls{0}, late{0};
    } held;
    auto heldListener =
        +[](void* c, AudioUnit, AudioUnitPropertyID, AudioUnitScope, AudioUnitElement) {
            auto& x = *static_cast<Held*>(c);
            ++x.calls;
            x.entered = true;
            // Fixture scheduling only; tests that off-audio removal acknowledges
            // an in-flight callback before its caller can destroy the context.
            while (!x.release.load())
                std::this_thread::yield();
            if (x.retired.load())
                ++x.late;
        };
    require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_BypassEffect, heldListener,
                                         &held) == noErr,
            "add held listener");
    std::thread publisher([&] { publish(1.0F); });
    while (!held.entered.load())
        std::this_thread::yield();
    std::thread removerThread([&] {
        held.attempting = true;
        require(AudioUnitRemovePropertyListenerWithUserData(
                    units[0], kAudioUnitProperty_BypassEffect, heldListener, &held) == noErr,
                "remove held listener");
        held.retired = true;
        held.returned = true;
    });
    while (!held.attempting.load())
        std::this_thread::yield();
    std::atomic<bool> secondAttempt{false}, secondReturned{false};
    std::thread secondRemover([&] {
        secondAttempt = true;
        require(AudioUnitRemovePropertyListenerWithUserData(
                    units[0], kAudioUnitProperty_BypassEffect, heldListener, &held) == noErr,
                "second removal acknowledges withdrawn frames");
        secondReturned = true;
    });
    while (!secondAttempt.load())
        std::this_thread::yield();
    // Finite scheduling check; ownership proof is the gate, not this delay.
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    bool waited = !held.returned.load() && !secondReturned.load();
    held.release = true;
    publisher.join();
    removerThread.join();
    secondRemover.join();
    publish(0.0F);
    bool heldOK = waited && held.returned && secondReturned && held.calls == 1 && held.late == 0;
    std::printf("concurrent removal waited=%u calls=%u late=%u expected=1/1/0 %s\n", waited,
                held.calls.load(), held.late.load(), heldOK ? "PASS" : "FAIL");

    // Independent SDK NickName storage and native bypass dispatch overlap.
    // These mutate distinct SDK/parameter fields, with one Render owner and
    // one off-audio property writer. Context counters support concurrent calls.
    struct Overlap {
        std::atomic<unsigned> entered{0}, finished{0};
        std::atomic<bool> release{false};
    } overlap;
    auto overlapListener =
        +[](void* c, AudioUnit, AudioUnitPropertyID, AudioUnitScope, AudioUnitElement) {
            auto& x = *static_cast<Overlap*>(c);
            ++x.entered;
            while (!x.release.load())
                std::this_thread::yield();
            ++x.finished;
        };
    for (auto id : {kAudioUnitProperty_BypassEffect, kAudioUnitProperty_NickName})
        require(AudioUnitAddPropertyListener(units[0], id, overlapListener, &overlap) == noErr,
                "add overlapping SDK/native contexts");
    std::thread nativeOverlap([&] { publish(1.0F); });
    while (overlap.entered == 0)
        std::this_thread::yield();
    std::thread sdkOverlap([&] {
        CFStringRef nickname = CFSTR("independent SDK notification");
        require(AudioUnitSetProperty(units[0], kAudioUnitProperty_NickName, kAudioUnitScope_Global,
                                     0, &nickname, sizeof(nickname)) == noErr,
                "SDK nickname overlapping native bypass");
    });
    const auto overlapDeadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
    while (overlap.entered < 2 && std::chrono::steady_clock::now() < overlapDeadline)
        std::this_thread::yield();
    bool overlapOK = overlap.entered == 2;
    overlap.release = true;
    nativeOverlap.join();
    sdkOverlap.join();
    for (auto id : {kAudioUnitProperty_BypassEffect, kAudioUnitProperty_NickName})
        require(AudioUnitRemovePropertyListenerWithUserData(units[0], id, overlapListener,
                                                            &overlap) == noErr,
                "remove overlapping SDK/native contexts");
    overlapOK &= overlap.finished == 2;
    publish(0.0F);
    std::printf("SDK/native concurrent callback frames entered=%u finished=%u expected=2/2 %s\n",
                overlap.entered.load(), overlap.finished.load(), overlapOK ? "PASS" : "FAIL");

    // Callback A removes B while B waits for A to return. A successful removal
    // cannot acknowledge B until B returns. Reject callback-origin waiting before
    // withdrawal; retain B and its context for an off-callback retry.
    bool cycleOK = true;
    for (auto testCase : {std::array{true, false}, std::array{false, false}, std::array{true, true},
                          std::array{false, true}}) {
        const auto specified = testCase[0], selfWithdraw = testCase[1];
        struct Cycle {
            AudioComponentPlugInInterface* interface;
            AudioUnitPropertyListenerProc target = nullptr;
            bool specified, selfWithdraw;
            std::atomic<bool> entered{false}, returned{false}, release{false};
            std::atomic<unsigned> calls{0};
            std::atomic<OSStatus> status{noErr};
        } cycle{firstInterface, nullptr, specified, selfWithdraw};
        auto target =
            +[](void* c, AudioUnit unit, AudioUnitPropertyID id, AudioUnitScope, AudioUnitElement) {
                auto& x = *static_cast<Cycle*>(c);
                ++x.calls;
                if (x.selfWithdraw)
                    require(AudioUnitRemovePropertyListenerWithUserData(unit, id, x.target, &x) ==
                                noErr,
                            "same-thread target withdraws but frame remains live");
                x.entered = true;
                while (!x.returned.load() && !x.release.load())
                    std::this_thread::yield();
            };
        auto removing =
            +[](void* c, AudioUnit unit, AudioUnitPropertyID, AudioUnitScope, AudioUnitElement) {
                auto& x = *static_cast<Cycle*>(c);
                while (!x.entered.load())
                    std::this_thread::yield();
                if (x.specified)
                    x.status = AudioUnitRemovePropertyListenerWithUserData(
                        unit, kAudioUnitProperty_NickName, x.target, &x);
                else {
                    auto removeAll = reinterpret_cast<AudioUnitRemovePropertyListenerProc>(
                        x.interface->Lookup(kAudioUnitRemovePropertyListenerSelect));
                    x.status = removeAll(x.interface, kAudioUnitProperty_NickName, x.target);
                }
                x.returned = true;
            };
        cycle.target = target;
        require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_BypassEffect, removing,
                                             &cycle) == noErr,
                "add callback-origin remover");
        require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_NickName, target,
                                             &cycle) == noErr,
                "add callback-origin target");
        std::thread sdk([&] {
            CFStringRef nickname = CFSTR("callback removal cycle");
            require(AudioUnitSetProperty(units[0], kAudioUnitProperty_NickName,
                                         kAudioUnitScope_Global, 0, &nickname,
                                         sizeof(nickname)) == noErr,
                    "SDK cycle target");
        });
        while (!cycle.entered.load())
            std::this_thread::yield();
        std::thread native([&] { publish(1.0F); });
        const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
        while (!cycle.returned.load() && std::chrono::steady_clock::now() < deadline)
            std::this_thread::yield();
        const bool returnedBeforeRelease = cycle.returned;
        cycle.release = true;
        sdk.join();
        native.join();
        require(AudioUnitRemovePropertyListenerWithUserData(
                    units[0], kAudioUnitProperty_BypassEffect, removing, &cycle) == noErr,
                "off-callback remover cleanup");
        CFStringRef nickname = CFSTR("failed removal retains target");
        require(AudioUnitSetProperty(units[0], kAudioUnitProperty_NickName, kAudioUnitScope_Global,
                                     0, &nickname, sizeof(nickname)) == noErr,
                "target remains after rejected removal");
        const bool passed = returnedBeforeRelease &&
                            cycle.status == kAudioUnitErr_CannotDoInCurrentContext &&
                            cycle.calls == (selfWithdraw ? 1U : 2U);
        std::printf("callback-origin removal specified=%u withdrawn=%u returned-before-release=%u "
                    "status=%d target-calls=%u expected=1/-10863/%u %s\n",
                    specified, selfWithdraw, returnedBeforeRelease, cycle.status.load(),
                    cycle.calls.load(), selfWithdraw ? 1U : 2U, passed ? "PASS" : "FAIL");
        cycleOK &= passed;
        require(AudioUnitRemovePropertyListenerWithUserData(units[0], kAudioUnitProperty_NickName,
                                                            target, &cycle) == noErr,
                "off-callback target retry");
        publish(0.0F);
    }

    // An older native frame exits while a newer SDK owner holds a nested stack.
    // Host waits are outside plugin measurements; the diagnostic counts actual
    // predecessor visits in native RAII unlink, not elapsed scheduling time.
    bool exitOK = true;
#ifdef SYS01_REGISTRY_EXIT_FIXTURE
    auto exitProbe = reinterpret_cast<void (*)(void (*)(AudioUnitPropertyID, std::size_t))>(
        symbol("Sys01AUFixtureRegistryExitProbe"));
    static std::atomic<std::size_t> exitVisits{0};
    auto countExit = +[](AudioUnitPropertyID id, std::size_t visited) {
        if (id == kAudioUnitProperty_BypassEffect)
            exitVisits = visited;
    };
    exitProbe(countExit);
#endif
    for (unsigned depth : {1U, 8U, 64U, 257U}) {
        struct ExitOrder {
            unsigned limit, depth = 0;
            std::atomic<bool> nativeEntered{false}, sdkEntered{false};
            std::atomic<bool> releaseNative{false}, releaseSDK{false};
            std::atomic<unsigned> calls{0};
        } order{depth};
        auto older =
            +[](void* c, AudioUnit, AudioUnitPropertyID, AudioUnitScope, AudioUnitElement) {
                auto& x = *static_cast<ExitOrder*>(c);
                x.nativeEntered = true;
                while (!x.releaseNative.load())
                    std::this_thread::yield();
            };
        auto newer =
            +[](void* c, AudioUnit unit, AudioUnitPropertyID id, AudioUnitScope, AudioUnitElement) {
                auto& x = *static_cast<ExitOrder*>(c);
                ++x.calls;
                if (++x.depth < x.limit) {
                    CFStringRef nickname = CFSTR("nested exit order");
                    require(AudioUnitSetProperty(unit, id, kAudioUnitScope_Global, 0, &nickname,
                                                 sizeof(nickname)) == noErr,
                            "nested exit SDK");
                } else {
                    x.sdkEntered = true;
                    while (!x.releaseSDK.load())
                        std::this_thread::yield();
                }
                --x.depth;
            };
        require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_BypassEffect, older,
                                             &order) == noErr,
                "add older exit");
        require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_NickName, newer,
                                             &order) == noErr,
                "add newer exit");
        std::thread native([&] { publish(1.0F); });
        while (!order.nativeEntered.load())
            std::this_thread::yield();
        auto sdkWork = [&] {
            CFStringRef nickname = CFSTR("exit overlap");
            require(AudioUnitSetProperty(units[0], kAudioUnitProperty_NickName,
                                         kAudioUnitScope_Global, 0, &nickname,
                                         sizeof(nickname)) == noErr,
                    "exit SDK owner");
        };
        // ASan's instrumented 257-level SDK recursion needs an explicit test
        // stack. This changes fixture storage only, never wrapper admission.
        pthread_attr_t attributes;
        require(pthread_attr_init(&attributes) == 0, "exit stack attributes");
        require(pthread_attr_setstacksize(&attributes, 8 * 1024 * 1024) == 0,
                "exit fixture stack size");
        pthread_t sdk;
        require(pthread_create(
                    &sdk, &attributes,
                    +[](void* work) -> void* {
                        (*static_cast<decltype(sdkWork)*>(work))();
                        return nullptr;
                    },
                    &sdkWork) == 0,
                "exit SDK thread");
        require(pthread_attr_destroy(&attributes) == 0, "exit stack attributes destroy");
        while (!order.sdkEntered.load())
            std::this_thread::yield();
        order.releaseNative = true;
        native.join(); // Older frame has returned; newer/ancestor frames remain.
        bool passed = order.calls == depth;
#ifdef SYS01_REGISTRY_EXIT_FIXTURE
        passed &= exitVisits == 0;
        std::printf("older native exit nested=%u predecessor-visits=%zu expected=0 %s\n", depth,
                    exitVisits.load(), passed ? "PASS" : "FAIL");
#endif
        // Both removers must still await all matching frames after older unlink.
        std::atomic<unsigned> attempts{0}, returns{0};
        auto remove = [&](bool specified) {
            ++attempts;
            OSStatus status;
            if (specified)
                status = AudioUnitRemovePropertyListenerWithUserData(
                    units[0], kAudioUnitProperty_NickName, newer, &order);
            else {
                auto removeAll = reinterpret_cast<AudioUnitRemovePropertyListenerProc>(
                    firstInterface->Lookup(kAudioUnitRemovePropertyListenerSelect));
                status = removeAll(firstInterface, kAudioUnitProperty_NickName, newer);
            }
            require(status == noErr, "exit order acknowledged removal");
            ++returns;
        };
        std::thread firstRemove([&] { remove(true); });
        std::thread secondRemove([&] { remove(false); });
        while (attempts < 2)
            std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        passed &= returns == 0;
        order.releaseSDK = true;
        require(pthread_join(sdk, nullptr) == 0, "exit SDK join");
        firstRemove.join();
        secondRemove.join();
        passed &= returns == 2 && order.depth == 0;
        require(AudioUnitRemovePropertyListenerWithUserData(
                    units[0], kAudioUnitProperty_BypassEffect, older, &order) == noErr,
                "remove older exit");
        publish(0.0F);
        std::printf("exit order nested=%u calls=%u acknowledged=%u expected=%u/2 %s\n", depth,
                    order.calls.load(), returns.load(), depth, passed ? "PASS" : "FAIL");
        exitOK &= passed;
    }
#ifdef SYS01_REGISTRY_EXIT_FIXTURE
    exitProbe(nullptr);
#endif

    // Both real SDK removal selectors; arbitrary nesting uses intrusive frames,
    // keeps same-thread active/ancestor contexts alive, and skips withdrawn tails.
    struct Nested {
        AudioUnit unit;
        AudioComponentPlugInInterface* interface;
        unsigned depth = 0, calls = 0, tail = 0;
        AudioUnitPropertyListenerProc self = nullptr, tailProc = nullptr;
    } nested{units[0], firstInterface};
    auto nestedTail = +[](void* c, AudioUnit, AudioUnitPropertyID, AudioUnitScope,
                          AudioUnitElement) { ++static_cast<Nested*>(c)->tail; };
    auto nestedListener =
        +[](void* c, AudioUnit unit, AudioUnitPropertyID id, AudioUnitScope, AudioUnitElement) {
            auto& x = *static_cast<Nested*>(c);
            ++x.calls;
            if (++x.depth < 64) {
                CFStringRef name = CFSTR("nested SYS 01");
                require(AudioUnitSetProperty(unit, id, kAudioUnitScope_Global, 0, &name,
                                             sizeof(name)) == noErr,
                        "nested ContextName property dispatch");
            } else {
                auto removeAll = reinterpret_cast<AudioUnitRemovePropertyListenerProc>(
                    x.interface->Lookup(kAudioUnitRemovePropertyListenerSelect));
                require(removeAll(x.interface, id, x.tailProc) == noErr, "legacy tail withdrawal");
                require(AudioUnitRemovePropertyListenerWithUserData(unit, id, x.self, &x) == noErr,
                        "same-thread active/ancestor withdrawal");
            }
            --x.depth;
        };
    nested.self = nestedListener;
    nested.tailProc = nestedTail;
    require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_ContextName, nestedListener,
                                         &nested) == noErr,
            "add nested ContextName listener");
    require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_ContextName, nestedTail,
                                         &nested) == noErr,
            "add nested tail duplicate 1");
    require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_ContextName, nestedTail,
                                         &nested) == noErr,
            "add nested tail duplicate 2");
    CFStringRef name = CFSTR("SYS 01 registry ownership");
    require(AudioUnitSetProperty(units[0], kAudioUnitProperty_ContextName, kAudioUnitScope_Global,
                                 0, &name, sizeof(name)) == noErr,
            "real SDK ContextName dispatch/wrapper-this listener");
    bool nestedOK = nested.calls == 64 && nested.tail == 0;
    auto removeAll = reinterpret_cast<AudioUnitRemovePropertyListenerProc>(
        firstInterface->Lookup(kAudioUnitRemovePropertyListenerSelect));
    require(removeAll(firstInterface, kAudioUnitProperty_ContextName, nestedTail) == noErr,
            "legacy remove all duplicate contexts");
    require(AudioUnitRemovePropertyListenerWithUserData(units[0], kAudioUnitProperty_ContextName,
                                                        nestedListener, &nested) == noErr,
            "remove nested with context");
    require(AudioUnitSetProperty(units[0], kAudioUnitProperty_ContextName, kAudioUnitScope_Global,
                                 0, &name, sizeof(name)) == noErr,
            "ContextName wrapper-this survives host removal");
    nestedOK &= nested.calls == 64 && nested.tail == 0;
    std::printf("SDK ContextName nesting/legacy duplicates calls=%u tail=%u expected=64/0 %s\n",
                nested.calls, nested.tail, nestedOK ? "PASS" : "FAIL");
#ifdef SYS01_NATIVE_RT_PROBE
    auto observe =
        reinterpret_cast<void (*)(void (*)(bool))>(symbol("Sys01AUFixtureRegistryProbe"));
    std::atomic<unsigned> probeCalls{0};
    auto probeListener = +[](void* c, AudioUnit, AudioUnitPropertyID, AudioUnitScope,
                             AudioUnitElement) { ++*static_cast<std::atomic<unsigned>*>(c); };
    require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_BypassEffect, probeListener,
                                         &probeCalls) == noErr,
            "add measured host callback");
    observe(registryProbe);
    std::thread firstRegistry([&] { publish(1.0F); });
    firstRegistry.join();
    observe(nullptr);
    require(AudioUnitRemovePropertyListenerWithUserData(units[0], kAudioUnitProperty_BypassEffect,
                                                        probeListener, &probeCalls) == noErr,
            "remove measured host callback");
    nestedOK &= probeCalls == 1;
    std::printf("fresh complete registry callback alloc=%llu free=%llu mutex=%llu unfair=%llu\n",
                registryCounts.allocation, registryCounts.deallocation, registryCounts.mutex,
                registryCounts.unfair);
    sys01::probe::printAllocation(registryCounts);
    // Blocking gate retained: characterize locks, require no reader reclamation.
    nestedOK &= registryCounts.allocation == 0 && registryCounts.deallocation == 0;
    publish(0.0F);
#endif

    struct Life {
        std::atomic<unsigned>* calls;
    };
    std::atomic<unsigned> lifeCalls{0};
    auto lifeListener = +[](void* c, AudioUnit, AudioUnitPropertyID, AudioUnitScope,
                            AudioUnitElement) { ++*static_cast<Life*>(c)->calls; };
    for (unsigned i = 0; i < 128; ++i) {
        auto life = std::make_unique<Life>(Life{&lifeCalls});
        require(AudioUnitAddPropertyListener(units[0], kAudioUnitProperty_BypassEffect,
                                             lifeListener, life.get()) == noErr,
                "add life");
        std::thread owner([&] { publish(1.0F); });
        owner.join();
        require(AudioUnitRemovePropertyListenerWithUserData(
                    units[0], kAudioUnitProperty_BypassEffect, lifeListener, life.get()) == noErr,
                "remove life");
        life.reset();
        std::thread after([&] { publish(0.0F); });
        after.join();
    }
    bool lifeOK = lifeCalls == 128;
    std::printf("context destruction fresh-thread callbacks=%u expected=128 %s\n", lifeCalls.load(),
                lifeOK ? "PASS" : "FAIL");
    ok &= crossOK && addOK && heldOK && overlapOK && cycleOK && exitOK && nestedOK && lifeOK;
    for (auto unit : units) {
        require(AudioUnitUninitialize(unit) == noErr, "uninitialize");
        require(AudioComponentInstanceDispose(unit) == noErr, "dispose");
    }
    CFRelease(bundle);
    return ok ? 0 : 1;
}
