# Sprint10 session14: final frame gate release before notification

S10-AU-REGISTRY-09 moves the required final matching frame broadcast after registry
gate release. Sprint10 remains IN PROGRESS; release NO-GO/development-unqualified.
Capture/admission/exit still acquire the BLOCKING gate, and final OS broadcasts
remain prohibited audio work. This bounded critical-section/order repair does not
establish nonwaiting audio or whole G01 acceptance.

## Reproduction frozen before repair

All155 source and27 reference inputs matched session13 before any product edit.
Unchanged session13 final ON Release AU fingerprint
809c8b4d022aff5f09973a7961f04f9fd563546a3a8576d4244d525204fdf8da,
binary edd909893414c2b9425fd9b41c06d2203f864a246f70d1d38e3a7d2ad85620aa,
reproduces four final matching frame broadcasts while the registry gate is held.
The inherited loaded fixture uses real SDK NickName recursion at depths1/8/64/257
and both real WithUserData and loaded SDK Lookup legacy removers, which wait then
acknowledge2. Original-call pthread mutex lock/unlock and condition broadcast
interposition observes the last acquired mutex still held when broadcast begins.
The real control broadcasts twice, once before and once after unlocking, and
reports calls2/held1. Stack unwinding is separately diagnostic-owned and observes
registry changed -> generated PropertyChanged -> SDK -> AudioUnitSetProperty.
No synthetic PropertyChanged or product negative observer was added. Host barriers,
SDK formatting and unwinding are separate from plugin-controlled notification.
This proves avoidable broadcast-under-gate ordering/critical-section work, not
an observed deadlock, data race, deadline miss or measured performance budget.

Complete source export, header, inherited fixture, new fixture source/executable,
interposer source/library, independent Release allocation probe source/header/library,
generated AU/SDK/VST3, cache/compile commands, pinned29-file SDK, bundles and actual
reports/logs were frozen BEFORE repair in session14 negative-identities.json and
negative-freeze.json. Original sandbox registration failure and authorized expected
negative are retained. Frozen bytes are never modified. In-process execution
installs nothing. Session13, rather than inherited session12 identities, is authority.

## One bounded implementation change

Only existing product implementation changed: AUPropertyRegistry.hpp Frame destructor.
It acquires the same gate with unique_lock, performs the same constant unlink and
successor backlink update, decrements Entry.liveFrames once, and computes a local
notification decision under the gate: transition to0 with waitingRemovals nonzero.
It explicitly unlocks, then broadcasts if that retained decision is true. There is
no Entry, live frame-list, snapshot, demand or host context access after unlock. The referenced
owner condition variable remains alive through complete dispatch return. No reader
shared_ptr operation, allocation, reclamation, listener cap/loss, TLS, table, queue,
retained host payload, event replay or gesture coalescing was introduced.

For any matching remover, its predicate check and condition wait atomically releasing
gate precede exit acquiring gate, or follow exit unlinking. In the former order,
notification occurs after the waiter has atomically released gate, so it cannot miss
the notification. In the latter order the predicate already sees the completed unlink
and can acknowledge without waiting. Spurious wakes are allowed and predicates always
recheck under gate. A new matching add/admission between unlock and broadcast retains
and counts EACH matching pending demand before publication; a wake sees that new frame
and waits again, whose eventual final exit broadcasts. A later remover that sees no
frame needs no notification. Independent and withdrawn removers retain their own
WaitDemand scopes through acknowledgement. A demand may disappear before the delayed
notification, making that broadcast unnecessary, but it cannot make acknowledgement
unsafe. This change does not promise zero redundant broadcasts or whole-predicate
notification rather than separate final Entry transitions.

The registry object and condition variable outlive the ENTIRE Frame destructor and
changed call, including this unlocked notification tail. Legal wrapper teardown must
exclude new entries and join ALL complete wrapper/processor dispatches and fixed
message-owner work before shutdown/destruction. A successful listener removal alone
acknowledges callback context use; it does not acknowledge the complete wrapper call,
notification tail, updateHostDisplay or final forwarding. No disposal inside a host
callback is permitted. Same-thread active/ancestor contexts remain alive until their
frames return. Errors permit no context release. Host context destruction excludes
new matching registrations. These are existing lifetime requirements, not new arbitrary
concurrent-lifecycle acceptance. Updater stop/clear precedes registry shutdown while
ContextName wrapper-this lives; GetMutex remains null.

ALL add publishers, remove's callback-origin wait rejection, absent early return,
active/withdrawn/new matching frame scans, unique retained awaited entries, intrusive
WaitDemand, per-Entry waiting counts and exceptional wait cleanup remain unchanged.
EVERY add reserves retention for EVERY matching demand before publication; throwing
work finishes before logical mutation. Ordered immutable snapshots preserve duplicates,
no cap and outer/nested visibility. Current/retired ownership, writer-only use_count1
reclamation and normal reader never-last-owner deletion remain. The gate cannot be
removed until EVERY writer/reader/context has replacement publication and acknowledged
reclamation. ALL SDK virtual Add/Remove/PropertyChanged paths route here; SDK vector
empty/accessor uncalled, conditional MIDI source-only.

## Verification and boundaries

Exact commands, outcomes and artifacts belong to checkpoint/reports/commands, not to
inferred realtime acceptance. Loaded ordering fixtures verify final broadcasts occur
after gate release, retain real held/unlocked controls, both selectors and nested257
with explicit8MiB SDK stacks and legal joins. Release and separate ASan-UBSan/TSan
instrument generated AU/SDK/VST3 and fixtures; sanitized condition checks do not prove
malloc interposition. Shipping defaults OFF are checked separately for hook/probe
absence and loaded ordering. Inherited active/withdrawn rejection, independent second
removers, pending matching publication, overlap both orders, constant unlink,
ContextName64, ordered257 duplicates, first-thread/context destruction128, updater,
editor/native/core and absent allocation-fault checks retain their named boundaries.

A supplemental DIRECT boundary fixture pauses the external original-call broadcast
after final unlink/unlock. During that pause an unrelated add writer and a second
remover complete for BOTH selectors. On resume the first remover acknowledges, exactly
one unlocked broadcast is observed, and complete publisher/writer/remover threads
are joined before context and registry teardown. A second direct case adds and admits
a matching Entry in that same paused tail; the delayed first broadcast wakes and
rechecks the new frame, and acknowledgement waits for its own final broadcast.
Both selectors retain calls2 and broadcasts1/1. This scheduling observation supports
the full source lifetime proof; it is distinct from the actual loaded negative.

Pinned-source review retains APVTS virtual authority -> raw atomic mirror -> recursive
LockedListeners callback -> flag stores; regular/final parameter listenerLock and
synchronized stable-editor removal. Processor updateHostDisplay reads size outside
lock, gets pointers under lock, calls unlocked; final forwarding traverses without
processor listenerLock. VST3 CachedParamValues uses SEPARATE atomic floats/grouped
atomic flags, immutable IDs after shared construction and component/controller retained
storage, not packed words or event history. Prepared maps/scratch/DSP/callback/FL locks,
resource acquisition and invalid concurrent lifecycle remain open. No replacement is
accepted without complete EVERY-writer/reader/lifetime protocol.

Preserve org.algorhythmia.sys01/Algr/Sys1,7keys/nativeIDs,4programs/order,schema1exactly40,
parameter-only deterministic empty-history recall,10ms smoothing,shared256 whole-batch
rejection/no replay,eight restores,single audio owner,stereo effect/generator/disabled
input/formats,stable editor atomicdirty/seven authoritative packed FLOAT-GENERATION
words/synchronized removal/message gestures/undo/recall/visibility.

Remaining blockers include capture/admission/exit waits, final OS notifications,
host-sized dispatch/remove/reclaim/add-demand, allocating mutating audio-reentered
writers, host/cross-instance cycles, parameter lock orders, immutable prepared resources
and safe invalid-lifecycle silence/error. Historical session09 genuine default-stack
ASan overflow257 and session10 allocator-control/leak assertions stay frozen. Finite
successful sanitizer logs cannot accept arbitrary stack/lifecycle/whole realtime.
Release editor is unpaced offscreen CPU, not DAW/UI budget qualification. Actual DAW,
external OS/architecture/signing/rights/accessibility/humans/budgets/soak/MB-04 remain
open. Sprint10 continues without advancement or waiver.

Session14 also retains a real TSan diagnostic tooling failure. Instrumented external
pthread mutex replacement entry functions were called by TSan InitializeInterceptors
through atexit during dyld startup, before main/AU loading, and crashed with SIGSEGV.
Exact retry and two insufficient bootstrap adjustments repeated it; original binaries,
libraries, empty logs and OS crash reports remain. no_sanitize(thread) suppresses
memory checks but still emits function-entry hooks. The corrected external shims use
disable_sanitizer_instrumentation, pass straight to originals until post-initialization
conditionBegin, and retain original-call measurement/control afterward. Only those
two external bootstrap entry functions are excluded; generated AU/SDK/VST3, product
registry, callbacks and direct protocol fixtures retain separate TSan instrumentation.
The corrected loaded TSan fixture passes on the SAME AU bytes, with held/unlocked
control2/1 and zero-held final broadcasts. This is not a sanitizer-clean-all-history
claim or an exclusion of product code.
