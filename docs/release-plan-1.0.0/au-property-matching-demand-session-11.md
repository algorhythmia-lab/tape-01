# Sprint 10 session 11: matching AU removal notification demand

S10-AU-REGISTRY-06 suppresses callback exit broadcasts for entries unrelated to
pending removal predicates. Sprint10 remains IN PROGRESS; release NO-GO. The
registry gate remains BLOCKING. Matching pending exits still broadcast, and
capture/admission/exit locks and host-sized writer work remain open.

The unchanged session10 final diagnostic AU (fingerprint
56422d1f8b5277374ba3b1c38d2fd2b72f5a95c579c0565d3221767ee58d9f7a,
binary b13fbb54fc40d9be1cbab781f005dadda9b1a4290be228c3045d453b628445f5)
reproduces one broadcast on real native bypass Render while an off-callback
remover waits for a held SDK NickName frame using the same callback function.
Both real removal selectors report broadcasts1/waited1/calls1, expected0/1/1.
An external original-call interposer observes actual pthread_cond_wait and
pthread_cond_broadcast; the captured stack reaches registry changed, bypass
parameter listener, Render and SDK. No synthetic PropertyChanged or product
observer was added. This is unnecessary plugin-triggered OS work, not an
observed deadlock, race or deadline miss. Fixture barriers and unwinding belong
to the host diagnostic. Complete negative source/generated/cache/SDK/bundle/
fixture/probe/report were frozen before product repair in session11 evidence.
Loader-rpath and sandbox registration setup failures are retained separately.

Only product implementation changed is AUPropertyRegistry.hpp. Each Entry has
a gate-owned removal-demand count. Before withdrawal, remove allocates a unique
retained list of matching other-thread entries from ALL live frames, including
withdrawn entries. Stack WaitDemand increments counts, joins an intrusive writer
list, and retains entry owners until predicate acknowledgement. Its destructor
unlinks constant-work and decrements counts while the gate is held, including
exceptional wait exit. Frame references its immutable enclosing snapshot's
shared_ptr slot, so remove can retain storage without adding reader ownership
operations or allowing a reader last-owner delete.

Every add also participates: before publication it reserves retention for each
matching pending demand; after all throwing work it attaches the new Entry and
increments its counts under the gate. A newly matching frame can therefore wake
a remover whose existing predicate includes it. Failed reserve changes capacity
only, with no admission, withdrawal, publication or count mutation. No reader
traverses the demand list. Same-thread immediate removals never release the gate
with an otherwise absent predicate. New matching registrations must still be
excluded by the host context destruction protocol; this repair adds no teardown
permission. Callback-origin other-thread wait rejection remains before allocation.

Exit unlinks with one mandatory pointer update and at most one successor backlink
under the retained gate, then broadcasts only if its own Entry demand is nonzero.
Exit-first leaves no matching frame for a later remover; remover-first publishes
demand before wait releases gate. Independent removers retain separate counts,
and withdrawn entries remain owned until all predicates acknowledge. Global
condition broadcasts may still wake unrelated predicates when a matching exit
requires them. No callback loss, cap, TLS, deferred payload or gesture coalescing.

All SDK virtual writers/readers, native bypass, fixed updater and ContextName
wrapper-this still route to the same registry; SDK vector remains empty and
const accessor uncalled. Conditional MIDI remains source-only. Legal teardown
excludes/joins ALL wrapper/processor entries, fixed message owner, and forbids
disposal in callbacks. Updater stop/clear precedes registry shutdown. Processor
removeListener alone acknowledges neither full updateHostDisplay nor final
forwarding. Normal readers never reclaim final snapshot/entry storage; writer
reclamation still uses retired snapshot use_count1.

APVTS virtual authority -> raw atomic mirror -> LockedListeners recursive lock ->
flag stores, regular/final parameter listenerLock, processor unlocked callback
lifetime, separate VST3 atomic floats/grouped flags and immutable IDs/shared
component-controller storage remain unchanged. Prepared maps/scratch/DSP/callback/
FL locks and invalid concurrent lifecycle require complete replacement ownership.
No registry gate removal until EVERY writer, reader and context has replacement
publication and acknowledged reclamation. Preserve seven keys/IDs, four programs,
schema1 exactly40, deterministic parameter-only empty-history recall,10ms smoothing,
shared256 whole-batch rejection/no replay,eight restores,single audio owner,
stereo/generator/disabledinput/formats and stable editor relay/removal/gestures.

Final verification is authoritative only at the exact checkpoint bytes and
successful logs. Separate sanitizer fixtures omit allocator-interposition claims;
Release allocation/mutex proof is separate. Retained session09 ASan default-stack
overflow and session10 allocator-control/leak assertions remain historical; all
nested257 fixtures retain explicit8MiB SDK stacks and legal joins. Affected AU
callback/concurrency/host/lifecycle/performance/soak/signing evidence is invalidated.
Actual DAW/external platform/signing/rights/accessibility/humans/budgets/soak/MB-04
remain open; finite checks do not close G01. No Sprint11 advancement or waiver.
