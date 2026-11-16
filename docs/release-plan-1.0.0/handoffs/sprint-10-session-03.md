# SYS 01 Sprint 10 session 03 handoff

## Checkpoint

2026-10-19 18:23 America/Santiago (21:23 UTC). Sprint/session10/03.
Objective S10-VST3-SUPPRESS-01: first-audio-thread VST3 notification suppression
and shared component/controller ownership. **LOCALLY VERIFIED at the bounded
single-process-owner / serialized-VST3-UI publication boundary. Sprint10 IN PROGRESS.**
Release remains NO-GO/development-unqualified. Sprint09/S09-OWN-01 COMPLETE AT
THEIR NAMED LOCAL BOUNDARY. S10-NOTIFY-01/S10-AU-SUPPRESS-01 locally verified.
G01 whole native realtime, actual DAW-origin/external AU qualification and MB-04
DC remain open. No Sprint11 advancement or gate waiver.

Git root /Users/lhommeduchili/algorhythmia, SYS01 root
/Users/lhommeduchili/algorhythmia/sys-01; HEAD unchanged
5c45cf6bed431317a1b412b1fc12ee62b66cfdd5. No applicable AGENTS.md. All139 source
inputs and6 reference inputs matched session02 before edits; session02 export
SHA256e0c4ea85e0c24b49f8dd8deff4896cb29fbce1a7b6a2b7d335305cf8e0563fa5 verified.
Dirty/untracked SYS01 and unrelated monorepo work preserved. No staging/reset/
discard/stash/commit/tag/publish/sub-agent/app installation/personal plugin
mutation/contact. New evidence directory evidence/sprint-10-session-03 retains
command registry argv/cwd/UTC/exit/tool/log hashes,reports,negative bytes,generated
wrappers,caches/compile flags,ON/OFF bundles,source delta/export and inventories.

141-input export source-checkpoint.tar.gz SHA256
b31fda2086096dd42b0cb793d5b687e502752deb32fd2dc79f1fcccf0c199b3c;
resume-inputs.json SHA256
56b0c2166a785a664ced66fc132245b54220306492a365e32c4f771c01f3c90e.
Checkpoint/final-identities/sanitizer-flags/evidence-inventory JSON bind the named
inputs and artifacts. Handoff and compact rolling state are separate reference
files; no circular self-checksum is used.

## Changes and decisions

Two inherited engineering inputs changed: scripts/native_vst3_adapter.py and
apps/desktop/CMakeLists.txt. New apps/desktop/VST3SuppressionTests.cpp and
native-vst3-suppression-session-03.md. Shared processor/DSP/editor/state encoding
and AU generators are unchanged. AU rebuilt identities changed with rebuild;
new reports name exact rebuilt bytes. Both AU and VST3 ownership options and
native RT probe defaultOFF; final three trees are OFF.

Frozen negative VST3 bundle fingerprint
76cf1dc7ca8b42cea1f56a0fa76629a8861591159d8547fe41442a87f5d7eb15;
binary SHA2563dd4788184c634dce661894ba7b74d583cde99758e2051cc593928e1f1381a68.
Two fresh threads allocate1 each at calloc->dyld ThreadLocalVariables->tlv_get_addr
->processParameterChanges. Stopped controller FLUX reenters BIAS native controller
publication: suppressed2 instead of3,leaked2. Independent same-thread instance
write before inner publication observes independent0 instead of1. Thus cross-
instance interference is now reproduced, not just a hypothesis. Actual first
fixture and refined v2 failure,source and bytes were frozen before repair. Original
first fixture cross write after inner reset passed; both reports remain truthful.

Repair stores two fixed lock-free atomic ThreadID lanes in reference-counted
JuceAudioProcessor shared by component/controller. Process lane has one audio
owner; control lane has serialized VST3 UI owner. They can overlap while transport
is stopped (vst3IsPlaying follows ProcessContext, not setProcessing). Controller
Param::setNormalized,component processParameterChanges,setBypassed/private-state
and final listener are all mapped. Stack RAII restores predecessor,including
same-thread nested publications; readers never consume/reset. No native TLS,
dynamic thread registry,heap,global suppression flag,spin/retry,reclamation or
lock deletion. ThreadID accessor is pinned pthread_self on this macOS path.

Controller inSetState is also a scoped atomic thread-identity lane; old plain bool
was read by simultaneous processor callbacks. UI component state is permitted
during Processing by pinned SDK; suppression now applies to that synchronous UI
publisher only. Concurrent independent audio/external notifications pass. Nested
state scope restores predecessor; recursive SYS01 restore under stateMutex is
not asserted safe. Suppression does not repair invalid concurrent role/lifecycle
writers. Controller removeListener and shared ref-counted lifetime still require
legal callback exclusion. Detailed all-writer map in ownership record.

No ingress held over reentrant listeners and no listener/callback/state/FL lock
removed. Stable editor relay retained: off-message atomic dirty,no payload/UI
pointer/OS posts; authoritative seven-word polling; relay survives synchronized
removeListener. Gestures/undo/recall/visibility unchanged. Preserved
org.algorhythmia.sys01/Algr/Sys1,seven keys/native IDs,four programs/order,schema1
exactly40,parameter-only deterministic empty-history recall,10ms smoothing,
shared256 whole-batch rejection/no replay,eight restores,single audio owner,
VST3 and stereo effect/generator/disabled-input behavior/release scope.
No product decision required for bounded repair. External gates remain pending.

## Verification

| Check | Command registry | Exit/result | Boundary |
|---|---|---|---|
| Inputs/tools/pins | initial-state.json,s10s03-tools/ctest-version/format-version/compiler/platform/juce-pin/doctest-pin |139+6 match,pinned4377/270 files,CMake/CTest4.4.4,format18.1.8 | Before repair |
| Frozen actual failures | s10s03-negative/negative-v2 |exit1,freshalloc1,nested2/3,leaked2,cross0/1 | Exact retained Release VST3 |
| Focused expanded repair | s10s03-repair-probe/expanded-probe |exit0 freshalloc/free0/0 mutex3,nested3/3,cross1/1,overlap2/1/1,concurrent128/128 native+external,state7/2/1 | Complete64-frame dedicated diagnostic entry; fixture barriers outside measured segment |
| Whole entries | s10s03-default-vst3-probe/default-au-probe |exit0 VST3fresh1024 alloc0/mutex9,AUfresh1 alloc0/mutex9;hostileAUmutex603 remains | Final default Release; characterization, not G01 acceptance |
| HookON | s10s03-repair/asan/tsan-on-authorized-verify |each exit0 suppression AU/VST3,S09 ownership,strict1274,callbacks,VST3,seals,3/3 | Separate Release/ASan+UBSan/TSan generated wrappers |
| HookOFF | s10s03-repair/asan/tsan-off-authorized-verify |each exit0 strict1274,callbacks,VST3,seals,adapter/recall/VST3 3/3 | Final default bytes |
| Notifications/editor | s10s03-repair/asan/tsan-notifications/editor |each exit0,nested/direct7/64attach-detach;Release3072/four scales;sanitizer gestures/undo/recall/closePASS | Finite local UI/lifetime checks |
| Core/format/generators | s10s03-core/format/final-format/generation |16/16,formatPASS,drift3 rejected | Core sources unchanged; checksum-pinned generators |
| Frozen rechecks | s10s03-negative-recheck/negative-authorized-recheck |exit1,original TLS/nested/cross failure | Frozen original bundle,unchanged bytes |
| Ownership/integrity | s10s03-ownership-trace/checkpoint/inventory |PASS141 inputs,hooks/probe absent,sanitizer flags verified,historical291/505/357/364 unchanged | Exact source,report,artifact and historical integrity |

Final default bundle fingerprints:

| Tree | AU | VST3 |
|---|---|---|
|Release|a921b18f2b5deac90437365e46e2d7f18b31e32fc092e299d16c0288a3582347|e3c6c8d7bb83ec1acc1d062499b9d8ab6d0ad9a9233390cb12d74a8bba327faf|
|ASan/UBSan|7733f34b755543a3a8c54bc57791bbcb30c179b190cc879dc29b6e163b0e335d|1f9ff167fe015fb4a7f054a71166e2e162a4dd8f615fd8fed92af78593ff2147|
|TSan|a3c9a3f5333e1a9d9337fcd0c47cc7a992466ff7264d83adbb32f26b65d09b6c|3f20a91f5c24ec8762d9f4709958151269ad896f05727f8f3283d6bf5921af0e|

No observed final assertion/sanitizer/race failure in these finite runs. Generated
AU/SDK/VST3 and both suppression fixture sanitizer flags verified; dispatch is
includedSDK. No diagnostic symbols/strings/probe dylib linkage in final defaults.
Ad-hoc sealing is local development integrity, not production signing.
CMake/CTest4.4.4 at
/Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin;
clang-format18.1.8 same archive/site-packages/clang_format/data/bin;
AppleClang21/macOS26.6.2/25G83 arm64. JUCE8.0.12
/private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850 treeSHA256
34954713e005f05801972439db07a78356b36bd421b51f2957b3e38605022f85;
doctest2.4.12 build/_deps/doctest-src treeSHA256
5778f85f9828756663bd8271eb7462ef9390f72a1cb92f5c1c50fff8d20fb90a.

Retained initial Release/TSan matrix AU registration failures are sandbox failures,
not tested plugin results; explicitly authorized temporary in-process retries pass.
No installation. First repair build failed private member access; public accessor
fix rebuilt successfully. Redundant authorized frozen recheck followed an incorrect
inference that exit1 meant load failure; actual logs in both runs reproduce the
original negative. No evidence reclassified or erased. A sandbox ps inventory
attempt was denied; every known launched job has completed through its tool session.
Warm zeros and finite sanitation do not accept whole realtime. Historical Sprint08
live-source comparison intentionally invalidated; old failures and seals untouched.

VST3 listener/state changes invalidate affected native/host/lifecycle/performance/
soak/signing proof; local affected checks renewed,new bytes only. auval cannot bind
a temporary AU by path without forbidden installation; pluginval absent at PATH/
recorded paths. Installed Ableton Live12 Suite confirmed,actual DAW-origin tests
unperformed. REAPER skipped. External architecture/OS/signing/rights/accessibility/
humans/listening/performance/soak and MB-04 unqualified.

## Resume

Immediate next action: session04 AU host notification/listener ownership. Trace
scripts/native_au_adapter.py and generated Sys01AU.mm sendAUEvent1452/shared
auEvent1457-1459/RestoreState955 restoringState/bypass parameterValueChanged1498
PropertyChanged1499; pinned AUBase.cpp944-984 mutable property-listener vector
add/remove/dispatch. Establish a deterministic loaded-AU failure or prohibited
plugin work and freeze exact negative before the next bounded repair.
PropertyChanged directly walks host-controlled listener count; AddPropertyListener
reserves/pushes,RemovePropertyListener erases. AudioProcessorChangedUpdater2198
explicitly notes possible allocation/AsyncUpdater. Current shared auEvent and
non-atomic restoringState are trace findings, not new reproduced defects.

APVTS ParameterAdapter148/LockedListeners185,regular/final processor listeners,
CachedParamValues,arbitrary reentry and lifecycle resources follow. Do not remove
locks without ownership/reclamation proof for every writer. Sprint10 still needs
immutable prepared resources,nonwaiting audio acquisition,off-audio waits,
acknowledged reclamation,safe silence/error for invalid concurrent lifecycle,
complete zero-prohibited plugin callback instrumentation and concurrency/lifecycle
checks on shipping formats. Stay Sprint10,external proof laterSprint13.

No background job to resume; all known jobs completed. Native trees
/private/tmp/sys01-s09-repair,sys01-s09-asan,sys01-s09-tsan final hooksOFF/probeOFF;
core /private/tmp/sys01-s09-session02-core. Reconstruct absent temp paths from
retained pins/cache/export. Frozen probe rpath points repair/apps/desktop;
use DYLD_LIBRARY_PATH=<absolute session03 evidence> if absent,never modify bytes.
Only installed Ableton,no testing apps/install/personal plugin changes/contact/
sub-agents/publish/tags. Continue independent local implementation when external
fixtures unavailable. No AU timing or other gate waiver.

## Complete next session prompt

```text
Work in /Users/lhommeduchili/algorhythmia/sys-01. Continue SYS 01 v1.0.0 Sprint 10, session 04: AU host notification and listener ownership. This is an implementation session. Reproduce one concrete prohibited plugin-controlled operation or notification defect, freeze actual failure and exact artifact identity before repair, implement one coherent bounded change and verify it. Stay Sprint10 until named exit criteria are met.

Read docs/release-plan-1.0.0/rolling-state.md; handoffs/sprint-10-session-03.md; native-vst3-suppression-session-03.md; native-suppression-session-02.md; native-notifications-session-01.md; au-ingress-ownership-session-03.md; docs/release-1.0.0/native-ownership.md; Sprint10/acceptance/evidence-invalidation/handoff sections of docs/release-plan-1.0.0/audit-production-plan-2026-10-18.md. Check Git status/applicable AGENTS. Before editing compare live against evidence/sprint-10-session-03/resume-inputs.json and resume-reference-inputs.json; resolve intervening user changes.

HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5; Git root /Users/lhommeduchili/algorhythmia. Preserve substantial dirty/untracked SYS01 and unrelated monorepo work. No bulk staging/reset/discard/stash/overwrite/commit/tag/publish/sub-agents/app installation/personal plugin mutation/contact. Only installed Ableton allowed; skip REAPER.

Sprint09/S09-OWN-01 complete at named local boundary. S10-NOTIFY-01,S10-AU-SUPPRESS-01,S10-VST3-SUPPRESS-01 locally verified at bounded ownership boundaries; Sprint10 IN PROGRESS; Release NO-GO/development-unqualified. G01 whole native realtime, actual DAW-origin/external AU qualification and MB-04 DC remain open.

Session03 replaced VST3 TLS with two fixed shared-processor lock-free atomic ThreadID lanes: single process owner and serialized VST3 UI control owner can overlap. RAII stack predecessor restores nesting; final listener reads both without consuming. Controller state bool now a separate atomic identity lane, so UI restore only suppresses its synchronous publishing thread. Component/controller reference-counted shared lifetime and all native writers mapped in ownership record. Invalid concurrent same-role writers/lifecycle remain unresolved; no lock removed, no TLS/registry/heap/global suppression/reclamation introduced. Do not blindly extend legal-role assumptions to arbitrary writers.

Frozen VST3 negative fingerprint76cf1dc7ca8b42cea1f56a0fa76629a8861591159d8547fe41442a87f5d7eb15; binary SHA2563dd4788184c634dce661894ba7b74d583cde99758e2051cc593928e1f1381a68. It still reproduces fresh-thread calloc->dyld TLS->processParameterChanges alloc1, nested suppressed2 expected3/leaked2 and same-thread cross-instance independent0 expected1. Frozen fixture v2 and actual failures retained. Probe SHA25659af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54. Frozen LC_RPATH points /private/tmp/sys01-s09-repair/apps/desktop; if absent use DYLD_LIBRARY_PATH=<absolute session03 evidence>; never alter frozen bytes.

Start scripts/native_au_adapter.py/generated Sys01AU.mm: sendAUEvent1452 writes shared auEvent1457-1459 then AUEventListenerNotify; RestoreState955 scopes non-atomic restoringState, read by sendAUEvent1454/bypass parameterValueChanged1498. Bypass calls SDK PropertyChanged1499 during native publication. Pinned AUBase.cpp944-984 adds/removes/walks mutable mPropertyListeners vector; add reserves/pushes, PropertyChanged invokes host callbacks. AudioProcessorChangedUpdater2198 explicitly notes PropertyChanged may allocate and uses AsyncUpdater. These are trace findings, not yet reproduced new defects. Instrument/reproduce exact plugin-controlled work, distinguish host/OS callback implementations, and map registry writers/lifetime before repair. APVTS ParameterAdapter148/LockedListeners185, regular/final processor listener synchronization, arbitrary reentry and prepared resources follow as needed. Never hold ingress across reentrant listeners or remove locks without every-writer ownership/reclamation proof.

Preserve session01 stable editor relay: off-message only atomic dirty, no payload/UI pointer/OS post; editor polls seven authoritative packed float/generation words. Relay lives until synchronized removeListener returns. Gestures/undo/recall/visibility retained. Preserve org.algorhythmia.sys01/Algr/Sys1, seven keys/nativeIDs, four programs/order, schema1 exactly40, deterministic parameter-only empty-history recall,10ms smoothing, shared256 whole-batch rejection/no replay,eight restores,single audio owner,VST3,stereo effect/generator/disabled input and release scope.

Session03 Release,separate ASan/UBSan,TSan hookON/OFF matrices all pass. Focused VST3 fresh alloc/free0/0 mutex3; nested3/3,cross1/1,overlap2/1/1,concurrent native128/128 external128/128,state7/2/1 pass. Final default complete VST3 fresh1024 alloc0/mutex9,AU fresh1 alloc0/mutex9; hostile AU mutex603 remains. StrictAU1274/callbacks/VST3/seals/adapter-recall-VST3 3/3 each; notifications nested/direct7/64 attach-detach; Release editor3072/four scales; sanitizer gesture/undo/recall/close; core16/16; format/drift3 pass. No observed final assertion/sanitizer/race failure in finite runs. Generated AU/SDK/VST3 and both suppression fixture flags verified; hooks/probe absent final default binaries. Original failures/seals/historical inventories291/505/357/364 unchanged; Sprint08 live-source comparison intentionally invalidated, never reseal.

Verify tools/pins before use: CMake/CTest4.4.4 in /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin; clang-format18.1.8 same archive/site-packages/clang_format/data/bin; JUCE /private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850; doctest build/_deps/doctest-src; cmake/dependencies.lock.json. Native /private/tmp/sys01-s09-repair,sys01-s09-asan,sys01-s09-tsan final hooksOFF/probeOFF; core /private/tmp/sys01-s09-session02-core. All jobs finished, none to resume. Reconstruct absent paths from retained pins/cache/export.

141-input source export evidence/sprint-10-session-03/source-checkpoint.tar.gz SHA256b31fda2086096dd42b0cb793d5b687e502752deb32fd2dc79f1fcccf0c199b3c; resume-inputs SHA25656b0c2166a785a664ced66fc132245b54220306492a365e32c4f771c01f3c90e. Final Release AUa921b18f2b5deac90437365e46e2d7f18b31e32fc092e299d16c0288a3582347,VST3e3c6c8d7bb83ec1acc1d062499b9d8ab6d0ad9a9233390cb12d74a8bba327faf. Separate sanitizer identities in final-identities.json.

Sprint10 still requires immutable prepared resources/nonwaiting audio acquisition/off-audio waits/acknowledged reclamation for every writer,safe silence/error for invalid concurrent lifecycle,complete plugin-controlled callback zero-prohibited instrumentation and concurrency/lifecycle tests on shipping formats. Warm runs/finite sanitizer passes cannot prove whole realtime safety. Do not advance Sprint11 or waive gates. auval cannot bind temporary AU by path without forbidden installation; pluginval absent. External DAW timing/architecture/OS/signing/rights/accessibility/humans/performance/soak remain unqualified. Continue independent authorized local work.

Use new evidence/sprint-10-session-04 with unique command IDs; retain commands/results/tool/dependency/source/artifact hashes,actual failures and invalidation. Renew affected strictAU/callbacks/VST3/adapter/recall/editor/core and separate sanitizer checks including generated wrappers/first-thread/lifecycle. AU/VST3 ownership switches defaultOFF; verify hooks/probe absent final defaults. Before ending update compact rolling-state and save handoffs/sprint-10-session-04.md from handoff-template.md,retain exact checkpoint/next action and provide complete copy-paste next prompt. If incomplete continue Sprint10.
```
