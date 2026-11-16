# SYS 01 Sprint 10 session 10 handoff

## Checkpoint

2026-10-20 16:17 America/Santiago (19:17 UTC). Sprint/session10/10. Objective S10-AU-REGISTRY-05:
avoid unnecessary OS condition broadcasts on idle AU callback exit while retaining
acknowledged matching-frame removal. LOCALLY VERIFIED at idle notification demand /
waiting remover acknowledgement boundary. Sprint10 IN PROGRESS, release NO-GO/
development-unqualified. Registry gate BLOCKING, whole G01/actual DAW-origin/
external AU/MB-04 open. No Sprint11 advancement or waiver.

Gitroot /Users/lhommeduchili/algorhythmia, SYS01root
/Users/lhommeduchili/algorhythmia/sys-01; HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5
unchanged. No applicable AGENTS. All151source+19reference inputs matched session09
BEFORE edits. Dirty/untracked SYS01/unrelated monorepo work preserved. No stage/
reset/stash/discard/commit/tag/publication/subagents/contact/install/personal plugin
changes. All jobs complete, none to resume. Source inputs152, export SHA256
122a55584e3b7f6a0f0bdfaf650c54dd6c2c08694d991d5fbe992251b6d49b4f; resume-inputs
9213e7850a25b757a618b1d71f3c7705248b6f5b953ee338b3b445e1ebb75f85. Exact commands/UTC/exits/tool/log/artifact hashes in
commands.json/reports/checkpoint/final-identities.

## Changes and decisions

Only product implementation changed: apps/desktop/AUPropertyRegistry.hpp.
New ownership record ../au-property-notification-demand-session-10.md. Condition
fixture/interposer live in this session evidence; shipping tests/generator/SDK/pins/
shared processor/DSP/editor/VST3 implementation unchanged.

Actual negative unchanged session09 diagnostic Release AU fingerprint
c74530ef1f986775c3dbdb73300cad9af1ca7357f3116c58426c65325aa7f3b7,
binary64f2fd1e6843e00cee785e445e6986d2c7ee4457fe3423de711c55315a565c59.
Native bypass SetParameter consumed in real Render calls1/8/64/257 duplicates.
With no remover pending, exits call pthread_cond_broadcast1/8/64/257 instead0.
Actual stack libc++ notify_all -> registry changed -> bypass -> Render/SDK.
This is observed unnecessary plugin-triggered OS notification work, NOT a reproduced
deadlock/race/deadline miss. No product observer or synthetic PropertyChanged for
negative. External interposer counts original calls, stack unwinding separately
excluded from allocation/mutex probe. Host callbacks only increment atomic counters.
Exact negative source export/header/fixture source+executable/interposer source+
library/generatedAU+SDK+VST3/generator/cache/compile/bundles/pinned29fileSDK/report
frozen BEFORE product repair. Never modify frozen bytes. Historical shipping/default
and diagnostic bundles remain separately retained in session09.

A gate-owned size_t waitingRemovals counts stack WaitDemand scopes after throwing
allocations, withdrawal/publication/reclaim, before condition wait releases gate.
Demand spans wakeups; RAII decrements while gate is owned, including exceptional
wait exit. Frame exit keeps constant-work unlink under same gate; broadcast only
when demand nonzero. If exit wins first, remover predicate sees no frame; if
removal wins first, exit sees published demand and wakes it. Each simultaneous
remover owns demand until its own predicate acknowledgement. No wakeup gap.
Immediate no-match/same-thread scopes cannot expose demand to another gate-held
exit. Callback-origin rejection still precedes allocation/withdrawal/demand.
Both selectors/second removers/withdrawn frames retain complete acknowledgement;
errors authorize NO context destruction. Same-thread active/ancestor contexts live
until frames return. Host off-callback retry joins owners/excludes matching new add.

Ordered immutable snapshots/duplicates/no cap/nested visibility/active checks/
writer-only retired use_count1 reclamation/normal reader never last-owner delete
unchanged. EVERY SDK virtual writer/reader still routes registry, SDKvector empty,
constaccessor uncalled, conditionalMIDI source-only. ContextName wrapper-this retained
through legal shutdown. Updater stop/clear first; teardown excludes/joins ALL complete
wrapper/processor entries, fixed message owner, never disposal inside callbacks.
Processor removeListener alone acknowledges neither updateHostDisplay nor final
forwarding. Restore same-role serialized offaudio/recursive stateMutex limits stay.
GetMutex null; message owner migration/arbitrary concurrent lifecycle unqualified.

Pinned ParameterAdapter sequence is virtual authority -> atomic raw mirror -> APVTS
listener call under recursive CriticalSection -> flag stores; add/remove same lock.
Parameter reverse regular+final listeners retain parameter listenerLock/stable
editor synchronized removal. Processor updateHostDisplay reads size outside lock,
gets pointer under lock then invokes unlocked; final forwarder traversal has no
processor listenerLock. VST3 cache separate atomicfloat/flagwords; relaxed float
exchange then acq_rel flag publish, acq_rel flag drain then relaxed float load;
immutableIDs and component/controller shared storage remain legally constructed/
destroyed with entry exclusion. No event history/packed floatflag claim. Prepared
maps/scratch/DSP/callback/FL locks remain open; no listener/resource replacement.

Preserve org.algorhythmia.sys01/Algr/Sys1,seven keys/nativeIDs,four programs/order,
schema1exactly40,parameter-only deterministic empty-history recall,10ms smoothing,
shared256whole-batch rejection/no replay,eight restores,singleaudioowner,stereo/
generator/disabledinput/formats. Stable editor atomicdirty/seven authoritative
packed float-generation words/synchronized removal/message gestures/undo/recall/
visibility unchanged. No cap, notification loss or gesture coalescing.

## Verification

|Check|Command registry reference|Result|Boundary|
|---|---|---|---|
|Input comparison|initial-state.json|151+19 match|Before edits; HEAD/dirty work retained|
|Loaded negative|s10s10-negative-authorized|exit1,broadcasts1/8/64/257|Frozen unchanged session09 ON AU|
|Focused repair|s10s10-focused|idle0/pending1/8/64/257,ack2 PASS|Initial repair exact bundle|
|Final condition/exit|s10s10-condition-repair/asan-only/tsan-only|control1,idle0,pending counts,unlink0 PASS|Final formatted source/separate sanitizers|
|Final native ON/OFF|s10s10-*-final-{on,default}-native|strictAU1274/callback/registry/ownership/suppression/VST3/seals/adapter/recall3of3 PASS|Release/separateASan-UBSan/TSan|
|Editor/updater|s10s10-*-final-{on,default}-editor/notifications,*-final-on-updater|nested/direct7/64detach,updater128fresh/4x256/64teardown PASS|Release3072/four scales;sanitizercontrols/gestures/undo/recall/close|
|Core/pins/format/generation|s10s10-core/juce-pin/doctest-pin/format-final/generation|16of16,4377/270 pins,format,3drift rejections PASS|Unchanged shared sources/pins|
|Whole default probes|s10s10-default-au-probe/default-vst3-probe|alloc/free0/0,mutex remain|G01 open|
|Final integrity|s10s10-checkpoint,finalization.json|PASS|OFFhooks/probeabsent,separateflags,source/artifact/frozeninventories|

Final whole probes freshAU1mutex9/hostile603,VST3fresh1024mutex9,alloc/free0/0. Complete single-listener registry0/0/3/0; larger1/8/64/257 callback gate counts
3/17/129/515 remain. Pending condition broadcasts remain prohibited. Capture/
admission/exit block; dispatch/remove/reclaim host-sized; audio-reentered writers
allocate/free. Cross-instance cycles, callback waiting on its own off-callback
remover, arbitrary parameter reentry/lock orders and invalid lifecycle open.
Finite warm zeros/sanitizers do not establish whole realtime acceptance.

Initial sandbox registration failed before tests; authorized temporary in-process
negative reproduced actual broadcasts, installs nothing. Formatting check found
real new scope/comment violations; formatting corrected then final matrix rebuilt
and renewed exact bytes. Session09's genuine ASan default-worker stack overflow
at257 remains frozen; all new fixtures retain explicit8MiB SDK stack and join.
Initial ASan condition fixture used incompatible Release malloc interposition; control0alloc/0free/1mutex/3unfair failed, early std::exit leaked loaded instances and emitted retained JUCE assertions. Separate condition-only sanitized fixture passes on SAME bundle, with real broadcast control and legal teardown. Release allocation/mutex proof remains separate. No observed final successful assertion/ASan/UBSan/TSan diagnostic. Never claim no
sanitizer failure ever. Release editor measurements are unpaced offscreen CPU,
not DAW/driver/UI budget qualification. Sanitizer controls omit image budgets.
Eleven historical inventories291/505/357/364/409/366/397/422/492/470/546 unchanged.
Sprint08 live-source comparison intentionally invalid; never reseal.

CMake/CTest4.4.4 at recorded uv archive path, clang-format18.1.8 sibling,
AppleClang21/macOS26.6.2/25G83arm64. JUCE8.0.12 revision29396c22c93392d6738e021b83196283d6e4d850,
doctest2.4.12 revision1da23a3e8119ec5cce4f9388e91b065e20bf06f5; pinslock unchanged.
Probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.
Exact tool/dependency/sanitizer flags and fixture hashes in evidence.
Affected AU callback/lifecycle/host/validator/performance/soak/signing evidence
invalidated; renewed only at named local byte boundary. Only installedAbletonLive12
Suite permitted; REAPER skipped/pluginval absent/temporaryAU auval binding requires
forbidden installation. ActualDAW/externalOS/architecture/signing/rights/accessibility/
humans/budgets/soak/MB-04 remain unqualified. No gate waiver/advancement.

Final diagnosticsOFF fingerprints; binary SHA256 in final-identities.json:

|Tree|AU|VST3|
|---|---|---|
|repair|b176242561f746c3551ab3f6b24d8a3064b0ef8187611fe81d192bd4f61056e0|41719ca2b332a4a1fe13a54a533f3b5b8af954cbbc5cfd88ca54132f291cc0c1|
|asan|9acf36c293bb56a0fdc2936fc04d2d28d99113951365c2302a83ca47c0de604e|6acceb96765d77406ee6721576e73f5ccb2766771ff04344377483cf5e600c55|
|tsan|fa2025310ec41b5e7ecd17f18ce9ff211fcf573957b06ba8e487cbc6399cf78b|ccbd6c33372490d6cdbcb72c542fb138f2b1e2bace8053fe06e13c80a6a9e1d9|

## Resume

All jobs completed, none to resume. Native temporary trees /private/tmp/sys01-s09-
repair,asan,tsan finalOFF; core/private/tmp/sys01-s09-session02-core. Reconstruct
missingtemps from exact pins/cache/source export; frozen rpaths can use absolute
DYLD_LIBRARY_PATH, never alter frozen bytes.

Session11 next: compare LIVE with session10 inventories FIRST; read complete gate/
capture/admission/exit/conditional-notify/WaitDemand/remove/reclaim and generated
routing. Freeze one remaining actual loaded wait/order or callback ownership defect
BEFORE one bounded repair. Retain gate until EVERY writer/reader/context follows
replacement publication and acknowledged reclamation. Include bothselectors,active/
withdrawn,nested/overlap,firstthread,ContextName/legal teardown. Continue APVTS/
regular/final listeners,CachedParamValues,immutable resources/lifecycle review.
Sprint10 exit needs nonwaiting audio/offaudio configuration waits/everywriter
acknowledgement/safe invalid silence-error/complete zero-prohibited shipping callback
and concurrency/lifecycle checks. Remain Sprint10 without waiver.

## Complete next session prompt

```text
Work in /Users/lhommeduchili/algorhythmia/sys-01.

Continue SYS 01 v1.0.0 Sprint 10, session 11: remaining AU registry admission/exit wait/order and acknowledged callback ownership. This is an implementation session. Reproduce and freeze one concrete remaining plugin-controlled loaded-AU wait/order or callback ownership defect, implement one coherent bounded repair, and verify it. Remain Sprint10; no Sprint11 advancement or gate waiver.

Read docs/release-plan-1.0.0/rolling-state.md and handoffs/sprint-10-session-10.md FIRST, then au-property-notification-demand-session-10.md, au-property-exit-order-session-09.md, au-property-removal-context-session-08.md, au-property-callback-ownership-session-07.md, au-updater-ownership-session-06.md, au-property-registry-session-05.md, handoffs/sprint-10-session-08.md, au-host-notifications-session-04.md (SDK guard claim superseded), native-vst3-suppression-session-03.md, native-suppression-session-02.md, native-notifications-session-01.md, au-ingress-ownership-session-03.md, docs/release-1.0.0/native-ownership.md, and Sprint10/acceptance/evidence-invalidation/handoff sections of audit-production-plan-2026-10-18.md. Read session10 checkpoint.json, final-identities.json, ownership-trace.json, ownership-trace-provenance.json, initial-state.json, negative-identities.json, negative-freeze.json, fixture-identities.json, failures.json, invalidation.md, finalization.json and this COMPLETE prompt, plus session09 checkpoint/failures/finalization for the retained stack-overflow boundary. Consult exact logs for counts/limits.

Check Git status/applicable AGENTS and compare LIVE with session10 resume-inputs.json and resume-reference-inputs.json BEFORE edits. Session10 began with all151source+19reference inputs matching session09. Session10 final source inventory contains152 inputs and final reference inventory contains21; consult checkpoint for exact hashes. Gitroot /Users/lhommeduchili/algorhythmia, HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5 unchanged. No applicable AGENTS found. Preserve dirty/untracked SYS01 and unrelated monorepo work, all frozen evidence. No staging/reset/stash/discard/overwrite/commit/tag/publication/subagents/contact/install/personal plugin directory changes. No dependent background job remains to resume; consult finalization authority.

Sprint09/S09-OWN-01 complete at named local boundary. S10-NOTIFY-01/AU-SUPPRESS/VST3-SUPPRESS/AU-UPDATER-01 remain locally verified at bounded contracts. AU-NOTIFY only caller-serialized off-audio restore identity/per-call stack events. S10-AU-REGISTRY-02 releases the gate around host callbacks; REGISTRY-03 rejects callback-origin required other-thread matching-frame wait before any allocation/withdrawal/publication. Session09 S10-AU-REGISTRY-04 locally verifies constant-work intrusive frame unlink and acknowledged exit order. Session10 S10-AU-REGISTRY-05 locally verifies idle exit notification demand while retaining acknowledged waiting removers. Registry gate remains BLOCKING, Sprint10 IN PROGRESS, release NO-GO/development-unqualified. Whole G01/actualDAW-origin/externalAU/MB-04 remain open.


Session10 actual loaded negative uses unchanged session09 ON Release AU fingerprint c74530ef1f986775c3dbdb73300cad9af1ca7357f3116c58426c65325aa7f3b7, binary64f2fd1e6843e00cee785e445e6986d2c7ee4457fe3423de711c55315a565c59. Actual native bypass SetParameter consumed in real Render dispatches1/8/64/257 registered duplicates on fresh workers. With no remover pending, each frame exit calls pthread_cond_broadcast:1/8/64/257 instead0. External condition interposer counts original calls and captures actual stack condition_variable::notify_all -> AUPropertyRegistry::changed -> bypass -> Render. No product observer/admission decision added for negative, no synthetic PropertyChanged. Existing diagnostics delimit full registry. This is actual unnecessary plugin-triggered OS notification work, not reproduced deadlock/race/timing miss. Host empty callbacks/barriers and probe stack unwinding are separately owned. Real condition control observes1; allocation/mutex control1/1/1/1 retained. Frozen complete source/header/fixture source+executable/interposer source+library/generatedAU+SDK+VST3/generator/cache/compile/bundles/pinned29fileSDK/report copied before repair. Sandbox first factory registration failure and formatting violations are retained separately. Authorized in-process testing installs nothing.

Session10 ONLY product change is AUPropertyRegistry.hpp. A gate-owned size_t waitingRemovals tracks stack WaitDemand scopes after allocations/withdrawal/publication/reclaim, before acknowledged.wait can atomically release gate. RAII decrements with gate owned, including exceptional wait exit; demand spans spurious wakeups. Frame exit still gate-held constant-work unlink, then notify_all only when demand nonzero. If exit wins gate first, later remover predicate sees no frame; if remover wins first, exit sees demand and broadcasts. All simultaneous removers retain demand until their own acknowledgement; no required wakeup is lost. Same-thread/no-match immediate scopes cannot expose demand to other gate-held exits. Callback-origin rejection remains before any mutation/allocation/demand. Both selectors/second removers/withdrawn frames retain full acknowledgement. Idle broadcasts0 at1/8/64/257; pending broadcasts1/8/64/257, both removers wait then ack2. No cap/TLS/table/reader allocation/deletion/notification loss/gesture coalescing. Capture/admission/exit mutex remain, complete registry0/0/3/0; pending broadcasts and host-sized dispatch/remove/reclaim/audio-reentered allocating writers remain prohibited/open. This is NOT nonwaiting exit or whole G01 qualification.

Final session10 exact formatted-source Release and separate ASan-UBSan/TSan ON/OFF native/editor matrices, condition controls, first-thread and lifecycle tests, strictAU1274, inherited registry/ownership/suppression/VST3/seals/adapter/recall3of3, updater128fresh/4x256/64teardown, editornested/direct7/64detach/Release3072four scales/sanitizercontrols/gestures/undo/recall/close, core16of16, format and3generator drift rejections are recorded in checkpoint/logs. Consult actual counts and diagnostics; do not infer more than successful final logs. Final rebuilt defaultsOFF hooks/probe absent, generated/fixture separate sanitizer flags verified. Session09 original real ASan default-stack overflow remains frozen; all nested257 fixtures retain explicit8MiB worker stack and join. All jobs completed at finalization; no job to resume. Eleven historical inventories291/505/357/364/409/366/397/422/492/470/546 remain unchanged. Final session10 inventory is authority; never reseal historical bytes. Source/binary integrity is not release acceptance.

Session09 actual diagnostic Release negative AU fingerprint92abd2f1546ae62063516334d406da0d1d8f47e9f5bf0c1aed7eb038c97dba37, binaryf3b183212166fd41a0ca731bd940f01f55a22fb2b8845ac2ed51cfb739ad07ec. Native bypass SetParameter consumed by real Render admits older A; newer real SDK NickName callback nests1/8/64/257 frames and holds. A returns before those frames. Old RAII unlink visits1/8/64/257 predecessors under gate, expected0. This is observed host-sized exit work, NOT reproduced deadlock/race/timing miss. DefaultOFF observer counts only actual predecessor traversals; added to old algorithm for negative, does not change admission/order, no synthetic PropertyChanged. Historical shipping/default baseline separately retained. Negative complete source export/fixture source+executable/generatedAU+SDK+VST3/generator/cache/compile/bundle/pinned29fileSDK copy and report frozen BEFORE repair. Never alter those bytes. Baseline sandbox register failure separately retained; authorized in-process execution installs nothing. Unique command ID collision retained as tooling failure before configure.

Session09 repair only AUPropertyRegistry.hpp and AURegistryTests.cpp implementation. Admission under retained gate sets stack frame incoming-link pointer to registry head, rewires previous head incoming link to new frame.next, publishes head. Exit under same gate writes *previous=next, then if next exists next->previous=previous; notifies waiters. Predecessor exiting first transfers its successor's incoming link before its own stack disappears; successor exiting first updates live predecessor.next. Head/middle/tail/concurrent/nested frames use same invariant. Inactive unlinked frame does nothing. No list search/TLS/table/cap/reader allocation or reclamation. Observer reports0 visits; source proves one mandatory pointer update and at most one successor backlink update. Gate acquire and notify still prohibited; this is NOT nonwaiting exit.

Remove still scans ALL live frames under gate including withdrawn frames/second removers. ANY same-instance caller-thread callback frame plus matching OTHER-thread frame returns CannotDoInCurrentContext(-10863) before allocation/withdrawal/publication. Error authorizes NO context destruction; host retains/retries outside callbacks after owners joined, excluding new matching registration. Same-thread active/ancestor withdrawal remains legal but contexts live until frames return. Off-callback remove withdraws then CV-waits with gate released for all matching other-thread frames. Active check/admission linearize with withdrawal under gate. Ordered immutable snapshots/duplicates/no cap/outer-vs-nested visibility/writer-only retired use_count1 reclaim/normal reader never last-owner deletes remain. DO NOT remove registry gate until EVERY writer, reader and context has replacement publication and acknowledged reclamation.

All SDK virtual Add/Remove/PropertyChanged callers still route: state/RestoreState/layout/channel/stream/input connection/render callback/MaximumFramesPerSlice/LastRenderError/name/preset and conditionalMIDI/hotmap. SDK vector empty, const accessor no pinned caller. MIDI runtime unselected/source-only. ContextName constructor callback owns wrapper-this through legal shutdown; nested64 and surviving registration verified. Native bypass and fixed message/timer updater use same registry. Legacy no-context public function unavailable arm64; real loaded SDK Lookup selector tested. Generator/SDK/pins/shared processor/DSP/editor/VST3 implementation unchanged.

Legal wrapper teardown excludes and joins ALL complete wrapper/processor entries, fixed message owner, never disposal inside callback. Updater stop/clear before registry shutdown while wrapper-this lives. Processor removeListener alone acknowledges neither complete updateHostDisplay nor final forwarding. Optional AUEntryGuard locks only nonnull GetMutex; SYS01 null. Restore lanes assume caller-serialized same-role off-audio; concurrent same-role/recursive stateMutex restore, owner migration and arbitrary concurrent lifecycle unqualified. Updater60Hz construction timer/off-message atomicflags+immutable pthread comparison/message synchronous nesting/existing flags coalescing retained; stopped loop delays, closed editor polls.

Start with complete registry probe0/0/3/0 and gate/capture/admission/exit/conditional-notify/WaitDemand/remove/reclaim in AUPropertyRegistry.hpp, native_au_adapter.py generated overrides and ownership trace. Capture/admission/exit still block; dispatch/remove/reclaim host-sized, reentered audio writers allocate/free. Cross-instance cycles, callback waiting for its own off-callback remover, arbitrary parameter reentry/lock orders remain unqualified. Choose an actual remaining defect before repair; no timing acceptance from finite warm zeros. Distinguish plugin scaffolding from host fixture barriers/OS bodies. Include first-thread entry, both removals, active/withdrawn/nested/overlapping frames, ContextName wrapper-this and legal teardown. No listener cap, loss or gesture coalescing.

Then APVTS ParameterAdapter/LockedListeners, regular/final parameter and processor listeners, CachedParamValues and immutable resources/lifecycle. Pinned source confirms virtual authority -> atomic raw mirror -> APVTS listener call under recursive CriticalSection -> listenersNeedCalling=false/needsUpdate=true; add/remove same lock. Flag stores actually follow listener call; raw mirror precedes it. Parameter reverse regular+final dispatch retains parameter listenerLock and stable editor synchronized removal. Processor updateHostDisplay reads size outside lock, obtains each pointer under lock then invokes unlocked; final ParameterChangeForwarder list traversal has no processor listenerLock. Every writer/reader/lifetime must be reviewed before replacement. VST3 CachedParamValues uses SEPARATE atomic float values and grouped atomic change flags, immutable IDs after shared construction; component/controller shared refs retain storage. Off-message paramChanged writes cache, process drains changed flags and calls host output queues. No packed float/flag claim. Cache storage setup/destruction/resource acquisition remain legally excluded, not arbitrary-lifecycle safe. Prepared maps/scratch/DSP/callback/FL locks open.

Session09 initial ASan registry and observer tests genuinely overflowed default worker stack at257-level real SDK recursion;1/8/64 passed. Original failing logs/executables retained. Fixture now uses pthread_create explicit8MiB SDK worker stack in ALL trees, joins before destruction; same loaded AU bytes pass full257. No product stack-depth/lifecycle waiver. Release/separateASan-UBSan/TSan ON/OFF strictAU1274/callbacks/registry/inherited ownership/suppression/VST3/seals/adapter/recall3of3/editor checks pass at exact checkpoint boundaries. New older-exit1/8/64/257 tests both selectors wait then acknowledge2; observer0. Inherited crossadd/order257/secondremover/overlap2/2/nested64/active+withdrawn rejection/destruction128 pass. Updater128fresh/4x256/64legalteardown. Editor nested/direct7/64detach, Release3072renders/four scales, separate sanitizercontrols/gestures/undo/recall/close. Core16of16/format/3generator drift rejections. Final successful logs have no observed assertion/ASan/UBSan/TSan diagnostic; initial actual ASan stack-overflow is retained, never claim no sanitizer failure ever occurred. GeneratedAU/SDK/VST3 and fixture sanitizer flags verified; hooks/probe absent finalOFF defaults. Consult final identities for exact hashes and logs for whole default lock counts; G01 stays open.

Preserve org.algorhythmia.sys01/Algr/Sys1, seven keys/nativeIDs, four programs/order, schema1exactly40, parameter-only deterministic empty-history recall,10ms smoothing, shared256whole-batch rejection/no replay, eight restores, single audio owner, stereo effect/generator/disabledinput and formats. Stable editor off-message atomicdirty only/no payload/post/UIpointer, seven authoritative packed FLOAT/GENERATION words (distinct from VST3 cache), synchronized removal/message gestures/undo/recall/visibility remain.

Tools CMake/CTest4.4.4 /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin; format18.1.8 siblingclang_format/data/bin; AppleClang21/macOS26.6.2/25G83arm64. JUCE8.0.12 revision29396c22c93392d6738e021b83196283d6e4d850 at /private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850, doctest2.4.12 revision1da23a3e8119ec5cce4f9388e91b065e20bf06f5 build/_deps/doctest-src; pins cmake/dependencies.lock.json verified4377/270. Native/private/tmp/sys01-s09-repair,sys01-s09-asan,sys01-s09-tsan finalOFF; core/private/tmp/sys01-s09-session02-core. Reconstruct missing temps from pins/cache/source export. Frozen rpaths may require absoluteDYLD_LIBRARY_PATH; NEVER modify frozen bytes. Probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54. Eleven previous evidence inventories unchanged; Sprint08 live-source intentionally invalid,never reseal.

Only installed AbletonLive12Suite permitted; REAPER skipped/pluginval absent/temporaryAU auval bind requires forbidden install. ActualDAW-origin/externalOS/architecture/signing/rights/accessibility/humans/budgets/soak/MB-04 remain unqualified. Sprint10 exit needs immutable prepared resources,nonwaiting audio,off-audio configuration waits,acknowledged EVERY-writer reclamation,safe invalid concurrent lifecycle silence/error,complete zero-prohibited callbacks and shippingformat concurrency/lifecycle checks. No waiver or advancement.

Use new evidence/sprint-10-session-11 and unique command IDs, actual argv/cwd/UTC/exits/tool/dependency/source/artifact hashes/failures/invalidation. Freeze actual loaded negative before repair. Complete appropriate native/editor/core regressions,separate sanitizers generated-wrapper/first-thread/lifecycle checks and final diagnosticsOFF defaults with hook/probe absence. Finish compact rolling state,new handoff from handoff-template.md,exact checkpoint/remaining gates/next action and COMPLETE ready-to-paste following-session prompt. Continue Sprint10 if incomplete.

Session10 retained sanitizer tooling failure: the initial ASan condition fixture
used the Release malloc interposer control. ASan allocator interceptors superseded
those bindings (alloc/free0/0,mutex1,unfair3), so the control failed before focused
condition checks; early std::exit left loaded instances alive and emitted actual
JUCE leak assertions. Original executable/library/log/report remain retained. The
condition-only separate sanitized fixture retains real broadcast control1, full
loaded registry/nesting/both selectors/idle counts/legal teardown on SAME AU bytes;
Release separately retains allocation/free/mutex/unfair controls and0/0/3/0. Do not
claim sanitizer allocation interposition or no assertion failure ever occurred.

Exact session10 checkpoint source export SHA256 122a55584e3b7f6a0f0bdfaf650c54dd6c2c08694d991d5fbe992251b6d49b4f; resume-inputs SHA256 9213e7850a25b757a618b1d71f3c7705248b6f5b953ee338b3b445e1ebb75f85. Final diagnosticsOFF Release AU fingerprint b176242561f746c3551ab3f6b24d8a3064b0ef8187611fe81d192bd4f61056e0, binary 5cb34fe3b1d15d14b6c968455cf4dd42fabd81789a17b6d29f0d235440ec654e; VST3 fingerprint 41719ca2b332a4a1fe13a54a533f3b5b8af954cbbc5cfd88ca54132f291cc0c1, binary 27a564bd180a3bd18b38f98f443dfbafac0db6387b2999c28c6d651e2ca692a8. Complete registry single-listener alloc/free/mutex/unfair0/0/3/0, count8/64/257 mutex17/129/515. Whole default freshAU1-frame mutex9/hostile603,VST3fresh1024-frame mutex9,alloc/free0/0. Final condition command IDs s10s10-condition-repair, s10s10-condition-asan-only, s10s10-condition-tsan-only. Original s10s10-condition-asan FAIL is retained and is not a passing command. Final matrices completed via s10s10-resume-authorized after s10s10-complete-authorized stopped at that fixture control failure. All complete,none to resume; exact final inventory in finalization/evidence-inventory.
```
