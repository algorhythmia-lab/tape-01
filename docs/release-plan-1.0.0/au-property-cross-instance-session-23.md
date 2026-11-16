# Sprint10 Session23: cross-instance AU callback-origin removal

S10-AU-REGISTRY-18 closes a concrete callback-origin wait boundary across SYS01 AU
instances in one loaded module. Sprint10 remains IN PROGRESS; release remains
NO-GO/development-unqualified. The registry gate, whole G01, prepared resources,
arbitrary concurrent lifecycle, actual DAW/external AU and MB-04 remain open.

The unchanged Session22 shipping OFF AU accepts removal from a property callback on
another instance while its matching target callback is active on a different thread.
The target may depend on the caller returning. The actual loaded fixture drives SDK
NickName publication on two instances, both directions and BOTH removal selectors.
All four negative cases return only after a one-second HOST watchdog releases the held
target, return status0, and withdraw the target registration. This demonstrates an actual
forbidden wait and mutation; it does not claim a permanent deadlock. Contexts remain
alive and all calls join before disposal. Complete negative source/export/header, fixture
source/executable, generated AU/SDK/dispatch/VST3, cache/compile configuration, pinned SDK,
bundles and reports were frozen BEFORE repair in [Session23 evidence](evidence/sprint-10-session-23).

Only product [AUPropertyRegistry.hpp](../../apps/desktop/AUPropertyRegistry.hpp) changes.
The existing gate and intrusive live Frame head now belong to one loaded-module ownership
domain. Initial ALL-frame scan recognizes caller-thread callbacks across EVERY SYS01 AU
instance. Matching other-thread ownership and retention both require frame.owner==this.
Callback-origin removal requiring a target other-thread wait rejects -10863 BEFORE any
allocation, collection mark, withdrawal, publication, demand or reclamation. Errors authorize
NO context destruction; retry off callback after excluding new registrations/joining owners.
No TLS, registration table, cap, queue, payload retention, event loss or coalescing added.

EVERY writer/capture/admission/exit still uses that gate. Entries/publications/retired
snapshots/WaitDemand/CV remain instance-owned. EACH matching add reserves EVERY demand
before ANY attachment/count/publication. Collection marks append-before-mark and clean
under gate on success/exception. RetainedEntry liveFrames waiting predicate and independent
removers, withdrawn/newmatching entries and spurious wake handling remain. Same-thread
ancestors with no matching other-thread frame remain legal and own context until return.
Ordered duplicates/survivors, outer/nested visibility and writer-only use_count1 reclamation
remain. Normal readers never destroy the last snapshot owner.

The incoming link can now cross instances. Under the module gate, constant exit unlink
transfers a successor's incoming pointer before the predecessor stack disappears, regardless
of owning instance. FinalEntry local notification decision, unlock-before-broadcast and
COMPLETE owner/CV tail lifetime remain. Legal destruction excludes and joins COMPLETE own
wrapper/processor/message dispatches including unlocked tails; foreign instances can remain
active. Module unload/static destruction excludes and joins ALL participating entries.
Updater stops/clears before own registry shutdown while ContextName wrapper-this lives.

This increases gate contention and initial frame scans to every live instance in this
module. That work is still blocking and host-sized. Recognition does not cover unrelated
plugins, VST3 callbacks, separately loaded module images or arbitrary host dependency chains.
No gate removal without replacement publication and acknowledged reclamation across EVERY
participating writer/reader/context. No realtime, arbitrary lifecycle or DAW acceptance.

The unchanged pinned SDK virtual Add/Remove/PropertyChanged routes, empty SDK vectors,
uncalled accessor/conditional MIDI source-only/GetMutex null were reviewed. APVTS virtual
authority -> raw atomic mirror -> recursive LockedListeners callbacks -> flags remains;
regular/final parameter listenerLock/editor synchronized removal remains; processor size
outside lock/pointer under lock/invoke unlocked/final forwarding unlocked remains. VST3
cache stores separate atomic floats and grouped atomic flags with immutable IDs/shared
component-controller storage. Prepared maps/buffers/DSP scratch/history and serialized
same-role off-audio restore retain existing lifecycle exclusion. Those product blockers
remain independent next work, not inferred acceptance from this repair.

Preserve IDs org.algorhythmia.sys01/Algr/Sys1, seven keys/native IDs, four programs/order,
schema1 exactly40 bytes, parameter-only empty-history recall,10ms smoothing,shared256
whole-batch rejection/no replay,eight restores,single audio owner,formats and editor dirty/
seven authoritative packed FLOAT-GENERATION words/gestures/undo/recall/visibility.

Exact test counts, logs, sanitizer flags, artifact identities, failures and limits are in
checkpoint.json, contract-review.json and commands.json. Release initial driver omits only
the new direct cross test verified separately; ASan/TSan retained driver includes it.
Separate product/generated/callback/direct fixtures remain instrumented; only two external
bootstrap mutex shims exclude instrumentation. Explicit8MiB SDK stacks and complete joins
are fixture conditions, not arbitrary host stack/lifecycle proof. Historical failures remain.
