# SYS 01 Sprint 10 session 01 handoff

## Checkpoint

2026-10-19 16:00 America/Santiago (19:00 UTC). Sprint/session10/01.
Objective S10-NOTIFY-01, bounded editor notification ordering/publication repair
under S07-P1-03. **LOCALLY VERIFIED bounded repair; Sprint10 IN PROGRESS.**
Sprint09/S09-OWN-01 remain COMPLETE AT THEIR NAMED LOCAL BOUNDARY. Release
**NO-GO/development-unqualified**. G01 whole native realtime, actual DAW-origin
AU timing, external/human/platform gates and MB-04 DC remain open.

Git root /Users/lhommeduchili/algorhythmia; SYS01 root
/Users/lhommeduchili/algorhythmia/sys-01; HEAD unchanged
5c45cf6bed431317a1b412b1fc12ee62b66cfdd5. No applicable AGENTS.md.
All134 session03 source inputs and four references matched before editing.
Substantial dirty/untracked SYS01 and unrelated monorepo work preserved.
No staging/reset/discard/stash/commit/tag/publish/sub-agent/app installation or
personal plugin directory mutation. Exactly five inherited engineering inputs
changed, plus three new inputs.

Evidence ../evidence/sprint-10-session-01/ retains unique command IDs,
argv/cwd/UTC/exit/tool/log hashes, actual failures/frozen negative, ON/OFF native
bundles/wrappers/caches/compile flags, exact reports, editor images, source delta,
input hashes/export and preservation/integrity checks. Exact137-input source
export source-checkpoint.tar.gz SHA256
0200e38605be19c0ddf6f56693e681dc81a6efafd4942513264bbb2e1d580f63;
resume-inputs.json SHA256
f1943105f5f0a072945f3fcda0338e6d757f73e256bad127f4bd1c1193544d3d.
Rolling state/handoff excluded from export to avoid circularity. Session02
retained291/session03 retained505 inventory files unchanged. Historical Sprint08
live comparison remains intentionally invalidated.

## Changes and decisions

Actual baseline: dedicated fresh-audio-thread fixture constructs shipping
PluginEditor and registers a real reverse-order FLUX listener after attachment.
OuterFLUX0.2 reentersFLUX0.8/BIAS0.7, then lateobsolete0.2 notifies. Authority/raw
mirror0.8 but UI0.2 orderingFAIL. Frozen notification-negative executable SHA256
343e1904a5200b329191d647dcc8333c21bbf36854100ebca1057b882c9d3085;
probe dylib SHA256
59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.
Baseline first dispatch14 mutexes/warm2, allocation0 initially. Recheck same
frozen executable still fails ordering and observes audio realloc/free1 in
juce::MessageQueue::post, stack retained. Diagnostic negative control verifies
malloc/free/mutex/unfair interposition. This is not a DAW-origin claim.

New ControlAttachments.hpp replaces only SYS01 editor's APVTS slider/button/
combo attachments. Separate stable final relay marks atomic dirty off message
thread; never stores payload/posts OS messages/touches controls. Editor update
reads authoritative packed float/generation, compares the entire word and
updates seven controls on message thread. Old payloads cannot replay stale
values; direct writes/same AU external generation value changes are visible.
No wrapping UI sequence/queue capacity. Message-thread writers synchronously
poll, retaining gestures/undo/recall and existing visibility/timer behavior.
Automation itself is unchanged.

Relay vtable remains stable throughout derived attachment destruction and
relay lives until JUCE synchronized removeListener returns. Other threads touch
only relay dirty/immutable thread ID, never owner pointer. Message-thread
construction/destruction/control listeners remain serialized. Held gestures
finish while attachments still exist. No listener/ingress/callback/state/FL
lock removed, no ingress across listeners, no global suppression flag.
Legal lifecycle exclusion/single audio owner remain required. Full map:
[native-notifications-session-01.md](../native-notifications-session-01.md).

Changed inherited paths: apps/desktop/CMakeLists.txt adds native_notifications
fixture/probe linkage only to diagnostics; PluginEditor.hpp/.cpp use attachments/
poll; AUContractTests.cpp/VST3ContractTests.cpp place first nonzero entry on a
fresh worker and retain allocation stacks. New NotificationTests.cpp checks real
UI nested/late ordering, seven direct writes and64 concurrent editor attach/detach
cycles during dense seven-key publication. JUCE/wrapper generators untouched.

Preserved org.algorhythmia.sys01/Algr/Sys1, seven keys/IDs/ranges, four programs/
order, schema1 exactly40, parameter-only deterministic empty-history recall,
10ms smoothing, shared256 whole-batch rejection/no replay, eight restores,
single audio owner, VST3/AU stereo effect/generator/disabled-input behavior and
release scope. No product decision required for bounded implementation;
external decisions/evidence remain pending.

## Verification

| Check | Command registry reference | Exit/result | Boundary |
|---|---|---|---|
| Resume/tools/pins | initial-inputs/references; s10-juce-pin/doctest-pin/tools |134+4 match;4377 JUCE/270 doctest files verified | Before edit; baseline default bytes match session03 |
| Baseline | s10-notification-baseline | exit1 authority/raw0.8 UI0.2 orderingFAIL | Frozen dedicated first-thread fixture |
| Repair | s10-relay-notification | exit0 UI0.8/BIAS0.7;64 attach/detach;7 directPASS; first alloc/free0 mutex5/warm1 | Diagnostic fixture, framework locks remain |
| Complete entry probes | s10-release-probes-authorized | strict1274/callbacks/VST3/sealsPASS;3/3 | Exact Release; firstAU/VST3 alloc1/mutex9 each; not realtime acceptance |
| Final ON | s10-repair/asan/tsan-on-verify | exit0 each; post/before-capture/reentrant/Reset ownership,1274 strict,callbacks,VST3,seals;3/3 each | ProbeOFF/hookON; separate trees |
| Final OFF | s10-repair/asan/tsan-off-verify | exit0 each;1274 strict each,callbacks,VST3,seals;3/3 each | Default behavior bytes frozen |
| Notifications | s10-repair/asan/tsan-final-notifications | exit0 each nested/late/all7/direct/64 teardownPASS | Finite dedicated first-thread/race tests |
| Editor | s10-repair/asan/tsan-final-editor | Release3072 renders/four scales; sanitizer controls/gesture/undo/recall/closePASS | Local offscreen; no human/AT/DAW/budget acceptance |
| Core/format/generation | s10-core/format/generation |16/16; formatPASS;3 drift rejectionsPASS | Core/DSP unchanged, pins untouched |
| Frozen controls | s10-s09-02/03-negative; s10-notification-negative-recheck | old ownership exits1 actualFAIL; notification stillUI0.2/realloc stack | New reports; original seals untouched |
| Integrity/export | s10-checkpoint-v2/final-integrity |51 then52 command logs checked; reports/flags/hook/probe absencePASS;137-input export | Live/frozen identities match, historical inventories preserved |

Final default fingerprints (release_evidence.py algorithm):

| Tree | AU | VST3 |
|---|---|---|
| Release |11c22fb946420819f2bff0d30afafd70ed1a0baa5db63e66c88b25029a67445e|f9d53ef40ab2ead3147d5ec4154e8409e75d438d72c32cece3417606f0a234d2|
| ASan/UBSan |6269c30c08292f452194a8b57f4d8cda1a04003fe0def2cd3274c9e9402cba39|699e725079f4004539d70d855b2c873a7d6db70bc84af74087b631457a0bec7a|
| TSan |fe168f602479e99d4f2030aed5f05eaa0c19c65dcde5fd028e51b19a1f7dbcb2|843e3ef53af2427e9ea59a05f0a9f3ab23112354bb6501fca6e6ca0578f87b0c|

Tools: CMake/CTest4.4.4 directory
/Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin;
clang-format18.1.8 in same archive/site-packages/clang_format/data/bin;
AppleClang21/macOS26.6.2/25G83 arm64. JUCE8.0.12
/private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850,
treeSHA25634954713e005f05801972439db07a78356b36bd421b51f2957b3e38605022f85;
doctest2.4.12 build/_deps/doctest-src,
treeSHA2565778f85f9828756663bd8271eb7462ef9390f72a1cb92f5c1c50fff8d20fb90a.
Generated SDK includes dispatch in its instrumented translation unit (no separate
compile_commands entry); AU/SDK/VST3 sanitizer flags verified. Complete symbols/
byte-prefix scans and otool verify hooks/probe linkage absent final defaults.
Ad-hoc seals are local development integrity, not production signing.

Retained failures: sandbox strict/callback AU factory registration failed;
authorized in-process run passed. Initial ASan relative output emitted juce_File
assertion219 despite exit0; clean absolute-path/final runs supersede it. First
checkpoint script assumed separate dispatch compile entry, failed StopIteration;
corrected includedSDK check passes. Patch anchor attempts failed before mutation;
identity helper wrong function failed, corrected fingerprint. Sandbox ps denied;
approved read-only inspection showed no remaining session children after matrix.
No process terminated. Final finite sanitizer logs have no observed assertion/
ASan/UBSan/TSan failure. Assertion-bearing runs never accepted.

Unperformed: auval exists but cannot bind tempAU by path without prohibited
installation; pluginval absent PATH/recorded prior paths. Only installed Ableton
Live12 Suite permitted; no actual DAW-origin timing/stereo this session; REAPER
skipped. No testing app installed. Intel/Windows/minimumOS/native readers/humans/
listening/rights/signing/driver-DAW budgets/soak remain unqualified. MB-04 unresolved.
Editor/shared changes invalidate affected native/DAW/UI/performance/soak/signing
proof. New checks bind named bytes only, old reports/seals frozen. No gate waiver.

## Resume

Continue Sprint10 session02: first-thread AU suppression allocation/nesting.
Start scripts/native_au_adapter.py Render assignment inParameterChangedCallback,
final audioProcessorParameterChanged consume/reset, lifecycle publish/flush and
pinned ThreadLocalValue<bool>::get. Exact Release report
reports/release-probe-authorized-default-callback.log records fresh1-frame alloc1/
mutex9 operatornew inside JuceAU::Render. VST3 report records fresh1024 alloc1/
mutex9 calloc->dyldThreadLocalVariables->tlv_get_addr->processParameterChanges.
Freeze next negative on exact loaded bytes, test nested final listener suppression
and independent concurrent instance/thread notifications before replacing storage.
No shared global flag/native thread_local substitution moving allocation to dyld.

Remaining Sprint10: safe AU/VST3 suppression; APVTS/listener ownership beyond
editor relay; immutable prepared resources/nonwaiting acquisition/off-audio
reconfiguration/acknowledged reclamation for every writer before lock replacement;
invalid concurrent lifecycle silence/error; whole plugin-controlled callback
zero prohibited ops and available-format concurrency/lifecycle checks. External
platform proof Sprint13; host/OS costs/finite limits explicit. Stay Sprint10.

All session jobs complete, exec sessions closed; no background job to resume,
no global process absence claim. Native trees /private/tmp/sys01-s09-repair,
sys01-s09-asan,sys01-s09-tsan final hooksOFF/probeOFF; core
/private/tmp/sys01-s09-session02-core. Reconstruct absent paths from export/pins/
cache. Frozen notification negative has absolute LC_RPATH
/private/tmp/sys01-s09-repair/apps/desktop; dylib retained in evidence. If path
absent supply retained dylib using DYLD_LIBRARY_PATH=<absolute evidence directory>,
never modify frozen bytes. Continue independent authorized local work.

## Complete next session prompt

```text
Work in /Users/lhommeduchili/algorhythmia/sys-01. Continue SYS 01 v1.0.0 Sprint 10, session 02: native realtime ownership and notifications. This is implementation work. Reproduce a concrete prohibited operation/notification defect on exact loaded bytes or a dedicated fresh-audio-thread fixture, retain failure, implement one coherent bounded repair and verify it. Stay Sprint10 until named exit criteria met.

Read docs/release-plan-1.0.0/rolling-state.md, handoffs/sprint-10-session-01.md, native-notifications-session-01.md, au-ingress-ownership-session-03.md, docs/release-1.0.0/native-ownership.md and Sprint10/acceptance/invalidation/handoff sections of audit-production-plan-2026-10-18.md. Check Git status/AGENTS.md; compare live against evidence/sprint-10-session-01/resume-inputs.json and resume-reference-inputs.json before editing. Resolve intervening user changes. Git root /Users/lhommeduchili/algorhythmia, HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5. Preserve substantial dirty/untracked SYS01 and unrelated monorepo work; no bulk-stage/reset/discard/stash/overwrite.

Sprint09/S09-OWN-01 COMPLETE AT NAMED LOCAL BOUNDARY. Sprint10 IN PROGRESS, S10-NOTIFY-01 locally verified only. Release NO-GO/development-unqualified; G01, actual DAW-origin/external AU qualification and MB-04 DC open. Session10/01 reproduced outerFLUX0.2->nestedFLUX0.8/BIAS0.7, authority/APVTS0.8 UI0.2. Frozen notification-negative SHA256343e1904a5200b329191d647dcc8333c21bbf36854100ebca1057b882c9d3085 stillFAIL; recheck observes audio MessageQueue::post realloc/free1. Probe dylib SHA25659af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54 retained. Frozen executable LC_RPATH /private/tmp/sys01-s09-repair/apps/desktop; if absent supply retained dylib through DYLD_LIBRARY_PATH, never edit frozen bytes.

ControlAttachments.hpp replaces only SYS01 editor slider/button/combo attachments. Stable final relay marks atomic dirty off message thread, never payload/UI pointer/OS post. Editor polls authoritative packed float/generation for7 keys, coalesces visuals only, immediate message-thread gestures/undo/recall/timer visibility retained. Relay lives until synchronized JUCE removeListener returns. Release/ASan/UBSan/TSan notification tests pass nested/late/all7/direct/64 concurrent attach-detach. APVTS/arbitrary listeners/TLS/resource locks remain. Preserve legal lifecycle exclusion/single audio ownership; no ingress across reentrant listeners, no lock removal without every writer and reclamation proof, no global suppression flag.

Start scripts/native_au_adapter.py Render->publishNativeParameter->reverse regular parameter listeners->final AU processor listener, inParameterChangedCallback assignment/consume/reset/lifecycle publish/flush and pinned ThreadLocalValue<bool>::get. Exact Release report evidence/sprint-10-session-01/reports/release-probe-authorized-default-callback.log reproduces fresh1-frame alloc1/mutex9 operatornew inside JuceAU::Render. VST3 fresh1024 alloc1/mutex9 stack calloc->dyldThreadLocalVariables->tlv_get_addr->processParameterChanges persists. Choose AU first-thread suppression allocation/nesting as next smallest target; freeze exact negative, test nested final listener suppression and independent concurrent instance/thread notifications. Do not replace with native thread_local that moves allocation into dyld. Trace all writers/lifetime before replacement; then continue VST3 and prepared-resource/lifecycle protocol. Host/OS callbacks distinguished from plugin-triggered TLS/listener/resource/post work.

Final hookON/OFF Release/ASan+UBSan/TSan pass S09 post/before-capture/reentrant current/later target/Reset ownership,1274 strict independent loadedAU comparisons each,callbacks,VST3,seals and adapter/recall/VST3 3/3 each. Release editor3072 renders/four scales, sanitizer controls/gestures/undo/recall/close, core16/16 pass. Final observed assertion/sanitizer/race none, finite runs only. Generated AU/SDK(dispatch included)/VST3 flags verified; hooks/probe linkage absent final defaults. Session02 inventory291/session03 inventory505 unchanged; frozen ownership controls still actualFAIL. Earlier relative-path ASan File assertion exit0 retained/unaccepted; final absolute runs pass. Historical Sprint08 live comparison remains invalidated; never reseal old evidence.

Verify pins/tools before use. CMake/CTest4.4.4 /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin; clang-format18.1.8 same archive/site-packages/clang_format/data/bin; AppleClang21/macOS26.6.2 arm64. JUCE /private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850, doctest build/_deps/doctest-src against cmake/dependencies.lock.json. Native /private/tmp/sys01-s09-repair,sys01-s09-asan,sys01-s09-tsan final hooksOFF/probeOFF; core /private/tmp/sys01-s09-session02-core. All jobs complete. Reconstruct absent paths from pins/cache/source. Exact137-input export evidence/sprint-10-session-01/source-checkpoint.tar.gz SHA2560200e38605be19c0ddf6f56693e681dc81a6efafd4942513264bbb2e1d580f63; resume SHA256f1943105f5f0a072945f3fcda0338e6d757f73e256bad127f4bd1c1193544d3d. Final Release AU11c22fb946420819f2bff0d30afafd70ed1a0baa5db63e66c88b25029a67445e, VST3f9d53ef40ab2ead3147d5ec4154e8409e75d438d72c32cece3417606f0a234d2; separate sanitizer/ON identities retained.

Preserve org.algorhythmia.sys01/Algr/Sys1/all7 keys/nativeIDs/four programs/order/schema1 exactly40/parameter-only deterministic empty-history recall/10ms smoothing/shared256 whole-batch rejection/no replay/eight coherent restores/single audio owner/VST3/stereo effect-generator-disabledinput/release scope. ALGORHYTHMIA_AU_OWNERSHIP_FIXTURE defaultsOFF, hooks absent final bytes. New evidence/sprint-10-session-02 and unique IDs. Renew affected strictAU/callback/VST3/adapter/recall/editor/core/separateASan+UBSan/TSan/generated wrappers/first-thread/lifecycle checks. Do not infer whole realtime safety from finite sanitizer passes.

Only installed Ableton for current host tests; skipREAPER. Install no testing apps, modify no personal plugin dirs, contact nobody, spawn no sub-agents, publish nothing/create no tags. auval cannot bind tempAU by path without installation; pluginval absent recorded paths/PATH. ActualDAW-origin timing/externalarchitecture-OS/signing/rights/nativeaccessibility/humans/performance/soak unqualified, MB-04 unresolved, no timing/gate waiver. Continue independent authorized local work when external fixtures unavailable.

Before ending update compact rolling-state.md, save handoffs/sprint-10-session-02.md using handoff-template.md, retain commands/results/tool/dependency/source/artifact hashes/actual failures/invalidation/precise next action and complete copy-paste prompt. If incomplete hand off exact implemented/tested checkpoint and continue Sprint10, not Sprint11.
```
