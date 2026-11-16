# Sprint 10 session 04: AU host notification state and event ownership

S10-AU-NOTIFY-01 is a bounded repair under S07-P1-03. Sprint10 remains
IN PROGRESS; release remains NO-GO/development-unqualified. It repairs
cross-thread restore suppression and per-call event payload ownership, not the
property registry, arbitrary listeners, message posting or prepared resources.

## Frozen actual failure

Diagnostic Release AU fingerprint
`d3e86a4d481c2f56a37af30938e6a21fc1bf971eff07f522386664f3c83c68f1`,
binary SHA256 `008da982038b23705ee396e26cf0acd8aafc3c133f5393b3317f52bb248805eb`
is retained in evidence/sprint-10-session-04 before repair. Its diagnostic hook
only observes the real boundary immediately before AUEventListenerNotify. During
real ClassInfo restore, a regular FLUX listener pauses the restore thread. Another
thread writes BIAS and BYPASS, without entering the held FLUX listener lock. The
old plain restoringState suppresses both parameter events (0 instead of2) and the
real SDK bypass property callback (0 instead of1). Another instance sends1/1;
after restore the first instance sends2 events/1 property callback. Fixture
scheduling barriers are not shipping work and no Render overlaps this restore.

Refined fixture reenters BIAS at the outer FLUX event boundary. The shared event
changes parameter ID before the outer AUEventListenerNotify; intact0 instead of1,
with2 actual boundary calls. This proves shared payload corruption under forced
synchronous reentry, not that macOS actually reentered from its own implementation.
Both fixtures and original reports are frozen. The initial sandbox registration
failure occurred before any plugin test; authorized in-process runs reproduce
both actual failures without installation. No TSan failure is invented.

## Every state/event writer and lifetime

References below use the session04 generated Sys01AU.mm; trace evidence retains
source hashes and numbered excerpts. Upstream JUCE/SDK pins are unchanged.

| Path | Owner, lifetime and remaining boundary |
|---|---|
| Constructor/destructor | Suppression identity lanes initialize empty as instance members before callback publication. Constructor no longer initializes shared event storage. Wrapper/processor/parameter lifetime still requires host callback exclusion before destruction. Bypass removeListener is synchronized by the retained parameter lock. Processor is destroyed inside wrapper destruction; arbitrary concurrent teardown is not qualified. |
| RestoreState965 | Sole restore-lane publisher; ClassInfo dispatch is serialized by SDK AUInstanceGuard/GetMutex, and SYS01 restore producers retain stateMutex. A stack scope exchanges publisher identity, restores predecessor on every exit, including errors. Direct concurrent same-role RestoreState is invalid; recursive SYS01 restore under stateMutex remains unqualified. Render/resources are not legitimized by this lane. |
| sendAUEvent1462 | Parameter value and gesture final listeners only read restore identity. Suppression compares current thread, so synchronous restore-origin callbacks stay suppressed while independent threads pass. One zero-initialized AudioUnitEvent belongs to each stack frame; unit/scope/element/type/native ID are initialized before use. No shared event writer remains. |
| AUEventListenerNotify1477 | Called synchronously with const pointer to this frame's event. No plugin retains it, defers/replays it or stores event payloads. macOS owns its implementation and event-listener registry. Header declares CA_REALTIME_API; that annotation is not a measurement or proof of all OS behavior. Hook observes actual plugin submission, not eventual OS listener delivery. |
| audioProcessorParameterChanged/gesture begin/end | Existing native suppression lane reads remain separate from restore lane. Parameter regular/final locks and processor listeners unchanged. Seven different parameter writers can reach final AU event creation concurrently; stack events do not share writable bytes. |
| Bypass parameterValueChanged1514 | Reads restore identity, then synchronously calls PropertyChanged. Independent bypass notifications now pass during restore. Native bypass still calls PropertyChanged even when native parameter-event suppression is active. Registry traversal and host callback costs remain unresolved. |
| Diagnostic hooks | AU ownership option defaultOFF. Event/gesture hooks add no shipping state. Installed/removed quiescent; callback/context immutable until worker joins; hook event pointer is read only during the synchronous frame. Fixture FLUX listener removed only after external writer joins and restore returns. No hook lifetime/reclamation claim extends to shipping arbitrary listeners. |

Identity storage is the already-reviewed fixed lock-free atomic ThreadID class,
with separate native and restore instances. The restore lane has one serialized
off-audio publisher. Atomic readers retain no pointer and do not consume the lane.
Stack predecessor restoration supports notification nesting; it does not promise
recursive restore while holding stateMutex. No TLS bootstrap, registry, heap,
queue, spin/retry, ingress extension or lock deletion is introduced. Stack events
also handle overlap/reentry without a global flag or event history.

## Complete property registry writer and callback map (unrepaired)

Pinned AUBase.h716 owns std::vector<PropertyListener>. Only AddPropertyListener
(AUBase.cpp944) reserves/pushes and RemovePropertyListener961 erases. Construction
initializes it; destruction frees it after legal callback exclusion. Const
GetPropertyListeners527 has no caller in the pinned SDK. Host add/remove variants
(AUPlugInDispatch.cpp199 onward) hold AUInstanceGuard. Wrapper constructor adds
ContextName dispatcher255 before host publication. That dispatcher calls
updateTrackProperties3037; its context is wrapper `this`, valid until destruction.

AUBase::PropertyChanged977 walks the entire mutable vector and invokes callbacks
whose propertyID matches. SDK SetProperty/RestoreState/layout/input changes and
LastRenderError also call this virtual dispatcher, as do wrapper bypass and
AudioProcessorChangedUpdater. SDK dispatch-originated property work generally
has instance guard; processor-originated bypass/updater dispatch does not acquire
that guard. Guarded add/remove alone therefore does not synchronize all readers.
Reentrant add/remove from a host callback can invalidate vector traversal; no
acknowledged callback/context reclamation exists for that overlap. Off-audio
quiescent add/remove tests pass only their stated boundary, not this scenario.

The vector traversal, capacity growth, erasure and callback dispatch scaffolding
are plugin-controlled SDK work. Listener count is host-controlled and unbounded.
The invoked host callback implementation and macOS AU event registry are distinct
host/OS-owned work. The fixture callback only increments atomics; it does not
hide plugin registry traversal by attributing it to the host.

AudioProcessorChangedUpdater2216 owns atomic callbackFlags but uses AsyncUpdater:
off-message update can trigger message allocation/posting. Pending async message
lifetime uses JUCE cancellation/refcount semantics; instance teardown still needs
legal callback exclusion. Message-thread dispatch calls PropertyChanged for
latency/list/info/ClassInfo/preset and refreshCurrentPreset. These paths are not
fixed by event stack ownership. A future registry repair must cover *all* readers,
writers, reentry and callback-context lifetime before removing locks or deferring
work. A fixed listener cap or silently coalescing gestures is not authorized.

## Verification boundary

Release and separate ASan/UBSan/TSan hookON fixtures pass restore2/1/1,
after-restore2/1,nested intact1/events2,seven-key128 values/begin/end each and64
quiescent host listener attach/detach cycles. Dedicated fresh external bypass
probe alloc/free0/0,mutex2; real whole callback probes retain prohibited locks.
Probe negative control observes alloc/free/mutex/unfair1 each. These are finite
measurements, not whole realtime acceptance. Exact matrices/default identities,
editor/core checks and hook/probe absence are recorded in handoff and evidence.

Shared processor/DSP/editor code and state bytes are unchanged. Identity,
org.algorhythmia.sys01/Algr/Sys1,seven keys/native IDs,four programs/order,schema1
exactly40,parameter-only deterministic empty-history recall,10ms smoothing,
shared256 whole-batch rejection/no replay,eight restores,single audio owner and
stereo effect/generator/disabled input remain. Existing stable editor relay stays
atomic dirty off-message and polls seven authoritative words; synchronized
removeListener lifetime/gestures/undo/recall/visibility retained.

AU event/state changes invalidate affected native callback/concurrency/editor/
host/lifecycle/performance/soak/signing evidence. New reports bind new bytes;
historical failures and seals stay frozen, Sprint08 live-source comparison stays
intentionally invalidated. Actual DAW-origin timing/external platforms/signing/
rights/accessibility/humans/performance/soak and MB-04 remain unqualified.

Next: reproduce native bypass PropertyChanged host-count traversal or reentrant
registry mutation deterministically on a frozen loaded AU. Trace all SDK callers
above plus updater lifetime before a bounded replacement. APVTS ParameterAdapter/
LockedListeners, regular/final processor dispatch, CachedParamValues and prepared
resources follow. Sprint10 needs immutable prepared resources, nonwaiting audio
acquisition, off-audio waits, acknowledged reclamation for every writer, invalid
concurrent lifecycle silence/error, whole plugin callback zero-prohibited proof
and passing concurrency/lifecycle tests. No Sprint11 advancement.
