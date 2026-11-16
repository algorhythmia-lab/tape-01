# SYS 01 Sprint 09 session 01 handoff

## Checkpoint

Date/time: 2026-10-18 23:36 America/Santiago; commands span 2026-10-18/19 UTC.
Objective: reproduce strict AU collapsing offsets, implement bounded ingress,
verify compatible native behavior, retain exact source/evidence checkpoint.
Status: **IN PROGRESS; offsets/lifecycle locally verified, ownership incomplete**.
Release: **NO-GO / development-unqualified**. Sprint 10 not selected. G01/G02,
S07-P1-01 remain open; S09-OWN-01 is the next local objective. No gate waived.

Git root `/Users/lhommeduchili/algorhythmia`; SYS 01 root
`/Users/lhommeduchili/algorhythmia/sys-01`; HEAD
`5c45cf6bed431317a1b412b1fc12ee62b66cfdd5`. Initial status/input inventory and
final status are in `../evidence/sprint-09-session-01/`. Existing significant
tracked/untracked implementation and unrelated monorepo work preserved. No
applicable AGENTS.md found. No commits/staging/stash/reset/tag/publication.

Exact engineering export: `source-checkpoint-v3.tar.gz`, 130 files including
required untracked native sources; SHA256
`5b164d93ee9f8acfe49931cd8ad94d79fb7f87f545a9f65a7fb8d94d01f02a4a`.
`source-inventory-v3.json` SHA256
`fd318aaf32b07da35603aa5dc61c29b8cfb412b9d092a3eff21bee6747e39c26`.
Resume hash comparison file `resume-inputs-v3.json` SHA256
`55a2b77dda8d57adb47ddaf1c9532d0335576da42c231e856fa50e328944b5a8`.
This includes source plus ownership/timing checkpoint docs; rolling/handoff and
their future checksums are intentionally excluded to prevent circular hashing.

## Changes and decisions

- `scripts/native_au_adapter.py`: fixed pending parameter points/host targets,
  literal-ID mapping, bounded direct/scheduled immediate ingress, quantization,
  render epoch capture with MIDI, cached target acknowledgement, pending native
  readback/save, zero/cache length rule, bypass property, legal lifecycle target
  persistence. Existing MIDI/framework/listener locks retained.
- New `scripts/native_au_dispatch.py`: individually checksum-gated AU_2 and SDK
  dispatch generation; moves nonfinite/scheduled validation inside bounded
  wrapper instead of unbounded SDK prevalidation. Notices/upstream unchanged.
- `PluginProcessor.hpp/.cpp`: audio-owner captured control snapshot and restore
  epoch check; VST3 follows its previous path when snapshot absent. Shared DSP,
  catalogue/state encoding/keys/program order unchanged.
- `AUOffsetTests.cpp`: independent actual AU calls, strict expanded oracle.
  Characterization remains separately available. No direct ingress calls qualify
  the loaded binary.
- Desktop CMake: generated SDK compiled, strict AU CTest added; CI report runner
  now requires strict AU conformance.
- Manifest/timing checkpoint/rolling/docs record partial local repair without
  accepted timing exception. New ownership analysis names unresolved gap.

See [ownership checkpoint](../au-ingress-ownership-session-01.md) for producer,
cache/lifecycle/ramp/restore/bypass choices and exact remaining publication gap.
Serialized MPSC under retained `incomingMidiLock` is not SPSC or lock-free.
Captured storage is fixed audio-stack data; no pointer reclamation introduced.
Listeners remain outside ingress capture. No new lock object; new setter paths
use the existing ingress lock. G01 remains unqualified.

Compatibility preserved: macOS AU/VST3, Windows x64 VST3, stereo effect/generator
and disabled input, org.algorhythmia.sys01/Algr/Sys1, all seven keys/literal IDs,
four factory programs/order, schema 1 exactly 40 bytes, parameter-only empty-history
recall, 10 ms smoothing, 256 merged-event cap/whole rejection/no replay,
eight coherent restores and single audio owner. Scope/license/distribution unchanged.

## Verification

Evidence root: `docs/release-plan-1.0.0/evidence/sprint-09-session-01/`.
`commands.json` records exact argv/cwd/time/exit/tool/log hashes; all command IDs
below refer there. Each report includes complete artifact file hashes and tool
identity. Historical seals and failed reports were never overwritten.

| Check | Command IDs | Actual result | Boundary |
|---|---|---|---|
| Retained Sprint 08 boundary | retained-sprint08 | 0, 182 hashes/1182 baseline files | Before source edits |
| Pins | verify-juce, verify-doctest, pinned-dependency-final | 0; 4377/270 files match lock | JUCE 8.0.12/doctest 2.4.12 |
| Fresh strict reproduction | baseline-configure/build, baseline-strict-native | 1; direct/scheduled collapse; MIDI PASS | Rebuilt arm64 AU |
| First sandbox failure | baseline-strict | 1, factory registration unavailable | Not offset evidence |
| Expanded intermediate oracle | expanded-strict | 1, case 1190 invalid NaN batch | SDK rejected before wrapper; then repaired dispatch |
| Final native regressions | final-release-regressions, asan-regressions, tsan-regressions | 0; each 3/3 VST3/adapter/recall | Separate Release/ASan+UBSan/TSan |
| Broad strict AU | bypass-strict, asan-final-au, tsan-final-au | 0, 1249 comparisons | Before lifecycle regression addition |
| Lifecycle failure | lifecycle-strict | 1, S09-LIFE-01 pending FLUX lost | f339b0… AU; retained FAIL report |
| Final lifecycle repair | lifecycle-repair-build-2, lifecycle-repair-strict-2 | 0, 1251 comparisons | New abd6e5… AU |
| Final sanitized lifecycle | asan-lifecycle-strict, tsan-lifecycle-strict | 0, 1251 each | Generated glue included |
| Loaded AU callbacks | au-callback, asan-au-callback, tsan-au-callback, final-callbacks | 0; concurrent ClassInfo/MIDI/reset; final Release renewed | Finite workloads |
| Final old negative control | lifecycle-negative-control | 1 expected collapsing behavior | Frozen previous bundle |
| Core | core-rebuild/core-ctest | 0; 16/16 including musical | Verified unchanged shared DSP |
| Seals/conformance | final-byte-vst3, final-byte-vst3-seal, final-au-lifecycle-seal | 0 | Exact named bytes; ad-hoc development only |
| Generation/format | generation-final, final-format-check | 0; drift rejects; format clean | Pinned generated sources |
| Historical live invalidation | historical-live-invalidated | 1 expected AUOffsetTests.cpp mismatch | Historical packet not resealed |

The attempted lifecycle-repair-strict report before successful generation still
failed on the old f339… bytes; it is not evidence of the final repair. Generation
anchor failure logs (lifecycle-repair-build, asan/tsan-lifecycle-build) are retained;
the anchor was narrowed to the unique buffer-prepare sequence and rebuilt.
Two earlier fixture compile failures (missing state header and bypass switch
scope) were corrected; logs remain intact.

Final bundle fingerprints (algorithm defined by release_evidence.py inventory):

| Bundle | Fingerprint |
|---|---|
| Before AU | `936f1ecb0a82b35d2307d2932da25152f8424d196710b65e6b8eaf2e8229cdbc` |
| Final Release AU | `abd6e584f8590bd5cb0db3f9eac51f49bd125f6c87967d4c58103d214e46e53f` |
| Release VST3 | `ce988dc36ce77afdc13ca3a69c797fc138f6f08a1be1be232f5d17fc6fd404e8` |
| Final ASan/UBSan AU | `8fc461182aebf21aa0a5c4659c4f357ca706c071947e12055fe9c9a6c4fffc19` |
| Final TSan AU | `47bc28ee13684231e834b71c5b1a88a3f11d165ac73c7a1a0b7fceb4bfb6c85f` |

Tool versions: CMake/CTest 4.4.4, Apple Clang 21, macOS 26.6.2 arm64,
clang-format 18.1.8. Absolute CMake directory:
`/Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin`.
JUCE `/private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850`;
doctest `build/_deps/doctest-src`. Hashes/pins in command/dependency files.
Builds `/private/tmp/sys01-s09-repair`, `sys01-s09-asan`, `sys01-s09-tsan`.

Unperformed: actual Ableton-origin offset proof/stereo host qualification;
native Intel/Windows/minimum OS; final auval/pluginval; whole realtime listener/
locks/TLS proof, budgets/soak; sound human acceptance/signing/AT/musicians.
auval help confirms installed-ID selection, not direct temporary bundle selection.
Personal plugin directories may not change. Old pluginval/validator executables
were absent; no testing app installed. No external contacts/credentials requested.

## Resume

**Next: S09-OWN-01 deterministic loaded-AU regression and publication repair.**
Render captures a target and controlEpoch, releases ingress, then calls
publishParameter; restore/editor can publish newer controls during that gap.
The processor's epoch validation does not protect the APVTS write. No observed
TSan failure is claimed, but source-level ownership is incomplete. Introduce a
fixture-only deterministic barrier at that boundary and assert newer restore
readback/save plus next-block audio remain coherent. Keep production default free
of fixture hooks. Design generation-aware publication across participating writers;
do not hold ingress over callbacks or delete locks without proof.

Remaining Sprint 09 exit: ownership closure; exact AU stereo-transform and
disabled-input, restore overflow, cache-with-next-pending/Reset retention coverage;
renew changed strict/native/sanitizer runs and permitted validators. Local strict
offset passes do not waive G01/G02/DAW/external cells. Move to Sprint 10 only after
named Sprint 09 exit criteria are met.

All session-owned tool jobs completed; no retained background session. Process
listing was sandbox-denied, so global jobs were not certified. Temporary paths
may disappear; reconstruct verified pins/builds. Source export excludes unrelated
monorepo work; original live dirty work remains untouched.

## Complete next session prompt

```text
Continue SYS 01 v1.0.0 Sprint 09, session 02, in /Users/lhommeduchili/algorhythmia/sys-01. This is an implementation session.
Read docs/release-plan-1.0.0/rolling-state.md, handoffs/sprint-09-session-01.md, au-ingress-ownership-session-01.md and the saved sprint-09-kickoff.txt. Follow Sprint 09, acceptance, evidence invalidation and handoff rules in audit-production-plan-2026-10-18.md.
Preserve all significant dirty/untracked SYS 01 and unrelated monorepo work. HEAD remains 5c45cf6bed431317a1b412b1fc12ee62b66cfdd5. Compare inputs with docs/release-plan-1.0.0/evidence/sprint-09-session-01/resume-inputs-v3.json. Historical Sprint 08 live comparison is intentionally invalidated; never reseal old packets.
Current objective: close S09-OWN-01, captured AU targets publishing over a newer concurrent restore/editor generation. Start by tracing scripts/native_au_adapter.py Render capture -> publishParameter -> nativeControlsSnapshot and PluginProcessor.cpp restore/epoch acknowledgement. Add a deterministic fixture-only loaded-AU barrier regression proving newer native readback, saved schema state and next-block audio remain coherent; implement bounded generation-aware publication with justified participating writers. Do not hold ingress over reentrant listeners or remove locks without ownership/reclamation proof.
Verified checkpoint: ordinary/scheduled immediate offsets and legal lifecycle target retention pass 1251 independent strict AU comparisons on arm64 Release, ASan/UBSan and TSan; VST3/adapter/recall pass 3/3 in all three trees; core 16/16; final AU callback PASS; previous AU negative control still fails. S09-LIFE-01 was reproduced then repaired by lifecycle target flush; before failures remain. Final AU fingerprint abd6e584f8590bd5cb0db3f9eac51f49bd125f6c87967d4c58103d214e46e53f. Release remains NO-GO; Sprint 09 incomplete, Sprint 10 not selected.
Tools: CMake/CTest 4.4.4 at /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin; verified JUCE at /private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850; doctest build/_deps/doctest-src. Trees /private/tmp/sys01-s09-repair, sys01-s09-asan, sys01-s09-tsan may disappear; reconstruct from pins/source. No session jobs remain. New evidence/log IDs must be unique.
After ownership repair finish exact AU stereo-transform/disabled-input, restore overflow, cache-with-new-pending and Reset target retention fixtures; rerun affected strict/native/sanitizer checks and permitted exact-byte validators. Preserve IDs/keys, four program order, schema 1 exactly 40 bytes, parameter-only deterministic empty-history recall, 10 ms smoothing, 256 merged events/whole rejection/no replay, eight restores/single audio owner, VST3 behavior and release scope.
Continue independent local work when external fixtures are absent. Only installed Ableton for current host tests; skip REAPER, install no other testing apps, do not modify personal plugin directories/contact people/spawn sub-agents/publish/tag. No AU timing waiver or gate waiver. Whole native realtime/DAW/platform/signing/rights/human gates remain open.
Before ending update compact rolling state, save handoffs/sprint-09-session-02.md with actual commands/results/hashes/failures/invalidated evidence and precise next action, and return a complete copy-paste next prompt. Move to Sprint 10 only after Sprint 09's named exit criteria are met.
```
