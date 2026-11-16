# SYS 01 Sprint 09 session 03 handoff

## Checkpoint

2026-10-19 14:10 America/Santiago (17:10 UTC); execution/approval waits span
15:14–17:10 UTC. Sprint/session09/03. Objective: S09-OWN-01 deterministic
ownership acceptance for generation-aware AU publication.
**COMPLETE AT NAMED LOCAL BOUNDARY:** ownership review and Sprint09 local
exit criteria met. Sprint10 is dependency-ready, not yet implemented.
Release remains **NO-GO / development-unqualified**. G01 whole native realtime,
G02 DAW-origin timing and external release/platform/human gates remain open.

Git root `/Users/lhommeduchili/algorhythmia`; SYS01 root
`/Users/lhommeduchili/algorhythmia/sys-01`; HEAD unchanged
`5c45cf6bed431317a1b412b1fc12ee62b66cfdd5`. No applicable AGENTS.md.
All133 session02 resume inputs matched. Five inherited inputs changed:
AUOffsetTests.cpp, desktop CMakeLists.txt, EditorTests.cpp, PluginProcessor.hpp,
native_au_adapter.py. Substantial inherited SYS01 dirty/untracked and unrelated
monorepo work preserved. No staging/reset/discard/stash/commit/tag/publish/
sub-agent/app installation/personal plugin directory mutation.

Evidence `../evidence/sprint-09-session-03/`; actual argv/cwd/UTC/exit/tool/log
hashes in commands.json. Source delta, final status, resumed reference hashes,
ON/OFF generated wrappers/compile flags/caches, reports, frozen ON/OFF bundles,
failures and negative controls retained. **134-input source export**
SHA256 `597df523558022e71b430fc62099b23381bbacd9182cfe7edaf8f899088ee282`;
resume-inputs.json SHA256
`e010d94c393973bbddff6e6db13dacd5585cbde18a7e0a7b6c4f951044e7aace`.
The export includes all inherited inputs plus session03 ownership record; rolling
state/handoff excluded to avoid circular hashes. Retained session02 inventory:
291 files unchanged. Historical Sprint08 live comparison remains invalidated.

## Changes and decisions

**New actual S09-OWN-01 defect reproduced before repair:** AU accepted FLUX0.2;
editor published0.8 before capture. A second loaded-AU case attached a real
parameter Listener at the post-capture hook; notification of FLUX0.2 reentered
notifying setters writing FLUX0.8/BIAS0.7, invalidating both the current target
and later BIAS CAS. Authoritative virtual values/APVTS raw mirrors and exact
captured/following audio passed. GetParameter and ClassInfo schema40 save failed
inside both callbacks: stale pending AU targets overlaid newer external controls.
Frozen negative bundle
`c3c415836e9d9bf2d7c56580f4cd662cf14241168436251c657f71be2cd34427`
and executable retained before repair. New and session02 original negatives
still fail actual ownership assertions in new reports.

Readback/save pending overlays now require matching non-exhausted external
generation. Lifecycle flush uses conditional publication outside ingress; Reset
cannot overwrite a newer editor target. A further deterministic lifecycle case
checks readback/mirrors/schema40 before/after Reset and exact following audio.
Permanent exhaustion capture reports sentinel generation even if a delayed
pre-exhaustion writer exchanges later. Virtual float reads remain authoritative.
Existing unique tickets/exchange, single strong CAS, accepted-position epoch,
restore epoch/eight slots, stateMutex/ingress/framework/listener/TLS locks remain.

Full writer/listener review is in [ownership record](../au-ingress-ownership-session-03.md):
AU acceptance/capture/CAS/cleanup/readback/save/lifecycle, direct/notifying writers,
APVTS listeners/attachments, ValueTree/undo replay, restores/programs, native VST3,
reentry and snapshot acknowledgement. No ingress held across listeners, no lock
removed. Regular listeners run in reverse order under recursive listenerLock,
then final AudioProcessor listener; APVTS reads current getValue. General payload
ordering (ParameterAttachment consumes payload), APVTS concurrent mirror timing,
AU nested suppression, TLS first-thread allocation/traversal, recursive restore
stateMutex reentry and complete callback ownership remain explicit Sprint10/G01
work. Finite sanitizer checks cannot establish whole realtime safety.

Sanitizer strict checks exposed three pinned JUCE disabled-layout assertions
already present in session02 logs: tag0 denotes UseChannelDescriptions, not a
tag-only disabled layout. Generated AU initialization now explicitly constructs
AudioChannelSet::disabled() for zero channels. Upstream pins untouched. New
sanitizer editor check exposed nine ASCII constructor assertions on fixture Arabic
invalid input; explicit String::fromUTF8 repairs fixture input. Added fixture-only
--controls-only mode covers gesture/controls/undo/recall/teardown while avoiding
instrumented full image/time claims. Release retains3072 renders/four scales.

ALGORHYTHMIA_AU_OWNERSHIP_FIXTURE remains defaultOFF. Before-capture/read/listener
hooks and existing capture/editor hooks exist only in ON AU glue. Final AU/VST3
symbol inventories and binary-string scans verify absence in all OFF bundles.
Native IDs/seven keys, four factory programs/order, schema1 exactly40,
parameter-only deterministic empty-history recall,10ms smoothing, shared256 cap/
whole-batch rejection/no replay, eight restores, single audio owner, VST3 and
stereo effect/generator/disabled input contracts preserved. No scope/timing waiver.

## Verification

| Check | Registry/report reference | Actual result | Boundary |
|---|---|---|---|
| Resume/pins/tools | resume-comparison; s03-tools/juce-pin/doctest-pin |133 match; CMake/CTest4.4.4, JUCE4377/doctest270 pin files verified | Before edits |
| Actual baseline | s03-ownership-v3-baseline-authorized | exit1; both callbacks host/schema FAIL | Frozen new negative exact bytes |
| Intermediate repair | s03-release/asan/tsan-fixture-verify | exit0 each; all ownership/native cases pass | Intermediate reports retained, superseded for final source |
| Lifecycle check | s03-release-lifecycle-check | exit0 | Intermediate repaired Release loaded AU |
| Final ON | s03v2-repair/asan/tsan-final-fixture-verify | exit0 each; old/new ownership,1274 strict,callbacks,VST3,seals;3/3 each | Final v2 source; separate instrumented trees |
| Final OFF | s03v2-repair/asan/tsan-default-verify | exit0 each;1274 strict,callbacks,VST3,seals;3/3 each | Final default behavior bytes |
| Editor | s03v2-repair/asan/tsan-editor-check | exit0; Release3072 renders/four scales; sanitizer controls-only | Native local fixture, no DAW/AT/performance claim |
| Core | s03-core-renewal; s03v2-core-check |16/16 each | Unchanged core/DSP inputs, retained pinned tree |
| Generation/format | s03-generation-final; s03-format-source-final | exit0; all3 generators reject drift | Upstream unchanged; first-party format |
| Negative controls | s03-negative-controls/new-negative-recheck/original-negative-recheck | both actual failures retained, validator exits1 expected | Frozen session02/03 bytes |
| Final integrity | s03-checkpoint-integrity | exit0;61 then63 prior command logs/report logs verified, generated flags, hooks absent,current bytes match | Final s03-final-integrity verified64 completed registry entries |
| Source export/preservation | s03-export-checkpoint; session02-preservation.json |134 inputs, five inherited changes;291 old files unchanged | Exact source and historical evidence |

Final default bundle fingerprints (release_evidence.py algorithm):

| Tree | AU | VST3 |
|---|---|---|
| Release |`b8e86b5b4149f731e46f0bd6398c7fbb701d4ce1a7ebe4ec14d8a7744a5634f5`|`f9718f3b4cb363834a39ddc036dac84e52d919024f67646430a4ef679fd8235f`|
| ASan/UBSan |`e377f54ed3691c0de306fbdb09765d829208396320abf9a0bd34f4c4f47b1519`|`770dafece5058f6f2ad401e4f858cb51961a2b3ea00884b27318d2b132a6f344`|
| TSan |`8de973383d635b19228dde834904ea16135fc007fded1c89a8905b0ef0f835db`|`7042daee80f54bd02267e19255dd96d8594b159b05205b179c0e3003825852b6`|

CMake/CTest4.4.4 absolute directory
`/Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin`.
AppleClang21, clang-format18.1.8, macOS26.6.2/25G83 arm64. Verified JUCE
`/private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850`;
doctest `build/_deps/doctest-src`. Native trees `/private/tmp/sys01-s09-repair`,
`sys01-s09-asan`, `sys01-s09-tsan` finalOFF; core `sys01-s09-session02-core`.
Sanitizer flags confirmed in generated AU/SDK (dispatch included)/VST3 compile
commands; no final observed assertion/ASan/UBSan/TSan failure in finite runs.
Ad-hoc seals are development integrity, not production signing.

Preserved unsuccessful/incomplete attempts: initial fixture aggregate initializer
failed Werror (corrected); sandbox AU registration failed, escalated run reproduced
actual ownership failure; initial full ASan editor images emitted UTF8 assertions
and was deliberately SIGTERM-ended after723s with partial image output retained;
first final-matrix exited1 as a result, never qualified. Final-v2 renewals pass.
Intermediate assertion-bearing strict reports remain unchanged and are superseded.
A prior TSan matrix overlapped later rebuilding after its exact validators;
intermediate CTest is not final evidence, final-v2 runs/builds are sequential.
Scoped session process inspection/termination used explicit approval. No other
process was controlled and no global-process absence is claimed.

Unavailable: auval exists but selects registered IDs, cannot bind temporary AU
by path without prohibited installation; pluginval absent at four recorded paths/
PATH. Ableton Live12 Suite installed, actual host-origin automation/stereo not
performed. Intel/Windows/minimum OS/native readers/listening/musicians/performance/
soak/rights/signing unqualified. Only installed Ableton permitted, REAPER skipped,
no testing app installed or personal plugin directory modified. AU/shared changes
invalidate affected native/DAW/UI/performance/soak/signing evidence. Final local
renewals bind these bytes; historical seals stay untouched. MB-04 DC unresolved.

## Resume

Next: **Sprint10 session01**. Sprint09 local exit is met; G01 and external
qualification remain open. Start loaded-native whole callback probe/ownership
trace at AU Render -> publishNativeParameter -> listenerLock -> attachments/final
processor listener -> ThreadLocalValue suppression. Include VST3 equivalent,
first audio-thread entry, resource/lifecycle locks, MIDI traversal, attach/detach,
state/program/bypass and host callbacks. Reproduce smallest plugin-controlled
allocation/lock/payload defect, retain exact bytes, implement one coherent bounded
notification/lifetime change and renew affected strict/native/editor/sanitizers.
General asynchronous notification generation, ordering and attachment teardown
need explicit proof before replacing the current listener path. No new failure
is outstanding on final Sprint09 bytes.

All session-owned jobs complete (exec sessions closed). No background run to resume.
Temporary paths can disappear: finalON/OFF bundle snapshots, exact source export,
caches and full dependency pins retained. No user decision/access invented.
Continue independent authorized local work when external fixtures are absent.

## Complete next session prompt

```text
Work in /Users/lhommeduchili/algorhythmia/sys-01. Begin SYS 01 v1.0.0 Sprint 10, session 01: native realtime ownership and notifications. Implement and verify a coherent bounded step; do not return only another plan.

Read docs/release-plan-1.0.0/rolling-state.md, handoffs/sprint-09-session-03.md, au-ingress-ownership-session-03.md, docs/release-1.0.0/native-ownership.md and the Sprint 10, acceptance, evidence-invalidation and handoff sections of audit-production-plan-2026-10-18.md. Check Git status and applicable AGENTS.md. Compare live inputs to evidence/sprint-09-session-03/resume-inputs.json and resume-reference-inputs.json before editing; resolve intervening changes. Git root /Users/lhommeduchili/algorhythmia; HEAD 5c45cf6bed431317a1b412b1fc12ee62b66cfdd5. Preserve substantial dirty/untracked SYS 01 and unrelated monorepo work. No bulk-stage/reset/discard/stash/overwrite.

Sprint 09 and S09-OWN-01 are complete at their named LOCAL boundary. Release NO-GO/development-unqualified; G01 whole callback realtime and actual DAW-origin/external qualification remain open. Session03 reproduced stale pending AU overlays: newer editor/reentrant FLUX0.8/BIAS0.7 had correct authoritative/APVTS values but old pending0.2 appeared in GetParameter/schema40 during callbacks. Generation-aware overlays and conditional lifecycle flush repaired this. Permanent exhausted capture is sentinel-invalid. All writer/listener paths reviewed with explicit Sprint10 limits. Frozen session02 and session03 negatives still fail; preserve them.

Start tracing scripts/native_au_adapter.py generated AU Render -> PluginProcessor::publishNativeParameter -> pinned AudioProcessorParameter::sendValueChangedMessageToListeners (recursive listenerLock, reverse listeners then final owner) -> APVTS/ParameterAttachment -> final AU processor listener and ThreadLocalValue suppression. ParameterAttachment consumes payload and may async-post; outer stale payload after nested write and first-thread TLS allocation/traversal remain open. APVTS authoritative getValue/raw-mirror timing does not establish whole listener safety. Trace VST3 equivalents, callbacks/buffer mapping/MIDI, state/program/bypass/lifecycle resources and attachment teardown. Reproduce the smallest prohibited plugin-controlled callback allocation/lock/reentry defect through an exact loaded binary or dedicated first-audio-thread fixture; retain failure and artifact identity before repair. Build bounded generation-aware off-audio notification/lifetime ownership as appropriate. Do not remove locks until every writer follows replacement ownership/reclamation, hold ingress across listeners, use a global nesting/suppression flag, or infer whole realtime safety from finite sanitizer passes. Preserve single audio ownership and existing legal lifecycle exclusion.

Final session03 v2 Release, ASan/UBSan and TSan ON ownership cases pass post-capture restore/editor, before-capture editor, reentrant current/later target invalidation and Reset retention. All seven authoritative/APVTS mirrors/native readback/schema1 exactly40 and independent captured/following audio pass. Final OFF bundles pass1274 strict loaded-AU comparisons each, callbacks, VST3 exact checks and VST3/adapter/recall3/3 each. Core16/16; Release native editor3072 images/four scales; sanitizer controls/gesture/undo/recall/teardown checks pass without assertions. No observed final sanitizer/race failure. Generated AU/SDK/dispatch/VST3 sanitizer coverage verified, hook symbols/strings absent in all defaultAU/VST3 binaries. Disabled input uses explicit disabled layout, fixing retained JUCE debug assertions; editor invalid Unicode fixture now uses explicit UTF8. Earlier fullASan images were ended/incomplete and retained; never claim their pass.

Verify tools/pins before use. CMake/CTest4.4.4 directory /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin; AppleClang21; JUCE /private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850; doctest build/_deps/doctest-src. Native /private/tmp/sys01-s09-repair, sys01-s09-asan, sys01-s09-tsan are final hookOFF, core /private/tmp/sys01-s09-session02-core. All session jobs complete. Reconstruct missing paths from retained pins/caches/source. Source export evidence/sprint-09-session-03/source-checkpoint.tar.gz has134 inputs, SHA256597df523558022e71b430fc62099b23381bbacd9182cfe7edaf8f899088ee282; resume SHA256e010d94c393973bbddff6e6db13dacd5585cbde18a7e0a7b6c4f951044e7aace. Final Release AU fingerprint b8e86b5b4149f731e46f0bd6398c7fbb701d4ce1a7ebe4ec14d8a7744a5634f5, VST3f9718f3b4cb363834a39ddc036dac84e52d919024f67646430a4ef679fd8235f; separate identities and frozenON/OFF bundles retained.

Preserve native IDs/org.algorhythmia.sys01/Algr/Sys1, seven keys, four factory programs/order, schema1 exactly40, parameter-only deterministic empty-history recall,10ms smoothing, shared256-event cap/whole-batch rejection/no replay, eight coherent restores, single audio owner, VST3 behavior, stereo effect/generator/disabled input and release scope. Keep ownership hooks fixture-only/defaultOFF; verify absence in final bundles. Renew affected strictAU/callbacks/VST3/adapter/recall/editor/core and separateASan/UBSan/TSan checks including generated wrappers. Use new sprint10-session01 evidence/unique commandIDs; preserve all reports/failures/historical seals. Sprint08 live-source comparison remains intentionally invalidated; never reseal old evidence.

Only installed Ableton may be used for current host tests, skipREAPER. Install no testing apps, modify no personal plugin directories, contact nobody, spawn no sub-agents, publish nothing, create no tags. auval cannot bind temporaryAU by path without prohibited installation; pluginval absent at recorded paths/PATH. Actual Ableton-origin timing, external architecture/OS, signing/rights/human acceptance/performance/soak remain unqualified. MB-04DC unresolved. Do not waive timing or other gates; continue independent authorized local work when external fixtures unavailable.

Before ending update compact rolling state, save handoffs/sprint-10-session-01.md using handoff-template.md, retain commands/results/tool/dependency/source/artifact hashes/failures/invalidated evidence and exact next action, and provide a complete copy-paste next-session prompt. Remain Sprint10 until its named exit criteria are met; if incomplete hand off exact implemented/tested checkpoint.
```
