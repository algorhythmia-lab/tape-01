# SYS 01 Sprint 10 session 04 handoff

## Checkpoint

2026-10-19 19:50 America/Santiago (22:50 UTC). Sprint/session10/04.
Objective S10-AU-NOTIFY-01: AU host notification state and event payload ownership.
**LOCALLY VERIFIED at the serialized off-audio restore / synchronous per-call event
boundary. Sprint10 IN PROGRESS. Release NO-GO/development-unqualified.**
Sprint09/S09-OWN-01 COMPLETE AT NAMED LOCAL BOUNDARY; S10-NOTIFY-01,
S10-AU-SUPPRESS-01 and S10-VST3-SUPPRESS-01 locally verified. Whole native G01,
actual DAW-origin/external AU qualification and MB-04 DC remain open. No gate waiver.

Git root /Users/lhommeduchili/algorhythmia; SYS01 root
/Users/lhommeduchili/algorhythmia/sys-01; HEAD unchanged
5c45cf6bed431317a1b412b1fc12ee62b66cfdd5. No applicable AGENTS.md. All141 source
and9 reference inputs matched session03 before edits. Exact prior export/resume
hashes verified. Substantial dirty/untracked SYS01 and unrelated work preserved;
no stage/reset/discard/stash/commit/tag/publication/sub-agents/contact/app install/
personal plugin-directory mutation. Session evidence retains unique argv/cwd/UTC/
exit/tool/log hashes,source/artifacts/generated wrappers/caches/flags,frozen failures,
reports,source delta/export and invalidation. All known tool jobs completed.

143-input source export SHA256 `69d015466d6324d90f14fd90974b122ea2ae3ce19f98abdd48e574a63d1b2f5d`;
resume-inputs.json SHA256 `548de9ab25223dbe1e3aad2f154bbf0e2c9c64f054b34f6c9d8abf824aa69d7e`. Handoff/rolling are
separate reference hashes, avoiding circular checksums. Final byte identities in
final-identities.json; inventory and checkpoint verification retained.

## Changes and decisions

Changed inherited inputs: scripts/native_au_adapter.py and apps/desktop/CMakeLists.txt.
Added apps/desktop/AUNotificationTests.cpp and
[ownership map](../au-host-notifications-session-04.md). Shared processor/DSP/editor/
VST3 generator unchanged. Default ownership switches/probe remain OFF.

Frozen actual diagnostic AU fingerprint `d3e86a4d481c2f56a37af30938e6a21fc1bf971eff07f522386664f3c83c68f1`;
binary SHA256 `008da982038b23705ee396e26cf0acd8aafc3c133f5393b3317f52bb248805eb`. During real ClassInfo restore, fixture
FLUX regular listener pauses the restore thread; independent BIAS/BYPASS writer
sends events0/2,bypass property callbacks0/1,other-instance1/1. After restore2/1
passes. Refined fixture reenters BIAS at outer FLUX event boundary: shared event
intact0/1,events2. Both fixture versions,actual failure reports,source and exact
bundle are frozen before repair. This is deterministic forced synchronous reentry
at actual plugin submission boundary,not a claim macOS itself reentered.

Repair makes restoringState an independent fixed lock-free atomic ThreadID lane,
scoped by stack predecessor restoration. SDK ClassInfo dispatch serializes its
sole off-audio publisher with AUInstanceGuard. Readers suppress only synchronous
callbacks on that publishing thread; independent notifications pass. Native
suppression lane stays separate and unchanged. Every sendAUEvent constructs its
own zero-initialized stack event,including unit/scope/element/type/nativeID; nested
or concurrent events share no writable payload. No TLS/heap/global flag/queue/
replay/retry/lock removal/ingress extension. Recursive SYS01 restore under stateMutex,
invalid concurrent same-role writers and resource/lifetime mutation remain unqualified.

Registry map proves AUBase.cpp944 reserve/push,961 erase,977 entire-vector dispatch,
AUBase.h716 construction/destruction ownership and no const accessor caller. Host
add/remove/SetProperty use instance guard; processor bypass/updater dispatch does
not. Constructor ContextName listener uses wrapper this. SDK state/layout/input/
LastRenderError readers also participate. Registry mutation/reentry and callback
context reclamation remain unresolved; quiescent64-cycle checks do not qualify
concurrent removal. AsyncUpdater flags/message lifetime remain unresolved. Distinguish
plugin SDK traversal/allocation/posting from host callback implementation/macOS
AUEventListenerNotify registry. OS header CA_REALTIME_API is not whole-RT proof.

Preserved org.algorhythmia.sys01/Algr/Sys1,seven keys/nativeIDs,four programs/order,
schema1 exactly40,deterministic parameter-only empty-history recall,10ms smoothing,
shared256 whole-batch rejection/no replay,eight restores,single audio owner,VST3,
stereo effect/generator/disabledinput and release scope. Stable editor relay retains
off-message atomic dirty,no payload/UI pointer/OS posts,seven authoritative word
polling,synchronized removeListener lifetime,gestures/undo/recall/timer visibility.
No product decision required for this bounded repair.

## Verification

| Check | Command registry reference | Exit/result | Boundary |
|---|---|---|---|
| Inputs/tools/pins | initial-state.json,s10s04-tools/ctest-version/format-version/compiler/platform/juce-pin/doctest-pin |141+9 match,CMake/CTest4.4.4,format18.1.8,pinned4377/270 | Before repair |
| Actual failure | s10s04-negative-authorized/negative-v2 |exit1 restore0/2+0/1,nested intact0/1 | Frozen loaded diagnostic Release AU |
| Focused repair | s10s04-repair-focused |exit0 restore2/1/1,after2/1,nested1/2 | Actual event submission + real SDK property callback |
| Expanded hookON | s10s04-repair/asan/tsan-on-authorized-verify |each exit0 restore2/1/1,nested1/2,seven-key128 value/begin/end,64 property detach,suppression/ownership,strict1274,callbacks,VST3,seals,3/3 | Separate generated-wrapper builds |
| Final hookOFF | s10s04-repair/asan/tsan-off-authorized-verify |each exit0 strict1274,callbacks,VST3,seals,adapter/recall/VST3 3/3 | Exact rebuilt defaults |
| Notifications/editor | s10s04-repair/asan/tsan-notifications/editor |all exit0 nested/direct7/64detach;Release3072/four scales;sanitizer gestures/undo/recall/close | Finite local UI/lifetime |
| Probes | s10s04-focused-probe/default-au-probe/default-vst3-probe |exit0 external bypassalloc/free0/0 mutex2;AUfresh1 alloc0/mutex9;VST3fresh1024 alloc0/mutex9;hostileAU603 | Characterization,not G01 acceptance |
| Core/format/generators | s10s04-core/format/generation |16/16,formatPASS,drift3 rejected | Core sources unchanged |
| Integrity/ownership | s10s04-ownership-trace/checkpoint/inventory |143inputs,flags/hooks/probe/frozen/historical checks PASS | Histories291/505/357/364/409 unchanged |

Final default bundle fingerprints:

| Tree | AU | VST3 |
|---|---|---|
|Release|9eac0d1b9fe7ec23bb8716a62d6e835327a671f0a5a689c5d097d8add30e7459|e3c6c8d7bb83ec1acc1d062499b9d8ab6d0ad9a9233390cb12d74a8bba327faf|
|ASan/UBSan|1d8412c654edfeff195073eb2f41957c9105c3be322cf644914a7af84c1100ef|c54a61f23d7cf6ded5c53d953c05c0567e7091c4cdd2da08fd9594f81bfa37b2|
|TSan|6f802535d57e5f7db7425b4d1d47675bff170d95fca2c8732551292fb23cede4|87d947f05f72020852b965f2c1a69e6b58903f45687df611a4ec6371e8edf807|

Final generated AU/SDK/VST3 and AU notification/AU+VST3 suppression fixture flags
verified. SDK includes generated dispatch. Diagnostic hooks/symbols/strings and
probe linkage absent defaults. No observed final assertion/sanitizer/race failure
in finite runs. Ad-hoc seals are local development integrity,not production signing.

CMake/CTest4.4.4 at /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/
lib/python3.14/site-packages/cmake/data/bin;format18.1.8 siblingclang_format/data/bin;
AppleClang21/macOS26.6.2/25G83 arm64. JUCE8.0.12 at
/private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850,
tree34954713e005f05801972439db07a78356b36bd421b51f2957b3e38605022f85;
doctest2.4.12 build/_deps/doctest-src,
tree5778f85f9828756663bd8271eb7462ef9390f72a1cb92f5c1c50fff8d20fb90a.
Probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.

Retained initial sandbox failure is registration before tests,not plugin failure.
Explicitly authorized temporary in-process retries reproduce failures/pass matrices.
First repair build failed unknown type NativeParameterSuppression because member
preceded type definition; declaration moved beside defined type,successful rebuild.
No frozen bytes or original failure reports changed. Historical Sprint08 live-source
comparison intentionally invalidated. AU notification/state changes invalidate
native/host/lifecycle/performance/soak/signing evidence; affected local checks renewed,
new bytes only. Whole entry still has prohibited locks/registry/message work.

Only installed Ableton Live12 Suite allowed. REAPER skipped;pluginval absent at
recorded paths/PATH;auval cannot bind temporary AU by path without forbidden
installation. Actual DAW-origin AU timing,externalOS/architecture,production signing/
rights,accessibility/humans,performance/soak and MB-04 remain unqualified. No timing,
paint-budget,universal sanitizer or whole-realtime acceptance claimed.

## Resume

Immediate next action: session05 AU property-listener registry and callback ownership.
Read ownership-trace.json and au-host-notifications-session-04.md; start
scripts/native_au_adapter.py/generated Sys01AU.mm bypass PropertyChanged1517,
AudioProcessorChangedUpdater2216,pinned AUBase.cpp944-984 and all SDK virtual
PropertyChanged callers. Reproduce native bypass host-count traversal or reentrant
add/remove deterministically on loaded AU,freeze actual negative/source/artifact
before a bounded repair. Do not remove locks or cap listeners without complete
writer/reader/context lifetime publication and acknowledged reclamation proof.
APVTS ParameterAdapter/LockedListeners,regular/final processor listeners,
CachedParamValues,arbitrary reentry and prepared resources follow.

Sprint10 exit still requires immutable prepared resources,nonwaiting audio acquisition,
off-audio configuration waits,acknowledged reclamation for every writer,silence/error
for invalid concurrent lifecycle,complete zero-prohibited plugin callback instrumentation
and passing concurrency/lifecycle tests on shipping formats. Stay Sprint10.

Trees /private/tmp/sys01-s09-repair,/private/tmp/sys01-s09-asan,/private/tmp/sys01-s09-tsan
are final hooksOFF/probeOFF;core/private/tmp/sys01-s09-session02-core.
All known jobs completed through tool sessions;none to resume. Reconstruct missing
temporary paths from retained pins/cache/source export. Frozen fixture/probe rpaths
may use previous temp/evidence paths;use absolute DYLD_LIBRARY_PATH if missing,
never modify frozen bytes. Current local registration/native fixture execution needed
sandbox escalation;future environment restrictions must be honored.

## Complete next session prompt

```text
Work in /Users/lhommeduchili/algorhythmia/sys-01.

Continue SYS 01 v1.0.0 Sprint 10, session 05: AU property-listener registry and callback ownership. This is an implementation session. Reproduce a concrete prohibited plugin-controlled operation or notification/lifetime defect, freeze actual failure and exact artifact identity before repair, implement one coherent bounded change, and verify it. Remain on Sprint10 until its named exit criteria are met; no Sprint11 advancement or gate waiver.

Read and follow:
1. docs/release-plan-1.0.0/rolling-state.md
2. docs/release-plan-1.0.0/handoffs/sprint-10-session-04.md
3. docs/release-plan-1.0.0/au-host-notifications-session-04.md
4. docs/release-plan-1.0.0/native-vst3-suppression-session-03.md
5. docs/release-plan-1.0.0/native-suppression-session-02.md
6. docs/release-plan-1.0.0/native-notifications-session-01.md
7. docs/release-plan-1.0.0/au-ingress-ownership-session-03.md
8. docs/release-1.0.0/native-ownership.md
9. Sprint10, acceptance, evidence-invalidation and handoff sections of docs/release-plan-1.0.0/audit-production-plan-2026-10-18.md.

Check Git status and applicable AGENTS.md. Before editing compare live inputs against evidence/sprint-10-session-04/resume-inputs.json and resume-reference-inputs.json under docs/release-plan-1.0.0. Resolve intervening user changes. Git root /Users/lhommeduchili/algorhythmia; HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5. Preserve substantial dirty/untracked SYS01 and unrelated monorepo work. Do not bulk-stage,reset,discard,stash or overwrite existing work.

Status: Sprint09/S09-OWN-01 COMPLETE AT NAMED LOCAL BOUNDARY. S10-NOTIFY-01/S10-AU-SUPPRESS-01/S10-VST3-SUPPRESS-01 locally verified at bounded ownership boundaries. S10-AU-NOTIFY-01 locally verified only for serialized off-audio restore identity and per-call event payload ownership. Sprint10 IN PROGRESS; Release NO-GO/development-unqualified. G01 whole native realtime, actual DAW-origin/external AU qualification and MB-04 DC remain open.

Session04 changed scripts/native_au_adapter.py and apps/desktop/CMakeLists.txt, added AUNotificationTests.cpp and au-host-notifications-session-04.md. Shared processor/DSP/editor/VST3 generator unchanged. RestoreState now scopes its own fixed lock-free atomic ThreadID lane; independent threads are not suppressed. Every sendAUEvent constructs zero-initialized stack event with exact unit/scope/element/type/native ID. No shared mutable event storage,TLS,queue,heap,global flag,retry or lock removal. Serialized RestoreState ownership comes from SDK ClassInfo AUInstanceGuard; recursive SYS01 restore under stateMutex and invalid concurrent same-role writers remain unqualified. Native suppression lane is independent and unchanged.

Frozen session04 diagnostic AU bundle fingerprint d3e86a4d481c2f56a37af30938e6a21fc1bf971eff07f522386664f3c83c68f1. Exact binary SHA256 in negative-identities.json. Retained authorized loaded-AU failure: during paused real ClassInfo restore external BIAS/BYPASS sends events0/2 and bypass property callbacks0/1,other-instance1/1; after restore2/1 pass. Refined frozen fixture reenters BIAS at outer FLUX notification boundary: event intact0/1,events2. This is forced synchronous reentry at real plugin submission boundary,not proof macOS itself reentered. Original sandbox run failed registration before plugin tests; actual failures and both fixture versions frozen before repair. Never modify frozen bytes.

Session04 Release,separate ASan/UBSan and TSan ON/OFF matrices pass strictAU1274,callbacks,VST3,seals,adapter/recall/VST3 3/3. ON checks pass new restore2/1/1,after2/1,nested intact1/events2,seven keys128 value/begin/end each,64 quiescent property-listener attach/detach; inherited suppression/ownership checks pass. Notifications nested/direct7/64 editor detach pass all trees. Release editor3072/four scales and separate sanitizer controls/gesture/undo/recall/close pass. Core16/16,format,generator drift3 pass. Final generated AU/SDK/VST3 and all ownership fixture sanitizer flags verified; diagnostic hooks/probe linkage absent default binaries. No observed final assertion/sanitizer/race failure in finite runs. Fresh external bypass notification alloc/free0/0,mutex2; whole defaultAUfresh1 alloc0/mutex9,hostile603; VST3fresh1024 alloc0/mutex9. Whole realtime not accepted. Exact final identities/export/hashes in handoff/checkpoint.json/final-identities.json. Historical291/505/357/364/409 inventories unchanged; Sprint08 live-source comparison intentionally invalidated; never reseal historical evidence.

Next target: pinned AUBase.cpp944 AddPropertyListener reserves/pushes mutable mPropertyListeners; RemovePropertyListener961 erases; PropertyChanged977 walks entire host-sized vector and invokes callbacks. AUBase.h716 owns storage; construction/destruction are additional lifetime operations; const GetPropertyListeners527 has no SDK caller. AUPlugInDispatch add/remove and SetProperty take AUInstanceGuard, but processor-originated bypass PropertyChanged in generated Sys01AU.mm1517 and updater dispatch do not. Quiescent registration alone does not prove concurrent/reentrant traversal or callback-context reclamation. Wrapper adds ContextName dispatcher before publication; it uses wrapper this/updateTrackProperties. SDK SetProperty/RestoreState/layout/input/LastRenderError also call virtual PropertyChanged. AudioProcessorChangedUpdater2216 uses atomic flags plus AsyncUpdater; off-message update can allocate/post. Trace every registry/state/resource writer and callback lifetime. Separate plugin SDK traversal/allocation/message work from actual host/OS callback implementations. No fixed listener cap,silent gesture coalescing or lock deletion without authorized semantics and all-writer acknowledged reclamation.

Start with session04 ownership-trace.json/au-host-notifications-session-04.md, scripts/native_au_adapter.py and pinned AUBase.cpp944-984. Extend a dedicated loaded-AU fixture to force native bypass listener count traversal or reentrant add/remove, measure/freeze actual plugin operation/defect before repair. Diagnostic hooks defaultOFF and fixture callbacks must be distinguished from shipping work. Continue independent local work when external fixtures unavailable. APVTS ParameterAdapter/LockedListeners,regular/final processor listeners,CachedParamValues,arbitrary reentry and immutable prepared resources follow. Never hold ingress over reentrant listeners or remove locks without replacement ownership and acknowledged reclamation for every writer.

Preserve stable editor relay: off-message callbacks only atomic dirty,no retained payloads,OS posts or UI pointer access; editor polls authoritative packed float/generation for seven parameters; relay survives synchronized removeListener; gestures,undo/recall,timer visibility retained. Preserve org.algorhythmia.sys01/Algr/Sys1,seven keys/native IDs,four factory programs/order,schema1 exactly40 bytes,parameter-only deterministic empty-history recall,10ms smoothing,shared256 whole-batch rejection/no replay,eight coherent restore slots,single audio owner,VST3 behavior,stereo effect/generator/disabled input and release scope.

Sprint10 still requires immutable prepared resources,nonwaiting audio acquisition,off-audio configuration waits,acknowledged reclamation for every writer,safe silence/error for invalid concurrent lifecycle,complete plugin-controlled callback zero-prohibited instrumentation and passing concurrency/lifecycle tests on available shipping formats. Warm zeros/finite sanitizer passes do not prove whole realtime safety.

Verify tools/pins before use: CMake/CTest4.4.4 at /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin;clang-format18.1.8 sibling clang_format/data/bin;AppleClang21/macOS26.6.2 arm64;JUCE /private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850;Doctest build/_deps/doctest-src;pins cmake/dependencies.lock.json. Trees /private/tmp/sys01-s09-repair,/private/tmp/sys01-s09-asan,/private/tmp/sys01-s09-tsan final hooksOFF/probeOFF;core /private/tmp/sys01-s09-session02-core. All known jobs completed,none to resume. Reconstruct absent temporary paths from retained pins/caches/source export. Probe SHA25659af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54. Frozen rpaths may reference original temporary/evidence paths; use absolute DYLD_LIBRARY_PATH if absent,never edit frozen bytes.

Only installed Ableton Live12 Suite allowed;skip REAPER. Install no testing apps,modify no personal plugin directories,contact nobody,spawn no sub-agents,publish nothing,create no tags. auval cannot bind temporary AU by path without forbidden installation;pluginval absent recorded paths/PATH. Actual DAW-origin timing,external architecture/OS,signing,rights,native accessibility,human acceptance,performance/soak and MB-04 remain unqualified.

Use new evidence/sprint-10-session-05 and unique command IDs. Retain actual commands/results/tool/dependency/source/artifact hashes,failures,invalidation. Renew affected strictAU/callbacks/VST3/adapter/recall/editor/core and separate ASan/UBSan/TSan including generated wrappers and first-thread/lifecycle fixtures. Ownership switches defaultOFF; verify hooks/probe absent final rebuilt default binaries. Before ending update compact rolling-state.md,save handoffs/sprint-10-session-05.md from handoff-template.md with exact checkpoint/remaining gates/next action,retain evidence and provide complete copy-paste prompt. Continue Sprint10 if incomplete.
```
