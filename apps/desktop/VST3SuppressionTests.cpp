// Loads the built format binary and calls Steinberg's actual process interface.
// This intentionally does not instantiate our C++ processor directly.
#define INIT_CLASS_IID
#ifdef SYS01_NATIVE_RT_PROBE
#include "RealtimeProbe.hpp"
#endif
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/base/ipluginbase.h>
#include <pluginterfaces/gui/iplugview.h>
#include <pluginterfaces/vst/ivstaudioprocessor.h>
#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <pluginterfaces/vst/ivstevents.h>
#include <pluginterfaces/vst/ivstmessage.h>
#include <pluginterfaces/vst/ivstmidicontrollers.h>
#include <pluginterfaces/vst/ivstparameterchanges.h>
#include <pluginterfaces/vst/vstspeaker.h>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#if defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#else
#include <windows.h>
#endif
using namespace Steinberg;
namespace {
void require(bool value, const char* message) {
    if (!value) {
        std::fprintf(stderr, "%s\n", message);
        std::exit(1);
    }
}
struct Point {
    int32 offset;
    double value;
};
class Values final : public Vst::IParamValueQueue {
public:
    Vst::ParamID id = 0;
    std::array<Point, 300> points{};
    int32 count = 0;
    tresult PLUGIN_API queryInterface(const TUID, void** obj) override {
        *obj = nullptr;
        return kNoInterface;
    }
    uint32 PLUGIN_API addRef() override {
        return 1;
    }
    uint32 PLUGIN_API release() override {
        return 1;
    }
    Vst::ParamID PLUGIN_API getParameterId() override {
        return id;
    }
    int32 PLUGIN_API getPointCount() override {
        return count;
    }
    tresult PLUGIN_API getPoint(int32 i, int32& offset, double& value) override {
        if (i < 0 || i >= count)
            return kInvalidArgument;
        offset = points[static_cast<std::size_t>(i)].offset;
        value = points[static_cast<std::size_t>(i)].value;
        return kResultOk;
    }
    tresult PLUGIN_API addPoint(int32, double, int32&) override {
        return kNotImplemented;
    }
};
class Changes final : public Vst::IParameterChanges {
public:
    std::array<Values, 7> values{};
    int32 count = 5;
    tresult PLUGIN_API queryInterface(const TUID, void** obj) override {
        *obj = nullptr;
        return kNoInterface;
    }
    uint32 PLUGIN_API addRef() override {
        return 1;
    }
    uint32 PLUGIN_API release() override {
        return 1;
    }
    int32 PLUGIN_API getParameterCount() override {
        return count;
    }
    Vst::IParamValueQueue* PLUGIN_API getParameterData(int32 i) override {
        return &values[static_cast<std::size_t>(i)];
    }
    Vst::IParamValueQueue* PLUGIN_API addParameterData(const Vst::ParamID&, int32&) override {
        return nullptr;
    }
};
class StateStream final : public IBStream {
public:
    std::array<char, 4096> bytes{};
    int64 position = 0, length = 0;
    tresult PLUGIN_API queryInterface(const TUID, void** obj) override {
        *obj = nullptr;
        return kNoInterface;
    }
    uint32 PLUGIN_API addRef() override {
        return 1;
    }
    uint32 PLUGIN_API release() override {
        return 1;
    }
    tresult PLUGIN_API read(void* dst, int32 count, int32* read = nullptr) override {
        if (count < 0)
            return kInvalidArgument;
        const auto n = std::min<int64>(count, length - position);
        std::memcpy(dst, bytes.data() + position, static_cast<std::size_t>(n));
        position += n;
        if (read)
            *read = static_cast<int32>(n);
        return kResultOk;
    }
    tresult PLUGIN_API write(void* src, int32 count, int32* written = nullptr) override {
        if (count < 0 || position + count > static_cast<int64>(bytes.size()))
            return kInvalidArgument;
        std::memcpy(bytes.data() + position, src, static_cast<std::size_t>(count));
        position += count;
        length = std::max(length, position);
        if (written)
            *written = count;
        return kResultOk;
    }
    tresult PLUGIN_API seek(int64 offset, int32 mode, int64* result = nullptr) override {
        const auto next = offset + (mode == kIBSeekCur   ? position
                                    : mode == kIBSeekEnd ? length
                                                         : 0);
        if (next < 0 || next > length)
            return kInvalidArgument;
        position = next;
        if (result)
            *result = position;
        return kResultOk;
    }
    tresult PLUGIN_API tell(int64* result) override {
        if (result)
            *result = position;
        return kResultOk;
    }
};
} // namespace

int main(int argc, char** argv) {
    require(argc == 2, "supply exact hook-ON VST3 bundle");
    auto url =
        CFURLCreateFromFileSystemRepresentation(nullptr, reinterpret_cast<const UInt8*>(argv[1]),
                                                static_cast<CFIndex>(std::strlen(argv[1])), true);
    auto module = CFBundleCreate(nullptr, url);
    CFRelease(url);
    require(module && CFBundleLoadExecutable(module), "load VST3");
    const auto symbol = [&](const char* name) {
        auto key = CFStringCreateWithCString(nullptr, name, kCFStringEncodingUTF8);
        auto ptr = CFBundleGetFunctionPointerForName(module, key);
        CFRelease(key);
        require(ptr != nullptr, name);
        return ptr;
    };
    require(reinterpret_cast<bool (*)(CFBundleRef)>(symbol("bundleEntry"))(module), "bundle entry");
    auto* factory = reinterpret_cast<IPluginFactory* (*)()>(symbol("GetPluginFactory"))();
    using Observer = void (*)(void (*)(void*, void*, int, bool), void*);
    using Processor = void* (*)(Vst::IComponent*);
    using Write = void (*)(void*, int, float);
    using Listener = void* (*)(void*, int, void (*)(void*, void*, int, float), void*, void*);
    auto observer = reinterpret_cast<Observer>(symbol("Sys01VST3FixtureObserver"));
    auto processor = reinterpret_cast<Processor>(symbol("Sys01VST3FixtureProcessor"));
    auto write = reinterpret_cast<Write>(symbol("Sys01VST3FixtureWrite"));
    auto listener = reinterpret_cast<Listener>(symbol("Sys01VST3FixtureListener"));
    struct Instance {
        Vst::IComponent* component = nullptr;
        Vst::IAudioProcessor* audio = nullptr;
        Vst::IEditController* controller = nullptr;
        Vst::IConnectionPoint *cc = nullptr, *ec = nullptr;
        void* processor = nullptr;
        std::array<Vst::ParamID, 7> ids{};
        Changes changes;
        std::array<float, 64> left{}, right{};
        std::array<float*, 2> channels{left.data(), right.data()};
        Vst::AudioBusBuffers out{};
        Vst::ProcessData data{};
    };
    std::array<Instance, 2> instances;
    for (auto& x : instances) {
        for (int32 i = 0; i < factory->countClasses(); ++i) {
            PClassInfo info{};
            factory->getClassInfo(i, &info);
            if (std::strcmp(info.category, kVstAudioEffectClass) == 0)
                require(factory->createInstance(info.cid, Vst::IComponent::iid,
                                                reinterpret_cast<void**>(&x.component)) ==
                            kResultOk,
                        "component");
        }
        require(x.component && x.component->initialize(nullptr) == kResultOk, "initialize");
        require(x.component->queryInterface(Vst::IAudioProcessor::iid,
                                            reinterpret_cast<void**>(&x.audio)) == kResultOk,
                "audio");
        TUID id{};
        x.component->getControllerClassId(id);
        require(factory->createInstance(id, Vst::IEditController::iid,
                                        reinterpret_cast<void**>(&x.controller)) == kResultOk,
                "controller");
        require(x.controller->initialize(nullptr) == kResultOk, "controller initialize");
        x.component->queryInterface(Vst::IConnectionPoint::iid, reinterpret_cast<void**>(&x.cc));
        x.controller->queryInterface(Vst::IConnectionPoint::iid, reinterpret_cast<void**>(&x.ec));
        require(x.cc->connect(x.ec) == kResultOk && x.ec->connect(x.cc) == kResultOk, "connect");
        x.processor = processor(x.component);
        require(x.processor, "shared processor");
        for (unsigned i = 0; i < x.ids.size(); ++i) {
            Vst::ParameterInfo info{};
            x.controller->getParameterInfo(static_cast<int32>(i), info);
            x.ids[i] = info.id;
        }
        // Stable catalogue order FLUX/BIAS/DRIVE/STATE/DOMAIN/OUTPUT/BYPASS.
        require(x.ids[0] == 452784731U && x.ids[1] == 452662059U, "stable IDs");
        Vst::SpeakerArrangement layout = Vst::SpeakerArr::kStereo;
        require(x.audio->setBusArrangements(&layout, 1, &layout, 1) == kResultOk, "layout");
        Vst::ProcessSetup setup{Vst::kRealtime, Vst::kSample32, 64, 48000};
        require(x.audio->setupProcessing(setup) == kResultOk, "setup");
        x.component->activateBus(Vst::kAudio, Vst::kOutput, 0, true);
        require(x.component->setActive(true) == kResultOk &&
                    x.audio->setProcessing(true) == kResultOk,
                "activate");
        x.changes.count = 1;
        x.changes.values[0].id = x.ids[0];
        x.changes.values[0].count = 1;
        x.changes.values[0].points[0] = {0, 0.2};
        x.out.numChannels = 2;
        x.out.channelBuffers32 = x.channels.data();
        x.data.symbolicSampleSize = Vst::kSample32;
        x.data.numSamples = 64;
        x.data.numOutputs = 1;
        x.data.outputs = &x.out;
        x.data.inputParameterChanges = &x.changes;
    }
    struct Counts {
        std::array<void*, 2> processors;
        std::array<std::atomic<unsigned>, 2> suppressed{}, independent{};
        void clear() {
            for (unsigned i = 0; i < 2; ++i) {
                suppressed[i] = 0;
                independent[i] = 0;
            }
        }
    } counts{{instances[0].processor, instances[1].processor}};
    observer(
        +[](void* context, void* p, int, bool suppressed) {
            auto& c = *static_cast<Counts*>(context);
            const unsigned i = p == c.processors[0] ? 0 : 1;
            (suppressed ? c.suppressed[i] : c.independent[i]).fetch_add(1);
        },
        &counts);
    bool passed = true;
#ifdef SYS01_NATIVE_RT_PROBE
    sys01::probe::selfTest();
#endif
    for (unsigned i = 0; i < 2; ++i) {
        std::thread first([&] {
#ifdef SYS01_NATIVE_RT_PROBE
            sys01::probe::begin();
#endif
            require(instances[i].audio->process(instances[i].data) == kResultOk, "fresh process");
#ifdef SYS01_NATIVE_RT_PROBE
            const auto c = sys01::probe::end();
            std::printf("fresh VST3 instance %u alloc=%llu free=%llu mutex=%llu unfair=%llu\n", i,
                        c.allocation, c.deallocation, c.mutex, c.unfair);
            if (c.allocation)
                sys01::probe::printAllocation(c);
            passed &= c.allocation == 0 && c.deallocation == 0;
#endif
        });
        first.join();
    }
    // Actual stopped controller publication reenters a second native controller
    // publication, then external same-instance and cross-instance writers.
    for (auto& x : instances)
        require(x.audio->setProcessing(false) == kResultOk, "stop");
    counts.clear();
    struct Nested {
        Instance* x;
        void* other;
        Write write;
        bool entered = false;
    } nested{&instances[0], instances[1].processor, write};
    auto token = listener(
        instances[0].processor, 0,
        +[](void* c, void* p, int, float) {
            auto& n = *static_cast<Nested*>(c);
            if (std::exchange(n.entered, true))
                return;
            n.write(n.other, 1, 0.6F);
            require(n.x->controller->setParamNormalized(n.x->ids[1], 0.7) == kResultOk,
                    "nested controller");
            n.write(p, 0, 0.65F);
        },
        &nested, nullptr);
    require(instances[0].controller->setParamNormalized(instances[0].ids[0], 0.8) == kResultOk,
            "outer controller");
    listener(instances[0].processor, 0, nullptr, nullptr, token);
    std::printf("nested native final suppressed=%u expected=3; cross-instance independent=%u "
                "expected=1; leaked same-instance=%u\n",
                counts.suppressed[0].load(), counts.independent[1].load(),
                counts.independent[0].load());
    passed &= counts.suppressed[0] == 3 && counts.independent[1] == 1 && counts.independent[0] == 0;
    counts.clear();
    // Hold an audio native scope in a regular listener; external writes to a
    // different key on another thread must pass. UI stopped controller still
    // publishes on its own lane while that same instance's audio scope exists.
    struct Barrier {
        std::atomic<bool> entered = false, done = false;
    } barrier;
    token = listener(
        instances[0].processor, 0,
        +[](void* c, void*, int, float) {
            auto& b = *static_cast<Barrier*>(c);
            b.entered.store(true);
            while (!b.done.load())
                std::this_thread::yield();
        },
        &barrier, nullptr);
    instances[0].changes.values[0].points[0].value = 0.4;
    std::thread overlap([&] {
        require(instances[0].audio->process(instances[0].data) == kResultOk, "overlap process");
    });
    while (!barrier.entered.load())
        std::this_thread::yield();
    require(instances[0].controller->setParamNormalized(instances[0].ids[1], 0.3) == kResultOk,
            "overlap controller");
    write(instances[0].processor, 1, 0.5F);
    write(instances[1].processor, 1, 0.5F);
    barrier.done.store(true);
    overlap.join();
    listener(instances[0].processor, 0, nullptr, nullptr, token);
    std::printf("overlap native=%u expected=2; external same-thread=%u expected=1; instance=%u "
                "expected=1\n",
                counts.suppressed[0].load(), counts.independent[0].load(),
                counts.independent[1].load());
    passed &= counts.suppressed[0] == 2 && counts.independent[0] == 1 && counts.independent[1] == 1;
    counts.clear();
    std::array<std::thread, 2> workers;
    for (unsigned i = 0; i < 2; ++i)
        workers[i] = std::thread([&, i] {
            for (unsigned n = 0; n < 128; ++n) {
                instances[i].changes.values[0].points[0].value = n % 2 ? 0.25 : 0.75;
                require(instances[i].audio->process(instances[i].data) == kResultOk,
                        "concurrent process");
                write(instances[i].processor, 1, n % 2 ? 0.25F : 0.75F);
            }
        });
    for (auto& w : workers)
        w.join();
    std::printf("concurrent instances native=%u/%u external=%u/%u expected128 each\n",
                counts.suppressed[0].load(), counts.suppressed[1].load(),
                counts.independent[0].load(), counts.independent[1].load());
    for (unsigned i = 0; i < 2; ++i)
        passed &= counts.suppressed[i] == 128 && counts.independent[i] == 128;
    // Legal UI component restore scope unwinds; subsequent external notification passes.
    StateStream state;
    require(instances[0].component->getState(&state) == kResultOk, "save");
    counts.clear();
    Barrier stateBarrier;
    token = listener(
        instances[0].processor, 0,
        +[](void* c, void*, int, float) {
            auto& b = *static_cast<Barrier*>(c);
            b.entered.store(true);
            while (!b.done.load())
                std::this_thread::yield();
        },
        &stateBarrier, nullptr);
    std::thread stateExternal([&] {
        while (!stateBarrier.entered.load())
            std::this_thread::yield();
        // Different key avoids artificial waiting on the paused regular listener.
        write(instances[0].processor, 1, 0.2F);
        write(instances[1].processor, 1, 0.2F);
        stateBarrier.done.store(true);
    });
    require(instances[0].component->setState(&state) == kResultOk, "restore");
    stateExternal.join();
    listener(instances[0].processor, 0, nullptr, nullptr, token);
    write(instances[0].processor, 1, 0.1F);
    std::printf("state scope native=%u external=%u expected2 external; cross=%u expected1\n",
                counts.suppressed[0].load(), counts.independent[0].load(),
                counts.independent[1].load());
    passed &= counts.suppressed[0] == 7 && counts.independent[0] == 2 && counts.independent[1] == 1;
    observer(nullptr, nullptr);
    for (auto& x : instances) {
        x.component->setActive(false);
        x.cc->disconnect(x.ec);
        x.ec->disconnect(x.cc);
        x.cc->release();
        x.ec->release();
        x.controller->terminate();
        x.controller->release();
        x.audio->release();
        x.component->terminate();
        x.component->release();
    }
    factory->release();
    reinterpret_cast<bool (*)()>(symbol("bundleExit"))();
    CFRelease(module);
    std::puts(passed ? "VST3 suppression PASS" : "VST3 suppression FAIL");
    return passed ? 0 : 1;
}
