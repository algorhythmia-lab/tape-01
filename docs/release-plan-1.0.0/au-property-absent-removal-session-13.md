# Sprint10 session13: absent AU removal publication and reclamation

S10-AU-REGISTRY-08 removes allocation, retirement, publication and reclamation
from genuinely absent removal. Sprint10 IN PROGRESS; release NO-GO/development-
unqualified. The registry gate remains BLOCKING. Scanning remains host-sized,
mutating writers still allocate, and this is not whole G01 acceptance.

The unchanged session12 final ON Release AU fingerprint
63bfef40f73334c80da13d508a88328e0a479f16c4562934c99b3cb0dc23a8f1,
binary28b7b5854bd6b15713560a1dbca0cfe8792590cc9a3f6ef7ccc090c2155c9b21,
reproduces real native bypass SetParameter consumed inside Render on fresh workers.
Each host property callback removes a NickName registration that was never added.
Both real WithUserData and loaded SDK Lookup legacy selectors allocate and free
inside AUPropertyRegistry::remove. At callback counts1/8/64/257, legacy allocations
are2/16/128/514 and frees0/14/126/512; WithUserData allocations3/16/128/514 and
frees3/16/128/514. Each measured removal takes one mutex. The first extra allocation
reflects actual retirement capacity growth in the retained execution order.

The existing external original-call malloc/calloc/realloc/free/mutex/unfair probe
passes its real1/1/1/1 control. Captured allocation stacks reach registry remove ->
SDK removal -> host callback -> registry changed -> bypass parameter listener ->
publishNativeParameter -> Render -> SDK -> AudioUnitRender. No synthetic
PropertyChanged, product negative observer or admission change was added. The
measurement covers the actual removal call; host callback work, formatting and
first-call unwinding are separate diagnostic ownership. This is observed
plugin-controlled allocation/free work on audio, not a reproduced deadlock,
race or deadline miss. The sandbox registration failure is retained separately.
Authorized in-process loading installs nothing.

Complete source export/header/registry fixture, exact new fixture source/executable,
probe source/header/library, generated AU/SDK/VST3, caches/compile commands, unchanged
29-file pinned SDK, bundles and original reports/logs were frozen BEFORE repair.
The freeze and all identities are in session13 evidence. A tooling SDK lookup
StopIteration occurred before freeze completion because compile commands use SDK
unity; the unchanged pinned copy was then retained and complete freeze established
before the product edit. Never modify frozen bytes.

Only product implementation changed: AUPropertyRegistry.hpp. Under the retained
gate, remove first scans ALL live frames for same-instance caller callback ownership
and matching other-thread frames, including withdrawn Entries. Callback-origin
required other-thread waits still reject BEFORE allocations or mutation. Then,
only when no other-thread match exists and current has no matching registration,
remove returns noErr before allocation, retirement, publication, reclaim or demand.
The same gate linearizes that decision with EVERY add, withdrawal, capture,
admission and frame exit. A later add is a later publication; the host must still
exclude new matching add before context destruction. The optimization performs
no snapshot/shared_ptr ownership operation and cannot make readers reclaim.

WithUserData checks id+callback+context; legacy checks id+callback across all contexts.
An unadmitted published match still follows the existing mutation protocol. A
matching other-thread frame still follows the original acknowledgement protocol
even if current no longer contains the Entry. Independent second removers retain
withdrawn Entry ownership and demand until acknowledgement. A same-thread withdrawn
frame can use the early return, but its context must live until that frame returns;
no successful removal permits destroying active or ancestor contexts in callbacks.
Errors authorize no release. An absent removal changes no logical state and need
not publish an empty equivalent snapshot or reclaim unrelated retired storage.
Later mutating writers and legal shutdown remain the reclamation owners.

Retained protocol: immutable ordered snapshots preserve duplicates/no cap and
outer-versus-nested visibility. Frame references its immutable snapshot shared_ptr
slot. Gate-held admission/constant-work unlink/liveFrames and final Entry notification
remain; waitingRemovals and intrusive WaitDemand keep independent owners through
acknowledgement and exceptional wait cleanup. EVERY add reserves retention for
EACH matching demand BEFORE all throwing publication work completes, then attaches
and counts the new Entry under gate. Removal predicates scan active/withdrawn/new
matching frames. Writers reclaim retired snapshots only at use_count1; normal
readers never perform last-owner deletion. Gate removal still requires replacement
publication and acknowledged reclamation for EVERY writer, reader and context.

Verification binds exact new bytes. The loaded absent oracle checks both selectors
at1/8/64/257; Release separately measures alloc/free0/0, with mutex1 per call retained.
Separate ASan-UBSan and TSan absent fixtures verify calls/status/order and legal
teardown without allocator-interposition claims. Supplemental DIRECT allocation-
fault tests make any observed operator new throw: absent property/context removals
succeed with0 allocations/frees, preserve unrelated registrations, and repeated
same-thread withdrawn removal succeeds while context lives. These are direct
protocol checks, distinct from the loaded negative. Inherited loaded registry,
final Entry condition, new matching publication, both overlapping exit orders,
active/withdrawn rejection, second removers, ContextName64, first-thread/context
teardown, updater/editor/native/core and separate sanitizer checks retain their
named boundaries. Exact final outcomes are in checkpoint/reports and the handoff.

All SDK virtual Add/Remove/PropertyChanged paths route to the registry; SDK vector
empty/accessor uncalled, conditional MIDI source-only. Legal wrapper teardown
excludes/joins ALL complete wrapper/processor entries, fixed message owner, never
callback disposal. Updater stop/clear precedes registry shutdown while ContextName
wrapper-this lives. Processor removeListener alone acknowledges neither full
updateHostDisplay nor final forwarding. GetMutex remains null; restore assumes
caller-serialized same-role off-audio ownership. Recursive/concurrent same-role
restore, owner migration and arbitrary lifecycle remain unqualified.

Continued pinned-source review confirms ParameterAdapter virtual authority ->
raw atomic mirror -> recursive LockedListeners call -> flag stores. Add/remove
use the same lock. Regular/final parameter listeners retain listenerLock and stable
editor synchronized removal. Processor updateHostDisplay reads size outside lock,
gets each pointer under lock and invokes unlocked; ParameterChangeForwarder
traverses without processor listenerLock. VST3 CachedParamValues uses SEPARATE
atomic floats/grouped atomic flags; exchange float(relaxed), flag fetch_or/acq_rel,
drain flag exchange/acq_rel, load float(relaxed). Immutable IDs/storage are prepared
before shared processor publication and retained by component/controller references.
This is latest scalar state, not packed words/event history. Prepared AU/VST3 maps,
scratch, DSP prepare/release, callback/FL locks and invalid concurrent lifecycle
remain open. No lock replacement without complete every-writer/reader/lifetime
ownership.

Preserve org.algorhythmia.sys01/Algr/Sys1, seven keys/nativeIDs, four programs/order,
schema1exactly40, parameter-only deterministic empty-history recall,10ms smoothing,
shared256 whole-batch rejection/no replay, eight restores, single audio owner,
stereo effect/generator/disabledinput/formats and stable editor atomicdirty/seven
authoritative packed FLOAT-GENERATION words/synchronized removal/message gestures/
undo/recall/visibility. Capture/admission/exit waits, final broadcasts, host-sized
registry and mutating audio-reentered writers remain blockers. Host/cross-instance
cycles and parameter lock order remain unqualified. Session09 genuine ASan default-
stack overflow257 and session10 allocator-control/leak assertions remain frozen.
New nested257 fixtures use explicit8MiB SDK stacks/legal joins. Successful finite
sanitizer logs do not accept arbitrary stack/lifecycle or whole realtime behavior.
Actual DAW/external OS/architecture/signing/rights/accessibility/humans/budgets/soak/
MB-04 remain open. No Sprint advancement or gate waiver.
