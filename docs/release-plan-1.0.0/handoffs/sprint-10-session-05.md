# SYS 01 Sprint 10 session 05 handoff

## Checkpoint

2026-10-20 08:18 America/Santiago (11:18 UTC). Sprint/session10/05. Objective S10-AU-REGISTRY-01:
AU property-listener registry and callback-context lifetime.
**LOCALLY VERIFIED at bounded reentrant registry and gate-acknowledged
removal boundary. Sprint10 IN PROGRESS. Release NO-GO /
development-unqualified.** Sprint09/S09-OWN-01 complete at named local boundary;
S10-NOTIFY-01/AU-SUPPRESS/VST3-SUPPRESS locally verified. G01 whole native
realtime,actualDAW-origin/externalAU and MB-04 DC open. No Sprint11 or waiver.

Git root /Users/lhommeduchili/algorhythmia; SYS01 root
/Users/lhommeduchili/algorhythmia/sys-01; HEAD unchanged
5c45cf6bed431317a1b412b1fc12ee62b66cfdd5. No applicable AGENTS. All143 source
and10 reference inputs matched session04 before edits. Prior source export
69d015466d6324d90f14fd90974b122ea2ae3ce19f98abdd48e574a63d1b2f5d and resume
548de9ab25223dbe1e3aad2f154bbf0e2c9c64f054b34f6c9d8abf824aa69d7e matched.
Dirty/untracked SYS01 and unrelated monorepo work preserved; no staging/reset/
stash/discard/commit/tag/publication/subagents/contact/install/personal plugin edits.
New session05 evidence retains command argv/cwd/UTC/exits/tool/log hashes, frozen
negative bytes/fixture/source/generated wrapper and reports. Final source export/resume hashes and default identities are in checkpoint.json
and final-identities.json; evidence-inventory.json retains session files. References avoid circular hashes.

## Changes and decisions

Changed inherited inputs scripts/native_au_adapter.py and apps/desktop/CMakeLists.txt.
Added apps/desktop/AUPropertyRegistry.hpp, AURegistryTests.cpp and
[ownership record](../au-property-registry-session-05.md). Shared processor/DSP/
editor/VST3 generator unchanged. Defaults hooks/probeOFF retained.

Actual frozen diagnostic Release AU fingerprint
1a595176adc1a01f79a3519f78a43d5344b20c9b100a8cc7ad45121c3fb900a5;
binarySHA25606aadbb404c388e8911a77a53f48809e106b7b9cfe73ce8602f4226919f4d55a.
Native bypass SetParameter consumed by real Render invokes actual host listener;
that callback removes a later listener through actual AU API. SDK vector erase
makes trailing listener run twice: first/removed/tail/late1/0/2/0 expected1/0/1/0.
Actual failure/source/generated wrapper/cache/fixture/exact bundle frozen before
repair. No synthetic PropertyChanged hook or invented sanitizer failure.

IMPORTANT correction: AUInstanceGuard is optional AUEntryGuard(GetMutex).
mAUMutex=nullptr and pinned JuceAU never calls SetMutex; the guard does not
serialize SYS01 host writers/RestoreState. Session04 guard ownership attribution
is superseded. Its finite tests still prove caller-serialized restore identity
and stack-event ownership, not concurrent same-role RestoreState. Historical
session04 docs/bytes unchanged; new ownership record explicitly corrects claim.

All virtual SDK Add/Remove/PropertyChanged paths now route to instance registry.
SDK vector remains empty; const accessor has no caller; all SDK state/layout/
input/preset/name/LastRenderError/MIDI and updater/bypass readers traced. Immutable
structural snapshots preserve registration order/duplicates with no cap. One
recursive gate protects entries/active flags/publication and synchronous callbacks.
Removal withdraws matches; old snapshot skips removed entries. Writer retains old
storage before publishing; writer-only reclamation requires no active frame
reference. Normal dispatch never last-owner deletes snapshot/nodes.

Cross-thread removal acquires gate after active registry callbacks return; host
may then destroy context. Same-thread reentrant removal cannot wait for its own
active frame: context remains alive until that callback/ancestor returns. Constructor
ContextName callback retains wrapper this; synchronized bypass/processor removal
and registry shutdown precede wrapper context destruction. Host still must exclude
new wrapper entry/disposal inside callback and obey message/editor/resource lifetime.
No arbitrary concurrent wrapper teardown claimed. No ingress across callbacks,
queue/TLS/coalesced gestures/payload retention, dependency edits or removed locks.

This is bounded correctness repair with BLOCKING serialization. Registry still
traverses host-sized callbacks; writer reentry on audio allocates/frees. Arbitrary
host callback waiting on another thread reentering registry, parameter/registry
lock order and AsyncUpdater/preset/message work remain unqualified. Gate cannot
be deleted without complete writer/reader/context replacement ownership. No
nonwaiting/wholeRT acceptance follows from this step. No product approval needed
for observed duplicate-callback repair.

Preserved org.algorhythmia.sys01/Algr/Sys1,seven keys/nativeIDs,four programs/order,
schema1 exactly40,parameter-only deterministic empty-history recall,10ms smoothing,
shared256 whole-batch rejection/no replay,eight restores,single audio owner,VST3,
stereo effect/generator/disabledinput/release scope. Editor stable relay remains
atomic dirty off-message,no retained payload/UI pointer/OS post,seven authoritative
word polling,synchronized removeListener lifetime/gestures/undo/recall/visibility.

## Verification

| Check | Command registry | Exit/result | Boundary |
|---|---|---|---|
| Inputs/tools/pins | initial-state.json,s10s05-*-version/compiler/platform/juce-pin/doctest-pin |143+10 match,CMake/CTest4.4.4,format18.1.8,pinned4377/270|Before repair|
| Actual negative |s10s05-negative-authorized|exit1 tail2 vs1|Frozen loaded Release AU|
| Focused repair |s10s05-repair-focused|exit0 first/removed/tail/late1/0/1/0|Actual AU callback reentry|
| Expanded Release ON/OFF |s10s05-repair-on-verify/off-verify|exit0 registry,strict1274,callbacks,VST3,seals,3/3|New byte identities|
| Core/format/generation |s10s05-core/format-final/generation|16/16,formatPASS,3 drift rejections|Core unchanged|
| Probes |s10s05-registry-probe/default-au-probe/default-vst3-probe|exit0 alloc/free0/0;native bypassmutex7,AUfresh1mutex9/hostile603,VST3fresh1024mutex9|Characterization;G01 open|
| Notifications/editor |s10s05-repair-notifications/editor|exit0 nested/direct7/64detach;3072renders/four scales/controls|Finite local UI|

Release, separate ASan/UBSan and TSan ON/OFF matrices all passed;
s10s05-{repair,asan,tsan}-{on,off}-verify records exact results/bytes.
All generated wrapper/SDK/VST3 and ownership fixture sanitizer flags verified.
Diagnostic hooks/symbols/strings and probe linkage absent all final default binaries.
All notification/editor finite checks passed; Release3072renders/four scales and
separate sanitizer controls/gestures/undo/recall/close pass. No observed final
assertion/sanitizer/race failure in these finite runs. Same registry native fixture
runs against hooksOFF too, including reentrant add257, held cross-thread removal
and128 actual context destruction/fresh-thread cycles. No fixture scheduling waits
are shipping work. Retained compile/setup failures are corrected; sandbox register
failure precedes plugin tests. Actual negative is separate tail2 failure.

CMake/CTest4.4.4 path /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/
lib/python3.14/site-packages/cmake/data/bin;format18.1.8 siblingclang_format/data/bin;
AppleClang21/macOS26.6.2/25G83 arm64. JUCE8.0.12
/private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850,
tree34954713e005f05801972439db07a78356b36bd421b51f2957b3e38605022f85;
doctest2.4.12 build/_deps/doctest-src,
tree5778f85f9828756663bd8271eb7462ef9390f72a1cb92f5c1c50fff8d20fb90a.
Probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.

AU/listener/lifecycle changes invalidate affected callback/sanitizer/host/UI/
performance/soak/signing. New local reports name new bytes. Historical inventories291/505/357/364/409/366 verified unchanged; Sprint08 live-source comparison intentionally invalidated.
Only installed Ableton Live12 Suite available;REAPER skipped. pluginval absent,
auval cannot bind temporary AU without forbidden install. ActualDAW-origin timing,
externalOS/architecture,production signing/rights/accessibility/humans/performance/
soak and MB-04 unqualified. No human/DAW/soak/signing acceptance claimed.

Final146-input source export SHA256 `296d9daba7f6473417e1f092009a4b4bfe1bb19815f7e38a2f9b39e1dba9bb87`;
resume-inputs.json SHA256 `c7dc8bc7da4d31cd747149a64b1e9ec3c599702cbf10b43f71f4f4253ee68b27`. Final default bundle fingerprints:

| Tree | AU | VST3 |
|---|---|---|
|repair|d3f87dc78eb8499a9208d7ed17a1db3b8fc1b7c853fd9902055560bdf658e586|4b3daf8d628cf1d82545f2d22f1a54cd42bcddcce11b74f30dfd55ef7f080a1e|
|asan|2880fc91972bfcf396ba0365194204af9329e1455bb4b97123f9705176be6a21|b70b87d9b0a1fb0722d6b6050b66f47612d4a519251769bd647af599dd07a2f4|
|tsan|b5ad06a24047687326ca5a03230a3bb2a86d41a7a54cdb09f05ee62ce4e80b40|b9b2eb9a1518723efc7c413207d4d0ee19035a22006e464dc39475e05b01654f|

Exact executable hashes in final-identities.json; checkpoints/frozen/source hashes
verified by s10s05-checkpoint. Final source-matching native registry fixture
rebuild/verify s10s05-{repair,asan,tsan}-final-registry-{build,verify} passed too.

## Resume

Immediate next action session06: reproduce remaining registry wait/order or
AudioProcessorChangedUpdater off-message allocation/post on loaded AU, freeze exact
negative before coherent nonwaiting notification/publication/reclamation repair.
Read session05 ownership-trace.json, AUPropertyRegistry.hpp, scripts/native_au_adapter.py,
generated virtual overrides/bypass/updater. Include all SDK callers and context lifetime.
Do not delete gate/cap listeners/coalesce gestures/hold ingress over listeners. Then
APVTS LockedListeners, regular/final listeners,CachedParamValues/arbitrary reentry and
prepared resources. Same-role RestoreState caller serialization remains unqualified.

Sprint10 exit: immutable prepared resources,nonwaiting audio acquisition,off-audio
configuration waits,acknowledged reclamation every writer,safe silence/error invalid
concurrent lifecycle,complete zero-prohibited plugin callbacks and passing shipping
format concurrency/lifecycle checks. Remain Sprint10; external proofSprint13.

Native trees /private/tmp/sys01-s09-repair,sys01-s09-asan,sys01-s09-tsan; core
/private/tmp/sys01-s09-session02-core. All known tool jobs completed;none to resume. Reconstruct missing temporary paths
from retained pins/cache/source export. Frozen rpaths may need absolute
DYLD_LIBRARY_PATH; never alter bytes. Local registration/editor fixture execution
uses approved sandbox escalation; honor future environment restrictions.

## Complete next session prompt

```text
Work in /Users/lhommeduchili/algorhythmia/sys-01.

Continue SYS 01 v1.0.0 Sprint 10, session 06: nonwaiting AU registry/notification ownership. This is an implementation session. Reproduce one concrete remaining prohibited operation or callback-order/lifetime failure on an actual loaded bundle, freeze failure/fixture/source/generated wrapper/exact artifact bytes before repair, implement one coherent bounded change, and verify. Stay Sprint10 until its named exit criteria are met. No Sprint11 advancement or gate waiver.

Read:
1. docs/release-plan-1.0.0/rolling-state.md
2. docs/release-plan-1.0.0/handoffs/sprint-10-session-05.md
3. docs/release-plan-1.0.0/au-property-registry-session-05.md
4. docs/release-plan-1.0.0/au-host-notifications-session-04.md (guard ownership claim superseded by session05)
5. docs/release-plan-1.0.0/native-vst3-suppression-session-03.md
6. docs/release-plan-1.0.0/native-suppression-session-02.md
7. docs/release-plan-1.0.0/native-notifications-session-01.md
8. docs/release-plan-1.0.0/au-ingress-ownership-session-03.md
9. docs/release-1.0.0/native-ownership.md
10. Sprint10, acceptance, evidence-invalidation and handoff sections of docs/release-plan-1.0.0/audit-production-plan-2026-10-18.md.

Check Git status and applicable AGENTS.md. Before edits compare live against docs/release-plan-1.0.0/evidence/sprint-10-session-05/resume-inputs.json and resume-reference-inputs.json; resolve intervening user changes. Git root /Users/lhommeduchili/algorhythmia, recorded HEAD 5c45cf6bed431317a1b412b1fc12ee62b66cfdd5. Preserve substantial dirty/untracked SYS01 implementation and unrelated monorepo work. No bulk-stage/reset/discard/stash/overwrite/commit/tag/publication.

Status: Sprint09/S09-OWN-01 COMPLETE AT NAMED LOCAL BOUNDARY. S10-NOTIFY-01, S10-AU-SUPPRESS-01 and S10-VST3-SUPPRESS-01 locally verified at bounded ownership boundaries. S10-AU-NOTIFY-01 verified only for caller-serialized off-audio restore identity and synchronous per-call event ownership. S10-AU-REGISTRY-01 locally verified for stable reentrant add/remove traversal and gate-acknowledged context removal. Sprint10 IN PROGRESS. Release NO-GO/development-unqualified. G01 whole native realtime, actual DAW-origin/external AU and MB-04 DC open.

Session05 important correction: AUInstanceGuard passes AUBase::GetMutex to optional AUEntryGuard. mAUMutex is nullptr and pinned JuceAU never calls SetMutex. Therefore host dispatch guards do not serialize SYS01 registration/removal or RestoreState. Prior session04 guard attribution is superseded, historical evidence unchanged. Atomic restore lane still assumes serialized same-role caller ownership; concurrent same-role RestoreState/resource/lifecycle remains unqualified.

Session05 frozen loaded-AU negative bundle fingerprint 1a595176adc1a01f79a3519f78a43d5344b20c9b100a8cc7ad45121c3fb900a5; binary SHA256 06aadbb404c388e8911a77a53f48809e106b7b9cfe73ce8602f4226919f4d55a. Native bypass consumed by real Render invokes actual host listener, which removes a later listener through actual AU API. SDK vector erase makes tail callback run twice: first/removed/tail/late1/0/2/0 instead1/0/1/0. Not a synthetic PropertyChanged hook or invented sanitizer failure. Exact fixture/source/generated wrapper/cache/bytes and original actual reports frozen before repair. Initial compile/setup and sandbox registration failures retained separately. Never modify frozen evidence.

Session05 change: scripts/native_au_adapter.py overrides virtual AddPropertyListener, RemovePropertyListener and PropertyChanged to instance-owned apps/desktop/AUPropertyRegistry.hpp. Immutable structural snapshots preserve order/duplicates/no cap. One recursive mutex protects entries, active flags, publication and synchronous callbacks. Remove marks matches inactive; old snapshots skip withdrawn entries. Off-audio writer retains old snapshots before publication and reclaims only when no reader frame owns them. Normal dispatch never performs last-owner reclamation. Cross-thread removal waits for active registry callbacks; after return host may destroy context. Same-thread removal cannot acknowledge its own active frame: context survives until that callback/ancestor returns. Destruction removes synchronized bypass/processor listeners, then registry shutdown while wrapper alive; host must exclude new wrapper entries/disposal within callback and obey message/editor/resource lifetime. Upstream SDK registry stays empty; every pinned reader/writer is virtual and routed, const accessor has no caller. No dependency edits, removed locks, ingress extension, queue/TLS/gesture coalescing/payload retention.

This is correctness repair with blocking serialization, NOT realtime qualification. New gate still waits/traverses host-sized listeners; reentered writers on audio allocate/free. Arbitrary host callback waiting on another thread that reenters registry can deadlock; parameter-lock/registry-lock ordering across arbitrary callback reentry remains unqualified. AudioProcessorChangedUpdater still AsyncUpdater/atomic flags/preset work and can allocate/post off-message. Do not remove gate without a complete all-writer publication and acknowledged reclamation protocol. Quiescent registration and finite sanitizer zeros are not whole ownership proof.

Session05 Release, separate ASan/UBSan and TSan ON/OFF matrices and exact identities/results are in handoff/checkpoint.json/final-identities.json/commands.json. Registry fixture is real native bypass and runs on defaults too: reentrant removal1/0/1/0, add257 preserving outer snapshot2/257, held cross-thread removal1/1/0,128 context-destruction/fresh-thread callbacks. StrictAU1274, callbacks, inherited notification/suppression/ownership, VST3/seals/adapter/recall/editor/core renewed. Diagnostic hooks/probe defaultOFF, absence and generated wrapper/fixture sanitizer flags recorded. Probes characterize blockers: native bypass alloc/free0/0 mutex7; wholeAUfresh1 mutex9,hostile603; VST3fresh1024 mutex9. See actual logs for precise scope and any failures. Historical inventories291/505/357/364/409/session04 unchanged; Sprint08 live-source comparison intentionally invalidated. Source export/resume hashes in checkpoint/handoff; verify before use.

Start with session05 ownership-trace.json, apps/desktop/AUPropertyRegistry.hpp, scripts/native_au_adapter.py and generated Sys01AU.mm virtual registry overrides/bypass/AudioProcessorChangedUpdater. Reproduce remaining blocking lock/order or off-message updater allocation/post with loaded-AU fixture and probe before further repair. Include every SDK virtual caller (state/layout/input/LastRenderError/MIDI), ContextName wrapper-this lifetime, host removal variants, nested callbacks, first-thread and legal teardown. Distinguish plugin-controlled traversal/allocation/locks/messages from host/OS callback bodies. After this bounded step continue APVTS ParameterAdapter/LockedListeners, regular/final processor listeners, CachedParamValues, arbitrary reentry and immutable prepared resources. Never hold ingress across reentrant listeners.

Preserve stable editor relay: off-message only atomic dirty, no retained payloads/OS posts/UI pointer access; polls authoritative packed float/generation words for all7; relay survives synchronized removeListener; message-thread gestures,undo/recall/timer visibility retained. Preserve org.algorhythmia.sys01/Algr/Sys1,seven keys/nativeIDs,four factory programs/order,schema1 exactly40,parameter-only deterministic empty-history recall,10ms smoothing,shared256 cap/whole-batch rejection/no replay,eight coherent restore slots,single audio owner,VST3 behavior,stereo effect/generator/disabled-input and release scope.

Sprint10 requires immutable prepared resources,nonwaiting audio acquisition,off-audio configuration waits,acknowledged reclamation for every writer,safe silence/error invalid concurrent lifecycle,complete zero-prohibited plugin-controlled callback instrumentation and passing concurrency/lifecycle tests on shipping formats. Warm zeros/finite sanitizers do not meet whole realtime exit.

Verify tools/pins before use: CMake/CTest4.4.4 /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin; clang-format18.1.8 siblingclang_format/data/bin; AppleClang21/macOS26.6.2 arm64. JUCE /private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850, doctest build/_deps/doctest-src; cmake/dependencies.lock.json. Native trees /private/tmp/sys01-s09-repair,sys01-s09-asan,sys01-s09-tsan; core /private/tmp/sys01-s09-session02-core. Background completion/status in handoff; reconstruct missing temporary paths from pins/caches/source export. Frozen rpaths may need absolute DYLD_LIBRARY_PATH; never alter frozen bytes. Probe SHA25659af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.

Only installed Ableton Live12 Suite for current host tests;skip REAPER. Install no testing apps,modify no personal plugin directories,contact nobody,spawn no sub-agents,publish nothing,create no tags. auval cannot bind temporary AU by path without forbidden installation;pluginval absent recorded paths/PATH. Actual DAW-origin timing,external architecture/OS,signing,rights,native accessibility,human acceptance,performance/soak and MB-04 remain unqualified. Continue independent local work.

Use new evidence/sprint-10-session-06 and unique command IDs. Retain commands/results/tool/dependency/source/artifact hashes,failures,invalidation. Renew affected strictAU/callbacks/VST3/adapter/recall/editor/core and separate ASan/UBSan/TSan including generated wrappers and first-thread/lifecycle fixtures. Ownership switches defaultOFF;verify hooks/probe absent final rebuilt defaults. Before ending update compact rolling-state.md and handoffs/sprint-10-session-06.md using handoff-template.md with precise checkpoint/remaining gates/next action and complete copy-paste following-session prompt. If incomplete, hand off exact implemented/tested checkpoint and continue Sprint10.
```
