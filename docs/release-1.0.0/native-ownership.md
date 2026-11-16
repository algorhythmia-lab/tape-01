# Native callback ownership / Sprint 01

Inspected before changing synchronization, 2026-10-14. Source revision
5c45cf6bed431317a1b412b1fc12ee62b66cfdd5, pinned JUCE
29396c22c93392d6738e021b83196283d6e4d850. Line references describe that checkout.

## Actual entries and owners

VST3: `JuceVST3Component::process` (3683) takes the conditional FL Studio
specification-enforcement mutex, copies host context, clears MIDI, processes
parameter queues, converts input IEventList, maps buffers via
`ClientRemappedBuffer`, then `processAudio` (3825) takes AudioProcessor's callback
lock and checks suspend before calling SYS 01 `processBlock`. Output changes
invoke host IParameterChanges methods. Those virtual host methods can themselves
block/allocate; the plugin cannot certify their implementation. `setupProcessing`
(3574) configures precision/rate/buffer maps; `setActive` (2804) prepares/releases
DSP and storage. setProcessing(false) resets. Host lifecycle must exclude
processing; FL's workaround explicitly supports a violating host. Buffer maps,
scratch/channel vectors and plugin lifetime cannot be mutated during process.

AU: AudioUnitSDK dispatch/render entry -> `JuceAU::Render` (1461) pulls host
input, prepares/maps buffers and copies input, locks `incomingMidiLock` (1527),
swaps incomingEvents/midiEvents, calls `processBlock` (2131), which locks the
AudioProcessor callback lock, installs playhead, checks suspend/bypass, and
calls SYS 01; Render then copies output and clears MIDI. SDK dispatch, input
pull callbacks, property and parameter notifications are part of the full
boundary. `Initialize`/`prepareToPlay` (267), Cleanup (releaseResources) and
Reset own preparation. Offline-property change also takes callback lock and
reprepares (800). HandleMIDIEvent (1555), HandleSysEx (1575) and MIDIEventList
(1588) take incomingMidiLock; upstream UMP dispatch and MidiBuffer insert/copy
can allocate and traverse host-sized data before our processor bounds iteration.
AU SetParameter (1167) updates parameters immediately, ignoring positioned
musical scheduling. Sprint 02 owns AU offset parity; no new timing claim here.

## Parameters, restore and editor

Audio owns host::Processor, native ingress, cached control targets and render
scratch. Restore producers are serialized by stateMutex off audio; eight usable
SPSC slots carry immutable State. The epoch and captured queue boundary prevent
partially published or newer transactions from contaminating an older snapshot.
The audio drain is at most eight and applies one reset to the newest captured
state. State serialization uses stateMutex off audio. Zero blocks consume no
restores or UI edges. UI has one producer for 64 queued raw brake/STATE edges;
ties merge scalar targets, native parameter points, UI edges then MIDI, preserving
order within each source. Overflow rejects the whole musical batch; host final
targets are acknowledged without replay, UI edges remain queued for recovery.

Framework automation -> AudioProcessorParameter::sendValueChangedMessageToListeners
(listenerLock, processors/juce_AudioProcessorParameter.cpp:113) -> APVTS
ParameterAdapter::parameterValueChanged (utilities/juce_AudioProcessorValueTreeState.cpp:148)
-> LockedListeners::call (185) -> editor ParameterAttachment listener / async
message publication. VST3 controller forwarding can also enter AudioProcessor's
listenerLock and host callbacks. The editor registers/unregisters APVTS attachments
on the message thread, edits host parameters and undo history there, and polls
DSP telemetry at 30 Hz. No DSP pointer is exposed to it. Opening/closing an editor
can contend with callback parameter notification; this is a real unresolved path.
Synthetic VST3 program parameter setValue invokes setCurrentProgram -> restore,
including the state mutex, gestures/listeners and host notifications, on the
calling thread. It is not an ordinary position-automatable catalogue parameter.
Host state/program calls on audio remain outside the non-audio restore contract.

## Synchronization decision

Retain callback, listener, ingress and FL locks. Removing them without replacing
all participating lifecycle/property/editor paths would permit map mutation,
listener destruction or DSP preparation concurrent with rendering. A future
replacement needs a complete lifecycle gate: non-audio configuration publishes
immutable prepared resources; audio acquires without waiting; configuration
waits off audio for the previous render owner; reclamation follows acknowledgement;
invalid concurrent host lifecycle must return silence/error without racing maps.
Parameter notifications need audio-to-message-thread publication with ordered
restore/automation generations, truthful host readback/save and editor teardown
coverage. AU ingress needs a bounded multi-producer handoff covering legacy and
UMP dispatch. None of these proofs follows from deleting a ScopedLock.

Sprint 01 changes fixed VST3 MIDI ingress and whole-batch validation, adds
immutable telemetry and whole-wrapper probes. Outstanding lock/SDK/host paths
remain release blockers and are measured as such, not waived as upstream code.


## Measured follow-up / Sprint 01

Pinned native MIDI work is now bounded in both generated wrappers as described
in ADR 0003. Synchronization decisions above are unchanged. The dedicated-audio-thread
VST3 probe records one calloc on first entry; bounded stack capture resolves
malloc_type_calloc_outlined -> dyld ThreadLocalVariables::instantiateVariable /
tlv_get_addr -> processParameterChanges -> process. The upstream translation unit's
thread_local notification flag at line 829 is the relevant TLS path. Warm subsequent
editor automation still enters four mutexes typically (seven while first posting).
No shared global flag substitution was attempted: it would suppress unrelated UI
notifications and invalidate per-thread nesting semantics without a replacement proof.

ParameterAttachment takes a different branch on the message thread: synchronous
handleAsyncUpdate performs UI work. Earlier probe runs accidentally exercised that
branch; corrected normal audio-owner results live in ctest-probe-stack.log. Source
trace for normal audio is parameterValueChanged -> triggerAsyncUpdate -> MessageBase::post
-> macOS MessageQueue::post -> locked ReferenceCountedArray::add + CFRunLoop wakeup.
That queue may grow and contends with the message thread. Warm observed zeros cannot
qualify arbitrary queue/editor load. Allocation stack capture intentionally suppresses
its own unwinder instrumentation; use a separate probe-free build for timing.

AU SDK render-cache behavior can skip a same-timestamp call even with a different
block length. Fixtures use distinct timestamps for zero and subsequent blocks;
real-host zero/nonzero timestamp sequences are unresolved format/host evidence.
Native VST3 4096-frame coverage is also outstanding (AU/adapter already exercise it).
