#!/usr/bin/env python3
"""Pinned, build-only AU ingress; lifecycle/listener/ingress locks are retained."""
import hashlib
import pathlib
import sys

source, output = map(pathlib.Path, sys.argv[1:])
raw = source.read_bytes()
if hashlib.sha256(raw).hexdigest() != '16a5246f380a93c1f33f9afeef810702748039720ea094208765c0c8c916fe86':
    raise SystemExit('JUCE AU source differs from reviewed 8.0.12')
text = raw.decode().replace('\r\n', '\n')

def replace(old, new):
    global text
    if text.count(old) != 1:
        raise SystemExit('AU integration anchor is not unique: '+old[:80])
    text = text.replace(old, new)

replace('''                            if (prepared)
                                juceFilter->prepareToPlay (getSampleRate(), (int) GetMaxFramesPerSlice());''', '''                            // SYS 01 uses the same prepared DSP in both modes.
                            // A mode flag change must preserve running history,
                            // smoothing and pending positioned ingress. Only real
                            // lifecycle preparation resets those resources.''')

replace('''            midiEvents.ensureSize (2048);
            midiEvents.clear();
            incomingEvents.ensureSize (2048);
            incomingEvents.clear();''', '''            // 256 three-byte events fit both alternating buffers without growth.
            midiEvents.ensureSize (4096);
            midiEvents.clear();
            incomingEvents.ensureSize (4096);
            incomingEvents.clear();
            incomingMidiCount = 0;
            incomingMidiRejected = false;
            clearParameterIngress();
            renderCacheValid = false;''')
replace('''        lastTimeStamp = inTimeStamp;

        // prepare buffers''', '''        if (nFrames == 0)
        {
            ResetRenderTime(); // zero must not poison the SDK same-time cache
            return noErr; // consume no queued input or restore on a zero block
        }
        lastTimeStamp = inTimeStamp;

        // prepare buffers''')
# AUBase's ASBD safety checks prohibit zero-channel streams. Disable the fixed
# processor input through zero AU elements, retaining its layout slot. Lifecycle
# excludes Render; no resource pointer or audio-owner mutation is introduced.
replace('''    bool BusCountWritable ([[maybe_unused]] AudioUnitScope scope) override
    {''', '''    bool BusCountWritable ([[maybe_unused]] AudioUnitScope scope) override
    {
        return scope == kAudioUnitScope_Input;
    }
    bool upstreamBusCountWritable ([[maybe_unused]] AudioUnitScope scope)
    {''')
replace('''    OSStatus SetBusCount (AudioUnitScope scope, UInt32 count) override
    {''', '''    OSStatus SetBusCount (AudioUnitScope scope, UInt32 count) override
    {
        if (scope == kAudioUnitScope_Input)
        {
            if (count > 1 || IsInitialized())
                return kAudioUnitErr_PropertyNotWritable;
            const auto result = MusicDeviceBase::SetBusCount (scope, count);
            if (result == noErr)
            {
                currentInputLayout.resize (1);
                currentInputLayout.getReference (0) = count == 0 ? (AudioChannelLayoutTag) 0
                                                  : kAudioChannelLayoutTag_Stereo;
            }
            return result;
        }
        return upstreamSetBusCount (scope, count);
    }
    OSStatus upstreamSetBusCount (AudioUnitScope scope, UInt32 count)
    {''')
# Disabled input has no AU element and no channel descriptions. Do not pass
# tag zero (UseChannelDescriptions) to the tag-only SDK conversion.
replace('''                requestedBuses.add (CoreAudioLayouts::fromCoreAudio (currentLayoutTag));''', '''                requestedBuses.add (numChannels == 0 ? AudioChannelSet::disabled()
                                                    : CoreAudioLayouts::fromCoreAudio (currentLayoutTag));''')
replace('''    ComponentResult Render (AudioUnitRenderActionFlags& ioActionFlags,''', '''    OSStatus RenderBus (AudioUnitRenderActionFlags& flags, const AudioTimeStamp& stamp,
                        UInt32 bus, UInt32 frames) override
    {
        if (frames == 0)
            return noErr; // do not touch the SDK cache or consume an epoch
        if (renderCacheValid && stamp.mSampleTime == renderCacheTime
            && frames != renderCacheFrames)
            return kAudio_ParamError; // never copy stale differently-sized cached audio
        const auto result = MusicDeviceBase::RenderBus (flags, stamp, bus, frames);
        if (result == noErr)
        {
            renderCacheValid = true;
            renderCacheTime = stamp.mSampleTime;
            renderCacheFrames = frames;
        }
        return result;
    }

    ComponentResult Render (AudioUnitRenderActionFlags& ioActionFlags,''')
replace('''            incomingEvents.swapWith (midiEvents);''', '''            incomingEvents.swapWith (midiEvents);
            auto& native = static_cast<sys01::PluginProcessor&> (*juceFilter);
            native.beginNativeBlock ((int) nFrames);
            if (incomingMidiRejected)
                native.rejectNativeBlock();
            incomingMidiCount = 0;
            incomingMidiRejected = false;
            parameterEpoch = pendingParameters;
            pendingParameters.size = 0;
            pendingParameters.rejected = false;
            capturedEpoch = native.controlEpoch();
            for (size_t i = 0; i < sys01::contract::count; ++i)
            {
                capturedPublications[i] = native.parameterPublication (i);
                capturedControls.values[i] = std::bit_cast<float> ((uint32_t) capturedPublications[i]);
            }
            if (capturedEpoch != native.controlEpoch())
                capturedEpoch |= 1U; // unstable restore snapshot cannot be acknowledged
            for (size_t i = 0; i < sys01::contract::count; ++i)
                if (parameterEpoch.mask[i])
                    capturedControls.values[i] = parameterEpoch.targets[i];
            for (size_t i = 0; i < parameterEpoch.size; ++i)
            {
                const auto& point = parameterEpoch.points[i];
                native.nativePoint (point.index, point.offset, point.value);
            }
            if (parameterEpoch.rejected)
                native.rejectNativeBlock();''')
replace('''        // swap midi buffers
        {''', '''        ParameterIngress parameterEpoch;
        sys01::contract::State capturedControls;
        uint32_t capturedEpoch = 1;
        std::array<uint64_t, sys01::contract::count> capturedPublications {};
       #if SYS01_AU_OWNERSHIP_FIXTURE
        if (sys01BeforeCaptureHook != nullptr)
            sys01BeforeCaptureHook (sys01BeforeCaptureContext, juceFilter.get());
       #endif
        // Capture one epoch under the existing multi-producer MIDI lock.
        // swap midi buffers
        {''')
replace('''        // process audio
        processBlock''', '''       #if SYS01_AU_OWNERSHIP_FIXTURE
        if (sys01CaptureHook != nullptr)
            sys01CaptureHook (sys01CaptureContext, juceFilter.get());
       #endif
        // Listener publication is outside ingress ownership; it can reenter
        // host APIs. New setters write only the next epoch, not APVTS.
        for (size_t i = 0; i < sys01::contract::count; ++i)
            if (parameterEpoch.mask[i])
            {
                inParameterChangedCallback = true;
                static_cast<sys01::PluginProcessor&> (*juceFilter).publishNativeParameter (
                    i, parameterEpoch.targets[i], parameterEpoch.publication[i]);
            }
        {
            const ScopedLock sl (incomingMidiLock);
            for (size_t i = 0; i < sys01::contract::count; ++i)
                if (parameterEpoch.mask[i]
                    && pendingParameters.serial[i] == parameterEpoch.serial[i])
                    pendingParameters.mask[i] = false;
        }
        static_cast<sys01::PluginProcessor&> (*juceFilter).nativeControlsSnapshot (capturedControls, capturedEpoch);

        // process audio
        processBlock''')
replace('''        incomingEvents.clear();
        prepared = false;''', '''        incomingEvents.clear();
        flushParameterTargets();
        renderCacheValid = false;
        prepared = false;''')
replace('''        if (juceFilter != nullptr)
            juceFilter->reset();''', '''        flushParameterTargets();
        if (juceFilter != nullptr)
            juceFilter->reset();
        renderCacheValid = false;''')
replace('''            audioBuffer.prepare (AudioUnitHelpers::getBusesLayout (juceFilter.get()), (int) GetMaxFramesPerSlice() + 32);
            juceFilter->prepareToPlay (getSampleRate(), (int) GetMaxFramesPerSlice());''', '''            audioBuffer.prepare (AudioUnitHelpers::getBusesLayout (juceFilter.get()), (int) GetMaxFramesPerSlice() + 32);
            // Legal lifecycle excludes Render. Persist accepted host targets
            // before rebuilding DSP, but never retain their old offsets.
            flushParameterTargets();
            juceFilter->prepareToPlay (getSampleRate(), (int) GetMaxFramesPerSlice());''')
replace('''        const ScopedValueSetter<bool> scope { restoringState, true };''', '''        clearParameterIngress();
        const ScopedValueSetter<bool> scope { restoringState, true };''')
replace('''        juceFilter->setCurrentProgram (chosenPresetNumber);''', '''        clearParameterIngress();
        juceFilter->setCurrentProgram (chosenPresetNumber);''')
replace('''                const auto normValue = param->getValue();''', '''                const ScopedLock sl (incomingMidiLock);
                auto normValue = param->getValue();
                const auto i = parameterIndex (inID);
                if (i >= 0 && pendingParameters.mask[(size_t) i]
                    && pendingTargetCurrent ((size_t) i))
                    normValue = pendingParameters.targets[(size_t) i];''')
replace('''                    if (bypassParam != nullptr)
                        *(UInt32*) outData = (bypassParam->getValue() != 0.0f ? 1 : 0);
                    else
                        *(UInt32*) outData = isBypassed ? 1 : 0;''', '''                    {
                    Float32 value = 0;
                    const auto result = GetParameter (773352680U, kAudioUnitScope_Global, 0, value);
                    if (result != noErr)
                        return result;
                    *(UInt32*) outData = value != 0.0f ? 1 : 0;
                    }''')
replace('''                    const bool newBypass = *((UInt32*) inData) != 0;
                    const bool currentlyBypassed = (bypassParam != nullptr ? (bypassParam->getValue() != 0.0f) : isBypassed);

                    if (newBypass != currentlyBypassed)
                    {
                        if (bypassParam != nullptr)
                            bypassParam->setValueNotifyingHost (newBypass ? 1.0f : 0.0f);
                        else
                            isBypassed = newBypass;

                        if (! currentlyBypassed && IsInitialized()) // turning bypass off and we're initialized
                            Reset (0, 0);
                    }

                    return noErr;''', '''                    // SYS 01 exposes a native bypass parameter. Treat property
                    // updates as offset-zero steps without flushing other points.
                    return SetParameter (773352680U, kAudioUnitScope_Global, 0,
                                         *((UInt32*) inData) != 0 ? 1.0f : 0.0f, 0);''')
replace('''            juce::MemoryBlock state;

           #if JUCE_AU_WRAPPERS_SAVE_PROGRAM_STATES
            juceFilter->getCurrentProgramStateInformation (state);
           #else
            juceFilter->getStateInformation (state);
           #endif''', '''            juce::MemoryBlock state;
            // Preserve processor's off-audio restore serialization, then overlay
            // pending AU host targets before audio consumption.
            sys01::contract::State controls;
            juceFilter->getStateInformation (state);
            ignoreUnused (sys01::contract::decode ({ (const uint8_t*) state.getData(), state.getSize() }, controls));
            {
                const ScopedLock sl (incomingMidiLock);
                for (size_t i = 0; i < sys01::contract::count; ++i)
                    if (pendingParameters.mask[i] && pendingTargetCurrent (i))
                        controls.values[i] = pendingParameters.targets[i];
            }
            const auto encoded = sys01::contract::encode (controls);
            state.replaceAll (encoded.data(), encoded.size());''')
begin = text.index('        if (inScope == kAudioUnitScope_Global && juceFilter != nullptr)', text.index('    ComponentResult SetParameter ('))
end = text.index('\n    //==============================================================================\n    bool SupportsTail()', begin)
text = text[:begin] + '''        if (! prepared)
        {
            if (! validParameter (inID, inScope, inElement, inValue))
                return kAudioUnitErr_InvalidParameter;
            const auto i = (size_t) parameterIndex (inID);
            const auto steps = sys01::contract::catalogue[i].steps;
            if (steps > 0)
                inValue = std::round (inValue) / (float) (steps - 1);
            publishParameter (i, inValue); // lifecycle owner, no ingress over listeners
            return noErr;
        }
        const ScopedLock sl (incomingMidiLock);
        return enqueueParameter (inID, inScope, inElement, inValue, inBufferOffsetInFrames);
    }

    // Do not enter the SDK vector/split-render scheduler. The accepted contract
    // is step targets plus DSP smoothing; ramp events reject the whole epoch.
    bool CanScheduleParameters() const override { return false; }
    OSStatus ScheduleParameter (const AudioUnitParameterEvent* events, UInt32 count) override
    {
        if (! prepared)
        {
            if (count > sys01::contract::eventCapacity || (count != 0 && events == nullptr))
                return kAudio_ParamError;
            for (UInt32 i = 0; i < count; ++i)
                if (events[i].eventType != kParameterEvent_Immediate
                    || ! validParameter (events[i].parameter, events[i].scope, events[i].element,
                                         events[i].eventValues.immediate.value))
                    return kAudio_ParamError;
            for (UInt32 i = 0; i < count; ++i)
                SetParameter (events[i].parameter, events[i].scope, events[i].element,
                              events[i].eventValues.immediate.value, events[i].eventValues.immediate.bufferOffset);
            return noErr;
        }
        const ScopedLock sl (incomingMidiLock);
        if (count > sys01::contract::eventCapacity || (count != 0 && events == nullptr))
        {
            pendingParameters.rejected = true;
            return kAudio_ParamError;
        }
        // Validate the complete call before mutating targets or storage.
        for (UInt32 i = 0; i < count; ++i)
        {
            const auto& e = events[i];
            if (e.eventType != kParameterEvent_Immediate
                || ! validParameter (e.parameter, e.scope, e.element, e.eventValues.immediate.value))
            {
                pendingParameters.rejected = true;
                return kAudio_ParamError;
            }
        }
        for (UInt32 i = 0; i < count; ++i)
        {
            const auto& e = events[i];
            enqueueParameter (e.parameter, e.scope, e.element, e.eventValues.immediate.value,
                              e.eventValues.immediate.bufferOffset);
        }
        return noErr;
    }
''' + text[end:]
replace('''            incomingEvents.addEvent (data, 3, (int) inStartFrame);''', '''            if (reserveIncomingMidi())
                addIncomingCC (data, 3, inStartFrame);''')
replace('''            incomingEvents.addEvent (inData, (int) inLength, 0);''', '''            // SYS 01 has no SysEx mapping. Never copy host-sized payloads.
            ignoreUnused (inData, inLength);''')
start=text.index('        auto* packet = &list->packet[0];')
end=text.index('\n        return noErr;', start)
text=text[:start]+'''        if (list == nullptr || list->numPackets > 256)
        {
            incomingMidiRejected = true;
            return noErr;
        }
        auto* packet = &list->packet[0];
        uint32_t wordsRemaining = 1024;
        for (uint32_t i = 0; i < list->numPackets; ++i)
        {
            if (packet->wordCount > wordsRemaining)
            {
                incomingMidiRejected = true;
                break;
            }
            if (packet->timeStamp > (uint64_t) std::numeric_limits<int>::max()
                || inOffsetSampleFrame > (uint64_t) std::numeric_limits<int>::max() - packet->timeStamp)
            {
                incomingMidiRejected = true;
                break;
            }
            wordsRemaining -= packet->wordCount;
            for (uint32_t word = 0; word < packet->wordCount;)
            {
                const auto length = ump::Utils::getNumWordsForMessageType (packet->words[word]);
                if (length > packet->wordCount - word || ! reserveIncomingMidi())
                {
                    incomingMidiRejected = true;
                    return noErr;
                }
                const auto type = packet->words[word] >> 28;
                // Only channel voice packets can contain our CC mapping.
                // Skip SysEx before the stateful upstream dispatcher sees it.
                if (type == 2 || type == 4)
                    toBytestreamDispatcher.dispatch ({ packet->words + word, (size_t) length }, 0.0,
                        [this, packet, inOffsetSampleFrame] (const ump::BytesOnGroup& x, double)
                        {
                            const auto offset = (uint64_t) packet->timeStamp + inOffsetSampleFrame;
                            addIncomingCC (reinterpret_cast<const uint8*> (x.bytes.data()),
                                           (int) x.bytes.size(), offset);
                        });
                word += length;
            }
            packet = MIDIEventPacketNext (packet);
        }
''' +text[end:]
replace('''    MidiBuffer midiEvents, incomingEvents;''', '''    MidiBuffer midiEvents, incomingEvents;
    unsigned incomingMidiCount = 0;
    bool incomingMidiRejected = false;
    struct ParameterPoint { int index = 0, offset = 0; float value = 0; };
    struct ParameterIngress
    {
        std::array<ParameterPoint, sys01::contract::eventCapacity> points {};
        std::array<float, sys01::contract::count> targets {};
        std::array<bool, sys01::contract::count> mask {};
        std::array<uint64_t, sys01::contract::count> serial {};
        std::array<uint64_t, sys01::contract::count> publication {};
        size_t size = 0;
        bool rejected = false;
    } pendingParameters;
    uint64_t parameterSerial = 0; // retained across clear; captured epochs cannot alias
    bool renderCacheValid = false;
    double renderCacheTime = 0;
    UInt32 renderCacheFrames = 0;
    // Serialized MPSC: all setters, scheduled calls, legacy/UMP MIDI and epoch
    // capture use incomingMidiLock. Lifecycle excludes Render under SDK rules.
    static int parameterIndex (AudioUnitParameterID id) noexcept
    {
        constexpr std::array<AudioUnitParameterID, 7> ids {
            452784731U, 452662059U, 1763717364U, 834166325U,
            1194780756U, 1323076768U, 773352680U };
        for (size_t i = 0; i < ids.size(); ++i)
            if (ids[i] == id)
                return (int) i;
        return -1;
    }
    bool validParameter (AudioUnitParameterID id, AudioUnitScope scope,
                         AudioUnitElement element, float value) const noexcept
    {
        const auto i = parameterIndex (id);
        const auto maximum = i == 4 ? 2.0f : 1.0f;
        return i >= 0 && scope == kAudioUnitScope_Global && element == 0
            && std::isfinite (value) && value >= 0 && value <= maximum;
    }
    void publishParameter (size_t i, float value)
    {
        auto* param = juceFilter->getParameters()[(int) i];
        if (! approximatelyEqual (value, param->getValue()))
        {
            inParameterChangedCallback = true;
            param->setValueNotifyingHost (value);
        }
    }
    OSStatus enqueueParameter (AudioUnitParameterID id, AudioUnitScope scope,
                               AudioUnitElement element, float value, UInt32 offset)
    {
        if (! validParameter (id, scope, element, value))
        {
            if (prepared)
                pendingParameters.rejected = true;
            return kAudioUnitErr_InvalidParameter;
        }
        const auto i = (size_t) parameterIndex (id);
        const auto steps = sys01::contract::catalogue[i].steps;
        if (steps > 0)
            value = std::round (value) / (float) (steps - 1);
        pendingParameters.targets[i] = value;
        pendingParameters.mask[i] = true;
        pendingParameters.serial[i] = ++parameterSerial;
        pendingParameters.publication[i] = static_cast<sys01::PluginProcessor&> (*juceFilter).parameterPublication (i);
        if (offset > (UInt32) std::numeric_limits<int>::max()
            || pendingParameters.size + incomingMidiCount >= sys01::contract::eventCapacity)
            pendingParameters.rejected = true;
        else
            pendingParameters.points[pendingParameters.size++] = { (int) i, (int) offset, value };
        return noErr;
    }
    // Caller owns ingress. Pending targets remain host-authoritative only in
    // their accepted external generation; an editor/restore/reentrant writer
    // invalidates the overlay even before Render captures/clears that target.
    bool pendingTargetCurrent (size_t i) const noexcept
    {
        const auto current = static_cast<sys01::PluginProcessor&> (*juceFilter).parameterPublication (i) >> 32;
        return current != UINT32_MAX && current == (pendingParameters.publication[i] >> 32);
    }
    void clearParameterIngress()
    {
        const ScopedLock sl (incomingMidiLock);
        pendingParameters = {};
    }
    void flushParameterTargets()
    {
        ParameterIngress captured;
        {
            const ScopedLock sl (incomingMidiLock);
            captured = pendingParameters;
            pendingParameters = {};
        }
        // Lifecycle owner only; do not call listeners while holding ingress.
        // Setters/lifecycle are serialized by the legal host lifecycle contract.
        if (juceFilter != nullptr)
            for (size_t i = 0; i < sys01::contract::count; ++i)
                if (captured.mask[i])
                {
                    inParameterChangedCallback = true;
                    static_cast<sys01::PluginProcessor&> (*juceFilter).publishNativeParameter (
                        i, captured.targets[i], captured.publication[i]);
                }
    }
    // Called only under incomingMidiLock. Count all short messages; store only
    // the fixed musical contract. A rejected batch is forwarded atomically.
    bool reserveIncomingMidi() noexcept
    {
        if (incomingMidiCount + pendingParameters.size >= 256)
        {
            incomingMidiRejected = true;
            return false;
        }
        ++incomingMidiCount;
        return true;
    }
    void addIncomingCC (const uint8* data, int bytes, uint64_t offset)
    {
        if (bytes != 3 || (data[0] & 0xf0) != 0xb0)
            return;
        if (data[1] > 127 || data[2] > 127 || offset > (uint64_t) std::numeric_limits<int>::max())
        {
            incomingMidiRejected = true;
            return;
        }
        if (data[1] >= 20 && data[1] <= 23)
            incomingEvents.addEvent (data, 3, (int) offset);
    }''')
output.parent.mkdir(parents=True, exist_ok=True)
replace('#include <set>', '''#include <set>
#include "PluginProcessor.hpp"
#if SYS01_AU_OWNERSHIP_FIXTURE
static void (*sys01CaptureHook) (void*, void*) = nullptr;
static void* sys01CaptureContext = nullptr;
extern "C" __attribute__((visibility("default"))) void Sys01AUFixtureProcessorChange (void* processor, unsigned flags)
{
    // Invokes the real public processor listener dispatch, not the updater directly.
    juce::AudioProcessorListener::ChangeDetails details;
    details.latencyChanged = (flags & 1U) != 0;
    details.parameterInfoChanged = (flags & 2U) != 0;
    details.programChanged = (flags & 4U) != 0;
    static_cast<sys01::PluginProcessor*> (processor)->updateHostDisplay (details);
}
static void (*sys01BeforeCaptureHook) (void*, void*) = nullptr;
static void* sys01BeforeCaptureContext = nullptr;
extern "C" __attribute__((visibility("default"))) void Sys01AUFixtureBeforeCaptureHook (void (*hook) (void*, void*), void* context)
{
    sys01BeforeCaptureHook = hook;
    sys01BeforeCaptureContext = context;
}
extern "C" __attribute__((visibility("default"))) void Sys01AUFixtureRead (void* processor, float* values, float* mirrors)
{
    auto& native = *static_cast<sys01::PluginProcessor*> (processor);
    for (size_t i = 0; i < sys01::contract::count; ++i)
    {
        values[i] = native.getParameters()[(int) i]->getValue();
        mirrors[i] = native.parameters.getRawParameterValue (sys01::contract::catalogue[i].key.data())->load();
    }
}
struct Sys01FixtureListener final : juce::AudioProcessorParameter::Listener
{
    void* processor = nullptr;
    void* context = nullptr;
    void (*callback) (void*, void*, int, float) = nullptr;
    void parameterValueChanged (int index, float value) override { callback (context, processor, index, value); }
    void parameterGestureChanged (int, bool) override {}
};
static Sys01FixtureListener sys01FixtureListener;
extern "C" __attribute__((visibility("default"))) void Sys01AUFixtureListener (void* processor, void (*callback) (void*, void*, int, float), void* context)
{
    // Fixture owner installs/removes only while Render is quiescent or in its
    // before-notification hook; this static object never outlives registration.
    if (sys01FixtureListener.processor != nullptr)
        static_cast<sys01::PluginProcessor*> (sys01FixtureListener.processor)->getParameters()[0]->removeListener (&sys01FixtureListener);
    sys01FixtureListener.processor = callback != nullptr ? processor : nullptr;
    sys01FixtureListener.callback = callback;
    sys01FixtureListener.context = context;
    if (callback != nullptr)
        static_cast<sys01::PluginProcessor*> (processor)->getParameters()[0]->addListener (&sys01FixtureListener);
}
extern "C" __attribute__((visibility("default"))) void Sys01AUFixtureCaptureHook (void (*hook) (void*, void*), void* context)
{
    sys01CaptureHook = hook;
    sys01CaptureContext = context;
}
extern "C" __attribute__((visibility("default"))) void Sys01AUFixtureEditorWrite (void* processor, int index, float value)
{
    static_cast<sys01::PluginProcessor*> (processor)->getParameters()[index]->setValueNotifyingHost (value);
}
#endif''')
# One native publisher per instance under legal AU lifecycle / single Render
# ownership. Other parameter writers only read this atomic identity. A scope
# restores its predecessor, so nested publication and failed CAS cannot consume
# or leak suppression. No per-thread allocation, thread registry or shared flag.
replace('''
        inParameterChangedCallback = false;
''', '')
replace('''            inParameterChangedCallback = false;
            return;''', '            return;')
# Three generated native publication sites; each encloses synchronous listeners.
if text.count('inParameterChangedCallback = true;') != 3:
    raise SystemExit('Unexpected AU suppression writer count')
text = text.replace('inParameterChangedCallback = true;',
                    'const NativeParameterSuppression::Scope suppression (inParameterChangedCallback);')
replace('    ThreadLocalValue<bool> inParameterChangedCallback;', '''    class NativeParameterSuppression
    {
    public:
        class Scope
        {
        public:
            explicit Scope (NativeParameterSuppression& state) noexcept
                : state (state), previous (state.owner.exchange (Thread::getCurrentThreadId(),
                                                                std::memory_order_acq_rel)) {}
            ~Scope() { state.owner.store (previous, std::memory_order_release); }
            Scope (const Scope&) = delete;
            Scope& operator= (const Scope&) = delete;
        private:
            NativeParameterSuppression& state;
            Thread::ThreadID previous;
        };
        bool get() const noexcept
        {
            return owner.load (std::memory_order_acquire) == Thread::getCurrentThreadId();
        }
    private:
        static_assert (std::atomic<Thread::ThreadID>::is_always_lock_free);
        std::atomic<Thread::ThreadID> owner { nullptr };
    } inParameterChangedCallback;''')
# Diagnostic observes the real final processor listener; it changes no decision.
replace('    void audioProcessorParameterChanged (AudioProcessor*, int index, float /*newValue*/) override',
        '    void audioProcessorParameterChanged (AudioProcessor* processor, int index, float /*newValue*/) override')
replace('''        if (inParameterChangedCallback.get())
        {''', '''        const bool suppressed = inParameterChangedCallback.get();
       #if SYS01_AU_OWNERSHIP_FIXTURE
        if (sys01SuppressionHook != nullptr)
            sys01SuppressionHook (sys01SuppressionContext, processor, index, suppressed);
       #else
        ignoreUnused (processor);
       #endif
        if (suppressed)
        {''')
replace('static void* sys01BeforeCaptureContext = nullptr;', '''static void* sys01BeforeCaptureContext = nullptr;
static void (*sys01SuppressionHook) (void*, void*, int, bool) = nullptr;
static void* sys01SuppressionContext = nullptr;
extern "C" __attribute__((visibility("default"))) void Sys01AUFixtureSuppressionHook (void (*hook) (void*, void*, int, bool), void* context)
{
    // Installed quiescent; immutable until all callbacks have joined.
    sys01SuppressionHook = hook;
    sys01SuppressionContext = context;
}''')
replace('static void* sys01SuppressionContext = nullptr;', '''static void* sys01SuppressionContext = nullptr;
static void (*sys01EventHook) (void*, void*, const AudioUnitEvent*) = nullptr;
static void* sys01EventContext = nullptr;
extern "C" __attribute__((visibility("default"))) void Sys01AUFixtureEventHook (void (*hook) (void*, void*, const AudioUnitEvent*), void* context)
{
    // Observation at the actual notification boundary. Install quiescent and
    // retain context until every synchronous callback has returned.
    sys01EventHook = hook;
    sys01EventContext = context;
}
extern "C" __attribute__((visibility("default"))) void Sys01AUFixtureGesture (void* processor, int index, bool begin)
{
    auto* parameter = static_cast<sys01::PluginProcessor*> (processor)->getParameters()[index];
    if (begin) parameter->beginChangeGesture();
    else parameter->endChangeGesture();
}''')
replace('''        AUEventListenerNotify (nullptr, nullptr, &auEvent);''', '''       #if SYS01_AU_OWNERSHIP_FIXTURE
        if (sys01EventHook != nullptr)
            sys01EventHook (sys01EventContext, juceFilter.get(), &auEvent);
       #endif
        AUEventListenerNotify (nullptr, nullptr, &auEvent);''')
# Host notification state has a serialized off-audio RestoreState publisher.
# Parameter/gesture readers can run on other threads and must not be suppressed
# by that publisher. Reuse fixed identity storage, independent of native ingress.
replace('''        const ScopedValueSetter<bool> scope { restoringState, true };''',
        '''        const NativeParameterSuppression::Scope scope (restoringState);''')
replace('''    bool prepared = false, isBypassed = false, restoringState = false;''',
        '''    bool prepared = false, isBypassed = false;''')
replace('''    } inParameterChangedCallback;''',
        '''    } inParameterChangedCallback, restoringState;''')
replace('''        if (restoringState)
            return;''', '''        if (restoringState.get())
            return;''')
replace('''        if (! restoringState)
            PropertyChanged''', '''        if (! restoringState.get())
            PropertyChanged''')
# Each event belongs to one synchronous notification stack frame. No shared
# mutable payload, retained pointer, allocation, registry or event replay.
replace('''        zerostruct (auEvent);
        auEvent.mArgument.mParameter.mAudioUnit = GetComponentInstance();
        auEvent.mArgument.mParameter.mScope = kAudioUnitScope_Global;
        auEvent.mArgument.mParameter.mElement = 0;

''', '')
replace('''        auEvent.mEventType = type;''', '''        AudioUnitEvent auEvent {};
        auEvent.mArgument.mParameter.mAudioUnit = GetComponentInstance();
        auEvent.mArgument.mParameter.mScope = kAudioUnitScope_Global;
        auEvent.mArgument.mParameter.mElement = 0;
        auEvent.mEventType = type;''')
replace('''    AudioUnitEvent auEvent;
''', '')
# All AUBase virtual registry writers/readers route through one instance-owned
# access gate and callback acknowledgements; SDK storage remains empty. No pin is edited.
replace('#include "PluginProcessor.hpp"', '#include "PluginProcessor.hpp"\n#include "AUPropertyRegistry.hpp"')
replace('''    ~JuceAU() override
    {''', '''    OSStatus AddPropertyListener (AudioUnitPropertyID id, AudioUnitPropertyListenerProc callback,
                                  void* context) override
    {
        return propertyRegistry.add (id, callback, context);
    }
    OSStatus RemovePropertyListener (AudioUnitPropertyID id, AudioUnitPropertyListenerProc callback,
                                     void* context, bool specified) override
    {
        return propertyRegistry.remove (id, callback, context, specified);
    }
    void PropertyChanged (AudioUnitPropertyID id, AudioUnitScope scope, AudioUnitElement element) override
    {
       #if SYS01_AU_OWNERSHIP_FIXTURE
        if (sys01RegistryProbe != nullptr) sys01RegistryProbe (true);
        struct RegistryProbeExit
        {
            ~RegistryProbeExit() { if (sys01RegistryProbe != nullptr) sys01RegistryProbe (false); }
        } probeExit;
       #endif
        propertyRegistry.changed (GetComponentInstance(), id, scope, element);
    }
    ~JuceAU() override
    {''')
replace('''        deleteActiveEditors();
        juceFilter = nullptr;''', '''        juceFilter->removeListener (this);
        propertyRegistry.shutdown();
        deleteActiveEditors();
        juceFilter = nullptr;''')
replace('''    AudioProcessorChangedUpdater audioProcessorChangedUpdater { *this };''', '''    sys01::AUPropertyRegistry propertyRegistry;
    AudioProcessorChangedUpdater audioProcessorChangedUpdater { *this };''')
# Diagnostic scope spans the entire real wrapper processor-change callback.
replace('static void* sys01CaptureContext = nullptr;', '''static void* sys01CaptureContext = nullptr;
static void (*sys01UpdaterProbe) (bool) = nullptr;
static void (*sys01RegistryProbe) (bool) = nullptr;
extern "C" __attribute__((visibility("default"))) void Sys01AUFixtureRegistryProbe (void (*hook) (bool))
{
    // Quiescent installation; retained through complete joined dispatch.
    sys01RegistryProbe = hook;
}
extern "C" __attribute__((visibility("default"))) void Sys01AUFixtureUpdaterProbe (void (*hook) (bool))
{
    // Quiescent installation; retained until every worker joins.
    sys01UpdaterProbe = hook;
}''')
replace('''        audioProcessorChangedUpdater.update (details);''', '''       #if SYS01_AU_OWNERSHIP_FIXTURE
        if (sys01UpdaterProbe != nullptr) sys01UpdaterProbe (true);
       #endif
        audioProcessorChangedUpdater.update (details);
       #if SYS01_AU_OWNERSHIP_FIXTURE
        if (sys01UpdaterProbe != nullptr) sys01UpdaterProbe (false);
       #endif''')
# Processor-change updates publish fixed atomic flags off-message. A timer
# registered during wrapper construction owns all deferred registry/preset work.
# Keep synchronous message-thread delivery, including nested update ordering.
replace('class AudioProcessorChangedUpdater final  : private AsyncUpdater',
        'class AudioProcessorChangedUpdater final  : private Timer')
replace('''        explicit AudioProcessorChangedUpdater (JuceAU& o) : owner (o) {}
        ~AudioProcessorChangedUpdater() override { cancelPendingUpdate(); }''', '''        explicit AudioProcessorChangedUpdater (JuceAU& o)
            : owner (o), messageOwner (MessageManager::getInstance()->getCurrentMessageThread())
        {
            // Construction/configuration is off audio. Message ownership remains
            // fixed until legal message-thread teardown; no manager read on audio.
            startTimerHz (60);
        }
        ~AudioProcessorChangedUpdater() override { shutdown(); }
        void shutdown()
        {
            // Host excludes new wrapper entries; processor publishers have joined.
            // Message-thread destruction excludes an active timer/callback frame.
            stopTimer();
            callbackFlags.store (0, std::memory_order_release);
        }''')
replace('''                if (MessageManager::getInstance()->isThisTheMessageThread())
                    handleAsyncUpdate();
                else
                    triggerAsyncUpdate();''', '''                if (Thread::getCurrentThreadId() == messageOwner)
                    deliver();''')
replace('''        void handleAsyncUpdate() override
        {
            const auto flags = callbackFlags.exchange (0);''', '''        void timerCallback() override { deliver(); }
        void deliver()
        {
            const auto flags = callbackFlags.exchange (0, std::memory_order_acq_rel);
            if (flags == 0)
                return;''')
replace('''        JuceAU& owner;

        static constexpr int latencyChangedFlag''', '''        JuceAU& owner;
        const Thread::ThreadID messageOwner;
        static_assert (std::atomic<int>::is_always_lock_free);

        static constexpr int latencyChangedFlag''')
replace('''        juceFilter->removeListener (this);
        propertyRegistry.shutdown();''', '''        juceFilter->removeListener (this);
        audioProcessorChangedUpdater.shutdown();
        propertyRegistry.shutdown();''')
replace('/*  The call to AUBase::PropertyChanged may allocate hence the need for this class */',
        '// Off-message publication performs no registry, preset, or message work.')
output.write_text(text)
