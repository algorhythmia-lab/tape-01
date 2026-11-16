# SYS 01 // SPRINT 04 // NATIVE INSTRUMENT INTERFACE

2026-10-14 (America/Santiago). **PARTIAL.** Native implementation and local
fixtures are delivered. Full DAW interaction/scaling, pointer tasks, assistive
technology and performance qualification remain open. Release stays **BLOCKED /
development-unqualified**. No tag, publication or scope exception is authorized.

Revision `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus the preserved dirty
workspace. No applicable AGENTS.md was found in repository ancestors or SYS 01.
Read implementation-status, ADR 0002/0003, manifest, prior state, Sprint 03 native
UX contract, layouts, catalogue, native tokens, mapping helpers and telemetry
contract. The current sprint supersedes historical resume-only instructions.

## Prerequisites and preserved boundaries

Before changes, Sprint 03 verified 66/66 checksum entries; Sprint 00 35/36,
Sprint 01 99/100 and Sprint 02 186/187 matched, each sole difference the live
state ledger superseded by the next sprint. Historical ledger copies are checked
separately. See `evidence/sprint-04/prerequisite-verification.json` and
`historical-chain.json`. The pre-edit state, git status and **806** baseline
input hashes are retained. Prior packets are not silently resealed.

Only the native editor/build/test boundary changes. Seven stable parameters,
IDs, catalogue/preset order, DSP mappings, shared STATE, schema 1, coherent
restore, positioned wrappers, audio ownership, free full distribution, planned
formats and deferred scope remain unchanged. No general UI framework, DSP
mapping replacement, bundled fonts or new host parameters were introduced.

## Implemented

- Planar session/performance/signal bands; equal restrained vector spools;
  exactly eight b7..b0 elements with literal bits, domain color and actual
  preview/word telemetry. Unknown and stale registers are explicit.
- One centered uniform canvas transform at 100/125/150/200%; arbitrary sizes
  letterbox. The scale indicator reports the actual fitted transform, including
  nonpreset sizes. Minimum remains 640x440. Local preference storage was not
  added; new editors default to 100%.
- `EditorValues.hpp` owns mapped formatting, strict decimal/unit parsing and
  display history. It delegates transform detent and cellular rules to existing
  DSP helpers and output to `trimDb`. FLUX units stay beside the number; BIAS
  says `% MACRO`, with its coupled effects explained in context/DETAILS.
- Existing APVTS slider/button/choice attachments retained. Native slider,
  numeric edit, radio, toggle and dialog roles remain. Relative vertical/fine
  drag, focused wheel, keyboard steps/repeat grouping, mapped entry, default
  reset, undo and balanced gesture paths are implemented. Zero vertical wheel
  events are ignored. Numeric validation never clamps or writes invalid input.
  Typing survives host target changes; domain changes cancel unit-specific entry.
- A real defect found by the fixture was repaired: APVTS defers tree flushes,
  making immediate undo unavailable. User gesture/click/recall boundaries now
  flush its existing tree outside audio, preserving attachment/undo ownership.
  No alternate state or undo framework was introduced.
- Independent input, one-second limiting hold, cumulative rejection/history,
  telemetry health and brake pending/confirmed displays. New rejection history
  cannot mask clipping/brake/input. ACKNOWLEDGE changes only the UI unresolved
  marker; it never claims applied targets or repairs sound. Connectivity is
  explicitly UNKNOWN; observed quiet is reported separately after 500 ms.
- Modal, keyboard-scrollable DETAILS with CLOSE/Escape/focus return; saved vs
  actual domain/run, mapping/recovery, fixed performance-only CCs, timing limits
  and persistent parameter-only recall/undo consequences. No repetitive modal
  confirmation is added to presets/reset.
- Four real sampled IN L/R and OUT L/R peak lanes from the existing separate
  arrays, only in DETAILS. UI decay is 20 dB/s, elapsed time capped at 250 ms;
  lost/stale data shows `--`, silence `-INF`. No RMS/VU/true-peak claim.
- One bounded mailbox consumption at 30 Hz while showing. Reopen requires a
  later advancing sequence before LIVE; counters rebaseline on reset and compare
  modulo wrap. Timer stops while hidden/destroyed, gestures end and brake uses
  the existing release path. Register/status/meter regions repaint independently.
  Fault speech is coalesced to at most once/second; no register speech stream.

## Exact commands and results

Every recorded check's full argv, cwd, UTC timestamps, exit code and complete log
hash is in [commands.json](evidence/sprint-04/commands.json). Local CMake/CTest and
clang-format binaries are fully expanded there. `record_command.py` retains the
local recorder. Principal recipes, from repository root:

```sh
cmake -S sys-01 -B /private/tmp/sys01-sprint04-arm64 -G 'Unix Makefiles' \
  -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON -DALGORHYTHMIA_BUILD_DESKTOP=ON \
  -DALGORHYTHMIA_JUCE_SOURCE=/private/tmp/sys01-sprint00-juce \
  -DFETCHCONTENT_SOURCE_DIR_DOCTEST=/Users/lhommeduchili/algorhythmia/sys-01/build/_deps/doctest-src \
  -DPython3_EXECUTABLE=/usr/local/bin/python3 \
  -DCMAKE_OSX_ARCHITECTURES=arm64 -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0
cmake --build /private/tmp/sys01-sprint04-arm64 --parallel 4
ctest --test-dir /private/tmp/sys01-sprint04-arm64 -E native_editor --output-on-failure
/private/tmp/sys01-sprint04-arm64/apps/desktop/sys01_editor_tests /private/tmp/sys01-sprint04-guard-editor
python3 sys-01/scripts/check_format.py --check --clang-format <exact-bundled-path-in-commands.json>
codesign --verify --deep --strict --verbose=2 '<exact-final-bundle-in-commands.json>'
git diff --check -- sys-01
python3 sys-01/docs/release-1.0.0/evidence/sprint-04/seal_packet.py
python3 sys-01/docs/release-1.0.0/evidence/sprint-04/seal_packet.py --verify
```

| Boundary | Result / limit |
|---|---|
| Final Release regression suite | 15/15 PASS outside sandbox; includes existing loaded VST3/AU and core/host/source/theme/catalogue checks. AU characterization is not timing parity. |
| Final editor fixture | PASS; 3072 native state/scale renders, all 256 register values, strict parser/detent/rule boundaries, actual font extents, automation during typing, cancel/domain change, balanced numeric/keyboard/accessibility gestures, immediate undo/redo, close mid-gesture, no timer gestures, stale/drop/wrap/meter decay, hidden/reopened timer. |
| Capture coverage | 49 representative native component PNGs, 3328 raw render hashes and 9216 native glyph-extent rows; all domains and important faults/unknown/stale/preview/invalid/DETAILS at all four scales, plus 1000x700 letterbox. Input-missing matrix cells retain truthful UNKNOWN, not invented NO INPUT. |
| Native seals / formatting / packet | Final strict local development seals, first-party format check, diff hygiene, preservation and packet checks recorded below; no release-signing claim. |
| Failed attempts | First build lacked test include propagation (repaired); first editor test found immediate undo unavailable (repaired); one build used incorrect nested JUCE accessibility type name (repaired). An accidentally overlapping build was interrupted with Ctrl-C. Initial sandbox suite was 14/16: AU registration failed; the same two tests passed outside sandbox. All failures remain in evidence. |

The fixture uses actual JUCE rasterization/system font **Lucida Grande / Regular**
on macOS 26.6.2 arm64, Apple M1 Pro. Exact OS/compiler metadata is in
`platform.txt`; actual font file/version and fallback glyph versions remain
unresolved. No font files are distributed. Component PNGs are native editor
captures with deterministic injected snapshots, not a DAW/audio workload. Input
connectivity and applied continuous DSP values are not fabricated.

## Native host observations and measured limits

`native-final-*` screenshots/AX trees bind the named accessibility candidate in
`accessibility-candidate-identities.json`. Native accessible actions changed a
cellular FLUX target 180 -> 200 BPM and UNDO restored 180. Domain/STATE/brake/dry
bypass coexisted; keyboard traversal + Enter typed `-1 X`, Up changed it to
`-0.96 X`, invalid `NaN` stayed local without moving the slider, Escape recovered.
All four scale indicators changed; DETAILS exposed a modal dialog, mapped target
speed +0.96 X for negative FLUX/STATE 1, independent L/R descriptions, and Escape
returned focus to DETAILS. RESET recovered default targets and released brake.
The final wheel-only guard candidate (`guard-native-*`, final identities below)
separately repeated native value/undo, all four scale indicators/captures, real
Tab/Enter typed entry to 5000 TICKS/S and RESET. No DAW pass is transferred from
an earlier executable. The detailed negative-speed/invalid/modal observations
remain explicitly bound to the prior accessibility candidate.

**Native 200% visual qualification is not a pass:** the real window capture has
white/offscreen regions at its saved desktop position, cutting off right/bottom
controls in that capture, despite correct indicator and complete offscreen
component renders. Host placement/refusal/reachability must be resolved in real
DAW tests, not excused by the uniform-transform math. 125/150% captures and all
four deterministic component dimensions are retained. Pointer coordinate/AX
click attempts did not establish field focus or drag/fine/wheel/default reset;
keyboard entry and native value actions did. Do not infer pointer coverage.

Ableton Live 12.4.6 arm64 scanned and loaded the exact named VST3 candidates;
scanner/load excerpts and native trees retain the installed path/class ID.
The final wheel-only guard was verified in the native development harness, not
rerun in Live; its complete loaded VST3/AU fixtures did pass.
Show/Hide Plug-In Window reported ON, but the tool returned only the main Live
window; floating editor interaction was not observed. No completed DAW scaling,
gesture recording, pointer, offline audio or reopen qualification is claimed
by this sprint. Only owned empty Untitled sets were discarded. Existing test
apps were used; REAPER stayed skipped and no additional testing app was installed.

Timing is a short unpaced one-instance offscreen diagnostic, not a native
callback/DAW/driver or agreed minimum-machine workload. Earlier full snapshot
captures measured max **51.2571 ms** (mean 1.6907) and **75.4727 ms** (mean 1.5126);
update maxima were 1.1557 and 1.5303 ms. The later named candidate measured max
4.8690 ms, mean .8197, update max .0388. The final guard run measured max snapshot **5.4364 ms**, mean .8105 ms and max update .0492 ms; its report is retained separately.
These include full snapshot allocation/raster work, not isolated compositor
paint timing. Earlier overruns remain evidence; a later lower maximum does not
waive them. End-to-end <=33.33 ms response and timer+paint <16.67 ms under declared
load, separate native paint timing and an instance/minimum-machine budget remain
**UNQUALIFIED**. macOS XPC service warnings from sandboxed fixture runs are
retained; no screen-reader pass is inferred from an exit code.

No VoiceOver/Windows screen-reader session, all-scale keyboard task matrix,
announcement cadence listening, font-version/fallback audit, DAW resize refusal,
real pointer/fine drag/wheel/double-click, recorded host gestures, closed-editor
CPU profiling, controlled-load timing, musician test, listening/beta, Windows or
native Intel qualification was completed. Source assertions and the hidden timer
test do not measure whole-host CPU. Existing AU/realtime and other release gates
remain open. No sanitizer/validator rerun beyond the stated fixtures is claimed.

## Final binary identities

| Development artifact | executable SHA-256 |
|---|---|
| Final VST3 | `865127bf08ed8ddb5a3236741c81fc4733f42f18a73e33d9ee516613fa8fd27c` |
| Final AU | `e4642ab2ddcc24f2b7b2dab18b6213dd6b79cbeb61db513781b3ebd72b2800a0` |
| Final native harness | `c83a5fa2574fd1db9c68f4e6423f33ae7bafd9218ae380e3a5e1a24c81341456` |
| Accessibility/Live candidate VST3 | `5fbe2d5a0661dc4f9d686aa9ca888181e1b3b7fa48887d0a59969ce28f9f29dd` |

`final-native-bundles.tar.gz` retains final VST3/AU/development harness bundles;
`accessibility-candidate-bundles.tar.gz` retains the earlier named binaries.
Final bundle archive SHA-256:
`6c03943746edd25364ca8242c68b26d3f177b70233a5f398a3bfd8a9862c9cd2`.
Earlier accessibility bundle archive SHA-256:
`798c23cc9a3ca4c84ca92c77f095705d98a30d8ada6816330c7388b87feeb35f`.
All source/capture hashes are in the inventory. Final local
build is `/private/tmp/sys01-sprint04-arm64`; retained archives survive deletion
of this temporary build. Only the hash-verified Sprint 04 Library scan copy was
moved back to `/private/tmp`; no unrelated plugin was removed. Native harness
preferences may retain its default parameter state; no captured audio is saved.

## Changed files, hashes and resumable checkpoint

Changed existing runtime files: `apps/desktop/PluginEditor.cpp`, `PluginEditor.hpp`
and `CMakeLists.txt`. Added `EditorControls.cpp`, `EditorValues.hpp`,
`EditorTests.cpp`. Updated implementation-status, state and this ledger; added
`docs/release-1.0.0/evidence/sprint-04/`. Exact enumerated paths, byte sizes and
SHA-256 values are in `inventory.json`/`SHA256SUMS`; binary identities and retained
bundle archive hashes are separate entries there. Prior dirty processor, wrappers,
DSP, format fixtures, ADRs, manifest and unrelated work were preserved.

Checkpoint: Sprint 04 **PARTIAL**. Implementation + deterministic/native-harness
checks delivered; DAW floating UI, pointer tasks, 200% window placement and full
accessibility/performance remain open. Resume with `native-ux-spec.md` UX-03,
UX-08..14 and this packet. Obtain actual host editor access, reposition/qualify
all scales, record pointer/keyboard/automation gestures and native assistive
technology, then profile separately under a declared audio/instance workload.
Keep this scoped to Sprint 04; do not repair AU/realtime, add deferred features,
publish or tag. Source-export PNG/CSV/CJS/native-evidence packaging remains the
recorded Sprint 06 follow-up.
