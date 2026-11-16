# SYS 01 Sprint 10 session 06 handoff

## Checkpoint

2026-10-20 09:52 America/Santiago (12:52 UTC). Sprint/session10/06. Objective S10-AU-UPDATER-01:
remove observed off-message AU processor-change allocation/post and manager lock.
LOCALLY VERIFIED at nonposting off-message updater / fixed message-owner boundary.
Release, separate ASan/UBSan and TSan ON/OFF pass; final default flags, symbols,
source/artifact and historical inventories verified.
Sprint10 IN PROGRESS; release NO-GO/development-unqualified. G01 whole native
realtime,actual DAW-origin/external AU and MB-04 DC remain open. No Sprint11/waiver.
Git root /Users/lhommeduchili/algorhythmia; SYS01 root /Users/lhommeduchili/algorhythmia/sys-01;
HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5. No applicable AGENTS. All146source+11reference
inputs matched session05 before edits. Substantial dirty/untracked/unrelated work
preserved; no staging/reset/discard/stash/commit/tag/publish/subagents/contact/install.

## Changes and decisions

scripts/native_au_adapter.py replaces AU AudioProcessorChangedUpdater AsyncUpdater
with construction-registered60Hz Timer, fixed message owner and lock-free atomic
flags. Off-message callbacks never consult manager,post,allocate,visit UI or registry.
Message-thread delivery stays synchronous and nested; timer exchanges before callback
and polls even with editor closed. Timer post/allocation machinery remains on scheduler/
message threads. Existing flag coalescing unchanged; value/gesture payloads untouched.
apps/desktop/CMakeLists.txt adds actual loaded fixture AUUpdaterTests.cpp, probe only
on fixture. Ownership record ../au-updater-ownership-session-06.md details all callers.
Registry gate retained unchanged. Shared processor/DSP/editor/VST3 generator unchanged.

Frozen actual negative fingerprint26a6ab040623d762eada3fea22ad9c8827931c528297d07948fe8e41dfa970e4;
binary3dc02bb219a82110c98c93f71d15b5621e896614aa20504da618ee75bb2038a5.
Public updateHostDisplay on actual captured processor reaches complete wrapper callback;
alloc/free/mutex/unfair1/0/4/0; stack probeRealloc->MessageQueue::post->JuceAU callback.
Control1/1/1/1. Frozen before repair: bundle,fixture,source,wrapper/SDK,cache,compile commands,
probe and actual reports. Setup/compile/sandbox registration failures separately retained.
No invented sanitizer failure. No personal plugin directory touched.

Legal teardown requires fixed message owner, no active callback disposal, host-excluded/
joined processor and wrapper entries. Pinned processor change traversal does not take
removeListener lock; do NOT claim that lock acknowledges updateHostDisplay. Updater
stopped/cleared before registry shutdown and wrapper destruction. Arbitrary concurrent
lifecycle,manager-owner migration and processor listener mutation remain unqualified.
Session04 SDK guard claim remains superseded. All compatibility/editor relay contracts
in previous handoff retained; seven keys/IDs,fourprograms/schema1exactly40/smoothing10ms/
256whole-batch/eightrestores/singleaudioowner/formats unchanged.

## Verification

| Check | Command registry | Result | Boundary |
|---|---|---|---|
| Inputs/tools/pins | initial-state.json,s10s06-*-version/compiler/platform/*-pin |146+11 match,CMake4.4.4/format18.1.8,pins verified|Before edits|
| Actual negative |s10s06-negative-authorized|exit1 alloc1/mutex4|Frozen loaded diagnostic AU|
| Repair probe |s10s06-repair-probe|exit0 alloc/free/mutex/unfair0/0/0/0|Entire off-message wrapper updater|
| Release ON |s10s06-repair-verify-on-*|all pass|StrictAU,callbacks,registry,ownership,VST3,seals,3/3,editor|
| ASan/TSan ON |s10s06-{asan,tsan}-verify-on-*|all pass|Separateaddress,undefined/thread trees|
| Defaults |s10s06-{repair,asan,tsan}-verify-default-*|all pass|StrictAU,callbacks,registry,VST3,seals,3/3,editor|
| Expanded updater |s10s06-expanded-updater-probe|0/0/0/0;all flags,nested,128fresh,4x256publishers,64teardown PASS|Complete wrapper publisher|
| Final ownership/identity |s10s06-checkpoint|exit0|Sanitizer flags,hooks/probe absence,frozen bytes,7 historical inventories|
| Core/format/generator |s10s06-core/format/generation|16/16,formatPASS,3 drift rejections|Core inputs unchanged|

New evidence ../evidence/sprint-10-session-06 retains actual argv/cwd/UTC/exits/tool/log/
source/artifact identities and failures. Final identities in final-identities.json;148-input source export and resume hashes below.
Listener/wrapper changes invalidate affected native/host/lifecycle/performance/soak/signing
proof; historical evidence unchanged,Sprint08 live-source intentionally invalidated.
Only installed Ableton Live12Suite allowed;REAPER skipped. No pluginval/temporary-path
auval install; external architecture/OS/signing/rights/accessibility/humans/soak/MB-04 open.

## Resume

All jobs completed;none to resume. Native trees finalhooksOFF/probeOFF. Updater complete
wrapper probe0/0/0/0. WholeAUfresh1mutex9,hostile603,VST3fresh1024mutex9;all measured
alloc/free0/0. These retain G01 blocker. No final observed assertion,sanitizer/race failure.
Releaseeditor3072renders/four scales/gestures/undo/recall/close;separate sanitizercontrols
and seven-key nested readback/64concurrent attach-detach pass ON/OFF. Finite local proof.

CMake/CTest4.4.4,format18.1.8 at recorded uv archive path;AppleClang21/macOS26.6.2 arm64.
JUCE8.0.12 pinned4377 files,doctest2.4.12 pinned270; tree hashes in dependencies.lock.json
and s10s06-*-pin logs. Probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.
Native trees /private/tmp/sys01-s09-repair,sys01-s09-asan,sys01-s09-tsan;core
/private/tmp/sys01-s09-session02-core. Reconstruct missing temp paths from pins/cache/export;
frozen rpath missing: absoluteDYLD_LIBRARY_PATH;never edit frozen bytes.

Historical inventories291/505/357/364/409/366/397 verified unchanged. No historical reseal.
New session inventory excludes itself;references exclude self/circular hashes.
Source-checkpoint.tar.gz SHA256 6c2d0b8c394bbd574427cbbe1f157df995e938dfd71fc70b31e68cd20c80c1e8;
resume-inputs.json SHA256 f15608df8d18de8716bdf21dc408b661c55c8bff759493b82a9e610415b4867d.
Final default bundle fingerprints (binary hashes in final-identities.json):

| Tree | AU | VST3 |
|---|---|---|
|repair|f3e2673d7fd30cd7deb546a7029231afa3a736c2c70f37746adecaf5e9feb885|7cbc8c25a717c8363798db2006f77643d95619132c17a36bd40981ee8b51a874|
|asan|a686193867a5c34516fbdc2f72ef509d988376771bcc95117f8ee579613d6bef|4e05f0eddd242dead5c150c58482b480be46fd4fef47d285d9d7ae92248a7665|
|tsan|ebd239241d116236383ff179ce857a687a6d896a4eeaecf3e47a70de9b1f573e|d8b186fe4a49cba222afc117fce484c4e69b57f302de9f89a38b3253d3d33a4d|

Next bounded implementation session07: reproduce actual loaded-AU registry blocking/
lock-order issue,then every-writer/reader/context publication and acknowledged reclamation.
Do not delete gate prematurely or hold ingress across callbacks. Prepared resources,
APVTS/regular/final listeners,CachedParamValues,invalid concurrent lifecycle safe silence/
error and complete zero-prohibited shipping callbacks still required. Remain Sprint10.

## Complete next session prompt

Full ready-to-paste prompt is also retained at
../evidence/sprint-10-session-06/next-session-prompt.txt.

```text
Work in /Users/lhommeduchili/algorhythmia/sys-01.

Continue SYS 01 v1.0.0 Sprint 10, session 07: nonwaiting AU registry and callback ownership. This is an implementation session. Reproduce one concrete remaining prohibited plugin-controlled operation or callback-order/lifetime defect on an actual loaded bundle, freeze actual failure and exact artifact identity before repair, implement one coherent bounded change, and verify it. Remain Sprint10 until its named exit criteria; do not advance Sprint11 or waive gates.

Read rolling-state.md and handoffs/sprint-10-session-06.md under docs/release-plan-1.0.0 first. Then read au-updater-ownership-session-06.md, au-property-registry-session-05.md, handoffs/sprint-10-session-05.md, au-host-notifications-session-04.md (SDK guard serialization claim superseded by session05), native-vst3-suppression-session-03.md, native-suppression-session-02.md, native-notifications-session-01.md, au-ingress-ownership-session-03.md, docs/release-1.0.0/native-ownership.md, and Sprint10/acceptance/evidence-invalidation/handoff sections of audit-production-plan-2026-10-18.md. Read evidence/sprint-10-session-06/checkpoint.json, final-identities.json, ownership-trace.json and this prompt.

Check Git status and applicable AGENTS.md. Before editing compare live inputs against session06 resume-inputs.json and resume-reference-inputs.json; resolve intervening user changes. Git root /Users/lhommeduchili/algorhythmia; recorded HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5. Preserve substantial dirty/untracked SYS01 and unrelated monorepo work. Do not bulk-stage/reset/discard/stash/overwrite/commit/tag.

Sprint09/S09-OWN-01 complete at named local boundary. S10-NOTIFY-01,AU-SUPPRESS,VST3-SUPPRESS locally verified at bounded ownership boundaries. S10-AU-NOTIFY-01 only caller-serialized off-audio restore identity and per-call stack event ownership. S10-AU-REGISTRY-01 stable reentrant snapshots and BLOCKING gate-acknowledged property removal. S10-AU-UPDATER-01 nonposting off-message processor-change publication under fixed message owner and legal publisher exclusion. Sprint10 IN PROGRESS, release NO-GO/development-unqualified. G01 wholeRT,actualDAW-origin/externalAU and MB-04 DC open.

Session06 actual frozen loaded negative: fingerprint26a6ab040623d762eada3fea22ad9c8827931c528297d07948fe8e41dfa970e4, binary3dc02bb219a82110c98c93f71d15b5621e896614aa20504da618ee75bb2038a5. Fresh-thread public updateHostDisplay on actual processor reaches real JuceAU audioProcessorChanged; complete wrapper callback alloc/free/mutex/unfair1/0/4/0. Stack probeRealloc->MessageQueue::post->JuceAU callback->updateHostDisplay. Control1/1/1/1. Functional flags/nested/teardown already passed. Source/fixture/probe/generated wrapper/SDK/cache/compile/bundle and actual reports frozen before repair; separate setup/compile/sandbox registration failures retained. Never modify frozen evidence.

Session06 generator replaces updater AsyncUpdater with fixed60Hz Timer registered during off-audio fixed-message-owner construction. Off-message only computes flags,lock-free fetch_or,compares immutable pthread message identity. No manager lookup/thread mutex,post,registry,preset,UI,payload,allocation/free. Message updates deliver synchronously including nesting; timer exchanges before callbacks,zero flags no work. Existing flag coalescing retained; no parameter/gesture coalescing or history. Timer scheduler/message machinery may allocate/post off publisher thread. Editor closed still polls; stopped message loop delays delivery. Construction and destruction use fixed message owner; owner migration invalid. Host excludes/joins processor/wrapper entries and disposal within callbacks. Updater stops/clears before registry shutdown. Pinned AudioProcessor::updateHostDisplay traverses listeners without removeListener lock; that lock does NOT acknowledge concurrent processor-change dispatch. Arbitrary concurrent teardown remains unqualified. Shared processor/DSP/editor/VST3 generator unchanged; registry gate unchanged.

Session05 correction persists: optional AUEntryGuard locks only non-null GetMutex. AUBase mAUMutex=nullptr,JuceAU never SetMutex; no SDK serialization for register/remove/RestoreState. Atomic restore-lane stack assumes caller-serialized same role. Concurrent same-role RestoreState,recursive stateMutex restore and resources/lifecycle unqualified.

Start session06 ownership-trace.json,AUPropertyRegistry.hpp,native_au_adapter.py,generated Sys01AU.mm all registry overrides/bypass/updater and JUCE processor dispatch. Reproduce remaining registry blocking/order or callback ownership defect with deterministic actual loaded-AU fixture and complete callback probe. Cover every SDK virtual caller state/layout/input/LastRenderError/MIDI,ContextName wrapper-this lifetime,both removal selectors,nesting,first-thread entry and legal teardown. Legacy no-context C function absent arm64; session06 tracked factory uses actual instance SDK Lookup selector; WithUserData real public API. Distinguish plugin traversal/allocation/locks/resource/message from host/OS callback bodies. Never hold ingress across reentrant listeners.

Do not delete registry gate without every-writer/reader/context replacement publication and acknowledged reclamation. Current immutable structural snapshots preserve order/duplicates/no cap; recursive gate protects publication,active flags and callback context. Cross-thread removal waits; reentrant active/ancestor context survives until frame return. Writer-only retired reclaim; normal readers never last-owner delete. Host-sized traversal,writer reentry allocation/free,cross-thread callback waits/reentry deadlocks and parameter/registry lock order remain open. Next APVTS ParameterAdapter/LockedListeners,regular/final processor listeners,CachedParamValues,arbitrary reentry and immutable prepared resources/lifecycle. Quiescence/finite sanitizers are not concurrent reclamation proof.

Preserve stable editor relay: off-message atomic dirty only,no retained payload/post/UI pointer; editor polls authoritative packed float/generation for all7; stable relay survives synchronized parameter removeListener; message gestures,undo/recall/timer visibility retained. Preserve org.algorhythmia.sys01/Algr/Sys1,seven nativeIDs/keys,four factory programs/order,schema1exactly40,parameter-only deterministic empty-history recall,10ms smoothing,shared256 cap/whole-batch rejection/no replay,eight coherent restores,single audio owner,VST3,stereo effect/generator/disabled input and release scope.

Sprint10 requires immutable prepared resources,nonwaiting audio acquisition,off-audio waits,acknowledged reclamation every writer,safe silence/error invalid concurrent lifecycle,complete zero-prohibited plugin callback instrumentation and passing shipping-format concurrency/lifecycle tests. Warm zeros/finite sanitizer success do not prove whole realtime.

Verify CMake/CTest4.4.4 /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin,format18.1.8 siblingclang_format/data/bin;AppleClang21/macOS26.6.2 arm64. JUCE /private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850,doctest build/_deps/doctest-src,pins cmake/dependencies.lock.json. Native /private/tmp/sys01-s09-repair,sys01-s09-asan,sys01-s09-tsan;core/private/tmp/sys01-s09-session02-core. Reconstruct absent paths from pins/cache/source export. Frozen rpath absent: absolute DYLD_LIBRARY_PATH;never alter frozen bytes. Probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54. Consult handoff for exact final jobs/status and source export hashes.

Only installed Ableton Live12Suite allowed;skip REAPER. Install no testing apps,modify no personal plugin dirs,contact nobody,spawn no subagents,publish nothing/create no tags. auval temporary-path bind needs forbidden install;pluginval absent. ActualDAW-origin,externalOS/architecture,signing,rights,accessibility,human acceptance,performance/soak/MB-04 unqualified. Continue independent authorized work.

Use new evidence/sprint-10-session-07 and unique command IDs;retain actual argv/results/tool/dependency/source/artifact hashes,failures,invalidation. Renew affected strictAU/callbacks/VST3/adapter/recall/editor/core,separateASan/UBSan/TSan including generated wrappers/first-thread/lifecycle. Ownership switches defaultOFF;verify hooks/probe absent final rebuilt defaults. Before ending update compact rolling-state.md,save handoffs/sprint-10-session-07.md using handoff-template.md,retain exact tested checkpoint/remaining gates/next action and complete copy-paste following-session prompt. If incomplete continue Sprint10.
```
