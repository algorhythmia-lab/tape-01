# Sprint 10 session 06: AU processor-change notification ownership

S10-AU-UPDATER-01 repairs one prohibited off-message operation. Sprint10 stays
IN PROGRESS; release NO-GO/development-unqualified. Registry nonwaiting ownership,
resources, arbitrary reentry and complete callback qualification remain open.

## Frozen loaded negative

Diagnostic Release AU fingerprint
`26a6ab040623d762eada3fea22ad9c8827931c528297d07948fe8e41dfa970e4`, binary
`3dc02bb219a82110c98c93f71d15b5621e896614aa20504da618ee75bb2038a5`.
The fixture captures the actual processor during real first-thread Render, then
calls public AudioProcessor::updateHostDisplay with all three ChangeDetails flags
from a fresh worker. The hook spans the entire real JuceAU::audioProcessorChanged
callback; it neither calls the updater directly nor changes its decision.
Actual alloc/free/mutex/unfair=1/0/4/0. Stack: probeRealloc -> JUCE MessageQueue::post
-> JuceAU::audioProcessorChanged -> AudioProcessor::updateHostDisplay -> fixture.
Control alloc/free/mutex/unfair=1/1/1/1. Functional delivery already passed before
repair. This is a prohibited operation on a loaded bundle, not a fabricated race.
Sandbox registration and compile/setup failures are separately retained. Actual
in-process authorized run installs nothing. Source export, fixture, probe, generated
wrapper/SDK, cache, compile commands and bundle frozen before repair; never edit them.

## Bounded change

Pinned generator replaces this updater's AsyncUpdater with a Timer registered
at 60Hz during off-audio, fixed-message-owner wrapper construction. Off-message update computes flags,
performs one lock-free atomic fetch_or, compares pthread thread identity against
an immutable message owner and returns. No manager singleton lookup, manager
thread mutex, AsyncUpdater trigger, queue post, registry traversal, preset work,
retained payload, allocation or reclamation occurs in that path. Message owner is
captured from the initialized manager during construction and cannot migrate
while this wrapper lives. Construction and destruction run on that message owner, excluding timer dispatch
during partially constructed member publication. This is a fixed-message-owner contract, not a
concurrent manager reconfiguration protocol.

Message-thread updates still deliver synchronously. Timer exchanges dirty flags
before callbacks, preserving concurrent later publications and synchronous nested
updates. Zero flags do no property/preset work. Existing flag coalescing is retained;
no value or gesture payload is coalesced or replayed. Property order remains latency,
parameter list/info, ClassInfo, preset refresh/PresentPreset. A stopped message loop
can delay delivery; audio never starts/posts/waits for the timer. Timer machinery
may allocate/post on its own scheduling/message threads, outside this publisher.

Wrapper teardown removes bypass/processor listeners, explicitly stops updater
and clears pending flags, then shuts registry before destroying wrapper/processor.
Processor change dispatch has unsynchronized listener traversal in pinned JUCE:
its removeListener lock alone is NOT acknowledgement for arbitrary concurrent
updateHostDisplay. Host must exclude/join new processor/wrapper entries and destroy
on the fixed message owner outside callbacks; timer callback cannot overlap legal
message-owner destruction. Disposal from a callback, concurrent processor dispatch/
listener mutation, arbitrary wrapper entry and concurrent lifecycle remain invalid
and unqualified. No broader reclamation claim is made.

## Complete inherited registry caller map

All virtual SDK AddPropertyListener/RemovePropertyListener/PropertyChanged still
route to the unchanged instance registry. SDK GetPropertyListeners const accessor
has no caller and SDK vector remains empty. State/RestoreState, layout/channel/
stream changes, input connections/render callback, element/preset/name/property
removal, LastRenderError, AUEffectBase sample-rate parameter list, MIDI maps/hot map
and wrapper bypass use the same blocking recursive registry gate. MIDI map path
is conditional in the pinned SDK; no new supported mapping claim is made.
ContextName's constructor registration calls updateTrackProperties with wrapper
this; context lives through registry shutdown. Legacy no-context removal selector
and WithUserData remove preserve duplicates/context matching. Reentrant removal
skips withdrawn callbacks; same-frame/ancestor contexts survive until return;
cross-thread registry removal waits for callbacks. Nested add uses next snapshot,
normal readers never last-owner delete. All session05 writer/reader gates remain.

AudioProcessorChangedUpdater is the only changed caller: off-message only publishes
flags, message/timer calls actual virtual dispatcher. Host callback bodies/OS AU
event implementation are separate from plugin registry traversal and JUCE queue
scaffolding. Native bypass still synchronously invokes host listeners inside Render.
No ingress is held across listeners. Registry host-sized traversal, blocking gate,
writer allocations/free on audio, cross-thread reentry deadlocks and parameter-lock/
registry-lock order remain open. Do not remove the gate without complete every-
writer/reader/context publication and acknowledged reclamation.

APVTS ParameterAdapter/LockedListeners, regular/final processor listeners,
CachedParamValues, caller-serialized restore lanes, prepared resources and safe
invalid-lifecycle silence/error remain unqualified. Session04 optional SDK guard
serialization claim remains superseded by session05; no historical record changed.
Shared processor/DSP/editor/VST3 generator unchanged. All seven IDs/keys, four programs,
schema1 exactly40 bytes, deterministic empty-history recall,10ms smoothing,256-event
whole-batch rejection/no replay,eight restore slots,single audio owner and formats
remain. Stable editor dirty relay and synchronized parameter listener lifetime remain.

Verification and exact final artifact identities are retained in session06 evidence
and handoff. Finite zero updater measurements and sanitizers do not close whole G01.
