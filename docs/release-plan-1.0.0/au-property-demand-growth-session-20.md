# Sprint10 session20: geometric matching removal demand growth

S10-AU-REGISTRY-15 locally verifies amortized linear matching demand retention growth,
ordered publication, and acknowledged callback ownership. Sprint10 IN PROGRESS;
release NO-GO/development-unqualified. Registry gate BLOCKING; G01, actual DAW/external
AU, MB-04, immutable prepared resources and safe invalid lifecycle remain open.

Initial 161 source + 39 reference inputs and all 21 historical inventories matched.
The session19 exact-size retention algorithm, with quiescent count-only diagnostic,
was reproduced through real native bypass SetParameter consumed in Render -> host
callback -> actual SDK NickName callback -> AudioUnitAddPropertyListener. Two
independent off-callback removers use BOTH real WithUserData/SDK Lookup selectors.
With 1/8/64/257 matching additions, actual demand reallocations were 2/16/128/514 and
shared_ptr copies 2/72/4160/66306. Both removers waited, acknowledged two, observed
all additions in order and no withdrawn replay. This is avoidable quadratic
AUDIO-reentered gate-held retention work, not total-add linearity, allocation-free
audio, deadline, deadlock, race or budget evidence. The complete negative source,
fixture/executable, generated wrappers, pinned SDK, cache/compile records, AU/VST3
bundles and reports were frozen BEFORE the repair in session20/negative-*.

Only product AUPropertyRegistry.hpp changes. EACH matching demand reserves only when
its vector is full, doubling capacity with overflow-safe saturation at max_size.
At max_size, reserve(size+1) still throws before logical mutation. No capacity cap or
registration loss. ALL throwing snapshot/storage/Entry/demand-reserve/retired retention
work still precedes ANY attachment, waitingRemovals increment or publication. A failed
reserve can alter earlier capacity only; no logical ownership change. After successful
preparation EVERY matching independent owner receives the new shared_ptr and count
before publication, under the SAME gate. Nonmatching contexts/selectors remain separate.

Repaired ON loaded counts at 1/8/64/257 are growths 2/8/14/18 and copies 2/30/254/1022.
OFF checks verify delivery/status/lifetime with diagnostics absent. New matching
publication holds a separate callback after the original Render frame returns; BOTH
removers recheck and remain waiting, then acknowledge after the held frame returns.
All complete Render/property/remover threads join; later matching publication is
withdrawn before context destruction. ContextName wrapper-this survives legal updater
shutdown before registry shutdown. Explicit 8MiB worker stacks are fixture conditions,
not arbitrary host-stack/lifecycle qualification.

The initial ALL-frame callback-origin rejection precedes allocation/mutation; errors
never authorize context release. No-ack, absent and withdrawn-only paths unchanged.
CollectionMarks remains collection-only: append before mark, clear ALL unique marks
under gate on success/exception BEFORE publication or demand/wait. Ordered immutable
snapshots, duplicates, outer/nested visibility, writer use_count1 retired reclamation,
reader non-reclamation remain. Capture/admission/exit share gate; constant unlink,
final Entry liveFrames transition, independent demands, ALL-matching predicates and
spurious wake handling remain. Unlock before OS broadcast; owner CV lives through
COMPLETE dispatch return. Per-listener acknowledgement is not whole wrapper/processor
or unlocked-tail acknowledgement. No gate removal/TLS/table/queue/payload/cap/loss or
gesture coalescing.

APVTS virtual authority -> raw atomic mirror -> recursive LockedListeners call -> flag
stores retained. Regular/final parameter callbacks and synchronized editor removal
retain listenerLock. Processor size read outside lock, pointer under lock/invocation
unlocked, final forwarding without processor listenerLock require complete lifetime
exclusion. VST3 separate atomic float storage/grouped flags, immutable IDs and shared
component/controller storage retained; not packed value/flag or event history.
Prepared resources, maps/buffers/DSP scratch/history, callback/framework locks and
caller-serialized off-audio restore retain existing legal lifecycle exclusion.
Arbitrary concurrent/recursive restore, lifecycle/owner migration/host cycles remain
unqualified. Preserve identity, seven keys/IDs, four programs/order, schema40,
empty-history recall, smoothing10ms, shared256 whole-batch rejection/no replay,
eight restores/single audio owner/formats/editor dirty flag and seven packed
FLOAT-GENERATION words/gestures/undo/recall/visibility.

Exact final results, instrumentation and artifact identities are in session20
checkpoint.json, final-identities.json, sanitizer-flags.json, fixture-sanitizer-flags.json,
commands.json and reports. Historical sanitizer/fixture/tool failures remain frozen;
never claim failure-free history. Only two external bootstrap shims exclude
instrumentation; product/generated/callback/direct fixtures remain instrumented.
Finite successful probes and sanitizer checks do not qualify realtime/DAW/budgets.
Sprint08 live comparison remains intentionally invalid; integrity is not acceptance.
