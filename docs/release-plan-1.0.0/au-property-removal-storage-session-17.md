# Sprint10 session17: AU removal snapshot survivor storage

S10-AU-REGISTRY-12 is locally verified at removal snapshot sizing, ordered publication,
and retained acknowledged callback ownership. Sprint10 remains IN PROGRESS; release
NO-GO/development-unqualified. The registry gate remains BLOCKING. Removal still
allocates, scans host-sized state, can reclaim, and can wait off callback. This
change does not establish nonwaiting audio or whole G01 acceptance.

## Reproduction frozen before repair

All158 source and33 reference inputs matched session16 before product editing.
All18 historical inventories passed exact-byte checks, including session16's650files.
Unchanged session16 ON Release AU fingerprint
ed98a992e7ab5482ad02c2add9a3e786823a0c768b7a1c834f9563bb8fb2fc27,
binary e6590d9747557217991b1f94ca54abd39c0aafb769361fc28d9ecf57c28be458,
was loaded without installation. Actual native bypass SetParameter consumed in
Render calls a registered host callback, which removes1/8/64/257 NickName duplicates
using each real WithUserData and loaded SDK Lookup legacy selector. Removal allocates
snapshot48bytes plus storage48/160/1056/4144bytes, with allocation/free2/0. Only two
entries survive: constructor ContextName and the bypass writer, requiring32bytes
of shared_ptr storage. The excess is16/128/1024/4112 requested bytes. Delivery is
correct: subsequent real SDK NickName dispatch calls0 and repeated removal prevents
replay. The negative's combined printed publication FAIL includes the size mismatch.

Dedicated external SnapshotStorageProbe records actual malloc/calloc/realloc request
sizes and passes original-call alloc/free/mutex/unfair control1/1/1/1. First allocation
stack reaches registry.remove -> SDK removal -> host callback -> registry.changed ->
bypass listener -> publishNativeParameter -> Render -> SDK -> AudioUnitRender.
Unwinder work, host setup, callbacks and formatting are separately owned. No synthetic
PropertyChanged or product negative observer was added. This proves avoidable
plugin-controlled allocation size on audio, not allocator RSS, deadline/budget,
deadlock, race or arbitrary host qualification. Snapshot/storage allocations remain.

Complete source export/header/registry fixture/new fixture+executable/size probe
source+header+library/generatedAU+SDK+VST3/cache+compile/pinned29fileSDK/bundle/reports
were frozen BEFORE repair in session17 negative-identities.json/negative-freeze.json.
All frozen bytes remain unchanged. Sandbox registration failed before tests;
authorized in-process reproduction installed nothing and joined all workers before
legal uninitialize/disposal. Historical packets are never resealed.

## Bounded change and every participant

Only product implementation AUPropertyRegistry.hpp removal changed. After the
unchanged ALL-frame caller-thread/other-thread scan and callback-origin rejection,
count all matching published entries under the SAME gate. publishedMatch is the
nonzero count. Absent return and withdrawn-only paths are unchanged. For a published
match, construct an empty unpublished snapshot and reserve current.size minus that
count. Copy every survivor in original order without growth. Counting/copying see
identical immutable current because EVERY add/remove/capture/admission/exit shares
the gate. Empty survivors require no vector storage allocation. ALL throwing awaited
retention/snapshot/storage/copy/retired.push_back work still precedes withdrawal and
publication; a preparation failure makes no logical change and grants no context
release permission. Reserved shared_ptr copies cannot throw.

Add's full reserve/ordered copy/Entry construction/EACH pending independent demand
reserve/retired.push_back before attachment/count/publication are unchanged. Every
new matching Entry inherits BOTH pending remover owners. WaitDemand's unique retained
other-thread Entries, ALL-frame predicates including withdrawn/new matching frames,
spurious wake handling and gate-held exceptional RAII cleanup remain unchanged.
Frame admission/active checks/incoming-link constant unlink/liveFrames/local final
notification decision/unlock BEFORE broadcast and owner CV lifetime remain unchanged.
Normal readers never reclaim last-owner storage; only writers reclaim retired
snapshots at use_count1. Ordered duplicates/no cap/outer-nested visibility remain.

Complete wrapper/processor entries and fixed message owner must be excluded and
joined before shutdown/CV destruction, including unlocked notification tails.
Listener removal acknowledges neither full changed nor updateHostDisplay/final
forwarding. Same-thread active/ancestor contexts live until return; errors grant no
release; context destruction excludes new matching add and withdraws any later
publication. Never dispose inside callbacks. Updater stop/clear precedes registry
shutdown while ContextName wrapper-this lives; GetMutex remains null. ALL SDK virtual
Add/Remove/PropertyChanged routes remain, SDK vector empty/accessor uncalled, MIDI
runtime unselected/source-only. Caller-serialized same-role off-audio restore,
recursive/concurrent restore, owner migration, arbitrary lifecycle and host/cross-
instance dependencies remain unqualified. Gate removal requires EVERY writer-reader-
context replacement publication and acknowledged reclamation.

## Verification

Release and separate ASan-UBSan/TSan ON/OFF native/editor matrices PASS. Loaded
removal storage checks both selectors at1/8/64/257: Release snapshot48/storage32,
allocation/free2/0 and callbacks0/prevented replay with ON and shipping default OFF.
Sanitizers verify statuses/delivery/legal lifetime, not malloc interposition.
DIRECT operator-new fault oracle has120cases per tree: bothselectors, removed
1/8/64/257, survivors0/1/8/64/257, budgets0/1/2. Empty survivors require1snapshot
allocation; nonempty require2, with exact surviving-slot storage. Every failed
preparation preserves all registrations; successful/retried withdrawal preserves
all survivor order. Separate instrumentation covers product/generatedAU-SDK-VST3
and direct/callback fixtures; only2 inherited external mutex bootstrap shims exclude
sanitizer instrumentation.

Inherited loaded active/withdrawn rejection/independent second removers/new matching
publication/nested-overlap both exit orders/order257/ContextName64/first-thread context
destruction128/updater128fresh+4x256+64teardown/editor nested-direct7-64detach/Release
3072four scales/sanitizer gestures-undo-recall-close/strictAU1274/VST3201894points/
callbacks-suppression-seals/adapter-recall3of3 PASS. DIRECT addfault34cases/absent/
withdrawn-only/paused unlocked tail/newmatching/publication/exception cases PASS.
Core16of16/pins4377+270/format/3generator drift rejections PASS. Final shipping OFF
hooks/probe absent; generated wrappers remain byte-identical to session16. Release
VST3 bundles unchanged; sanitizer rebuild identities separately recorded.
Registry alloc/free/mutex/unfair0/0/3/0; whole default allocfree0/0 but AUfresh1mutex9,
hostile603,VST3fresh1024mutex9 remain. Larger idle counts not remeasured.

Pinned trace reconfirms APVTS virtual authority -> raw atomic mirror -> LockedListeners
callback -> flag stores; regular/final parameter listenerLock/editor synchronized
removal. Processor size outside lock/pointer under lock/invoke unlocked/final forwarding
unlocked. VST3 separate atomic floats/grouped flags/immutable IDs/shared component-
controller storage, not packed words/event history. Prepared maps/scratch/DSP/callback-
FL locks/resources/invalid concurrent lifecycle remain and need complete protocols.

Preserve org.algorhythmia.sys01/Algr/Sys1,7keys-nativeIDs,4programs-order,schema1exactly40,
parameter-only deterministic empty-history recall,10ms smoothing,shared256wholebatch
rejection/no replay,eight restores,single audio owner,stereo effect-generator-disabled
input formats,stable editor atomicdirty+7authoritative packed FLOAT-GENERATION words,
synchronized removal/message gestures-undo-recall-visibility. No cap/loss/TLS/table/
queue/retainedhostpayload workaround or gesture coalescing.

Successful final reports show no observed assertion/ASan/UBSan/TSan diagnostic.
Genuine historical session09defaultstackASanoverflow257/session10allocator-leaks/
session14externalTSanbootstrap+missingrpath/session15invalidfixtureleaks+TSanrace-SEGV
and checkpoint KeyError remain frozen. Explicit8MiB SDK stacks/legal joins do not
accept arbitrary host stack/lifecycle. Editor unpaced offscreen CPU is not DAW/UI
budget proof. Actual DAW/external AU/OS/architecture/signing/rights/accessibility/
humans/budgets/soak/MB-04 remain open. No advancement or waiver.
