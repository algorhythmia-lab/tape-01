# SYS 01 / Sprint 05 / musical behavior and recall trust

2026-10-15 (America/Santiago). **PARTIAL. Release remains BLOCKED /
development-unqualified.** Repeatable musical/state/undo fixtures and a concise
user guide are implemented. Human listening, a numeric DC finding and new actual
DAW saved-project cycles remain open. No publication or release tag.

Revision `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus preserved dirty inputs.
No applicable AGENTS.md found in repository ancestors or SYS 01. Read
implementation-status, ADR 0002/0003, manifest, existing state and Sprint 04 evidence.
Sprint 00/01/02/03/04 checksum entries verify respectively 36/100/187/66/223;
superseded state hashes verify against next-sprint baseline copies. This sprint's
pre-edit state, git status and 661 scoped input hashes are retained. Historical
packets are not resealed. This explicit request supersedes historical resume-only
instructions; no other sprint is executed.

## Delivered boundary

`sys01_musical_tests` renders **44 named cases at six rates** (264 total), comparing
511- and 127-frame callback partitions exactly. It uses the existing host processor,
not a replacement engine. Cases include 24 extreme FLUX/BIAS/STATE combinations,
four factory studies, four hard recall/RESET timelines, three 17-second performance
timelines, seven prefilled reverse/detent/near-zero/forward speeds, empty-input
transform and hostile NaN/infinity input. Each steady case is 11 seconds, with
10-second settled windows after one second. It exports explicit little-endian
stereo float WAVs, 40-byte initial states, normalized timeline recipes and
peak/RMS/mean DC/max adjacent-sample delta/rail/nonfinite measurements. Per-second
and 20 ms post-event windows retain transition context; these are investigation
measurements, not a click or perceptual loudness model.

`review_musical.py` hashes raw artifacts, reports the existing .01 DC/.1 delta/
3 dB adjacent-window triggers and creates fixed -24 dB PCM16 audition copies without
normalization or fades. Eleven canonical 48 kHz auditions are retained. Early
redundant 44.1/88.2/96 kHz audition copies were generated then omitted from the
final packet; their hashes/reason are recorded. Raw measurements/hashes for all
six rates remain. Full raw renders are temporary and reproducible from source;
540 raw artifact hashes and all metrics/timelines are retained in the packet.
At 48 kHz, **90/90** raw artifacts repeat byte for byte (44 WAVs, 44 states, two
CSV files). Cross-CPU/cross-rate bit identity is not promised.

`sys01_recall_tests` exercises **72 native adapter saved-state reopens** (four
programs × six rates × three cycles), before/after prepare, exact PCM against an
independently scheduled shared processor, canonical disk state saves, same-instance
rate changes, 48 malformed/future/valid-CRC-invalid-value cases preserving running
audio as well as parameters, input-disabled generation, performance-MIDI exclusion
from saved state, and destructive empty-tape/reset behavior. The saved blobs are
independently checked using literal schema values and Python CRC32. This is **not**
an actual DAW project or loaded-format saved-project test.

The native editor fixture now records captured transform audio, calls the actual
RESET/preset/UNDO UI callbacks, and verifies exact parameter restoration with empty
destroyed tape after the existing crossfade. Existing gesture/render coverage also
passes. A literal compatibility test guards all four factory names, order and
seven parameter values. The existing studies are curated by use in
[the musical guide](../musical-guide.md); their behavior is retained without a
migration. No new programs, parameter IDs, schema semantics or persistence format.

Production DSP, mappings, gain range, limiter, processor/wrappers, state codec,
factory values, planned formats, free full distribution and deferred scope are
unchanged. No speculative sound-processing change is presented as a verified fix.

## Measurements, failures and unresolved sound

All 264 renders contain finite samples bounded to [-1,1]. Nontransition steady
studies have zero rail samples across all six rates. These facts do not establish
good sound. At 48 kHz, default magnetic/cellular/tape left RMS is approximately
.242/.1205/.08794; bytebeat/cellular energy differs about 6.06 dB. Bytebeat edges,
cellular attacks, requested +20 dB hard clipping, bypass differences and hard
recall produce review flags. Recall-1 max delta is .536378. Explicit recall remains
destructive; a fade recommendation in the guide is not acceptance of every click.

**MB-04 remains OPEN:** at 88.2 kHz the transform performance fixture's right-channel
mean over seconds 1–11 is **−.011571761286181901**, above the .01 investigation
threshold. The +20 dB clipped second (10–11) averages **−.11665187326634631**.
An independent Python oracle reads float PCM, reproduces both figures and exits
**1**. A one-case renderer reproduces the full raw waveform exactly, SHA-256
`9808db814008b27d54d7ec69f1b43a0c00e1f961381ae113ddedb438c209b0ec`.
The strict review command also exits **1**. These failures are not folded into the
green regression suite or waived as intentional. `MB-04-recipe.json`, the independent
oracle and a compressed 194,199-byte raw one-second diagnostic are retained.
Review the limiter's asymmetry and a real musical source before choosing a fix;
any fix needs a focused regression and renewed listening/rate/recall evidence.

Full per-rate maxima/flags are in `review-summary.json` and each rate's metrics.
MB-01 hard recall, MB-02 gain/bypass/level comparisons and MB-03 transform/feedback/
brake/detent listening remain open in [listening-notes-05.md](listening-notes-05.md).
No human listening, calibrated monitoring or musician participation is claimed.
Eleven prepared auditions have fixed -24 dB attenuation; actual hardware monitoring
level/device/listener/duration remain UNPERFORMED. Transient-rich stereo music and
longer seam/feedback listening are still required.

The first UI undo test failed because it incorrectly equated the departing-domain
10 ms ramp with recovered tape. Diagnosis measured a .0283604395 transient peak;
the corrected oracle allows that accepted ramp and checks later silence. No DSP
change hides it. Initial native target build failed before CMake reconfiguration;
the first one-case CLI build failed on an unused counter and its subsequent stale
executable invocation rejected the new arguments. Both were repaired and rerun;
failed logs remain. The sandboxed native suite failed two Apple AU registrations;
the authorized outside-sandbox suite passes. AU timing remains characterization
of the known Sprint 02 gap, not a timing qualification.

## Host evidence and limits

The Ableton selection call `cua.getApp("com.ableton.live")` timed out with Computer
Use error -10005 after a reported 1028.1775 seconds. No host action succeeded or
new DAW project save/reopen/rate change is claimed. Do not infer a host crash or
plugin failure from the tool timeout. Historical Sprint 02 Live reports remain
bound to their original binaries; they do not become new Sprint 05 results.
REAPER remains skipped; no other app was installed for testing.

The native build reuses `/private/tmp/sys01-sprint04-arm64` and its pinned JUCE
checkout. CMake regenerated/resealed wrappers, but executable hashes remain exactly
the final Sprint 04 candidates. New native recall/editor test binaries have their
own hashes. No fresh format-validator, DAW, Windows, minimum-machine, sanitizer,
whole-format realtime or soak claim is made by this test/documentation boundary.

| Artifact | SHA-256 |
|---|---|
| Native VST3 executable | `865127bf08ed8ddb5a3236741c81fc4733f42f18a73e33d9ee516613fa8fd27c` |
| Native AU executable | `e4642ab2ddcc24f2b7b2dab18b6213dd6b79cbeb61db513781b3ebd72b2800a0` |
| Program 0 saved bytes | `16bd2eba209b8c5a400c00bd5287ebf6a386fb4bb1ec34e2b4c51d5472a29908` |
| Program 1 saved bytes | `31e6acb42f70d34e60cfef21dca93d447464dba6dbbe5de62d857ee2518b2975` |
| Program 2 saved bytes | `bc2776361e769d32a969042d439a1b314d56c2fd8f7bb49d9607b120c24e0b29` |
| Program 3 saved bytes | `0447fbace9b34d96027d1241fb9697bd2c313e375872ab3680acc4458299fdfb` |

## Exact commands and results

[commands.json](evidence/sprint-05/commands.json) records full argv/cwd/UTC times,
exit codes and log SHA-256, including failed attempts. The packet's inventory and
SHA256SUMS bind source, documents and retained artifacts. macOS 26.6.2 build 25G83,
arm64; Apple Clang 21.0.0.21000101 / SDK 26.5; Release, strict floating point,
JUCE 8.0.12 at `29396c22c93392d6738e021b83196283d6e4d850`, doctest at
`1da23a3e8119ec5cce4f9388e91b065e20bf06f5`. Cache/compile settings and exact tool
paths are retained. Recipes from repository root (`cmake`/`ctest` resolve to the
bundled paths in commands.json):

```sh
cmake -S sys-01 -B /private/tmp/sys01-sprint05-core \
  -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON \
  -DFETCHCONTENT_SOURCE_DIR_DOCTEST=/Users/lhommeduchili/algorhythmia/sys-01/build/_deps/doctest-src \
  -DPython3_EXECUTABLE=/usr/local/bin/python3
cmake --build /private/tmp/sys01-sprint05-core --parallel 4
ctest --test-dir /private/tmp/sys01-sprint05-core --output-on-failure
cmake -S sys-01 -B /private/tmp/sys01-sprint04-arm64
cmake --build /private/tmp/sys01-sprint04-arm64 --parallel 4
ctest --test-dir /private/tmp/sys01-sprint04-arm64 -E native_editor --output-on-failure
/private/tmp/sys01-sprint04-arm64/apps/desktop/sys01_recall_tests \
  sys-01/docs/release-1.0.0/evidence/sprint-05/host-state
/private/tmp/sys01-sprint04-arm64/apps/desktop/sys01_editor_tests /private/tmp/sys01-sprint05-editor
/private/tmp/sys01-sprint05-core/libs/algorhythmia_host/tests/sys01_musical_tests \
  /private/tmp/sys01-sprint05-renders/48000 48000
python3 sys-01/scripts/review_musical.py /private/tmp/sys01-sprint05-renders/48000 \
  sys-01/docs/release-1.0.0/evidence/sprint-05/musical-48000
# Repeat render/review for 44100, 88200, 96000, 176400, 192000; exact argv retained.
# Minimal unresolved DC reproduction and independent oracle (expected current exit 1):
/private/tmp/sys01-sprint05-core/libs/algorhythmia_host/tests/sys01_musical_tests \
  /private/tmp/sys01-sprint05-dc-case 88200 performance-d2
python3 sys-01/docs/release-1.0.0/evidence/sprint-05/dc-regression.py \
  /private/tmp/sys01-sprint05-dc-case/performance-d2.wav
python3 sys-01/scripts/review_musical.py /private/tmp/sys01-sprint05-dc-case \
  sys-01/docs/release-1.0.0/evidence/sprint-05/MB-04-single-case --require-settled-dc
```

Final regression results: **core CTest 12/12**, **native CTest 17/17 outside sandbox**
(excludes the separate editor fixture), **native editor PASS** including destructive
UI undo plus the inherited 3072-state/256-word suite. Six-rate renders, independent
saved-state bytes, repeated hashes, source/theme/catalogue contracts, formatting,
strict VST3/AU local seals and preservation/checksum checks pass. **Strict DC
investigation check FAIL (exit 1)** remains a separate required finding. No green
unit-test count closes G05.

Changed existing files: desktop CMakeLists and EditorTests; host tests CMakeLists
and host_tests; implementation-status, desktop-guide and this release state.
Added: RecallTests.cpp, musical_tests.cpp, review_musical.py, musical-guide.md,
listening-notes-05.md, sprint-05.md and `evidence/sprint-05/`. Exact paths/hashes are
in its inventory. Production processor/DSP/codec/catalogue/wrappers/ADRs/manifest,
prior evidence and unrelated work are preserved. Build-only caches and new fixture
executables live in /private/tmp. No source commit/tag/publication.

## Resumable checkpoint

Sprint 05 **PARTIAL**. Start with MB-04's one-case command and independent failing
oracle; inspect high-trim DC behavior and choose a minimal reviewed fix that keeps
accepted maps, bypass/dry routing, bounds and deterministic empty-tape recall.
Retest it at six rates and add a passing focused regression when resolved. Perform
and record controlled listening for MB-01..04 using the prepared fixed-level
packet and actual stereo music, with named monitoring conditions and reviewer.
Recover Ableton UI access and run three actual project reopen cycles per study,
reset/preset/undo and sample-rate changes on exact hashes, retaining project/PCM
artifacts. Do not infer those passes from the native saved-state harness. Preserve
REAPER/no-new-apps steering, all earlier AU/realtime/UI blockers and downstream
rights/packaging scope. No participant, sound exception or release gate is invented.


Final packet sealing (run after command logging is complete, so the checksum does
not become self-referential): both commands exit 0. The checksum file excludes
itself; inventory excludes its own entry.

```sh
python3 sys-01/docs/release-1.0.0/evidence/sprint-05/seal_packet.py
python3 sys-01/docs/release-1.0.0/evidence/sprint-05/seal_packet.py --verify
```
