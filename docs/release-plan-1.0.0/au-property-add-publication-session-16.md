# Sprint10 session16: AU listener add snapshot preparation

S10-AU-REGISTRY-11 removes one avoidable vector growth allocation/free pair from
listener addition reentered by a real AU Render callback. Sprint10 remains IN
PROGRESS; release NO-GO/development-unqualified. The registry gate is BLOCKING.
The repaired add still allocates snapshot/storage/Entry, copies host-sized listener
state and can allocate demand retention or reclaim retired storage. This local
change does not establish nonwaiting audio or whole G01 acceptance.

## Frozen loaded reproduction

All157 source and31 reference inputs matched session15 BEFORE product editing.
All17 historical evidence inventories were verified unchanged. Unchanged session15
final ON Release AU fingerprint
8ef64259b675bd072947ddc646a289e60d48ec3f87a9bd691b7300ba7bce9732,
binary3dd60af6a8a8b9b5909f94d3b309b9fd5c0be044da5cf2c109499e90157bd4c9,
was loaded without installation. A real native bypass SetParameter is consumed
in Render on a fresh worker. Its registered host callback calls the actual
AudioUnitAddPropertyListener API to append one NickName registration to existing
1/8/64/257 duplicates. The add performs alloc/free4/1, expected3/0, with both real
WithUserData and loaded SDK Lookup legacy selectors used for subsequent removal.
The next actual SDK NickName dispatch delivers exactly2/9/65/258 callbacks;
removal prevents replay. The negative's printed overall publication FAIL includes
the allocation mismatch; the delivery counts themselves are correct.

The original-call external probe passes alloc/free/mutex/unfair control1/1/1/1.
Actual captured allocation stack reaches registry add -> SDK add ->
AudioUnitAddPropertyListener -> host callback -> registry changed -> bypass
parameter listener -> publishNativeParameter -> Render -> SDK -> AudioUnitRender.
No synthetic PropertyChanged or product negative observer was added. Host setup,
callback invocation and stack unwinding are separately owned. Source identifies
the avoidable copied-vector growth: copy construction allocates storage for exactly
current size, then append grows and frees it. This is actual prohibited plugin
allocation/free on audio, not an observed deadlock/race/deadline miss or timing
budget claim. Snapshot and Entry allocation remain necessary in this protocol.

Complete source export/header/registry fixture/new fixture source+executable/
interposer source/probe source+header+library/generated AU+SDK+VST3/cache+compile/
pinned29-file SDK/bundles/actual reports were frozen BEFORE repair in session16
negative-identities.json and negative-freeze.json. All frozen bytes remain unchanged.
The sandbox registration failure is retained separately; authorized in-process
registration installs nothing. Every worker joined before legal instance disposal.

## One bounded change and every participant

Only product implementation changed: AUPropertyRegistry.hpp add. Under the same
gate, construct an empty unpublished Entries snapshot, reserve current size+1,
then insert the complete current ordered range. Construct the new immutable Entry
and append it without vector growth. No current snapshot or Entry is mutated by
preparation. All throwing construction/reserve/copy/Entry/append work still precedes
EVERY matching demand owner's reserve, retired storage retention, attachment/counts
and publication. Demand reserves may grow capacities before a later failure; they
cannot attach entries, increment counts or publish a partially prepared snapshot.
retired.push_back remains the last throwing operation before demand attachment.
Reserved shared_ptr copies cannot throw. Every pending matching remover retains
and counts the new Entry before current publication under the gate.

Capture, admission, withdrawal, absent return, withdrawn-only wait, ALL-frame
callback-origin rejection, independent WaitDemand scopes, spurious wake predicates,
gate-held exception cleanup and reclaim remain unchanged. Ordered duplicates/no
cap and outer-versus-nested visibility remain: an add is absent from an existing
outer snapshot and visible to subsequent/nested dispatch. Normal readers retain
the immutable snapshot; only writers reclaim retired snapshots at use_count1.
No new reader allocation/reclamation or demand traversal, cap, TLS/table/queue,
retained host payload, dropped callback or gesture coalescing is introduced.
The gate must remain until EVERY writer/reader/context has replacement publication
and acknowledged reclamation.

Every Entry's liveFrames and waitingRemovals still share the retained gate.
Constant incoming-link Frame unlink and the final local notify decision precede
unlock; actual condition broadcast follows unlock. Complete wrapper/processor
dispatch and fixed message-owner exclusion/joins keep owner/CV alive through the
unlocked notification tail. Listener removal alone acknowledges neither complete
changed nor updateHostDisplay/final forwarding. Same-thread active/ancestor contexts
live until return; errors authorize NO release. Context destruction must exclude
new matching add and withdraw any later publication before releasing its context.
Updater stop/clear precedes registry shutdown while ContextName wrapper-this lives;
never dispose within callbacks. ALL SDK virtual routes remain, SDK vector empty/
accessor uncalled, conditional MIDI source-only, GetMutex null. Restore remains
caller-serialized same-role off audio; arbitrary lifecycle, owner migration,
recursive/concurrent restore and host/cross-instance dependency cycles unqualified.

## Verification and remaining ownership

Release and separate ASan-UBSan/TSan ON/OFF native/editor matrices PASS. Loaded
add checks both removal selectors at1/8/64/257. Release observes3alloc/0free,
delivers all count+1 callbacks and prevents replay; sanitizer fixtures verify
statuses/delivery/legal lifetime without malloc-interposition claims. Separate
DIRECT add fault tests cover empty/populated snapshots at0/1/8/64/257: budgets0/1/2
throw without publication; budget3 succeeds with3alloc. With two independent
withdrawn-only waiting removers, budgets0..5 fail through snapshot/storage/Entry/
both demand reserves/retired storage, then a retry succeeds; budget6 succeeds
directly. Both owners acknowledge2 only after original and newly matching frames
return, calls2, unrelated listener1. Inherited DIRECT withdrawn-only budget,
publication, paused notification tail, overlap both orders and exception tests pass.

Inherited loaded active/withdrawn rejection/secondremovers/order257/ContextName64/
first-thread context destruction128, updater128fresh+4x256+64teardown, editor nested/
direct7+64detach/Release3072four scales/sanitizer controls-gestures-undo-recall-close,
strictAU1274/VST3201894points/callbacks/suppression/seals/adapter-recall3of3/core16of16/
pins4377+270/format/3generator drift rejections PASS. Final defaultsOFF exclude fixture
symbols and probe linkage. Generated wrappers remain byte-identical to session15;
VST3 source and Release bundle identities remain unchanged; sanitizer rebuild
identities are recorded separately. Registry Release0/0/3/0;
whole defaultsallocfree0/0 but AUfresh1mutex9/hostile603/VST3fresh1024mutex9 remain.
Larger idle registry counts were not remeasured. Exact identities and successful
logs are authority, finite observations are not release approval.

Pinned review reconfirms APVTS virtual authority -> raw atomic mirror -> recursive
LockedListeners callback -> flag stores. Regular/final parameter listenerLock and
stable editor synchronized removal remain. Processor updateHostDisplay reads size
outside lock, gets pointers under lock, invokes unlocked; final forwarding traverses
without processor listenerLock. VST3 CachedParamValues uses SEPARATE atomic floats
and grouped atomic flags, immutable IDs after construction and component/controller
shared storage. It is latest scalar state, not packed words/event history. Prepared
maps/scratch/DSP prepare-release/callback/FL locks, resource setup/destruction and
safe invalid concurrent lifecycle still require complete ownership replacement.

Preserve identityorg.algorhythmia.sys01/Algr/Sys1,7keys-nativeIDs,4programs-order,
schema1exactly40,parameter-only deterministic empty-history recall,10ms smoothing,
shared256 whole-batch rejection/no replay,eight restores,single audio owner,
stereo effect/generator/disabled-input formats,stable editor atomicdirty and seven
authoritative packed FLOAT-GENERATION words/synchronized removal/gestures-undo-recall.

Successful final reports have no observed assertion/ASan/UBSan/TSan diagnostic.
Session09 real default-stack ASan overflow257, session10 allocator/leak assertions,
session14 external TSan bootstrap/missing rpath and session15 invalid-fixture leaks/
TSan race-SEGV remain frozen. Only inherited2 external mutex bootstrap shims exclude
instrumentation; product/generated AU-SDK-VST3/callback-protocol fixtures are separately
instrumented. Explicit8MiB SDK stacks/legal joins do not accept arbitrary stack or
lifecycle. Editor unpaced offscreen CPU is not DAW/UI budget proof. Actual DAW/
external AU/OS/architecture/signing/rights/accessibility/humans/budgets/soak/MB-04
remain open. No advancement or gate waiver.
