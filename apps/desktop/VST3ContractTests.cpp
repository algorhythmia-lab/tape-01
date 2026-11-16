// Loads the built format binary and calls Steinberg's actual process interface.
// This intentionally does not instantiate our C++ processor directly.
#define INIT_CLASS_IID
#include "NativeEditorHost.hpp"
#include "algorhythmia/host/processor.hpp"
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
class MidiEvents final : public Vst::IEventList {
public:
    std::array<Vst::Event, 300> events{};
    int32 count = 0, calls = 0;
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
    int32 PLUGIN_API getEventCount() override {
        return count;
    }
    tresult PLUGIN_API getEvent(int32 i, Vst::Event& e) override {
        ++calls;
        if (i < 0 || i >= count)
            return kInvalidArgument;
        e = events[static_cast<std::size_t>(i)];
        return kResultOk;
    }
    tresult PLUGIN_API addEvent(Vst::Event&) override {
        return kNotImplemented;
    }
};
} // namespace
int main(int argc, char** argv) {
    require(argc == 2 || argc == 3, "supply the built VST3 path");
#if defined(__APPLE__)
    auto url =
        CFURLCreateFromFileSystemRepresentation(nullptr, reinterpret_cast<const UInt8*>(argv[1]),
                                                static_cast<CFIndex>(std::strlen(argv[1])), true);
    auto module = CFBundleCreate(nullptr, url);
    CFRelease(url);
    require(module && CFBundleLoadExecutable(module), "unable to load VST3 bundle");
    const auto symbol = [&](const char* name) {
        auto str = CFStringCreateWithCString(nullptr, name, kCFStringEncodingUTF8);
        auto ptr = CFBundleGetFunctionPointerForName(module, str);
        CFRelease(str);
        return ptr;
    };
    const auto entry = reinterpret_cast<bool (*)(CFBundleRef)>(symbol("bundleEntry"));
    require(entry && entry(module), "VST3 bundle initialization failed");
#else
    auto module = LoadLibraryA(argv[1]);
    require(module != nullptr, "unable to load VST3 DLL");
    const auto symbol = [&](const char* name) { return GetProcAddress(module, name); };
    const auto entry = reinterpret_cast<bool (*)()>(symbol("InitDll"));
    require(entry && entry(), "VST3 DLL initialization failed");
#endif
    const auto getFactory = reinterpret_cast<IPluginFactory* (*)()>(symbol("GetPluginFactory"));
    require(getFactory != nullptr, "missing VST3 factory");
    auto* factory = getFactory();
    Vst::IComponent* component = nullptr;
    for (int32 i = 0; i < factory->countClasses(); ++i) {
        PClassInfo info{};
        factory->getClassInfo(i, &info);
        if (std::strcmp(info.category, kVstAudioEffectClass) == 0) {
            require(FUID(info.cid) == FUID(0xABCDEF01, 0x9182FAEB, 0x416C6772, 0x53797331),
                    "stable VST3 component ID changed");
            require(factory->createInstance(info.cid, Vst::IComponent::iid,
                                            reinterpret_cast<void**>(&component)) == kResultOk,
                    "component creation failed");
        }
    }
    require(component && component->initialize(nullptr) == kResultOk,
            "component initialization failed");
    Vst::IAudioProcessor* audio = nullptr;
    require(component->queryInterface(Vst::IAudioProcessor::iid,
                                      reinterpret_cast<void**>(&audio)) == kResultOk,
            "missing audio processor interface");
    TUID controllerId{};
    require(component->getControllerClassId(controllerId) == kResultOk, "missing controller ID");
    require(FUID(controllerId) == FUID(0xABCDEF01, 0x1234ABCD, 0x416C6772, 0x53797331),
            "stable VST3 controller ID changed");
    Vst::IEditController* controller = nullptr;
    require(factory->createInstance(controllerId, Vst::IEditController::iid,
                                    reinterpret_cast<void**>(&controller)) == kResultOk,
            "controller creation failed");
    require(controller->initialize(nullptr) == kResultOk, "controller initialization failed");
    Vst::IConnectionPoint* componentConnection = nullptr;
    Vst::IConnectionPoint* controllerConnection = nullptr;
    require(component->queryInterface(Vst::IConnectionPoint::iid,
                                      reinterpret_cast<void**>(&componentConnection)) == kResultOk,
            "missing component connection");
    require(controller->queryInterface(Vst::IConnectionPoint::iid,
                                       reinterpret_cast<void**>(&controllerConnection)) ==
                kResultOk,
            "missing controller connection");
    require(componentConnection->connect(controllerConnection) == kResultOk,
            "component connection failed");
    require(controllerConnection->connect(componentConnection) == kResultOk,
            "controller connection failed");
    const auto paramId = [&](const char* name) {
        for (int32 i = 0; i < controller->getParameterCount(); ++i) {
            Vst::ParameterInfo info{};
            controller->getParameterInfo(i, info);
            bool same = true;
            std::size_t j = 0;
            for (; name[j]; ++j)
                same &= info.title[j] == name[j];
            if (same && info.title[j] == 0)
                return info.id;
        }
        require(false, "catalogue parameter not exported by VST3");
        return Vst::ParamID{};
    };
    Changes changes;
    auto& drive = changes.values[0];
    drive.id = paramId("DRIVE");
    drive.count = 3;
    drive.points[0] = {9, 1};
    drive.points[1] = {61, 0};
    drive.points[2] = {91, 1};
    auto& flux = changes.values[1];
    flux.id = paramId("FLUX");
    flux.count = 2;
    flux.points[0] = {13, 0.1};
    flux.points[1] = {67, 0.9};
    auto& bias = changes.values[2];
    bias.id = paramId("BIAS");
    bias.count = 1;
    bias.points[0] = {73, 0.8};
    auto& domain = changes.values[3];
    domain.id = paramId("DOMAIN");
    domain.count = 1;
    domain.points[0] = {188, 0.5};
    auto& bypass = changes.values[4];
    bypass.id = paramId("BYPASS");
    bypass.count = 2;
    bypass.points[0] = {600, 1};
    bypass.points[1] = {700, 0};
    Vst::SpeakerArrangement layout = Vst::SpeakerArr::kStereo;
    require(audio->setBusArrangements(&layout, 1, &layout, 1) == kResultOk,
            "stereo arrangement failed");
    Vst::ProcessSetup setup{Vst::kRealtime, Vst::kSample32, 4096, 48000};
    require(audio->setupProcessing(setup) == kResultOk, "process setup failed");
    component->activateBus(Vst::kAudio, Vst::kInput, 0, true);
    component->activateBus(Vst::kAudio, Vst::kOutput, 0, true);
    require(component->setActive(true) == kResultOk, "activation failed");
    require(audio->setProcessing(true) == kResultOk, "processing activation failed");
    std::array<float, 4096> dryL{}, dryR{}, actualL{}, actualR{}, expectedL{}, expectedR{};
    std::array<float*, 2> inputs{dryL.data(), dryR.data()}, outputs{actualL.data(), actualR.data()};
    Vst::AudioBusBuffers in{}, out{};
    in.numChannels = out.numChannels = 2;
    in.channelBuffers32 = inputs.data();
    out.channelBuffers32 = outputs.data();
    Vst::ProcessData data{};
    data.processMode = Vst::kRealtime;
    data.symbolicSampleSize = Vst::kSample32;
    data.numSamples = 1024;
    data.numInputs = data.numOutputs = 1;
    data.inputs = &in;
    data.outputs = &out;
    data.inputParameterChanges = &changes;
#ifdef SYS01_NATIVE_RT_PROBE
    sys01::probe::selfTest();
#endif
    unsigned callback = 0;
    const auto process = [&](Vst::ProcessData& block) {
#ifdef SYS01_NATIVE_RT_PROBE
        sys01::probe::begin();
#endif
        const auto result = audio->process(block);
#ifdef SYS01_NATIVE_RT_PROBE
        const auto counts = sys01::probe::end();
        std::printf("VST3 callback %u frames %d: alloc=%llu free=%llu mutex=%llu unfair=%llu\n",
                    callback, block.numSamples, counts.allocation, counts.deallocation,
                    counts.mutex, counts.unfair);
        if (counts.allocation != 0)
            sys01::probe::printAllocation(counts);
#endif
        ++callback;
        return result;
    };
    tresult firstResult = kResultFalse;
    std::thread firstAudio([&] { firstResult = process(data); });
    firstAudio.join();
    require(firstResult == kResultOk, "format process failed");
    using namespace algorhythmia::host;
    Processor reference;
    reference.prepare(48000);
    constexpr std::array events{Event{9, EventKind::Parameter, Id::Drive, 1},
                                Event{13, EventKind::Parameter, Id::Flux, 0.1F},
                                Event{61, EventKind::Parameter, Id::Drive, 0},
                                Event{67, EventKind::Parameter, Id::Flux, 0.9F},
                                Event{73, EventKind::Parameter, Id::Bias, 0.8F},
                                Event{91, EventKind::Parameter, Id::Drive, 1},
                                Event{188, EventKind::Parameter, Id::Domain, 0.5F},
                                Event{600, EventKind::Parameter, Id::Bypass, 1},
                                Event{700, EventKind::Parameter, Id::Bypass, 0}};
    require(reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(),
                              static_cast<std::size_t>(data.numSamples), events),
            "reference rejected events");
    require(std::equal(actualL.begin(), actualL.begin() + data.numSamples, expectedL.begin()) &&
                std::equal(actualR.begin(), actualR.begin() + data.numSamples, expectedR.begin()),
            "loaded VST3 did not preserve every automation point and offset");
    // Subsequent blocks must retain final values, without replaying points.
    data.inputParameterChanges = nullptr;
    require(process(data) == kResultOk, "second format block failed");
    require(reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(),
                              static_cast<std::size_t>(data.numSamples)),
            "reference second block failed");
    require(std::equal(actualL.begin(), actualL.begin() + data.numSamples, expectedL.begin()) &&
                std::equal(actualR.begin(), actualR.begin() + data.numSamples, expectedR.begin()),
            "VST3 replayed automation or lost final targets");
    // All seven stable IDs are verified against literal JUCE hash values.
    // Each integer offset of every declared nonzero block size is exercised,
    // in batches of at most 256. Reference scheduling never reads the adapter.
    const std::array<Vst::ParamID, 7> stableIds{452784731U,  452662059U,  1763717364U, 834166325U,
                                                1194780756U, 1323076768U, 773352680U};
    for (std::size_t i = 0; i < catalogue.size(); ++i) {
        const auto name = std::string(catalogue[i].label);
        require(paramId(name.c_str()) == stableIds[i], "stable VST3 parameter ID changed");
        std::printf("VST3 ID %s = %u\n", name.c_str(), stableIds[i]);
    }
    unsigned offsetPoints = 0;
    for (double rate : {44100., 48000., 88200., 96000., 176400., 192000.}) {
        audio->setProcessing(false);
        component->setActive(false);
        setup.sampleRate = rate;
        require(audio->setupProcessing(setup) == kResultOk &&
                    component->setActive(true) == kResultOk &&
                    audio->setProcessing(true) == kResultOk,
                "offset sweep rate setup failed");
        reference.prepare(static_cast<float>(rate));
        for (unsigned frames : {1U, 7U, 63U, 64U, 65U, 511U, 4096U}) {
            data.numSamples = static_cast<int32>(frames);
            for (const auto& parameter : catalogue) {
                // Keep every tested parameter audible, including trim and bypass.
                for (unsigned i = 0; i < frames; ++i) {
                    dryL[i] = 0.125F;
                    dryR[i] = -0.0625F;
                }
                changes.count = 3;
                const std::array<Id, 3> audibleIds{Id::Drive, Id::Domain, Id::Bypass};
                constexpr std::array audibleValues{1.F, 0.F, 0.F};
                std::array<Event, 3> audible{};
                for (std::size_t i = 0; i < audible.size(); ++i) {
                    changes.values[i].id = stableIds[index(audibleIds[i])];
                    changes.values[i].count = 1;
                    changes.values[i].points[0] = {0, audibleValues[i]};
                    audible[i] = {0, EventKind::Parameter, audibleIds[i], audibleValues[i]};
                }
                data.inputParameterChanges = &changes;
                require(process(data) == kResultOk &&
                            reference.process(dryL.data(), dryR.data(), expectedL.data(),
                                              expectedR.data(), frames, audible),
                        "audible sweep setup failed");
                changes.count = 1;
                drive.id = stableIds[index(parameter.id)];
                for (unsigned first = 0; first < frames; first += 256) {
                    drive.count = static_cast<int32>(std::min(256U, frames - first));
                    std::array<Event, 256> expectedEvents{};
                    for (int32 j = 0; j < drive.count; ++j) {
                        const auto offset = first + static_cast<unsigned>(j);
                        const float raw = offset % 2 ? 0.76F : 0.24F;
                        // Explicit thresholds, independent of nativePoint rounding.
                        const float quantized = parameter.steps == 2   ? (raw >= 0.5F ? 1.F : 0.F)
                                                : parameter.steps == 3 ? (raw < 0.25F   ? 0.F
                                                                          : raw < 0.75F ? 0.5F
                                                                                        : 1.F)
                                                                       : raw;
                        drive.points[static_cast<std::size_t>(j)] = {static_cast<int32>(offset),
                                                                     raw};
                        expectedEvents[static_cast<std::size_t>(j)] = {offset, EventKind::Parameter,
                                                                       parameter.id, quantized};
                    }
                    data.inputParameterChanges = &changes;
                    require(process(data) == kResultOk, "offset sweep native render failed");
                    require(reference.process(dryL.data(), dryR.data(), expectedL.data(),
                                              expectedR.data(), frames,
                                              std::span(expectedEvents.data(),
                                                        static_cast<std::size_t>(drive.count))),
                            "offset sweep reference failed");
                    require(
                        std::equal(actualL.begin(), actualL.begin() + frames, expectedL.begin()) &&
                            std::equal(actualR.begin(), actualR.begin() + frames,
                                       expectedR.begin()),
                        "seven-parameter offset/quantization sweep differs");
                    offsetPoints += static_cast<unsigned>(drive.count);
                    data.inputParameterChanges = nullptr;
                    require(process(data) == kResultOk, "offset sweep following block failed");
                    require(reference.process(dryL.data(), dryR.data(), expectedL.data(),
                                              expectedR.data(), frames),
                            "offset sweep following reference failed");
                    require(
                        std::equal(actualL.begin(), actualL.begin() + frames, expectedL.begin()) &&
                            std::equal(actualR.begin(), actualR.begin() + frames,
                                       expectedR.begin()),
                        "offset sweep replayed final targets");
                }
            }
        }
    }
    audio->setProcessing(false);
    component->setActive(false);
    setup.sampleRate = 48000;
    require(audio->setupProcessing(setup) == kResultOk && component->setActive(true) == kResultOk &&
                audio->setProcessing(true) == kResultOk,
            "restore sweep rate failed");
    reference.prepare(48000);
    std::printf("VST3 exhaustive offsets: %u points; seven parameters; six rates; "
                "1/7/63/64/65/511/4096 frames\n",
                offsetPoints);
    data.numSamples = 1024;
    dryL.fill(0);
    dryR.fill(0);
    changes.count = 7;
    const std::array<float, 7> baseline{0.9F, 0.8F, 1.F, 0.F, 0.5F, 0.75F, 0.F};
    std::array<Event, 7> baselineEvents{};
    for (std::size_t i = 0; i < baseline.size(); ++i) {
        changes.values[i].id = stableIds[i];
        changes.values[i].count = 1;
        changes.values[i].points[0] = {0, baseline[i]};
        baselineEvents[i] = {0, EventKind::Parameter, catalogue[i].id, baseline[i]};
    }
    data.inputParameterChanges = &changes;
    require(process(data) == kResultOk && reference.process(nullptr, nullptr, expectedL.data(),
                                                            expectedR.data(), 1024, baselineEvents),
            "post-sweep baseline failed");
    drive.id = paramId("DRIVE");
    // Same parameter, same offset: last point wins in input order.
    changes.count = 1;
    drive.count = 3;
    drive.points[0] = {19, 0};
    drive.points[1] = {19, 1};
    drive.points[2] = {19, 0};
    data.inputParameterChanges = &changes;
    constexpr std::array repeatedTie{Event{19, EventKind::Parameter, Id::Drive, 0},
                                     Event{19, EventKind::Parameter, Id::Drive, 1},
                                     Event{19, EventKind::Parameter, Id::Drive, 0}};
    require(process(data) == kResultOk && reference.process(nullptr, nullptr, expectedL.data(),
                                                            expectedR.data(), 1024, repeatedTie),
            "repeated tie failed");
    require(std::equal(actualL.begin(), actualL.begin() + 1024, expectedL.begin()) &&
                std::equal(actualR.begin(), actualR.begin() + 1024, expectedR.begin()),
            "same-queue tie order changed");
    // Exact half-step thresholds and their adjacent sides, including DOMAIN's
    // middle value, are tested separately from the exhaustive offset sweep.
    constexpr std::array<double, 8> thresholdRaw{0, 0.2499, 0.25, 0.4999, 0.5, 0.7499, 0.75, 1};
    constexpr std::array<float, 8> binaryExpected{0, 0, 0, 0, 1, 1, 1, 1};
    constexpr std::array<float, 8> domainExpected{0, 0, 0.5F, 0.5F, 0.5F, 0.5F, 1, 1};
    for (Id id : {Id::Drive, Id::State, Id::Domain, Id::Bypass}) {
        changes.count = 1;
        drive.id = stableIds[index(id)];
        drive.count = 8;
        std::array<Event, 8> thresholdEvents{};
        for (std::size_t i = 0; i < thresholdRaw.size(); ++i) {
            drive.points[i] = {static_cast<int32>(i * 101), thresholdRaw[i]};
            thresholdEvents[i] = {i * 101, EventKind::Parameter, id,
                                  id == Id::Domain ? domainExpected[i] : binaryExpected[i]};
        }
        data.inputParameterChanges = &changes;
        require(process(data) == kResultOk &&
                    reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(), 1024,
                                      thresholdEvents),
                "discrete thresholds failed");
        require(std::equal(actualL.begin(), actualL.begin() + 1024, expectedL.begin()) &&
                    std::equal(actualR.begin(), actualR.begin() + 1024, expectedR.begin()),
                "discrete half-step quantization differs");
    }
    // Restore the audible baseline before malformed/overflow negative cases.
    changes.count = 7;
    for (std::size_t i = 0; i < baseline.size(); ++i) {
        changes.values[i].id = stableIds[i];
        changes.values[i].count = 1;
        changes.values[i].points[0] = {0, baseline[i]};
    }
    data.inputParameterChanges = &changes;
    require(process(data) == kResultOk && reference.process(nullptr, nullptr, expectedL.data(),
                                                            expectedR.data(), 1024, baselineEvents),
            "post-threshold baseline failed");
    drive.id = paramId("DRIVE");
    // The declared maximum fits without an invisible adapter-reserved slot.
    changes.count = 1;
    drive.count = 256;
    std::array<Event, 256> dense{};
    for (std::size_t i = 0; i < dense.size(); ++i) {
        drive.points[i] = {static_cast<int32>(i), static_cast<double>(i % 2)};
        dense[i] = {i, EventKind::Parameter, Id::Drive, static_cast<float>(i % 2)};
    }
    data.inputParameterChanges = &changes;
    require(process(data) == kResultOk, "maximum-density block failed");
    require(reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(),
                              static_cast<std::size_t>(data.numSamples), dense),
            "reference dense block failed");
    require(std::equal(actualL.begin(), actualL.begin() + data.numSamples, expectedL.begin()) &&
                std::equal(actualR.begin(), actualR.begin() + data.numSamples, expectedR.begin()),
            "256 native points did not fit the declared capacity");
    // An overflowing native queue must reject the block atomically, then recover.
    changes.count = 1;
    drive.count = 300;
    for (auto& point : drive.points)
        point = {0, 0};
    data.inputParameterChanges = &changes;
    require(process(data) == kResultOk, "overflow block failed to render safely");
    require(reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(),
                              static_cast<std::size_t>(data.numSamples)),
            "reference overflow block failed");
    require(std::equal(actualL.begin(), actualL.begin() + data.numSamples, expectedL.begin()) &&
                std::equal(actualR.begin(), actualR.begin() + data.numSamples, expectedR.begin()),
            "overflow caused partial musical changes");
    data.inputParameterChanges = nullptr;
    require(process(data) == kResultOk, "overflow recovery failed");
    require(reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(),
                              static_cast<std::size_t>(data.numSamples)),
            "reference recovery failed");
    require(std::equal(actualL.begin(), actualL.begin() + data.numSamples, expectedL.begin()) &&
                std::equal(actualR.begin(), actualR.begin() + data.numSamples, expectedR.begin()),
            "overflow did not recover deterministically");
    // Cross-queue overflow: the wrapper has published valid final host targets
    // before discovering the limit. None may leak back as scalar changes in
    // the next block, including bypass.
    changes.count = 2;
    drive.count = 1;
    drive.points[0] = {0, 0};
    flux.id = paramId("BYPASS");
    flux.count = 256;
    for (std::size_t i = 0; i < 256; ++i)
        flux.points[i] = {static_cast<int32>(i), 1};
    data.inputParameterChanges = &changes;
    for (int block = 0; block < 2; ++block) {
        require(process(data) == kResultOk, "mixed overflow block failed");
        require(reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(),
                                  static_cast<std::size_t>(data.numSamples)),
                "reference mixed overflow block failed");
        require(
            std::equal(actualL.begin(), actualL.begin() + data.numSamples, expectedL.begin()) &&
                std::equal(actualR.begin(), actualR.begin() + data.numSamples, expectedR.begin()),
            "rejected host targets leaked into this or the following block");
        data.inputParameterChanges = nullptr;
    }
    // Nonfinite last points must not poison native state or UI parameter data.
    changes.count = 1;
    drive.points[0] = {17, std::numeric_limits<double>::quiet_NaN()};
    data.inputParameterChanges = &changes;
    for (int block = 0; block < 2; ++block) {
        require(process(data) == kResultOk, "invalid point block failed");
        require(reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(),
                                  static_cast<std::size_t>(data.numSamples)),
                "reference invalid point block failed");
        require(
            std::equal(actualL.begin(), actualL.begin() + data.numSamples, expectedL.begin()) &&
                std::equal(actualR.begin(), actualR.begin() + data.numSamples, expectedR.begin()),
            "nonfinite native parameter changed musical state");
        data.inputParameterChanges = nullptr;
    }
    // Actual native IEventList ingress: automation then MIDI ties, fixed CC
    // mapping, hostile lengths and ignored SysEx without copying its payload.
    MidiEvents midiEvents;
    auto& cc = midiEvents.events[0];
    cc.type = Vst::Event::kLegacyMIDICCOutEvent;
    cc.busIndex = 0;
    cc.sampleOffset = 17;
    cc.midiCCOut.controlNumber = 21;
    cc.midiCCOut.value = 32;
    midiEvents.count = 1;
    drive.count = 1;
    drive.id = paramId("BIAS");
    drive.points[0] = {17, 0.9};
    changes.count = 1;
    data.inputEvents = &midiEvents;
    data.inputParameterChanges = &changes;
    constexpr std::array tie{Event{17, EventKind::Parameter, Id::Bias, 0.9F},
                             Event{17, EventKind::Parameter, Id::Bias, 32.F / 127.F}};
    require(process(data) == kResultOk, "native MIDI tie block failed");
    require(reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(), 1024, tie),
            "MIDI tie reference failed");
    require(std::equal(actualL.begin(), actualL.begin() + data.numSamples, expectedL.begin()) &&
                std::equal(actualR.begin(), actualR.begin() + data.numSamples, expectedR.begin()),
            "automation/MIDI stable tie differs");
    data.inputParameterChanges = nullptr;
    midiEvents.count = 300;
    midiEvents.calls = 0;
    require(process(data) == kResultOk, "hostile MIDI count failed");
    require(midiEvents.calls == 0, "oversize list was traversed");
    require(reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(), 1024),
            "MIDI overflow reference failed");
    require(std::equal(actualL.begin(), actualL.begin() + data.numSamples, expectedL.begin()) &&
                std::equal(actualR.begin(), actualR.begin() + data.numSamples, expectedR.begin()),
            "MIDI overflow partially applied");
    midiEvents.count = 1;
    cc.midiCCOut.value = 127;
    cc.sampleOffset = -1;
    require(process(data) == kResultOk, "invalid MIDI offset failed");
    require(reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(), 1024),
            "invalid MIDI reference failed");
    require(std::equal(actualL.begin(), actualL.begin() + data.numSamples, expectedL.begin()) &&
                std::equal(actualR.begin(), actualR.begin() + data.numSamples, expectedR.begin()),
            "invalid MIDI changed sound");
    std::array<uint8, 65536> payload{};
    cc.type = Vst::Event::kDataEvent;
    cc.data.type = Vst::DataEvent::kMidiSysEx;
    cc.data.size = static_cast<uint32>(payload.size());
    cc.data.bytes = payload.data();
    require(process(data) == kResultOk, "ignored SysEx failed");
    require(reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(), 1024),
            "SysEx reference failed");
    require(std::equal(actualL.begin(), actualL.begin() + data.numSamples, expectedL.begin()) &&
                std::equal(actualR.begin(), actualR.begin() + data.numSamples, expectedR.begin()),
            "SysEx changed sound");
    data.inputEvents = nullptr;
    // Host synthetic MIDI parameter mapping also uses the fixed ingress.
    Vst::IMidiMapping* mapping = nullptr;
    require(controller->queryInterface(Vst::IMidiMapping::iid,
                                       reinterpret_cast<void**>(&mapping)) == kResultOk,
            "MIDI mapping missing");
    require(mapping->getMidiControllerAssignment(0, 0, 21, drive.id) == kResultOk,
            "CC21 assignment missing");
    mapping->release();
    drive.points[0] = {31, 0.25};
    data.inputParameterChanges = &changes;
    constexpr std::array mapped{Event{31, EventKind::Parameter, Id::Bias, 32.F / 127.F}};
    require(process(data) == kResultOk, "synthetic MIDI queue failed");
    require(reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(), 1024, mapped),
            "synthetic reference failed");
    require(std::equal(actualL.begin(), actualL.begin() + data.numSamples, expectedL.begin()) &&
                std::equal(actualR.begin(), actualR.begin() + data.numSamples, expectedR.begin()),
            "synthetic MIDI offset differs");
    data.inputParameterChanges = nullptr;
    // All fixed CCs use actual native IEventList, including same-offset ties.
    midiEvents.count = 4;
    constexpr std::array<int, 4> ccNumbers{20, 21, 22, 23};
    constexpr std::array<int, 4> ccValues{96, 32, 127, 64};
    for (std::size_t i = 0; i < ccNumbers.size(); ++i) {
        auto& event = midiEvents.events[i];
        event = {};
        event.type = Vst::Event::kLegacyMIDICCOutEvent;
        event.sampleOffset = 23;
        event.midiCCOut.controlNumber = static_cast<uint8>(ccNumbers[i]);
        event.midiCCOut.value = static_cast<int8>(ccValues[i]);
    }
    data.inputEvents = &midiEvents;
    constexpr std::array allCC{Event{23, EventKind::Parameter, Id::Flux, 96.F / 127},
                               Event{23, EventKind::Parameter, Id::Bias, 32.F / 127},
                               Event{23, EventKind::Parameter, Id::Drive, 1},
                               Event{23, EventKind::Parameter, Id::Domain, 0.5F}};
    require(process(data) == kResultOk && reference.process(nullptr, nullptr, expectedL.data(),
                                                            expectedR.data(), 1024, allCC),
            "all native CCs failed");
    require(std::equal(actualL.begin(), actualL.begin() + 1024, expectedL.begin()) &&
                std::equal(actualR.begin(), actualR.begin() + 1024, expectedR.begin()),
            "CC20-23 offsets differ");
    data.inputEvents = nullptr;
    // Invalid intermediate points with valid final targets reject atomically.
    // Test both infinities, NaN, out-of-range values and both invalid endpoints.
    for (double bad :
         {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::infinity(),
          -std::numeric_limits<double>::infinity(), -0.1, 1.1}) {
        changes.count = 1;
        drive.id = paramId("BIAS");
        drive.count = 2;
        drive.points[0] = {11, bad};
        drive.points[1] = {19, 0.7};
        data.inputParameterChanges = &changes;
        for (int next = 0; next < 2; ++next) {
            require(
                process(data) == kResultOk &&
                    reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(), 1024),
                "invalid point render failed");
            require(std::equal(actualL.begin(), actualL.begin() + 1024, expectedL.begin()) &&
                        std::equal(actualR.begin(), actualR.begin() + 1024, expectedR.begin()),
                    "invalid intermediate point leaked or replayed");
            data.inputParameterChanges = nullptr;
        }
    }
    for (int offset : {-1, 1024}) {
        drive.count = 1;
        drive.points[0] = {offset, 0.8};
        data.inputParameterChanges = &changes;
        for (int next = 0; next < 2; ++next) {
            require(
                process(data) == kResultOk &&
                    reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(), 1024),
                "invalid offset render failed");
            require(std::equal(actualL.begin(), actualL.begin() + 1024, expectedL.begin()) &&
                        std::equal(actualR.begin(), actualR.begin() + 1024, expectedR.begin()),
                    "out-of-block point leaked or replayed");
            data.inputParameterChanges = nullptr;
        }
    }
    // Native automation plus MIDI shares the same total capacity.
    drive.id = paramId("DRIVE");
    drive.count = 256;
    for (unsigned i = 0; i < 256; ++i)
        drive.points[i] = {static_cast<int32>(i), 0};
    midiEvents.count = 1;
    data.inputParameterChanges = &changes;
    data.inputEvents = &midiEvents;
    for (int next = 0; next < 2; ++next) {
        require(process(data) == kResultOk &&
                    reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(), 1024),
                "merged overflow render failed");
        require(std::equal(actualL.begin(), actualL.begin() + 1024, expectedL.begin()) &&
                    std::equal(actualR.begin(), actualR.begin() + 1024, expectedR.begin()),
                "merged overflow leaked or replayed");
        data.inputParameterChanges = nullptr;
        data.inputEvents = nullptr;
    }
    // Zero flush, variable blocks, suspend/resume and repeated native lifecycle.
    data.numSamples = 0;
    require(process(data) == kResultOk, "zero native block failed");
    for (int frames : {1, 7, 63, 64, 65, 511, 4096}) {
        data.numSamples = frames;
        require(process(data) == kResultOk, "variable native block failed");
        require(reference.process(nullptr, nullptr, expectedL.data(), expectedR.data(),
                                  static_cast<std::size_t>(frames)),
                "variable reference failed");
        require(std::equal(actualL.begin(), actualL.begin() + frames, expectedL.begin()) &&
                    std::equal(actualR.begin(), actualR.begin() + frames, expectedR.begin()),
                "variable native block differs");
    }
    for (double rate : {44100., 48000., 88200., 96000., 176400., 192000.}) {
        audio->setProcessing(false);
        component->setActive(false);
        setup.sampleRate = rate;
        require(audio->setupProcessing(setup) == kResultOk, "repeated setup failed");
        require(component->setActive(true) == kResultOk, "repeated prepare failed");
        require(audio->setProcessing(true) == kResultOk, "resume failed");
        require(process(data) == kResultOk, "reprepared block failed");
        for (int i = 0; i < data.numSamples; ++i)
            require(std::isfinite(actualL[static_cast<std::size_t>(i)]) &&
                        std::isfinite(actualR[static_cast<std::size_t>(i)]),
                    "reprepare nonfinite output");
    }
#if defined(__APPLE__)
    auto* view = controller->createView(Vst::ViewType::kEditor);
    require(view != nullptr, "native editor view unavailable");
    auto* parent = sys01OpenEditorParent();
    require(view->attached(sys01EditorView(parent), kPlatformTypeNSView) == kResultOk,
            "native editor attach failed");
    sys01PumpEditor();
    drive.id = paramId("FLUX");
    drive.count = 1;
    changes.count = 1;
    data.inputParameterChanges = &changes;
    std::atomic<bool> automationDone{false};
    std::thread automatedAudio([&] {
        for (int i = 0; i < 256; ++i) {
            drive.points[0] = {0, i % 2 ? 0.1 : 0.9};
            require(process(data) == kResultOk, "editor-open automated native block failed");
        }
        automationDone.store(true);
    });
    while (!automationDone.load()) {
        sys01PumpEditor();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    automatedAudio.join();
    data.inputParameterChanges = nullptr;
    // VST3 state restore stays on the message thread while real native
    // processing runs on its single audio owner, with the editor attached.
    StateStream saved;
    require(component->getState(&saved) == kResultOk, "native state capture failed");
    std::thread restoreAudio([&] {
        for (int i = 0; i < 2000; ++i)
            require(audio->process(data) == kResultOk, "concurrent native restore block failed");
    });
    for (int i = 0; i < 200; ++i) {
        require(component->setState(&saved) == kResultOk, "concurrent native state restore failed");
        sys01PumpEditor();
    }
    restoreAudio.join();
    // Optional local diagnostic timing, never minimum-machine/DAW qualification.
    if (argc == 3) {
#ifdef SYS01_NATIVE_RT_PROBE
        require(false, "profiling requires a separate build without the probe");
#endif
        setup.sampleRate = 48000.;
        data.numSamples = 128;
        // Exactly 256 merged events: 224 ordinary automation points plus
        // 32 positioned CCs, repeated domain/drive/bypass changes and ties.
        changes.count = 5;
        const std::array<const char*, 5> names{"DRIVE", "FLUX", "BIAS", "DOMAIN", "BYPASS"};
        const std::array<int32, 5> pointCounts{64, 64, 64, 16, 16};
        for (std::size_t i = 0; i < names.size(); ++i) {
            auto& queue = changes.values[i];
            queue.id = paramId(names[i]);
            queue.count = pointCounts[i];
            for (int32 j = 0; j < queue.count; ++j)
                queue.points[static_cast<std::size_t>(j)] = {
                    j % 128, i == 3 ? static_cast<double>(j % 3) / 2 : static_cast<double>(j % 2)};
        }
        midiEvents.count = 32;
        for (int32 i = 0; i < midiEvents.count; ++i) {
            auto& event = midiEvents.events[static_cast<std::size_t>(i)];
            event = {};
            event.type = Vst::Event::kLegacyMIDICCOutEvent;
            event.sampleOffset = i * 4;
            event.midiCCOut.controlNumber = 21;
            event.midiCCOut.value = static_cast<int8>(i * 4);
        }
        data.inputParameterChanges = &changes;
        data.inputEvents = &midiEvents;
        std::vector<double> clockCost(20000);
        for (auto& duration : clockCost) {
            const auto start = std::chrono::steady_clock::now();
            duration =
                std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - start)
                    .count();
        }
        std::sort(clockCost.begin(), clockCost.end());
        std::printf("timer-pair overhead count=20000 p50_us=%.3f p99_us=%.3f max_us=%.3f; "
                    "unadjusted native durations follow\n",
                    clockCost[10000], clockCost[19800], clockCost.back());
        std::vector<double> durations(20000);
        const auto profile = [&](const char* editor, int frames) {
            data.numSamples = frames;
            for (int32 i = 0; i < midiEvents.count; ++i)
                midiEvents.events[static_cast<std::size_t>(i)].sampleOffset = (i * 4) % frames;
            const double deadlineUs = 1000000.0 * frames / setup.sampleRate;
            std::atomic<bool> done{false};
            std::thread worker([&] {
                unsigned block = 0;
                for (auto& duration : durations) {
                    // Host prepares a changing final target outside the timed entry.
                    changes.values[1].points[63].value = static_cast<double>(block++ % 128) / 127;
                    const auto start = std::chrono::steady_clock::now();
                    require(audio->process(data) == kResultOk, "profile process failed");
                    duration = std::chrono::duration<double, std::micro>(
                                   std::chrono::steady_clock::now() - start)
                                   .count();
                }
                done.store(true);
            });
            while (!done.load()) {
                sys01PumpEditor();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            worker.join();
            std::sort(durations.begin(), durations.end());
            std::printf("local diagnostic editor=%s frames=%d count=%zu p50_us=%.3f p95_us=%.3f "
                        "p99_us=%.3f p999_us=%.3f max_us=%.3f deadline_misses=%zu "
                        "half_deadline_budget_breaches=%zu\n",
                        editor, frames, durations.size(), durations[10000], durations[19000],
                        durations[19800], durations[19980], durations.back(),
                        static_cast<std::size_t>(
                            std::count_if(durations.begin(), durations.end(),
                                          [&](double us) { return us >= deadlineUs; })),
                        static_cast<std::size_t>(
                            std::count_if(durations.begin(), durations.end(),
                                          [&](double us) { return us >= deadlineUs * 0.5; })));
        };
        audio->setProcessing(false);
        component->setActive(false);
        require(audio->setupProcessing(setup) == kResultOk &&
                    component->setActive(true) == kResultOk &&
                    audio->setProcessing(true) == kResultOk,
                "profile prepare failed");
        profile("open", 128);
        profile("open", 64);
        view->removed();
        sys01CloseEditorParent(parent);
        view->release();
        profile("closed", 128);
        profile("closed", 64);
    } else {
        view->removed();
        sys01CloseEditorParent(parent);
        view->release();
    }
#endif
    audio->setProcessing(false);
    component->setActive(false);
    componentConnection->disconnect(controllerConnection);
    controllerConnection->disconnect(componentConnection);
    componentConnection->release();
    controllerConnection->release();
    controller->terminate();
    controller->release();
    audio->release();
    component->terminate();
    component->release();
    factory->release();
    const auto exit = reinterpret_cast<bool (*)()>(symbol(
#if defined(__APPLE__)
        "bundleExit"
#else
        "ExitDll"
#endif
        ));
    require(exit && exit(), "module shutdown failed");
#if defined(__APPLE__)
    CFBundleUnloadExecutable(module);
    CFRelease(module);
#else
    FreeLibrary(module);
#endif
    std::puts("loaded VST3: every automation point, offset and final target matches the shared "
              "scheduler");
}
