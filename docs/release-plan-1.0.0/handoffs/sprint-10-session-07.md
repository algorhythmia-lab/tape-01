# SYS 01 Sprint 10 session 07 handoff

## Checkpoint

2026-10-20 10:41 America/Santiago (13:41 UTC). Sprint/session10/07.
Objective S10-AU-REGISTRY-02: repair actual registry-held callback cross-thread
registration wait, with acknowledged callback contexts.
LOCALLY VERIFIED at gate-released host callback / per-frame removal boundary.
Sprint10 IN PROGRESS; release NO-GO/development-unqualified. Registry access gate
remains BLOCKING. G01 whole native realtime,actualDAW-origin/external AU and
MB-04 DC open. No Sprint11/waiver. Other bounded prior issue statuses retained.

Gitroot /Users/lhommeduchili/algorhythmia, SYS01root
/Users/lhommeduchili/algorhythmia/sys-01; HEAD unchanged
5c45cf6bed431317a1b412b1fc12ee62b66cfdd5. No applicable AGENTS. All148source+
13reference inputs matched session06 before edits. Dirty/untracked SYS01 and
unrelated monorepo work preserved; no staging/reset/stash/discard/commit/tag/
publish/subagent/contact/install/personal plugin directory edits.
149-input source export SHA256 3a2efaa947ad83815a35ad54e80fac11eb68425cc4a846d96b7126c9e0345a02;
resume-inputs SHA256 8fa7e4b6af0afeb1a55625cdf4f0a22342ef852c4a8dd2baafb52a37fa38d36f.
Final identities/checkpoint/source delta/export/ownership trace and command registry
in ../evidence/sprint-10-session-07. Reference hashes avoid self/circular inputs.

## Changes and decisions

AUPropertyRegistry.hpp releases access gate around every host callback, admits
stack frames atomically with active check, withdraws before publication and waits
for matching other-thread frames including previously withdrawn frames. Callback
RAII unlinks/notifies on return/unwind. Gate retained for every publication/active/
frame access; writer-only retired reclaim,normal reader never last-owner deletes.
Both removal variants/order/duplicates/no cap/nested snapshot visibility preserved.
Reentrant active/ancestor context survives until same-thread frame returns.
Throwing allocation completes before withdrawal; failed remove never authorizes
host context destruction. No ingress held across callback.

native_au_adapter.py adds defaultOFF complete virtual PropertyChanged probe;
CMake wires probe only into registry fixture; AURegistryTests expands actual
loaded callback/add,second-remover,SDK/native overlap and nested64/legacy variants.
Shared processor/DSP/editor/VST3 generator unchanged. Ownership record
../au-property-callback-ownership-session-07.md describes every caller and limits.

Frozen actual negative AU fingerprint
3870a2aa3a1b8ffb6c53e93dd2a1f59b11cae6ec417d5be1d959973319cb63ee;
binaryb35f36b4150b03923485aa63ab79cbf0bdf6612a00e3c40c3f39af2900c3f5d1.
Real native bypass Render callback waits for other-thread actual AU add; old gate
holds it until watchdog release2seconds. completed-before-return0/calls1 instead1/1.
Actual failure,bundle,fixture,source,generated AU/SDK/dispatch,cache/compile and
generated SDK frozen before repair; unchanged pinned SDK copy retained afterward. Early run used previous executable during build
and failed sandbox factory registration; checkpoint copied filename later failed.
Both separately retained and corrected; no observed sanitizer failure invented.

Host callback contexts must support overlapping concurrent property dispatches;
within-dispatch order/duplicates remain, no total cross-thread order. Removal
acknowledges only matching frames; host excludes new matching registration during
destruction. Same-thread ancestors stay alive. Legal wrapper destruction excludes/
joins ALL complete wrapper/processor dispatches, runs on fixed message owner and
never within callback. Access gate alone no longer acknowledges whole dispatch
teardown. Updater stop/clear precedes registry shutdown while wrapper-this alive.
Processor removeListener alone does not acknowledge updateHostDisplay. Arbitrary
concurrent lifecycle/state/layout/ContextName writers remain unqualified.

Gate capture/admission/exit,exit notify/unlink and host-sized traversal still
block/unbounded; reentered audio writers allocate/free. Callback waiting for
its own removal on another thread and cross-thread mutual-removal cycles remain
unqualified. No false acknowledgement/deleted gate/listener cap/gesture coalescing.
All SDK virtual state/layout/input/LastRenderError/MIDI/updater/bypass callers
route here; SDKvector empty,constaccessor no caller. Conditional MIDI source
coverage only. ContextName wrapper-this tested; legacy selector real Lookup.
AU optional GetMutex null correction and caller-serialized restore lanes retained.

Preserved org.algorhythmia.sys01/Algr/Sys1,sevenkeys/nativeIDs,fourprograms/order,
schema1exactly40,parameter-only deterministic empty-history recall,10ms smoothing,
shared256whole-batch rejection/no replay,eightrestores,singleaudioowner,VST3/stereo
input/generator/disabledinput/release scope. Stable editor dirty relay,seven packed
word polling and synchronized parameter removeListener/gestures/undo/recall remain.
No pending product decision required for this bounded repair; no external approval
or qualification inferred.

## Verification

| Check | Command registry | Exit/result | Boundary |
|---|---|---|---|
| Inputs/tools/pins |initial-state.json,s10s07-versions/compiler/platform/juce-pin/doctest-pin|148+13match,pins4377/270|Before editing|
| Actual negative |s10s07-negative-authorized|exit1 completed0vs1|Frozen loaded AU|
| Focused repair |s10s07-repair-focused|exit0 crossadd/order257/secondremove/nested64/destruction128|Loaded AU|
| Expanded registry |s10s07-{repair,asan,tsan}-expanded-registry|allPASS SDK/native2/2,nested64/0,128destruction|Separate instrumented bundles|
| Complete registry probe |s10s07-registry-probe,expanded-registry-probe|0/0/3/0,PASS control1/1/1/1|Complete PropertyChanged;locks remain|
| ON/default matrices |s10s07-{repair,asan,tsan}-verify-{on,default}-*|allPASS strict1274/callback/VST3/seals/3/3/editor|Final named bytes|
| Updater probe |s10s07-expanded-updater-probe|0/0/0/0;128fresh/4x256publishers/64teardown|Inherited complete wrapper updater|
| Core/format/generation |s10s07-core/format-final/generation|16/16,formatPASS,3driftrejections|Core source unchanged|
| Final checkpoint |s10s07-checkpoint-final|exit0|OFFflags/hooks/probe absence,sanitizerflags,historical inventories|

Release editor3072renders/four scales and gestures/undo/recall/close pass ON/OFF.
Separate ASan/UBSan and TSan editorcontrols pass; seven-key nested/direct64detach
pass all. Final registry defaults also include SDK/native overlap2/2 and nested64.
No observed final assertion/ASan/UBSan/TSan failure in finite runs. Full generated
AU/SDK/VST3 and fixture sanitizer flags retained in sanitizer-flags.json.
Whole default AUfresh1mutex9/hostile603,VST3fresh1024mutex9,alloc/free0/0 remain
G01 blockers; finite warm zeros do not prove whole realtime/reclamation.

CMake/CTest4.4.4 at /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/
lib/python3.14/site-packages/cmake/data/bin;format18.1.8 siblingclang_format/data/bin;
AppleClang21/macOS26.6.2/25G83arm64. JUCE8.0.12 pinned revision29396c22c93392d6738e021b83196283d6e4d850,
tree34954713e005f05801972439db07a78356b36bd421b51f2957b3e38605022f85;
doctest2.4.12 revision1da23a3e8119ec5cce4f9388e91b065e20bf06f5,
tree5778f85f9828756663bd8271eb7462ef9390f72a1cb92f5c1c50fff8d20fb90a.
Probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.
Command records retain tool/log/argv/cwd/UTC/exits and exact report fingerprints.
Eight historical inventories291/505/357/364/409/366/397/422 verified unchanged.
Sprint08 live-source intentionally invalidated;never reseal.

Affected AU callback/concurrency/host/lifecycle/performance/soak/signing proof
invalidated; new reports retain their local named boundary. ActualDAW-origin,
externalOS/architecture,signing/rights/accessibility/humans/performance/soak and
MB-04 remain unqualified. Only installed AbletonLive12Suite allowed;REAPER skipped;
pluginval absent; temporary-path auval requires forbidden install. No external
DAW/soak/human/signing acceptance claimed.

Final default bundle fingerprints (binary SHA256 in final-identities.json):

| Tree | AU | VST3 |
|---|---|---|
|repair|e37e3b2e5e3b85ee0c33a9bd03efb5771323c6ba9f35d7d7238c44aff25c3f55|32b56f1664647e7d0082b4b39c7aab994c9202e3ce206e491c084118f7517f16|
|asan|b81a71cd801768d8a97c57d217b6474a10c765d772f6f631cf0d444e3f1c2959|3029bbdf681aab2de5cd10aa85ddc854a3aafd821d84b2f0707c26f03e72046b|
|tsan|e4bb1897c2da1a525d9f7e220481b7e239cf75c56162996266ecd6bb1c871de9|e08c9c1d349adedc44708a1a5b3fcc21b8241556a3c363099b6890506979ac80|

## Resume

All jobs completed;none to resume. Native trees/private/tmp/sys01-s09-repair,
sys01-s09-asan,sys01-s09-tsan finalhooksOFF/probeOFF;core/private/tmp/sys01-s09-session02-core.
Reconstruct missing temporary trees from pins/cache/source export. Frozen rpaths
use absolute DYLD_LIBRARY_PATH if needed; never edit frozen bytes.

Session08 immediate action: read ownership trace,AUPropertyRegistry.hpp gate/
capture/Frame/remove/reclaim,generated overrides and complete probe0/0/3/0.
Reproduce actual remaining registry admission/exit wait/order or callback ownership,
freeze it and choose complete every-writer/reader/context publication/reclamation
before removing access gate. Callback mutual-removal/lifecycle invalidation remain
unqualified. Then APVTS/regular/final listeners,CachedParamValues,immutable prepared
resources. Sprint10 requires nonwaitingaudio,off-audiowaits,acknowledged every-writer
reclamation,safe silence/error invalid concurrent lifecycle and complete zero-
prohibited shippingcallbacks/concurrency tests. No Sprint11 or waiver.

## Complete next session prompt

Also saved verbatim at ../evidence/sprint-10-session-07/next-session-prompt.txt.

```text
Work in /Users/lhommeduchili/algorhythmia/sys-01.

Continue SYS 01 v1.0.0 Sprint 10, session 08: nonwaiting AU registry admission/exit and callback ownership. This is an implementation session. Reproduce and freeze one concrete remaining plugin-controlled wait/order or callback ownership defect on an actual loaded bundle, implement one coherent bounded repair, and verify it. Remain Sprint10; do not advance Sprint11 or waive gates.

Read docs/release-plan-1.0.0/rolling-state.md and handoffs/sprint-10-session-07.md first. Then read au-property-callback-ownership-session-07.md, au-updater-ownership-session-06.md, au-property-registry-session-05.md, handoffs/sprint-10-session-06.md, au-host-notifications-session-04.md (SDK guard serialization claim superseded), native-vst3-suppression-session-03.md, native-suppression-session-02.md, native-notifications-session-01.md, au-ingress-ownership-session-03.md, docs/release-1.0.0/native-ownership.md, and the Sprint10/acceptance/evidence-invalidation/handoff sections of audit-production-plan-2026-10-18.md. Read session07 checkpoint.json, final-identities.json, ownership-trace.json and this complete prompt.

Check Git status and applicable AGENTS.md. Before editing compare live against evidence/sprint-10-session-07/resume-inputs.json and resume-reference-inputs.json; resolve intervening changes. Git root /Users/lhommeduchili/algorhythmia; recorded HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5. All148source+13reference matched session06 at session07 start. Preserve dirty/untracked SYS01 and unrelated monorepo work. No bulk staging/reset/discard/stash/overwrite/commits/tags/publication. No subagents, installation, personal plugin directory changes or contact with others.

Sprint09/S09-OWN-01 complete at named local boundary. S10-NOTIFY-01/AU-SUPPRESS/VST3-SUPPRESS and S10-AU-UPDATER-01 locally verified at their bounded contracts. S10-AU-NOTIFY-01 only caller-serialized off-audio restore identity/per-call stack events. S10-AU-REGISTRY-02 locally verified for registry gate released around host callbacks with per-frame removal acknowledgement, retaining BLOCKING access gate. Sprint10 IN PROGRESS; release NO-GO/development-unqualified. G01 whole native realtime,actualDAW-origin/external AU and MB-04 DC open.

Session07 actual frozen diagnostic AU fingerprint3870a2aa3a1b8ffb6c53e93dd2a1f59b11cae6ec417d5be1d959973319cb63ee; binaryb35f36b4150b03923485aa63ab79cbf0bdf6612a00e3c40c3f39af2900c3f5d1. Native BYPASS consumed by actual Render calls real host callback. Callback waits for another thread's real AudioUnitAddPropertyListener; old recursive gate spans callback and blocks add until two-second watchdog release. completed-before-return0/calls1 instead1/1. Actual failure/source/fixture/generated AU/SDK/dispatch/cache/compile/bundle frozen before repair; SDK copy verified pinned. Early load used preceding fixture while build ran and failed sandbox factory registration; checkpoint script later failed copied filename. Both setup/evidence failures retained separately. No invented sanitizer failure; frozen bytes never change.

Current registry keeps one mutex for every current/retired/active/frame access. Dispatch captures immutable ordered snapshot; active check and stack Frame insertion linearize under gate; callback runs after unlock. RAII unlink/notify acknowledges return or unwind. No TLS bootstrap/table/cap/payload retention. Remove prepares allocations/old retention before withdrawal/publication, then waits with gate RELEASED for all matching other-thread Frames, including already withdrawn Frames for simultaneous removers. Same-thread active/ancestor contexts survive until frame return. Concurrent property dispatches may invoke the same host callback concurrently; host contexts must support that and exclude new matching registration/publication during destruction. No total cross-thread order promised; within-dispatch order/duplicates and nested visibility preserved. Writers retain old snapshots before publication and only writers reclaim retired use_count1; normal readers never last-owner delete.

The gate remains BLOCKING at reader capture/admission/exit. Exit condition-variable notification and active-frame unlink, host-sized traversal and writer reentry alloc/free remain plugin-controlled blockers. Host callback waiting for another thread to remove its own active context cannot be falsely acknowledged. Cross-thread mutual removal cycles/parameter-lock orders/arbitrary reentry remain unqualified. No ingress is held across listeners. Do NOT remove gate without complete every-writer/reader/context replacement publication and acknowledged reclamation. No listener cap, notification loss or gesture coalescing.

All SDK virtual writers/readers still route to registry: state/layout/stream/input/LastRenderError/MIDI and wrapper bypass/updater. SDK vector empty; const accessor has no caller. ContextName constructor registration owns wrapper this, survives host removal, nested64 callback test passes. NickName SDK storage plus single-owner native bypass overlap2/2 passes. Legacy no-context C function absent arm64; actual instance SDK Lookup selector tested, WithUserData uses real public API. Conditional MIDI map paths have source coverage, no new runtime support claim.

Legal wrapper shutdown now requires exclusion/join of ALL complete wrapper/processor dispatches, fixed message owner and no disposal within callbacks. Access gate alone no longer acknowledges whole-wrapper destruction. Updater stops/clears before registry shutdown. Processor removeListener does NOT acknowledge concurrent updateHostDisplay. Session06 updater stays60Hz Timer registered during construction; off-message only flags/lock-free fetch_or/immutable thread comparison, no manager lookup/post/registry/preset/UI/alloc/free. Message delivery remains synchronous/nested; existing flag coalescing unchanged, editor-closed polls, stopped loop delays. Owner cannot migrate. SDK AUEntryGuard only locks non-null GetMutex; AUBase mAUMutex=nullptr and JuceAU never SetMutex. Concurrent same-role RestoreState/recursive stateMutex restore remain unqualified.

Start session07 complete registry probe (fresh alloc/free/mutex/unfair0/0/3/0), AUPropertyRegistry.hpp gate/capture/Frame/remove/reclaim, native_au_adapter.py and generated Sys01AU.mm overrides/callers. Trace every writer and reader before choosing nonwaiting publication/reclamation. Freeze actual next failure and exact identity before editing. Include first-thread entry, both removals, nested/overlapping dispatches, SDK ContextName wrapper-this and legal teardown. Then APVTS ParameterAdapter/LockedListeners,regular/final processor listeners,CachedParamValues and immutable resources/lifecycle. Distinguish plugin scaffolding from host/OS bodies; finite warm zeros/sanitizers are not whole ownership proof.

Session07 Release,separateASan/UBSan and TSan ON/OFF matrices pass strictAU1274/callbacks/registry/inherited ownership/suppression/VST3/seals/adapter/recall3/3/editor. Expanded registry cross-add/order257/second-remover/overlap2/2/nesting64/destruction128 pass all three. Updater complete probe0/0/0/0;128fresh/4x256publishers/64legal teardown pass. Editor nested/direct7/64detach pass ON/OFF;Release3072renders/four scales;sanitizercontrols/undo/recall/close. Core16/16,format and3generator drift rejections pass. No observed final assertion/sanitizer/race failure. Generated AU/SDK/VST3/fixture flags verified,all hooks/probe absent final defaults. WholeAUfresh1mutex9/hostile603,VST3fresh1024mutex9,alloc/free0/0 remain G01 blockers.

149-input source export SHA2563a2efaa947ad83815a35ad54e80fac11eb68425cc4a846d96b7126c9e0345a02; resume-inputs SHA2568fa7e4b6af0afeb1a55625cdf4f0a22342ef852c4a8dd2baafb52a37fa38d36f. Exact tested ON/OFF identities in final-identities.json;Release default AUe37e3b2e5e3b85ee0c33a9bd03efb5771323c6ba9f35d7d7238c44aff25c3f55,VST332b56f1664647e7d0082b4b39c7aab994c9202e3ce206e491c084118f7517f16. Eight historical inventories291/505/357/364/409/366/397/422 unchanged;Sprint08 live-source intentionally invalidated,never reseal. All jobs completed;none to resume.

Preserve org.algorhythmia.sys01/Algr/Sys1,sevenkeys/nativeIDs,fourprograms/order,schema1exactly40,parameter-only deterministic empty-history recall,10ms smoothing,shared256whole-batch rejection/no replay,eightrestores,singleaudioowner,VST3/stereo effect/generator/disabledinput/release scope. Stable editor relay off-message atomicdirty only,no payload/post/UI pointer;polls seven authoritative packed words,survives synchronized parameter removeListener;message gestures/undo/recall/visibility remain.

Tools: CMake/CTest4.4.4 /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin;format18.1.8 siblingclang_format/data/bin;AppleClang21/macOS26.6.2arm64. JUCE /private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850,doctest build/_deps/doctest-src,pins cmake/dependencies.lock.json. Native/private/tmp/sys01-s09-repair,sys01-s09-asan,sys01-s09-tsan finalOFF;core/private/tmp/sys01-s09-session02-core. Reconstruct missing temp trees from cache/pins/source export. Frozen rpaths need absolute DYLD_LIBRARY_PATH if absent;never modify frozen bytes. Probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.

Only installed AbletonLive12Suite allowed;REAPER skipped,pluginval absent,auval cannot bind temporary AU without forbidden install. ExternalOS/architecture,actualDAW-origin,signing/rights/accessibility/humans/performance/soak/MB-04 unqualified. Sprint10 exit still requires immutable prepared resources,nonwaiting audio acquisition,off-audio configuration waits,acknowledged every-writer reclamation,safe silence/error invalid concurrent lifecycle,complete zero-prohibited plugin callbacks and passing shipping-format concurrency/lifecycle checks. No waivers.

Use new evidence/sprint-10-session-08 and unique command IDs;retain actual argv/results/tool/dependency/source/artifact hashes,failures,invalidation. Renew affected native/editor/core regressions and separate sanitizers including generated wrappers/first-thread/lifecycle. Ownership diagnostics defaultOFF;verify hooks/probe absent final rebuilt defaults. Finish with compact rolling-state.md,new handoff using handoff-template.md,exact tested checkpoint/remaining gates/next action and complete ready-to-paste following-session prompt. Continue Sprint10 if incomplete.
```
