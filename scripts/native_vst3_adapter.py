#!/usr/bin/env python3
"""Build-only, checksum-pinned JUCE VST3 integration for positioned parameters.

Preserves the upstream source/license in the generated translation unit. Never
modifies the supplied/fetched dependency. Any upstream source change fails shut.
See ADR 0003 and the loaded-binary conformance fixture.
"""
import hashlib
import pathlib
import sys

source, output = map(pathlib.Path, sys.argv[1:])
raw = source.read_bytes()
if hashlib.sha256(raw).hexdigest() != "16031325b31be2e32b23689f07338bd6c631941475b9bbf87d176d1f2565c202":
    raise SystemExit("JUCE VST3 source differs from reviewed 8.0.12; review the adapter before changing the pin")
text = raw.decode().replace("\r\n", "\n")
old = """                    if (auto* param = comPluginInstance->getParamForVSTParamID (vstParamID))
                        setValueAndNotifyIfChanged (*param, (float) change->value);"""
new = """                    if (auto* param = comPluginInstance->getParamForVSTParamID (vstParamID))
                    {
                        auto& positioned = static_cast<sys01::PluginProcessor&> (getPluginInstance());
                        // Never publish an invalid final target to native UI/state.
                        if (! std::isfinite (change->value) || change->value < 0.0 || change->value > 1.0)
                        {
                            positioned.rejectNativeBlock();
                            continue;
                        }
                        // Bounded by the adapter even for a hostile point count.
                        for (Steinberg::int32 point = 0; point < numPoints; ++point)
                        {
                            if (const auto item = getPointFromQueue (paramQueue, point))
                            {
                                if (! positioned.nativePoint (param->getParameterIndex(), item->offsetSamples,
                                                               (float) item->value))
                                    break;
                            }
                            else
                            {
                                positioned.rejectNativeBlock();
                                break;
                            }
                        }
                        setValueAndNotifyIfChanged (*param, (float) change->value);
                    }"""
begin = """        midiBuffer.clear();

        if (data.inputParameterChanges != nullptr)"""
begin_new = """        midiBuffer.clear();
        static_cast<sys01::PluginProcessor&> (getPluginInstance()).beginNativeBlock (data.numSamples);

        if (data.inputParameterChanges != nullptr)"""
limit_params = "        const auto numParamsChanged = paramChanges.getParameterCount();"
limit_params_new = limit_params + """
        Steinberg::int32 totalPoints = 0;
        if (numParamsChanged < 0 || numParamsChanged > (Steinberg::int32) sys01::contract::eventCapacity)
        {
            static_cast<sys01::PluginProcessor&> (getPluginInstance()).rejectNativeBlock();
            return;
        }"""
limit_points = "                const auto numPoints  = paramQueue->getPointCount();"
limit_points_new = limit_points + """
                if (numPoints < 0 || numPoints > (Steinberg::int32) sys01::contract::eventCapacity - totalPoints)
                {
                    static_cast<sys01::PluginProcessor&> (getPluginInstance()).rejectNativeBlock();
                    return;
                }
                totalPoints += numPoints;"""
midi_begin = text.index("        // If the parameter is mapped to a MIDI CC message then insert it into the midiBuffer.")
midi_end = text.index("\n    tresult PLUGIN_API process (", midi_begin)
text = text[:midi_begin] + """        int channel = 0, ctrlNumber = 0;
        auto& positioned = static_cast<sys01::PluginProcessor&> (getPluginInstance());
        if (! std::isfinite (value) || value < 0.0 || value > 1.0)
        {
            positioned.rejectNativeBlock();
            return;
        }
        if (juceVST3EditController->getMidiControllerForParameter (id, channel, ctrlNumber)
            && ctrlNumber >= 0 && ctrlNumber <= 127)
            positioned.nativeMidiCC (offsetSamples, ctrlNumber, jlimit (0, 127, (int) (value * 128.0)));
    }
""" + text[midi_end:]
event_old = "            MidiEventList::toMidiBuffer (midiBuffer, *data.inputEvents);"
event_new = """        {
            auto& positioned = static_cast<sys01::PluginProcessor&> (getPluginInstance());
            const auto count = data.inputEvents->getEventCount();
            if (count < 0 || count > (Steinberg::int32) sys01::contract::eventCapacity)
                positioned.rejectNativeBlock();
            else
                for (Steinberg::int32 i = 0; i < count; ++i)
                {
                    Vst::Event event {};
                    if (data.inputEvents->getEvent (i, event) != kResultOk)
                    {
                        positioned.rejectNativeBlock();
                        break;
                    }
                    // Only CC20-23 affect SYS 01. No SysEx copy, note allocation,
                    // MidiBuffer growth or host-sized conversion traversal.
                    if (event.type == Vst::Event::kLegacyMIDICCOutEvent && event.busIndex == 0)
                        if (! positioned.nativeMidiCC (event.sampleOffset, event.midiCCOut.controlNumber,
                                                       event.midiCCOut.value))
                            break;
                }
        }"""
for old_text, new_text in ((old, new), (begin, begin_new), (limit_params, limit_params_new), (limit_points, limit_points_new), (event_old, event_new)):
    if text.count(old_text) != 1:
        raise SystemExit("VST3 integration anchor is not unique")
    text = text.replace(old_text, new_text)
# Default-OFF loaded-binary observer, installed only while callbacks are quiescent.
def replace(old, new):
    global text
    if text.count(old) != 1:
        raise SystemExit("VST3 suppression anchor is not unique: " + old[:80])
    text = text.replace(old, new)

replace('static thread_local bool inParameterChangedCallback = false;', '''#if SYS01_VST3_OWNERSHIP_FIXTURE
static void (*sys01VST3SuppressionHook) (void*, void*, int, bool) = nullptr;
static void* sys01VST3SuppressionContext = nullptr;
extern "C" __attribute__((visibility("default"))) void Sys01VST3FixtureObserver (void (*hook) (void*, void*, int, bool), void* context)
{
    sys01VST3SuppressionHook = hook;
    sys01VST3SuppressionContext = context;
}
struct Sys01VST3FixtureRelay final : AudioProcessorParameter::Listener
{
    void* processor;
    void* context;
    void (*callback) (void*, void*, int, float);
    void parameterValueChanged (int index, float value) override { callback (context, processor, index, value); }
    void parameterGestureChanged (int, bool) override {}
};
extern "C" __attribute__((visibility("default"))) void* Sys01VST3FixtureProcessor (Vst::IComponent* component)
{
    JuceAudioProcessor* shared = nullptr;
    if (component->queryInterface (JuceAudioProcessor::iid, (void**) &shared) != kResultOk)
        return nullptr;
    auto* result = shared->get();
    shared->release();
    return result;
}
extern "C" __attribute__((visibility("default"))) void* Sys01VST3FixtureListener (void* processor, int index, void (*callback) (void*, void*, int, float), void* context, void* token)
{
    auto* param = static_cast<AudioProcessor*> (processor)->getParameters()[index];
    if (token != nullptr)
    {
        auto* listener = static_cast<Sys01VST3FixtureRelay*> (token);
        param->removeListener (listener);
        delete listener;
        return nullptr;
    }
    auto* listener = new Sys01VST3FixtureRelay;
    listener->processor = processor;
    listener->context = context;
    listener->callback = callback;
    param->addListener (listener);
    return listener;
}
extern "C" __attribute__((visibility("default"))) void Sys01VST3FixtureWrite (void* processor, int index, float value)
{
    static_cast<AudioProcessor*> (processor)->getParameters()[index]->setValueNotifyingHost (value);
}
#endif
static thread_local bool inParameterChangedCallback = false;''')
replace('''        if (inParameterChangedCallback || inSetState)
            return;''', '''        const bool suppressed = inParameterChangedCallback || inSetState;
       #if SYS01_VST3_OWNERSHIP_FIXTURE
        if (sys01VST3SuppressionHook != nullptr)
            sys01VST3SuppressionHook (sys01VST3SuppressionContext, getPluginInstance(), parameterIndex, suppressed);
       #endif
        if (suppressed)
            return;''')

# Native publications have two independently concurrent roles: one process
# owner and one serialized VST3 UI owner. No dynamic thread registration.
replace('''class InParameterChangedCallbackSetter
{
public:
    explicit InParameterChangedCallbackSetter (bool& ref)
        : inner ([&]() -> auto& { jassert (! ref); return ref; }(), true, false) {}

private:
    ScopedValueSetter<bool> inner;
};''', '''class NativeParameterSuppression
{
public:
    class Lane
    {
    public:
        class Scope
        {
        public:
            explicit Scope (Lane& lane) noexcept
                : lane (lane), previous (lane.owner.exchange (Thread::getCurrentThreadId(), std::memory_order_acq_rel)) {}
            ~Scope() { lane.owner.store (previous, std::memory_order_release); }
            Scope (const Scope&) = delete;
            Scope& operator= (const Scope&) = delete;
        private:
            Lane& lane;
            Thread::ThreadID previous;
        };
        bool get() const noexcept { return owner.load (std::memory_order_acquire) == Thread::getCurrentThreadId(); }
    private:
        static_assert (std::atomic<Thread::ThreadID>::is_always_lock_free);
        std::atomic<Thread::ThreadID> owner {};
    };
    // Role ownership, not a table of threads: process is single-owner; control
    // is serialized by the VST3 UI contract. State may overlap process too.
    Lane process, control;
    bool get() const noexcept { return process.get() || control.get(); }
};''')
replace('''    Array<Vst::ParamID> vstParamIDs;''', '''    NativeParameterSuppression parameterSuppression;
    Array<Vst::ParamID> vstParamIDs;''')
replace('''    explicit JuceAudioProcessor (AudioProcessor* source) noexcept''', '''    NativeParameterSuppression& getParameterSuppression() noexcept { return parameterSuppression; }

    explicit JuceAudioProcessor (AudioProcessor* source) noexcept''')
replace('static thread_local bool inParameterChangedCallback = false;', '')
replace('static void setValueAndNotifyIfChanged (AudioProcessorParameter& param, float newValue)',
        'static void setValueAndNotifyIfChanged (NativeParameterSuppression::Lane& lane, AudioProcessorParameter& param, float newValue)')
replace('const InParameterChangedCallbackSetter scopedSetter { inParameterChangedCallback };',
        'const NativeParameterSuppression::Lane::Scope scopedSetter { lane };')
replace('setValueAndNotifyIfChanged (param, (float) v);',
        'setValueAndNotifyIfChanged (owner.audioProcessor->getParameterSuppression().control, param, (float) v);')
replace('setValueAndNotifyIfChanged (*bypassParam, shouldBeBypassed ? 1.0f : 0.0f);',
        'setValueAndNotifyIfChanged (comPluginInstance->getParameterSuppression().control, *bypassParam, shouldBeBypassed ? 1.0f : 0.0f);')
replace('setValueAndNotifyIfChanged (*param, (float) change->value);',
        'setValueAndNotifyIfChanged (comPluginInstance->getParameterSuppression().process, *param, (float) change->value);')
replace('''
        inParameterChangedCallback = false;
''', '')
replace('const bool suppressed = inParameterChangedCallback || inSetState;',
        'const bool suppressed = audioProcessor->getParameterSuppression().get() || inSetState.get();')
# State is another UI-only scope, read by simultaneous processor callbacks.
# Identity comparison suppresses only its synchronous thread, never other writers.
replace('bool inSetState = false;', 'NativeParameterSuppression::Lane inSetState;')
if text.count('! inSetState &&') != 2:
    raise SystemExit('Unexpected state gesture reader count')
text = text.replace('! inSetState &&', '! inSetState.get() &&')
replace('''        bool unusedState = false;
        auto& flagToSet = juceVST3EditController != nullptr ? juceVST3EditController->inSetState : unusedState;
        const ScopedValueSetter<bool> scope (flagToSet, true);''', '''        NativeParameterSuppression::Lane unusedState;
        auto& flagToSet = juceVST3EditController != nullptr ? juceVST3EditController->inSetState : unusedState;
        const NativeParameterSuppression::Lane::Scope scope (flagToSet);''')

output.parent.mkdir(parents=True, exist_ok=True)
output.write_text('#include "PluginProcessor.hpp"\n' + text)
