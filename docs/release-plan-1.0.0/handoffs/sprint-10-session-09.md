# SYS 01 Sprint 10 session 09 handoff

## Checkpoint

2026-10-20 13:16 America/Santiago (16:16 UTC). Sprint/session10/09.
Objective S10-AU-REGISTRY-04: eliminate host-sized predecessor search during
older callback frame exit while newer nested frames remain active.
LOCALLY VERIFIED at constant-work intrusive unlink / acknowledged exit-order
boundary. Sprint10 IN PROGRESS, release NO-GO/development-unqualified.
Registry gate BLOCKING retained; G01,actualDAW-origin/externalAU,MB-04 open.
No Sprint11 advancement or waiver.

Gitroot /Users/lhommeduchili/algorhythmia, SYS01root
/Users/lhommeduchili/algorhythmia/sys-01, HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5
unchanged; no applicable AGENTS. All150source+17reference inputs matched session08
before edits. Dirty/untracked SYS01 and unrelated monorepo work preserved. No
staging/reset/stash/discard/commit/tag/publication/subagents/contact/install/personal
plugin directory changes. Exact commands/exits/tool/log hashes in commands.json.
151-input source export SHA256 f7b28362f1f9552077bec32a89123b83f1556f9ef3542ad60fffa88797e20a9d;
resume-inputs SHA256 15462b06d4ba90673174b3d69c042820862f044687f3b8425b18fb79085a89ba.

## Changes and decisions

Only apps/desktop/AUPropertyRegistry.hpp and AURegistryTests.cpp implementation
changed. New ownership record ../au-property-exit-order-session-09.md. Generator/
SDK/pins/shared processor/DSP/editor/VST3 implementation unchanged.

Negative actual diagnostic Release AU fingerprint
92abd2f1546ae62063516334d406da0d1d8f47e9f5bf0c1aed7eb038c97dba37, binary
f3b183212166fd41a0ca731bd940f01f55a22fb2b8845ac2ed51cfb739ad07ec.
Native bypass consumed by real Render admits older callback; another worker
sets real SDK NickName and nests1/8/64/257 callbacks. Older native callback exits
while newer frames remain. Original unlink under gate visits1/8/64/257 predecessors
instead0. This is actual host-sized plugin exit work, not a reproduced deadlock,
race or timing miss. DefaultOFF observer counts actual old algorithm traversals
without changing decisions. Old shipping/default baseline separately retained.
Complete negative report/bundle/fixture/source export/generatedAU+SDK+VST3/cache/
compile/generator/pinned29fileSDK frozen BEFORE repair. Frozen bytes unchanged.

Each stack Frame has incoming-link pointer to registry head or live predecessor.next.
Admission updates former head backlink before publishing new head. Exit writes
incoming link to next and transfers successor backlink before own stack disappears.
One mandatory pointer update, at most one successor update; no list search, cap,
TLS/table/reader allocation/reclamation. All pointer work retains SAME registry gate.
RAII still unlinks/notifies. Active check/admission/withdrawal atomic under gate;
remove scans all live/withdrawn frames and waits gate-released, both selectors and
second removers. Callback-origin required matching-other-thread wait still returns
-10863 BEFORE mutation/allocation; failure authorizes NO context destruction.
Host retains/retries off callbacks after owners joined/excluding matching new add.
Same-thread active/ancestor context survives until return. Ordered snapshots/
duplicates/no cap/nested visibility/writer-only retired use_count1 reclaim retained.
Do not remove gate until EVERY writer/reader/context has replacement publication
and acknowledged reclamation.

All SDK virtual writers/readers still route registry; SDKvector empty/constaccessor
no pinned caller. ConditionalMIDI source-only. ContextName wrapper-this lives
through legal shutdown; updater stops/clears first. Legal teardown excludes/joins
ALL complete wrapper/processor entries, fixed message owner, never within callback.
Processor removeListener alone acknowledges neither complete updateHostDisplay nor
final forwarding. Restore same-role caller serialization and recursive stateMutex
limits retained. Updater60Hz/off-message flags/message nesting/stopped-loop behavior
unchanged. Captures/admission/exit mutex+notify, host-sized dispatch/remove/reclaim,
audio allocating writers, cross-instance/own-off-callback-remover cycles still open.

Preserve org.algorhythmia.sys01/Algr/Sys1,sevenkeys/nativeIDs,fourprograms/order,
schema1exactly40,parameter-only deterministic empty-history recall,10ms smoothing,
shared256whole-batch rejection/no replay,eight restores,singleaudioowner,VST3/stereo/
generator/disabledinput scope. Stable editor off-message atomicdirty/seven packed
FLOAT-GENERATION authoritative words/synchronized removal/message gestures/undo/
recall/visibility unchanged. VST3 cache values/flags are separate atomics, not that
editor packing. APVTS/regular/final listener/resources/lifecycle remain open.
No product decision pending for this bounded repair.

## Verification

| Check | Command registry reference | Result | Boundary |
|---|---|---|---|
| Input/tool/pins |initial-state,s10s09-versions/compiler/platform/juce-pin/doctest-pin|150+17match,4377/270pins|Before edits/pinned sources|
| Frozen baseline probe |s10s09-baseline-probe-authorized|0/0/3/0,PASS|Session08 exact loaded diagnostic bytes|
| Actual exit negative |s10s09-negative-authorized|exit1,visits1/8/64/257|Loaded observer AU, frozen before repair|
| Focused repair |s10s09-focused-authorized,exit-{repair,asan,tsan}-stack|zero visits/all depths PASS|Separate exact bundles/fixtures|
| Registry regression |{repair,asan}-on-registry-stack,tsan-on-native,all-default-native|both selectors await/ack2,all inherited PASS|ON/OFF loaded bytes|
| Native regressions |s10s09-*-{on,default}-native|strictAU1274/callback/ownership/suppression/VST3/seals/adapter/recall3of3 PASS|Release/separateASan-UBSan/TSan|
| Editor/updater |s10s09-*-{on,default}-notifications/editor,*-on-updater|nested/direct7/64detach,Release3072/four scales,updater128fresh/4x256/64teardown PASS|Sanitizercontrols/undo/recall/close|
| Probe |s10s09-registry-probe/default-au-probe/default-vst3-probe|registry0/0/3/0,wholelocks remain|G01 blocker retained|
| Core/format/generation |s10s09-core/format-final-stack/generation|16of16/format/3driftrejections PASS|Shared source unchanged|
| Final checkpoint |s10s09-checkpoint|PASS|OFFflags/hooks/probeabsence,sanitizerflags,source/artifact/frozeninventories|

Initial sandbox baseline register failed before plugin tests; authorized in-process
retry passed, nothing installed. Initial rebuild collided with unique command ID
before configure; final named rebuild passed. Initial ASan registry and observer
runs REALLY overflowed default worker stack at257 recursive SDK calls;1/8/64 passed.
Logs/executables retained. Fixture now pthread_create explicit8MiB SDK worker stack
in ALL trees and joins before destruction, same loaded bundles pass257. No product
stack-depth/lifecycle waiver. No observed final successful assertion/ASan/UBSan/TSan
diagnostic; do not claim no sanitizer failure ever occurred. Finite sanitizer
success cannot prove universal realtime/lifecycle safety.

Whole final Release fresh AU1mutex9/hostile603,VST3fresh1024mutex9,alloc/free0/0
remain blockers. Probe control1/1/1/1 verifies interposition. Release editor snapshots
are unpaced offscreen CPU measurements, not DAW/driver/UI budget qualification.
Separate sanitizer controls intentionally omit full images/paint budget claim.
GeneratedAU/SDK/VST3 and fixture sanitizer flags in sanitizer-flags.json; hooks and
probe absent finalOFF defaults. Ten previous inventories291/505/357/364/409/366/
397/422/492/470 unchanged; Sprint08 live-source intentionally invalid,never reseal.
Exact identities in final-identities.json; evidence/source delta/export/failures/
invalidation/ownership trace/commands retained. Source/binary integrity != release.

Tools CMake/CTest4.4.4 /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/
lib/python3.14/site-packages/cmake/data/bin,format18.1.8 siblingclang_format/data/bin,
AppleClang21/macOS26.6.2/25G83arm64. JUCE8.0.12 revision29396c22c93392d6738e021b83196283d6e4d850,
doctest2.4.12 revision1da23a3e8119ec5cce4f9388e91b065e20bf06f5,pins
cmake/dependencies.lock.json. Probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.
Only installed AbletonLive12Suite permitted;REAPER skipped,pluginval absent,auval
requires forbidden install to bind temporaryAU. ActualDAW/externalOS/architecture/
signing/rights/accessibility/humans/budgets/soak/MB-04 unqualified. Affected AU
callback/concurrency/lifecycle/host/validator/performance/soak/signing proofs invalidated,
renewed only at local named boundary. No external acceptance inferred.

Final diagnosticsOFF fingerprints (binary SHA256 in final-identities.json):

| Tree | AU | VST3 |
|---|---|---|
|repair|a02ece59543e558707e44bcd6d3672ce4417849583da4d60b7dfec07b4e5874e|86d67343c7cb2920f293de3ba2b8b460f177b2d66a93acbf1801eb4d96bf7e41|
|asan|3c6939211701c9e43024ea07bc362a58cdb6f0046a48540da6d167958f9855ec|39b23938bd8ac30f705f86f515d608e1ff994132cdcf5581d098bf8dacfe87d6|
|tsan|dae53807a24242d6deabf5c948d0de7c11c05232a63ce72e50a6c97020fa8e59|069b8b18f65aafcd5817c6907672337ce8ae3f6ba160ab95a9a745a2bad8be34|

## Resume

All jobs complete;none to resume. Native/private/tmp/sys01-s09-repair,sys01-s09-asan,
sys01-s09-tsan finalOFF;core/private/tmp/sys01-s09-session02-core. Reconstruct missing
temps from pins/cache/source export. Frozen rpaths use absoluteDYLD_LIBRARY_PATH;
never modify frozen bytes.

Session10 next: read complete ownership map, AUPropertyRegistry.hpp gate/capture/
admission/exit/notify/remove/reclaim and complete probe0/0/3/0. Reproduce and freeze
one actual remaining loaded wait/order or callback ownership defect before repair.
Retain gate for EVERY writer/reader/context replacement publication/reclamation.
Then APVTS ParameterAdapter/LockedListeners,regular/final processor listeners,
CachedParamValues,immutable prepared resources/lifecycle. S10 needs nonwaiting
audio/off-audio waits/every-writer acknowledgement/invalid lifecycle silence-error/
complete zero-prohibited shipping callbacks and concurrency/lifecycle tests.
Remain Sprint10 with no waiver. Complete following prompt is also standalone in
../evidence/sprint-10-session-09/next-session-prompt.txt.

## Complete next session prompt

```text
Work in /Users/lhommeduchili/algorhythmia/sys-01.

Continue SYS 01 v1.0.0 Sprint 10, session 10: remaining AU registry admission/exit wait/order and acknowledged callback ownership. This is an implementation session. Reproduce and freeze one concrete remaining plugin-controlled loaded-AU wait/order or callback ownership defect, implement one coherent bounded repair, and verify it. Remain Sprint10; no Sprint11 advancement or gate waiver.

Read docs/release-plan-1.0.0/rolling-state.md and handoffs/sprint-10-session-09.md FIRST, then au-property-exit-order-session-09.md, au-property-removal-context-session-08.md, au-property-callback-ownership-session-07.md, au-updater-ownership-session-06.md, au-property-registry-session-05.md, handoffs/sprint-10-session-08.md, au-host-notifications-session-04.md (SDK guard claim superseded), native-vst3-suppression-session-03.md, native-suppression-session-02.md, native-notifications-session-01.md, au-ingress-ownership-session-03.md, docs/release-1.0.0/native-ownership.md, and Sprint10/acceptance/evidence-invalidation/handoff sections of audit-production-plan-2026-10-18.md. Read session09 checkpoint.json, final-identities.json, ownership-trace.json, initial-state.json, negative-identities.json, fixture-identities.json, failures.json, invalidation.md and this COMPLETE prompt. Consult exact logs for counts/limits.

Check Git status/applicable AGENTS and compare LIVE with session09 resume-inputs.json and resume-reference-inputs.json BEFORE edits. Session09 began with all150source+17reference inputs matching session08. Gitroot /Users/lhommeduchili/algorhythmia, HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5 unchanged. No applicable AGENTS found. Preserve dirty/untracked SYS01 and unrelated monorepo work, all frozen evidence. No staging/reset/stash/discard/overwrite/commit/tag/publication/subagents/contact/install/personal plugin directory changes. No dependent background job remains to resume; consult finalization authority.

Sprint09/S09-OWN-01 complete at named local boundary. S10-NOTIFY-01/AU-SUPPRESS/VST3-SUPPRESS/AU-UPDATER-01 remain locally verified at bounded contracts. AU-NOTIFY only caller-serialized off-audio restore identity/per-call stack events. S10-AU-REGISTRY-02 releases the gate around host callbacks; REGISTRY-03 rejects callback-origin required other-thread matching-frame wait before any allocation/withdrawal/publication. Session09 S10-AU-REGISTRY-04 locally verifies constant-work intrusive frame unlink and acknowledged exit order. Registry gate remains BLOCKING, Sprint10 IN PROGRESS, release NO-GO/development-unqualified. Whole G01/actualDAW-origin/externalAU/MB-04 remain open.

Session09 actual diagnostic Release negative AU fingerprint92abd2f1546ae62063516334d406da0d1d8f47e9f5bf0c1aed7eb038c97dba37, binaryf3b183212166fd41a0ca731bd940f01f55a22fb2b8845ac2ed51cfb739ad07ec. Native bypass SetParameter consumed by real Render admits older A; newer real SDK NickName callback nests1/8/64/257 frames and holds. A returns before those frames. Old RAII unlink visits1/8/64/257 predecessors under gate, expected0. This is observed host-sized exit work, NOT reproduced deadlock/race/timing miss. DefaultOFF observer counts only actual predecessor traversals; added to old algorithm for negative, does not change admission/order, no synthetic PropertyChanged. Historical shipping/default baseline separately retained. Negative complete source export/fixture source+executable/generatedAU+SDK+VST3/generator/cache/compile/bundle/pinned29fileSDK copy and report frozen BEFORE repair. Never alter those bytes. Baseline sandbox register failure separately retained; authorized in-process execution installs nothing. Unique command ID collision retained as tooling failure before configure.

Repair only AUPropertyRegistry.hpp and AURegistryTests.cpp implementation. Admission under retained gate sets stack frame incoming-link pointer to registry head, rewires previous head incoming link to new frame.next, publishes head. Exit under same gate writes *previous=next, then if next exists next->previous=previous; notifies waiters. Predecessor exiting first transfers its successor's incoming link before its own stack disappears; successor exiting first updates live predecessor.next. Head/middle/tail/concurrent/nested frames use same invariant. Inactive unlinked frame does nothing. No list search/TLS/table/cap/reader allocation or reclamation. Observer reports0 visits; source proves one mandatory pointer update and at most one successor backlink update. Gate acquire and notify still prohibited; this is NOT nonwaiting exit.

Remove still scans ALL live frames under gate including withdrawn frames/second removers. ANY same-instance caller-thread callback frame plus matching OTHER-thread frame returns CannotDoInCurrentContext(-10863) before allocation/withdrawal/publication. Error authorizes NO context destruction; host retains/retries outside callbacks after owners joined, excluding new matching registration. Same-thread active/ancestor withdrawal remains legal but contexts live until frames return. Off-callback remove withdraws then CV-waits with gate released for all matching other-thread frames. Active check/admission linearize with withdrawal under gate. Ordered immutable snapshots/duplicates/no cap/outer-vs-nested visibility/writer-only retired use_count1 reclaim/normal reader never last-owner deletes remain. DO NOT remove registry gate until EVERY writer, reader and context has replacement publication and acknowledged reclamation.

All SDK virtual Add/Remove/PropertyChanged callers still route: state/RestoreState/layout/channel/stream/input connection/render callback/MaximumFramesPerSlice/LastRenderError/name/preset and conditionalMIDI/hotmap. SDK vector empty, const accessor no pinned caller. MIDI runtime unselected/source-only. ContextName constructor callback owns wrapper-this through legal shutdown; nested64 and surviving registration verified. Native bypass and fixed message/timer updater use same registry. Legacy no-context public function unavailable arm64; real loaded SDK Lookup selector tested. Generator/SDK/pins/shared processor/DSP/editor/VST3 implementation unchanged.

Legal wrapper teardown excludes and joins ALL complete wrapper/processor entries, fixed message owner, never disposal inside callback. Updater stop/clear before registry shutdown while wrapper-this lives. Processor removeListener alone acknowledges neither complete updateHostDisplay nor final forwarding. Optional AUEntryGuard locks only nonnull GetMutex; SYS01 null. Restore lanes assume caller-serialized same-role off-audio; concurrent same-role/recursive stateMutex restore, owner migration and arbitrary concurrent lifecycle unqualified. Updater60Hz construction timer/off-message atomicflags+immutable pthread comparison/message synchronous nesting/existing flags coalescing retained; stopped loop delays, closed editor polls.

Start with complete registry probe0/0/3/0 and gate/capture/admission/exit/notify/remove/reclaim in AUPropertyRegistry.hpp, native_au_adapter.py generated overrides and ownership trace. Capture/admission/exit still block; dispatch/remove/reclaim host-sized, reentered audio writers allocate/free. Cross-instance cycles, callback waiting for its own off-callback remover, arbitrary parameter reentry/lock orders remain unqualified. Choose an actual remaining defect before repair; no timing acceptance from finite warm zeros. Distinguish plugin scaffolding from host fixture barriers/OS bodies. Include first-thread entry, both removals, active/withdrawn/nested/overlapping frames, ContextName wrapper-this and legal teardown. No listener cap, loss or gesture coalescing.

Then APVTS ParameterAdapter/LockedListeners, regular/final parameter and processor listeners, CachedParamValues and immutable resources/lifecycle. Session09 trace confirms virtual authority -> atomic raw mirror/flags -> APVTS listener call under recursive CriticalSection, add/remove same lock. Parameter reverse regular+final dispatch retains parameter listenerLock and stable editor synchronized removal. Processor updateHostDisplay reads size outside lock, obtains each pointer under lock then invokes unlocked; final ParameterChangeForwarder list traversal has no processor listenerLock. Every writer/reader/lifetime must be reviewed before replacement. VST3 CachedParamValues uses SEPARATE atomic float values and grouped atomic change flags, immutable IDs after shared construction; component/controller shared refs retain storage. Off-message paramChanged writes cache, process drains changed flags and calls host output queues. No packed float/flag claim. Cache storage setup/destruction/resource acquisition remain legally excluded, not arbitrary-lifecycle safe. Prepared maps/scratch/DSP/callback/FL locks open.

Session09 initial ASan registry and observer tests genuinely overflowed default worker stack at257-level real SDK recursion;1/8/64 passed. Original failing logs/executables retained. Fixture now uses pthread_create explicit8MiB SDK worker stack in ALL trees, joins before destruction; same loaded AU bytes pass full257. No product stack-depth/lifecycle waiver. Release/separateASan-UBSan/TSan ON/OFF strictAU1274/callbacks/registry/inherited ownership/suppression/VST3/seals/adapter/recall3of3/editor checks pass at exact checkpoint boundaries. New older-exit1/8/64/257 tests both selectors wait then acknowledge2; observer0. Inherited crossadd/order257/secondremover/overlap2/2/nested64/active+withdrawn rejection/destruction128 pass. Updater128fresh/4x256/64legalteardown. Editor nested/direct7/64detach, Release3072renders/four scales, separate sanitizercontrols/gestures/undo/recall/close. Core16of16/format/3generator drift rejections. Final successful logs have no observed assertion/ASan/UBSan/TSan diagnostic; initial actual ASan stack-overflow is retained, never claim no sanitizer failure ever occurred. GeneratedAU/SDK/VST3 and fixture sanitizer flags verified; hooks/probe absent finalOFF defaults. Consult final identities for exact hashes and logs for whole default lock counts; G01 stays open.

Preserve org.algorhythmia.sys01/Algr/Sys1, seven keys/nativeIDs, four programs/order, schema1exactly40, parameter-only deterministic empty-history recall,10ms smoothing, shared256whole-batch rejection/no replay, eight restores, single audio owner, stereo effect/generator/disabledinput and formats. Stable editor off-message atomicdirty only/no payload/post/UIpointer, seven authoritative packed FLOAT/GENERATION words (distinct from VST3 cache), synchronized removal/message gestures/undo/recall/visibility remain.

Tools CMake/CTest4.4.4 /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin; format18.1.8 siblingclang_format/data/bin; AppleClang21/macOS26.6.2/25G83arm64. JUCE8.0.12 revision29396c22c93392d6738e021b83196283d6e4d850 at /private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850, doctest2.4.12 revision1da23a3e8119ec5cce4f9388e91b065e20bf06f5 build/_deps/doctest-src; pins cmake/dependencies.lock.json verified4377/270. Native/private/tmp/sys01-s09-repair,sys01-s09-asan,sys01-s09-tsan finalOFF; core/private/tmp/sys01-s09-session02-core. Reconstruct missing temps from pins/cache/source export. Frozen rpaths may require absoluteDYLD_LIBRARY_PATH; NEVER modify frozen bytes. Probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54. Ten previous evidence inventories unchanged; Sprint08 live-source intentionally invalid,never reseal.

Only installed AbletonLive12Suite permitted; REAPER skipped/pluginval absent/temporaryAU auval bind requires forbidden install. ActualDAW-origin/externalOS/architecture/signing/rights/accessibility/humans/budgets/soak/MB-04 remain unqualified. Sprint10 exit needs immutable prepared resources,nonwaiting audio,off-audio configuration waits,acknowledged EVERY-writer reclamation,safe invalid concurrent lifecycle silence/error,complete zero-prohibited callbacks and shippingformat concurrency/lifecycle checks. No waiver or advancement.

Use new evidence/sprint-10-session-10 and unique command IDs, actual argv/cwd/UTC/exits/tool/dependency/source/artifact hashes/failures/invalidation. Freeze actual loaded negative before repair. Complete appropriate native/editor/core regressions,separate sanitizers generated-wrapper/first-thread/lifecycle checks and final diagnosticsOFF defaults with hook/probe absence. Finish compact rolling state,new handoff from handoff-template.md,exact checkpoint/remaining gates/next action and COMPLETE ready-to-paste following-session prompt. Continue Sprint10 if incomplete.
```
