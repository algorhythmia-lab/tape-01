# Sprint 10 session 09: constant-work AU frame unlink

S10-AU-REGISTRY-04 repairs host-sized RAII frame unlink at the local exit-order
boundary. Sprint10 remains IN PROGRESS, release NO-GO/development-unqualified.
The registry gate remains BLOCKING; this does not establish nonwaiting admission,
exit or complete realtime behavior.

## Frozen loaded result

Actual diagnostic Release AU fingerprint
92abd2f1546ae62063516334d406da0d1d8f47e9f5bf0c1aed7eb038c97dba37.
Native bypass SetParameter consumed by real Render admits an older callback.
Another worker sets real SDK NickName and nests its synchronous property callback
1, 8, 64 and 257 levels. While those newer frames remain active, the older native
callback returns. Original RAII unlink scans 1, 8, 64 and 257 predecessors under
the registry gate, violating the fixed-work exit requirement. Both removers still
correctly wait for matching contexts, and inherited checks pass.

The observer increments only for actual predecessor visits. It does not delay,
change admission, invoke synthetic PropertyChanged or make a timing assertion.
Host barriers are fixture work outside measured plugin unlink. This is an observed
unbounded-work defect, not a reproduced deadlock, data race or timing miss. Negative
source includes a defaultOFF observer added to the unchanged old unlink algorithm.
The prior shipping/default bytes are separately retained; no claim that the old
shipping binary contains the observer. Exact fixture executable/source, complete
source export, generated AU/SDK/VST3, generator, cache, compile commands, loaded
bundle and pinned SDK copy are frozen before repair in session09 evidence.

All150 source and17 reference inputs matched session08 before edits. No historical
packet was changed. The baseline sandbox register failure is separately retained;
authorized in-process runs load exact temporary bundles without installation.

## Every frame participant and ownership invariant

Admission still holds the same mutex as withdrawal, active checks, frame scanning,
publication and reclamation. Each admitted stack Frame gains an incoming-link
pointer: either the registry head address or a live predecessor's next address.
At head insertion, its incoming link is the head and the former head's incoming
link becomes the new frame's next address. Unlink writes the incoming link to next;
if a successor exists, its incoming link becomes the exiting frame's incoming link.
All these operations occur under the gate before that frame's stack disappears.

Thus when a predecessor exits first, it transfers the successor's incoming link
to its own predecessor/head. When a successor exits first, it updates its live
predecessor's next. Middle, head and tail exits share this rule. Concurrent exits
serialize with insertion and each other; a frame never dereferences a departed
stack predecessor. Unadmitted inactive frames have linked=false and do no unlink.
The destructor still performs RAII cleanup and notifies removal waiters after
unlink under the gate. It now performs one mandatory pointer update and at most
one successor backlink update, without list search or reader allocation/reclamation.
The observer reports zero predecessor visits; source establishes fixed pointer
work. Finite zero observations alone are not a universal ownership proof.

Remove still scans the entire live list under the gate, including withdrawn
frames and second removers. Callback-origin same-instance wait rejection occurs
before allocation/withdrawal/publication and errors acknowledge no context release.
Successful off-callback remove withdraws then waits with the gate released for all
matching other-thread frames. Same-thread active/ancestor contexts live until their
frames return. Ordered immutable snapshots, duplicates/no cap, nested visibility,
writer-only retired use_count1 reclamation and normal-reader non-last-owner behavior
are unchanged. Gate stays until EVERY writer, reader and context has replacement
publication and acknowledged reclamation; backlinks alone are not that replacement.

All SDK virtual writers/readers still route here; SDK vector empty, const accessor
has no pinned caller. State/layout/channel/stream/input/render callback/max frames,
LastRenderError/preset/name and conditional MIDI paths retain session08 routing.
Conditional MIDI remains source-only. ContextName constructor callback owns wrapper
this through legal shutdown. Updater stop/clear precedes registry shutdown. Legal
teardown excludes and joins ALL complete wrapper/processor entries, fixed message
owner, never disposal in callbacks. The gate does not acknowledge whole wrapper
lifetime. Generator/SDK/pins/shared processor/DSP/editor/VST3 implementation unchanged.

## Verification limits and fixture correction

Tests overlap actual native/SDK dispatch, return the older frame while newer frames
remain, then run both real removal selectors against the live nested frames. Both
must remain waiting until release; after SDK unwind both acknowledge. Depths
1/8/64/257 pass. Inherited first-thread entry, active/withdrawn callback-origin cases,
ContextName64/legacy duplicates, order257, cross-add, second remover, overlap2/2 and
128 fresh-thread context destruction remain. Updater legal teardown64 is retained.

Initial ASan runs exhausted the default worker stack at the 257-level real SDK
recursion; depths1/8/64 passed. Retained stacks show repeated fixture callback ->
AudioUnitSetProperty -> SDK -> registry dispatch. This is a genuine observed ASan
stack-overflow, not evidence of a backlink race. The fixture now uses pthread_create
with an explicit8MiB stack for this SDK recursion in all trees, retains full depth,
and joins before destruction. Product recursion/lifecycle semantics are unchanged;
no arbitrary host stack-depth acceptance is inferred. Separate ASan/UBSan and TSan
checks include generated wrappers and SDK as well as observer fixtures.

## Listener/cache/resources follow-up

Pinned ParameterAdapter reads virtual authority, publishes atomic raw mirror,
then calls APVTS LockedListeners under recursive CriticalSection. Add/remove use
that same lock. Regular reverse parameter listeners and final ParameterChangeForwarder
run under parameter listenerLock, including synchronized stable editor removal.
Processor updateHostDisplay reads size outside its listenerLock, gets each pointer
under lock, releases lock before callback. Final forwarding traverses processor
listeners without processor listenerLock. Processor removeListener alone acknowledges
neither complete path. Those lifetime and lock-order boundaries remain unresolved.

VST3 JuceAudioProcessor constructs maps and CachedParamValues before publication;
component/controller references retain shared storage. Off-message paramChanged
calls setParameterValue; process readout uses forAllChangedParameters. Cache float
values are separate atomic floats; grouped atomic flags publish/clear change bits.
There is no packed float/flag word. IDs are immutable after construction, cache
storage setup/destruction still needs legal exclusion. Atomic updates/readout do
not establish every-writer callback/reclamation or resource lifecycle safety.
Prepared AU mapper/scratch, VST3 maps/buffers, DSP prepare/release and callback/FL
locks remain Sprint10 blockers, as do capture/admission/exit mutex, exit notify,
host-sized dispatch/remove/reclaim and audio-reentered allocating writers.

Cross-instance cycles, callback waiting for its own off-callback remover, arbitrary
parameter reentry and concurrent same-role/recursive state restore remain unqualified.
No listener cap, dropped notification, gesture coalescing, TLS/table/queue or retained
host payload is introduced. Preserve identity/IDs/seven keys/four programs/schema1
exactly40, parameter-only deterministic empty-history recall,10ms smoothing,shared256
whole-batch rejection/no replay,eight restores,single audio owner and formats. Stable
editor atomicdirty/seven packed authoritative float-generation words/synchronized
removal/message gestures/undo/recall/visibility remain.

Affected AU callback/concurrency/lifecycle/host/validator/performance/soak/signing
evidence is invalidated and renewed only for named locally tested bytes. Historical
seals stay frozen; Sprint08 live-source remains intentionally invalid. ActualDAW,
externalOS/architecture,signing/rights/accessibility/humans/budgets/soak/MB-04 remain
open. No Sprint11 advancement or waiver. Exact final results/identities are in the
session09 checkpoint and handoff.
