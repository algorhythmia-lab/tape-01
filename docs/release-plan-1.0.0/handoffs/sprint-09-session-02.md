# SYS 01 Sprint 09 session 02 handoff

## Checkpoint

Date/time: 2026-10-19 12:00 America/Santiago (15:00 UTC); commands span
2026-10-19 02:55–15:00 UTC, including approval waits.
Sprint/session: 09/02. Objective: reproduce and repair S09-OWN-01 and renew
loaded-native evidence. Status: **LOCALLY VERIFIED at forced publication-gap
boundary; Sprint 09 IN PROGRESS**. Release **NO-GO / development-unqualified**;
G01/G02 and external AU/DAW/platform gates remain open. Sprint 10 not selected.

Git root `/Users/lhommeduchili/algorhythmia`; SYS 01 root
`/Users/lhommeduchili/algorhythmia/sys-01`; HEAD unchanged
`5c45cf6bed431317a1b412b1fc12ee62b66cfdd5`. All 132 session-01 resume inputs
matched before editing. No applicable AGENTS.md found. Existing dirty/untracked
SYS 01 and unrelated monorepo work preserved. Changed only five inherited
engineering inputs: AUOffsetTests.cpp, desktop CMake, PluginProcessor.cpp/.hpp,
native_au_adapter.py. `session-source-changes.json` records exact before/after.
No staging, reset, stash, discard, commit, tag, publish, sub-agent or install.

Evidence root `../evidence/sprint-09-session-02/`. Exact engineering export
`source-checkpoint.tar.gz`: 133 inputs, SHA256
`5c89be530abc8313a732bfa2a5dfc3f2ecdf26c0783addb742cc7388b8706aaa`.
Next resume inventory `resume-inputs.json`: SHA256
`59ddc3876f7a22ec6ec60eef5df1f541a47bfeeb0d4f79851a29c6f04ba6ce2a`.
It excludes rolling/handoff to avoid circular hashes. Baseline/repaired fixture
bundles, generated wrappers, compile commands, caches, logs and failed reports
are preserved. Final source docs describe finite boundaries, not G01 closure.

## Changes and decisions

**S09-OWN-01 reproduced through the loaded AU before repair.** Fixture-only
exported hook pauses Render after ingress capture. Another thread completes a
real ClassInfo restore or the exact virtual setter used by editor/APVTS writers.
Old FLUX 0.2 overwrote newer 0.8; readback, schema40 and following audio failed
for both writers. Frozen negative control fingerprint
`1b568a3f1528a206651f74f02e61192f071085d048de9abe23989f0ecbfb2f00` still fails.

`PluginProcessor.hpp/.cpp` retain JUCE parameter metadata but use authoritative
packed atomic uint64 scalar/ticket publication. External virtual setValue
writers allocate unique tickets and exchange the word. AU acceptance records the
external generation; Render makes one strong conditional CAS per target outside
ingress. A later external writer wins. Later AU captures in the same external
generation may change float bits. No retry loop/wait/allocation/reclamation on
this publication path. Irreversible exhaustion disables conditional publication
at 2^32-1 external tickets rather than wrap. Every participating direct/notifying
setter, restore/program, APVTS attachment, undo and ValueTree replay enters the
virtual setter. Concrete base get()/assignment shortcuts are not used by SYS 01;
pinned metadata/text/ranges remain unchanged.

Successful CAS calls listeners outside ingress without reentering unconditional
setValueNotifyingHost. Pinned APVTS listener reads current virtual getValue;
processor controls/save use that authoritative value, not a delayed raw mirror.
Existing restore epoch/eight-slot queue/stateMutex and single audio owner remain.
General listener payload ordering, raw-mirror timing, host callbacks/TLS/framework
locks are still whole-callback work. No lock removed. Inactive ordinary/scheduled
setters now also notify outside ingress on legal lifecycle owner.

**S09-BUS-01 independently reproduced and repaired:** zero AU input elements
were rejected (-10865). Zero-channel ASBD is not a legal SDK stream. Generated
AU now accepts input ElementCount 0/1 only before initialization, keeps one
processor layout slot, maps disabled/stereo layout and delegates SDK resources.
Output remains fixed stereo; legal host lifecycle excludes Render. Strict fixture
now covers distinct-channel stereo transform at six rates, both disabled-input
generators and silent disabled-input transform, cache with next pending, Reset
retention and ninth restore rejection after eight accepted slots.

Fixture option `ALGORHYTHMIA_AU_OWNERSHIP_FIXTURE` defaults OFF; CTest ownership
entry exists only when ON. All final trees are OFF; final AU exported symbol
inventories prove hooks absent. Frozen ON bundles retain deterministic proof.

Compatibility preserved: native IDs/seven keys, four programs/order, schema 1
exactly 40 bytes, parameter-only empty-history recall, 10 ms smoothing, shared
256-event whole-batch rejection/no replay, eight coherent restores, single audio
ownership, VST3 behavior, stereo output/generator/disabled input, release scope.
No scope or timing waiver. User steering remains the authority for host/install/
publication boundaries; no pending product decision was invented.

## Verification

`commands.json` records unique IDs, actual argv/cwd/UTC/exit/tool/log hashes.
Detailed exact-bundle reports live in `reports/`; command/log integrity verified.

| Check | Command-registry ID | Actual result | Boundary |
|---|---|---|---|
| Resume/pins | resume-comparison.json; s02-verify-juce-v2/doctest-v2 | 132 match; 4377/270 pin files match | No intervening input changes |
| Ownership reproduction | s02-ownership-baseline | 1; both writer/readback/schema/audio FAIL | Loaded baseline fixture AU |
| Conditional publication | s02-ownership-repaired-v3 | 0; both writer/readback/schema/audio PASS | Intermediate repaired bytes |
| Disabled input baseline | s02-disabled-baseline | 1; ElementCount=-10865 | Independent actual AU API |
| Bus repair | s02-bus-repair-strict-v2 | 0; 1274 comparisons | Intermediate repaired bytes |
| Final ON fixture matrices | s02-release/asan/tsan-fixture-verify | 0 each; ownership/strict/callback/VST3/seals; 3/3 native regressions | Separate Release/ASan+UBSan/TSan, generated glue |
| Final OFF matrices | s02-default-matrix | 0; 1274 each, callbacks, VST3/seals; 3/3 each | Final default behavior bytes |
| Core | s02-core-final + s02-core-stress | 0; 15/15 + 1/1 = 16 checks | Fresh Release core tree |
| Editor/APVTS/undo | s02-editor-check | 0; gestures/undo/recall plus 3072 renders/four scales | Native local fixture, no AT/DAW claim |
| Pin generation/format | s02-generation; s02-format-check | 0; 3 generators reject drift, format clean | Full pinned JUCE unchanged |
| Negative control | s02-negative-control | 1 expected; original ownership failures persist | Frozen baseline bytes |
| Integrity | s02-checkpoint-integrity | 0 | Command/report log hashes |

Final default bundle fingerprints (release_evidence.py inventory algorithm):

| Tree | AU | VST3 |
|---|---|---|
| Release | `2d1097f023aca1f3838c93ff940535852c4017681f6cb9ba00bf1d121bf22c72` | `23e7cf76e1447eef732675fd61b0778fc038e86d1b89a9836f9907d8f5514a1b` |
| ASan/UBSan | `380f9b5f1468d2b64c4cc563030e175609a746cae9bc9d7d76d1ce8bd7edd6d0` | `d31dbc0e4955e651872c3a27a95702d6f64bc935a054adf3a687514c96d36820` |
| TSan | `50793c5b9c19ee4275a2ad92033a059d75ce96d110b92681d23520682abaf2f5` | `d5db421d1cbb8bb651b75e4cacbfe8fa4b891ed02211fdce56f922f0e51040bf` |

CMake/CTest 4.4.4 absolute directory
`/Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin`.
Apple Clang 21; macOS 26.6.2/25G83 arm64; clang-format 18.1.8.
JUCE verified at `/private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850`;
doctest `build/_deps/doctest-src`. Native trees `/private/tmp/sys01-s09-repair`,
`sys01-s09-asan`, `sys01-s09-tsan`; fresh core `sys01-s09-session02-core`.
Tool/pin JSON and exact generated compile flags retained. No observed sanitizer
or race failure in finite runs. Ad-hoc seals are development integrity only.

Preserved unsuccessful attempts: wrong verify_dependency CLI flags; first
constructor compile failure; report named ownership-repaired ran old baseline
bytes after failed build (not repair evidence); editor intermediate oracle omitted
accepted offset-zero 0.8 point and failed, corrected oracle now verifies captured
block exactly; initial bus build used nonassignable JUCE Array operator[], fixed
getReference; its first repair report also tested old bytes. Core `build` was an
old monorepo cache with missing paths; sandbox failure and authorized five-test
failure retained, replaced by fresh pinned tree. Copy attempt used apps/CMakeCache
path, corrected before export. No original report was overwritten.

Unperformed: exact-byte auval (registered-ID selector cannot bind temporary AU
without prohibited personal installation); pluginval executable absent at checked
paths; actual Ableton-origin timing/stereo session tests; Intel/Windows/minimum OS,
AT/listening/musicians/signing/performance/soak. Only installed Ableton permitted;
REAPER skipped; no app installed or personal plugin directories changed. Shared
processor/editor publication changes invalidate affected AU/VST3/recall/UI/DAW/
performance/signing/soak evidence. Session-01 and historical Sprint-08 results
remain attached to their bytes; Sprint-08 live comparison stays intentionally
invalidated. No historical packet/seal was modified or resealed. MB-04 DC remains
unresolved and no human sound acceptance is claimed.

## Resume

Immediate next action: remain Sprint 09, strengthen **S09-OWN-01 ownership
acceptance** at `native_au_adapter.py` acceptance/capture/publication and
`PluginProcessor.hpp` ControlPublication. Existing post-capture real restore and
editor setter checks pass. Add deterministic loaded-AU coverage for editor write
between AU acceptance and capture, and a reentrant parameter listener that writes
newer controls during publication; verify APVTS/raw mirror, host readback,
schema40 and following-block audio. Review all participating writer/listener
paths against the ownership document before calling the ownership exit complete.
This is a precise remaining proof/coverage step, not a currently observed failure
on final bytes. The fixture has not yet tested all callback reentry interleavings.

Remaining Sprint 09 exit: reviewed complete ownership of its new publication
path, exact local regressions renewed if changes, and permitted validators.
External AU/DAW/platform cells remain unqualified; continue independent local work.
Do not start Sprint 10 until named Sprint 09 criteria are met. General complete
callback lock/listener/TLS/budget proof remains G01; offsets do not waive it.
All session-owned jobs completed; no background tool session remains. Temporary
paths may vanish, reconstruct from recorded pins/source. No global-process claim.

## Complete next session prompt

```text
Work in /Users/lhommeduchili/algorhythmia/sys-01. Continue SYS 01 v1.0.0 Sprint 09, session 03. This is an implementation/ownership-verification session; remain Sprint 09 until its named exit criteria are met.

Read docs/release-plan-1.0.0/rolling-state.md, handoffs/sprint-09-session-02.md, au-ingress-ownership-session-02.md, sprint-09-kickoff.txt and the Sprint 09/acceptance/evidence-invalidation/handoff sections of audit-production-plan-2026-10-18.md. Check status and applicable AGENTS.md; compare live inputs to evidence/sprint-09-session-02/resume-inputs.json before edits. All 132 inherited session-01 inputs matched at session-02 start; only five inherited engineering inputs changed. Preserve the substantial dirty/untracked SYS 01 and unrelated monorepo work. Git root /Users/lhommeduchili/algorhythmia; HEAD unchanged 5c45cf6bed431317a1b412b1fc12ee62b66cfdd5. No staging/reset/discard/stash/overwrite.

Current objective: finish S09-OWN-01 ownership acceptance for the new generation-aware publication. The loaded AU deterministic post-capture restore/editor interleaving was reproduced (newer 0.8 overwritten by older 0.2 in host readback/schema40/following audio) and repaired. Frozen baseline still fails. All final Release, ASan/UBSan and TSan ON fixtures pass ownership; final OFF bundles pass 1274 exact AU comparisons, callbacks, VST3/adapter/recall 3/3 and ad-hoc seals. Core 15 default plus stress smoke pass 16 checks; native editor/undo/gesture fixture passes. No observed finite sanitizer failures; G01 remains open.

Start by tracing scripts/native_au_adapter.py acceptance -> capture -> conditional publish -> nativeControlsSnapshot; PluginProcessor.hpp ControlPublication/Parameter and PluginProcessor.cpp controls/restore/acknowledgement. Every external virtual setValue allocates a unique ticket/exchanges a packed atomic word; AU acceptance records external generation, Render uses one CAS outside ingress. Permanent exhaustion disables conditional publication at 2^32-1 tickets. Successful publication notifies outside ingress; authoritative controls/save read virtual getValue. Existing restore epoch/eight slots and all locks retained. Review pinned APVTS listener getValue/raw-mirror behavior, attachments, undo/ValueTree replay, native direct/notifying setters and reentrant listeners.

Add deterministic fixture-only loaded-AU coverage for an editor setter between AU acceptance and capture, and a reentrant parameter listener publishing newer controls during publication. Assert authoritative/APVTS mirrors, host readback, schema 1 exactly 40 bytes and following-block audio. Retain any actual failure before repair. Existing hook option ALGORHYTHMIA_AU_OWNERSHIP_FIXTURE defaults OFF; frozen fixture bundles/executables in session-02 evidence. Keep hooks out of final default behavior. Do not hold ingress across listeners, remove locks without replacement ownership/reclamation proof, or claim exhaustive realtime ownership from finite passes. Review writer ownership before closing S09-OWN-01 or selecting Sprint 10.

S09-BUS-01 was separately reproduced (-10865 zero input elements) and repaired: AU accepts input ElementCount 0/1 before initialization, retains fixed processor layout slot and stereo output; zero-channel ASBD is illegal. Exact loaded-AU distinct-channel stereo transform at six rates, disabled generators/silent transform, ninth restore rejection, cache with new pending and Reset target retention now pass. Preserve native IDs/seven keys, four factory programs/order, schema40, parameter-only deterministic empty-history recall, 10 ms smoothing, shared 256-event cap/whole-batch rejection/no replay, eight coherent restore slots, single audio owner, VST3 behavior and release scope.

Verify tools/pins before use. CMake/CTest 4.4.4 directory /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin; JUCE /private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850; doctest build/_deps/doctest-src. Native trees /private/tmp/sys01-s09-repair, sys01-s09-asan, sys01-s09-tsan are hook-OFF; core /private/tmp/sys01-s09-session02-core. Reconstruct absent paths from pins/source. Final default Release AU fingerprint 2d1097f023aca1f3838c93ff940535852c4017681f6cb9ba00bf1d121bf22c72. Exact 133-input export evidence/sprint-09-session-02/source-checkpoint.tar.gz SHA256 5c89be530abc8313a732bfa2a5dfc3f2ecdf26c0783addb742cc7388b8706aaa. All session-owned jobs completed.

Use new session-03 evidence/unique command IDs; preserve session-01/02 reports and all historical failures. Renew affected strict AU/native/recall/editor and separate ASan/UBSan/TSan including generated wrappers after changes; verify hook-OFF final bytes and permitted validators. Historical Sprint-08 live-source comparison intentionally invalidated; never reseal old evidence. Release NO-GO, Sprint 09 incomplete, Sprint 10 not selected. auval cannot bind temporary AU by path; pluginval absent at recorded checked paths. Actual Ableton-origin AU timing, external architecture/OS, signing/rights/human/performance gates remain unqualified. Continue independent local work; no waiver.

Use only installed Ableton for current host tests, skip REAPER, install no testing apps, modify no personal plugin directories, contact nobody, spawn no sub-agents, publish nothing and create no tags. Before ending update compact rolling state and save handoffs/sprint-09-session-03.md from handoff-template.md with actual commands/results/hashes/failures/invalidated evidence and precise next action; provide a complete copy-paste next-session prompt. If incomplete, hand off exact implemented/tested checkpoint in Sprint 09.
```
