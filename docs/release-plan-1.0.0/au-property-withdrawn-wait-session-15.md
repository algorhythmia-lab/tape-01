# Sprint10 session15: withdrawn-only removal acknowledgement

S10-AU-REGISTRY-10 skips equivalent snapshot publication and reclamation when a
remover finds matching withdrawn frames but no matching published registration.
Sprint10 remains IN PROGRESS; release NO-GO/development-unqualified. The registry
gate remains BLOCKING. This is an off-callback writer/wait critical-section repair;
it does not establish allocation-free removal, nonwaiting audio, or whole G01.

## Frozen actual loaded reproduction

All156 source and29 reference inputs matched session14 before product editing.
All16 historical evidence inventories were verified unchanged. Unchanged session14
final ON Release AU fingerprint80df706b75e8177939fadb55a95b2bf4ff6d52280d689bb2af504dc557aab980,
binaryd922f48229654697eda34605e019024bd92f8ef58718271c9b7b26d73e7f6ecc,
was loaded without installation. Real SDK NickName SetProperty recursively admits
one registration at depths1/8/64/257 on an explicit8MiB SDK worker stack. An
independent first remover withdraws that registration and atomically enters the
actual condition wait. A second remover then sees matching live withdrawn frames
but no matching published registration. Both real WithUserData and loaded SDK
Lookup legacy selectors wait until callback return and acknowledge2.

The second remover nonetheless allocates and frees3/3 for every case except the
first legacy case4/4. The existing original-call malloc/calloc/realloc/free/mutex/
unfair probe passes control1/1/1/1 and captures AUPropertyRegistry::remove -> real
SDK removal -> fixture remover stack. Source accounts for one necessary awaited
Entry retention allocation, an equivalent snapshot and vector allocation, and
possible retired capacity growth. No synthetic PropertyChanged or product observer
was added. Host barriers and probe unwinding are diagnostic-owned. This is observed
avoidable allocation/publication/reclamation in an off-callback wait path, not an
audio allocation claim or reproduced deadlock/race/deadline miss. Necessary demand
retention and condition wait remain. The first sandbox run failed registration
before tests; the authorized run produced the expected loaded negative and legally
joined all workers before teardown.

Complete source export/header/registry fixture/new source+executable/condition
interposer source+library/allocation probe source+header+library/generated AU+SDK+
VST3/cache+compile commands/pinned29-file SDK/bundles/actual reports and logs were
frozen BEFORE repair in negative-identities.json and negative-freeze.json. Original
negative fixture/binaries and historical bytes are never modified.

## One bounded change and every participant

Only existing product implementation changed: AUPropertyRegistry.hpp remove.
Under the retained gate, remove first performs the unchanged ALL-frame callback-
origin/other-thread acknowledgement scan and rejects callback-origin required
waits before allocation or mutation. It records whether current has a published
match. No other-thread frame and no published match retains the immediate absent
no-op return. If other-thread frames remain, the unique retained awaited Entry
list is still allocated before any logical change, including withdrawn entries.
Only when a published match exists does remove construct/retain a new snapshot,
withdraw entries, publish and reclaim. A withdrawn-only remover skips all that
structural work and continues through the SAME independent WaitDemand and SAME
ALL matching-other-thread-frame predicate. Retention is still necessary and can
throw; failure acknowledges nothing and changes no logical state.

The gate linearizes the published-match decision with EVERY add, capture,
admission, withdrawal and exit. An add cannot interleave until this remover has
published demand and condition wait atomically releases the gate. EVERY matching
add still reserves retention in EVERY pending demand before ALL throwing work
completes, then attaches/counts the new Entry under gate before publication. New
matching frames therefore extend both first and second removers' predicates and
have final notification demand. Each independent remover retains unique Entry
owners, counts and intrusive links through spurious wakeups and acknowledgement;
RAII unlink/decrement occurs with gate held, including exceptional wait exit.
Skipping unrelated reclaim leaves retired storage owned for a later mutating
writer or legal shutdown. It adds no reader ownership or reclamation operation.

Published matches still mutate, including not-yet-admitted registrations. Active,
withdrawn, newly matching and independent second removers retain acknowledgement.
Same-thread active/ancestor contexts live until their frames return; errors permit
NO release. Host context destruction must exclude new matching adds, and a newly
published registration must itself be withdrawn before releasing its context.
No successful listener removal acknowledges complete wrapper dispatch or disposal.

Capture/admission/frame lifetime/constant incoming-link unlink/liveFrames/final
local notification decision/unlock-before-broadcast/ownerCV complete-dispatch
lifetime remain unchanged. Ordered immutable snapshots preserve all duplicates,
no cap and outer/nested visibility. Current/retired use_count1 writer-only reclaim
keeps normal readers from last-owner deletion. No gate removal, listener loss,
TLS/table/queue/retained host payload/gesture coalescing or allocation-free waiting
claim. The gate remains until EVERY writer/reader/context has replacement
publication and acknowledged reclamation.

ALL SDK virtual Add/Remove/PropertyChanged paths route here; SDK vector empty,
GetPropertyListeners accessor uncalled, conditional MIDI source-only. Legal teardown
excludes and joins ALL COMPLETE wrapper/processor entries and fixed message-owner
work, including unlocked notification tails, before shutdown/ownerCV destruction.
Updater stop/clear precedes registry shutdown while ContextName wrapper-this lives.
Processor removeListener alone acknowledges neither full updateHostDisplay nor
final forwarding. GetMutex remains null. Restore assumes caller-serialized same-
role off-audio ownership; recursive/concurrent restore, owner migration, arbitrary
host/cross-instance dependency cycles and concurrent lifecycle remain unqualified.

## Verification and retained listener/resource boundaries

Actual command/report outcomes and identities are in session15 checkpoint and
handoff. Loaded ON and shipping default-OFF checks use both real selectors at
1/8/64/257. Release measures retained-demand alloc/free1/1, ack2, no callback replay;
separate ASan-UBSan/TSan verify statuses/order/lifetime without malloc interposition
claims. A separate DIRECT fault oracle permits one allocation: withdrawn-only
second remover succeeds with1/1 while no-retention-budget failure acknowledges
nothing. Both demand owners wait for a newly matching publication after original
frame return, then acknowledge2; unrelated listeners survive. These direct tests
support the source proof and are distinct from the loaded negative.

Inherited active/withdrawn rejection, independent second removers, overlapping
frames/both exit orders, paused broadcast/newmatching admission, ordered257,
outer/nested visibility, ContextName64, first-thread/context destruction128,
updater128fresh/4x256/64legalteardown, native/editor/recall/core and separate sanitizer
checks retain their named boundaries. Shipping defaults exclude product fixture
symbols/probe dependencies. Exact successful logs qualify finite local behavior.
Historical session09 actual default-stack ASan overflow257, session10 allocator/
leak assertions and session14 external TSan bootstrap/missing-rpath failures remain
frozen. Session15 condition shims use session14's bootstrap-safe external lock/
unlock entry exclusions only; product/generated AU+SDK/VST3 and callbacks/protocol
fixtures remain separately instrumented. Explicit8MiB SDK stacks/legal joins do
not accept arbitrary host stack or lifecycle. Editor offscreen unpaced CPU checks
do not qualify a DAW or UI budget.

Pinned source review confirms APVTS ParameterAdapter virtual authority -> raw
atomic mirror -> recursive LockedListeners callback -> flag stores. Regular/final
parameter listeners retain listenerLock and synchronized stable-editor removal.
Processor updateHostDisplay reads size outside lock, gets each pointer under lock,
invokes unlocked; final forwarding traverses without processor listenerLock.
VST3 CachedParamValues has SEPARATE atomic floats/grouped change flags, immutable
IDs after shared construction and component/controller retained storage. It is
latest scalar state, not packed float/flag words or event history. Prepared maps,
scratch, DSP prepare/release, callback/FL locks and resource setup/destruction still
require complete ownership replacement and safe invalid-lifecycle silence/error.
None was changed or accepted here.

Preserveorg.algorhythmia.sys01/Algr/Sys1,7keys-nativeIDs,4programs-order,schema1exactly40,
parameter-only deterministic empty-history recall,10ms smoothing,shared256 whole-
batch rejection/no replay,eight restores,single audio owner,stereo effect/generator/
disabled-input formats,stable-editor atomicdirty/seven authoritative packed FLOAT-
GENERATION words/synchronized removal/message gestures/undo/recall/visibility.
Capture/admission/exit waits, final broadcasts, host-sized dispatch/remove/reclaim/
add-demand and allocating mutating audio-reentered writers remain open. WholeG01,
actualDAW/externalAU, external OS/architecture/signing/rights/accessibility/humans/
budgets/soak and MB-04 remain open. Remain Sprint10; no advancement or waiver.

Session15 actual tooling failure: the initial default ordering compiler omitted
SYS01_DEFAULT_ORDERING for all3trees. It requested the shipping-absent idle hook
after inherited checks. Original binaries/logs/reports remain. Early std::exit
left instances alive: ASan/TSan emitted actual JUCE leak assertions; TSan also
reported appDelegate destruction versus live TimerThread posting and a timer
post mutex SEGV during invalid fixture teardown. Corrected separate executables
use the existing flag, skip only diagnostic idle instrumentation, complete legal
joins/disposal, and pass on SAME AU bytes. No product change or sanitizer exclusion
was introduced for this correction. Never claim no failure/assertion/race ever.
