# SYS 01 Sprint 10 session 02 handoff

## Checkpoint

2026-10-19 17:03 America/Santiago (20:03 UTC). Sprint/session 10/02. Objective
S10-AU-SUPPRESS-01: remove first-audio-thread AU suppression allocation and
consumable nested/stale suppression at a bounded legal lifecycle boundary.
Status: LOCALLY VERIFIED bounded repair; Sprint 10 IN PROGRESS.
Release remains NO-GO/development-unqualified. Sprint09/S09-OWN-01 COMPLETE AT
THEIR NAMED LOCAL BOUNDARY; S10-NOTIFY-01 locally verified. G01 whole native
realtime, actual DAW-origin/external AU timing, external/human/platform gates
and MB-04 DC remain open. No Sprint 11 advancement or gate waiver.

Git root /Users/lhommeduchili/algorhythmia; SYS01 root
/Users/lhommeduchili/algorhythmia/sys-01; HEAD unchanged
5c45cf6bed431317a1b412b1fc12ee62b66cfdd5. No applicable AGENTS.md. All 137 source
inputs and five reference inputs matched session01 before editing. Source export
SHA2560200e38605be19c0ddf6f56693e681dc81a6efafd4942513264bbb2e1d580f63 verified.
Substantial dirty/untracked SYS01/unrelated monorepo work preserved. No staging,
reset/discard/stash/commit/tag/publish/sub-agent/app installation/personal plugin
directory mutation. New evidence ../evidence/sprint-10-session-02/ has unique
registry IDs, argv/cwd/UTC/exit/log/tool hashes, exact reports/frozen negatives,
wrappers/caches/flags, ON/OFF bundles and source export/delta. Exact 139-input export source-checkpoint.tar.gz SHA256
e0c4ea85e0c24b49f8dd8deff4896cb29fbce1a7b6a2b7d335305cf8e0563fa5;
resume-inputs.json SHA256
d7527c5ddf4b22a178ee7a8ef84fdc5a7819da7a0bc748d9080df965c1e68fbe. Final hashes/inventory are in
checkpoint.json/final-identities.json/evidence-inventory.json.

## Changes and decisions

Two inherited engineering inputs changed: scripts/native_au_adapter.py and
apps/desktop/CMakeLists.txt. New apps/desktop/AUSuppressionTests.cpp and ownership
record native-suppression-session-02.md. Existing editor/processor/DSP/state and
VST3 generator inputs remain unchanged from session01. Fixture is built as a local
diagnostic executable and requires ownership-ON AU; new observer uses the existing
default-OFF ownership switch. Default binaries have no fixture/probe linkage.

Actual frozen negative bundle fingerprint
8ef83849f74ae5b3d9ab3c3ff8d5caaeb7d9aa1ba2d360df9a3e3ac96091637e;
executable SHA2563c4a76ca73ea3386f9b6dbf090b045af52d8f934aab5e8d5ef84cda03a4a473b.
Each fresh audio-thread instance allocates once in operator new inside JuceAU::Render.
Native FLUX notification reenters FLUX/BIAS external setters: final suppression1
(expected3), independent4 (expected2). Failed conditional publication leaves
suppression true, suppressing a following independent BIAS notification. Original
reports/logs, executable, bundle, generated source, generator and fixture source
were frozen before repair. Initial sandbox registration failure is retained;
explicitly authorized in-process run reproduces the actual defect, no installation.

Repair replaces AU ThreadLocalValue<bool> with instance-owned lock-free atomic
publisher ThreadID. RAII scope exchanges current ID, retains predecessor on stack,
restores it on exit, including CAS failure. Three generated sites cover Render,
inactive setter and lifecycle flush. Final processor listener reads without
consuming/resetting. Same-thread nested native/external notifications inherit the
complete scope; other threads/instances pass independently. No historical thread
entries, heap/TLS bootstrap/traversal/reclamation/global flag. pinned macOS thread
ID accessor is pthread_self. Scope identities cannot remain after thread exit.

Legal lifecycle excludes Render from inactive publication/prepare/Reset/Cleanup;
there is one audio owner per instance. Those are the only native suppression
writers. External parameter/undo/restore/editor/APVTS/listener setters only read
suppression in the final listener. Constructor initializes before publication;
destructor requires callbacks excluded. Scope is synchronous and survives reverse
regular-listener reentry until final listener returns. No ingress held over
listeners; no ingress/listener/callback/state/FL lock removed. Existing editor
relay/order/teardown repair retained. Invalid concurrent lifecycle/resource mutation
still needs the prepared-resource gate. Full map: native-suppression-session-02.md.

Preserved org.algorhythmia.sys01/Algr/Sys1/seven keys/nativeIDs/four factory programs
and order/schema1 exactly40/parameter-only deterministic empty-history recall/
10ms smoothing/shared256 whole-batch rejection-no replay/eight restores/single
Render owner/stereo effect-generator-disabledinput/VST3 behavior/release scope.
No product decision required for this bounded repair. External acceptance pending.

## Verification

| Check | Command registry reference | Exit/result | Boundary |
|---|---|---|---|
| Inputs/tools/dependencies | s10s02-initial/juce-pin/doctest-pin/tools |137+5 match;4377+270 files pinned;CMake/CTest4.4.4/format18.1.8 | Before repair |
| Actual negative | s10s02-negative-authorized/authorized-v2 |exit1 freshalloc1 each,nestedFAIL,failedCASFAIL | Exact retained loaded AU / fresh threads |
| Dedicated repair | s10s02-repair-suppression/expanded-probe-verify |exit0 freshalloc/free0/0, nested3/2,CAS0/2,lifecycle2/1,concurrent128/128 | Diagnostic Release; mutex5 remains |
| Complete probes | s10s02-expanded-probe-verify/default-au-probe |AU fresh1frame alloc0/mutex9;VST3 fresh1024 alloc1/mutex9 | Full entry; whole realtime NOT accepted |
| HookON | s10s02-repair/asan/tsan-on-verify |each exit0 suppression native nesting3/1,concurrent128/128,S09 ownership,strict1274,callback,VST3,seals,3/3 | Separate instrumented wrapper trees |
| HookOFF | s10s02-repair/asan/tsan-off-verify |each exit0 strict1274,callbacks,VST3,seals,3/3 | Default bytes |
| Notifications/editor | s10s02-repair/asan/tsan-notifications/editor |each exit0 nested/direct7/64teardowns;Release3072/four scales;sanitizer controlsPASS | Finite local UI/teardown |
| Core/format/generator | s10s02-core/format/final-format/generation |16/16,formatPASS,three drift rejectionsPASS | Unchanged core; pinned generators |
| Frozen negative recheck | s10s02-negative-recheck |exit1 freshalloc1/nestedFAIL/CASFAIL | Frozen bytes, original seals unchanged |
| Checkpoint/integrity | s10s02-checkpoint/inventory |PASS139 inputs;historical291/505/357 unchanged;hooks/probe absent | Tool/log/report/source/artifact hashes and historical preservation |

CMake/CTest4.4.4 directory
/Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin;
clang-format18.1.8 same archive/site-packages/clang_format/data/bin.
AppleClang21/macOS26.6.2/25G83 arm64. JUCE8.0.12 pinned
/private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850,
treeSHA25634954713e005f05801972439db07a78356b36bd421b51f2957b3e38605022f85;
doctest2.4.12 build/_deps/doctest-src treeSHA256
5778f85f9828756663bd8271eb7462ef9390f72a1cb92f5c1c50fff8d20fb90a.

Final default bundle fingerprints:

| Tree | AU | VST3 |
|---|---|---|
|Release|17685079622b7fe15c1bef685353b72383e4794356e3922fd5fdbf8079c1d4bb|1bce64d3aaba3d26ae73e8e9c0ce47f7c745b53fcfb3fa05a4957d12edbd2b74|
|ASan/UBSan|3bf1f348b98bd0b3c52ad5f6a942b08d99aa0e4bd3196e30b8043344dce9ed8f|db18a218082a56dd26d8a93059ca1c24ab334d8d4ca6cf0fb0e8071273539cfa|
|TSan|e46149235cc8add0c1b08c584f27271754ac9da654d4e58ac93483df615d1280|9c778874b1cbcc06676e5dc68fb05c71f5fcd66b0018c8635e75f0aa2203655d|

Tool executables hashed in commands.json. Generated AU/SDK/VST3 plus suppression
fixture sanitizer flags verified; dispatch is included in SDK translation unit.
Ad-hoc seals are local development integrity, not production signing.

Retained failures: first sandbox AU registration failure, then actual authorized
negative on frozen bytes. First repair generation rejected a nonunique constructor
reset anchor before generated output; narrowed to newline-exact anchor, successful
rebuild. Old session01 notification and S09 ownership controls are preserved.
Historical Sprint08 live comparison intentionally invalidated, never resealed.
No accepted assertion-bearing report; earlier session01 relative-path File assertion
remains historical/unaccepted. Finite runs do not prove universal safety.

AU wrapper/listener changes invalidate affected strict AU/callback/concurrency/
sanitation/auval/actual host/performance/soak/signing proof. New reports bind exact
new bytes only. Actual DAW-origin timing/stereo, external architecture/OS, signing,
rights, native readers/humans/listening, performance/soak and MB-04 remain unqualified.
auval cannot bind temp AU by path without prohibited installation; pluginval absent
at prior paths/PATH, no install attempted. Only installed Ableton permitted, no DAW
operation this session; REAPER skipped. No external evidence/decision inferred.

## Resume

Immediate next action: Sprint10 session03 VST3 first-thread suppression. Read
native-suppression-session-02.md VST3 writer notes. Inspect generated/pinned
VST3 static thread_local bool/setValueAndNotifyIfChanged/controller stopped
Param::setNormalized/paramChanged/OwnedParameterListener/component constructor/
setBypassed/state inSetState/processParameterChanges. Full new Release probe
reports/repair-probe-on-vst3.log retains fresh1024 alloc1/mutex9 and
calloc->dyld ThreadLocalVariables->tlv_get_addr. Freeze exact new negative and
verify nested/independent instance/thread notifications before storage replacement;
prove controller/processor lifetime and every writer, not merely Render ownership.

Remaining Sprint10 exit: AU/VST3 APVTS/arbitrary listener and host notification
ownership, safe VST3 suppression, immutable prepared resources/nonwaiting acquisition/
off-audio configuration waits/acknowledged reclamation for EVERY writer before
callback locks replaced, invalid concurrent lifecycle silence/error, complete
plugin-controlled callback zero prohibited ops and available-format concurrency/
lifecycle tests. AU bypass PropertyChanged/shared auEvent/restoringState also need
tracing. External platform proof Sprint13. Stay Sprint10, no gate waiver.

Native trees /private/tmp/sys01-s09-repair,sys01-s09-asan,sys01-s09-tsan;
core /private/tmp/sys01-s09-session02-core. All session jobs completed; no background job to resume, no process terminated.
Final native trees hookOFF/probeOFF. No observed final assertion/ASan/UBSan/TSan
failure in these finite runs. All 40+ unique command logs/report hashes checked.
Reconstruct absent temp paths from source export/pins/cache. Frozen suppression
negative LC_RPATH /private/tmp/sys01-s09-repair/apps/desktop; retained probe dylib
SHA25659af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54 can be
supplied with DYLD_LIBRARY_PATH=<absolute session02 evidence directory>, no byte edits.
Same rule for session01 frozen notification negative. No host/apps/personal plugin
writes/contact/sub-agents/tags/publication authorized. Continue independent local
implementation when external prerequisites unavailable.

## Complete next session prompt

```text
Work in /Users/lhommeduchili/algorhythmia/sys-01. Continue SYS 01 v1.0.0 Sprint 10, session 03: native realtime ownership and notifications. This is an implementation session. Stay on Sprint 10 until its named exit criteria are met. Reproduce a concrete prohibited operation or notification defect, freeze exact failing bytes/report before repair, implement one coherent bounded change, and verify it.

First read docs/release-plan-1.0.0/rolling-state.md, handoffs/sprint-10-session-02.md, native-suppression-session-02.md, native-notifications-session-01.md, au-ingress-ownership-session-03.md, docs/release-1.0.0/native-ownership.md, and Sprint 10/acceptance/evidence-invalidation/handoff sections of audit-production-plan-2026-10-18.md. Check Git status and applicable AGENTS.md. Compare live inputs against evidence/sprint-10-session-02/resume-inputs.json and resume-reference-inputs.json; resolve intervening user changes before editing. Git root /Users/lhommeduchili/algorhythmia, HEAD 5c45cf6bed431317a1b412b1fc12ee62b66cfdd5. Preserve substantial dirty/untracked SYS 01 and unrelated monorepo work. No bulk-stage/reset/discard/stash/overwrite/commit/tag/publication.

Sprint 09/S09-OWN-01 COMPLETE AT THEIR NAMED LOCAL BOUNDARY. S10-NOTIFY-01 and S10-AU-SUPPRESS-01 locally verified only. Sprint 10 IN PROGRESS. Release NO-GO/development-unqualified. G01 whole native realtime, actual DAW-origin/external AU qualification and MB-04 DC remain open.

Session02 froze loaded AU suppression negative bundle fingerprint 8ef83849f74ae5b3d9ab3c3ff8d5caaeb7d9aa1ba2d360df9a3e3ac96091637e and executable SHA256 3c4a76ca73ea3386f9b6dbf090b045af52d8f934aab5e8d5ef84cda03a4a473b. Fresh AU instances each allocated once inside JuceAU::Render. Nested final callback suppression was 1 versus expected3; independent4 versus expected2. Failed conditional publication left TLS flag suppressing an unrelated notification. Keep exact frozen failures, including initial registration failures. Probe dylib SHA256 59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54 retained. Frozen executable LC_RPATH /private/tmp/sys01-s09-repair/apps/desktop; use DYLD_LIBRARY_PATH=<absolute evidence/sprint-10-session-02 directory> if missing, never edit frozen bytes.

Repair is generated in scripts/native_au_adapter.py: per-instance NativeParameterSuppression lock-free atomic current publisher ThreadID with RAII stack predecessor restoration. Three sites cover active Render, inactive SetParameter and lifecycle flush. Final listener reads without consuming/resetting; failed CAS unwinds too. Legal single Render owner/lifecycle exclusion means one native publisher thread per instance; external writers only read suppression. Nested same-thread native/external notifications inherit scope; independent threads/instances pass. No thread table/TLS/global flag/heap/reclamation, no existing lock removed or ingress held over listeners. Invalid concurrent Render/lifecycle resource mutation remains unresolved. AUSuppressionTests.cpp observes actual final listener via default-OFF ownership hooks, tests fresh threads/nested external and native reentry/failed CAS/Reset/inactive/concurrent two instances. Preserve stable editor relay polling all seven authoritative packed words and synchronized teardown.

Next bounded target VST3 suppression. Start scripts/native_vst3_adapter.py and pinned juce_audio_plugin_client_VST3.cpp: static thread_local bool line829, setValueAndNotifyIfChanged line831, InParameterChangedCallbackSetter, controller Param::setNormalized line967 (stopped path), paramChanged line1472/final processor and OwnedParameterListener line1647, component constructor reset line2677, setBypassed line2850, setStateInformation/inSetState line2903 and processParameterChanges line3657. Session02 repair-probe-on-vst3.log still records fresh1024-frame alloc1/mutex9 calloc -> dyld ThreadLocalVariables -> tlv_get_addr. Prove every writer's ownership and shared processor/controller lifetime before storage replacement; do not blindly reuse AU per-instance assumption. Reproduce exact first-thread negative and nested/independent instance/thread behavior before repair. No shared global suppression/nesting flag, no native thread_local substitution that merely moves bootstrap allocation. Keep host/OS callback implementations distinct from plugin-triggered TLS/listener/resource/message work. Then continue APVTS/arbitrary listener ownership and prepared resources as needed.

Sprint10 exit requires reviewed ownership map, complete plugin-controlled callback zero prohibited operations, passing concurrency/lifecycle tests on available shipping formats. Prepared-resource protocol must provide immutable resources, nonwaiting audio acquisition, off-audio configuration waits and acknowledged reclamation for EVERY writer before callback locks are replaced; invalid concurrent lifecycle silence/error. Retain callback, ingress, parameter/listener, state and FL Studio locks until replacement proof. Finite sanitizer/warm probes never prove whole realtime safety. Do not advance Sprint11.

Verify tools/dependency pins before use: CMake/CTest4.4.4 /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin; clang-format18.1.8 same archive/site-packages/clang_format/data/bin; AppleClang21/macOS26.6.2 arm64. JUCE /private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850 (4377 files/tree SHA25634954713e005f05801972439db07a78356b36bd421b51f2957b3e38605022f85); doctest build/_deps/doctest-src (270/treeSHA2565778f85f9828756663bd8271eb7462ef9390f72a1cb92f5c1c50fff8d20fb90a), cmake/dependencies.lock.json. Native trees /private/tmp/sys01-s09-repair,sys01-s09-asan,sys01-s09-tsan; core /private/tmp/sys01-s09-session02-core. Read handoff for exact final status/jobs. Reconstruct absent paths from retained source-checkpoint.tar.gz/pins/cache. Read checkpoint.json/final-identities.json for exact export/resume/default AU/VST3 hashes. Never reseal historical evidence; Sprint08 live comparison intentionally invalidated. Session09 inventories and all frozen controls remain unchanged.

Preserve native IDs org.algorhythmia.sys01/Algr/Sys1/all seven keys/native IDs/four factory programs/order/schema1 exactly40 bytes/parameter-only deterministic empty-history recall/10ms smoothing/shared256-event cap/whole-batch rejection/no replay/eight coherent restore slots/single audio owner/stereo effect-generator-disabledinput/VST3 behavior/release scope. New evidence/sprint-10-session-03 and unique command IDs; freeze old negatives and retain commands/results/tool/dependency/source/artifact hashes. Renew affected strict AU/callbacks/VST3/adapter/recall/editor/core and separate ASan/UBSan/TSan, including generated wrappers and first-thread/lifecycle checks. ALGORHYTHMIA_AU_OWNERSHIP_FIXTURE defaults OFF; verify hooks/probe absent final default AU/VST3 binaries.

Only installed Ableton allowed for current host tests; skip REAPER. Install no testing apps, modify no personal plugin directories, contact nobody, spawn no sub-agents, publish nothing/create no tags. auval cannot bind temporary AU by path without prohibited installation; pluginval absent recorded paths/PATH. Actual DAW-origin timing/external architecture-OS/signing/rights/native accessibility/human acceptance/performance/soak remain unqualified. MB-04 DC unresolved. Waive no AU timing/other gate; continue independent authorized local work when external fixtures unavailable.

Before ending update compact rolling-state.md, save handoffs/sprint-10-session-03.md using handoff-template.md, retain actual commands/results/hashes/failures/invalidation and precise next action, and provide complete copy-paste next-session prompt. If incomplete hand off exact implemented/tested checkpoint and continue Sprint10.
```
