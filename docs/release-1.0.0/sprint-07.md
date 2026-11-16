# Sprint 07 / host, accessibility and musician qualification

2026-10-15 (America/Santiago). **PARTIAL. Release BLOCKED /
development-unqualified. Three assigned P1 issues remain open.** Available local
candidate checks and a real Live generation/save/reopen/bounce cycle were run.
The owner confirmed only themselves and this computer are available. Five-musician,
external-machine, native screen-reader, complete host and signed clean-system
tests remain pending. No requirement was downgraded. No tag/publication/commit.

Revision `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus preserved dirty inputs.
Required implementation-status, ADR 0002/0003, manifest and previous state were
read; no applicable ancestor or SYS 01 AGENTS.md was found. Initial Git status,
1,121 file hashes and the prior state are retained in
[baseline.json](evidence/sprint-07/baseline.json) and baseline-state.md.
All work added by this sprint is under SYS 01's release ledger/evidence; only the
existing state.md is edited. Production/test foundations, IDs, catalogue, schema,
accepted formats, free distribution and deferred scope are preserved.

## Exact candidate and prerequisites

The existing Sprint 06 macOS arm64 development packet is the candidate; it was
not rebuilt, signed or repackaged. The full Sprint 06 verifier passed **268/268**
hashes and report bindings **before editing state.md**. Its saved state checksum
is now historical and matches this sprint's baseline-state.md; do not reseal it.
All seven earlier packets' retained internal evidence was checked:
**34 / 96 / 184 / 35 / 214 / 143 / 154** entries for Sprints 00–06.
Candidate build inputs still match current source/provenance. Details/tool hashes:
[prerequisite-verification.json](evidence/sprint-07/prerequisite-verification.json).

| Artifact | SHA-256 |
|---|---|
| Existing packet ZIP | `cf4f2515a81c35751c7dcfc2a37d867f8eee9353f40e62e938724146cde7ecd4` |
| candidate.json | `0b348fb85761e2bab4267247c50b93d4dfde02ced4b2cbbf6e9b020a26dba21f` |
| VST3 executable | `bdf225221f23bd28c75394f814cc212d0c922bc563b2c08a281fa5b176dbd968` |
| AU executable | `709acf92683d65c5de0ed712cca7cf6173d1c20114615d4e1b98aef3ce334996` |
| Complete VST3 file-content fingerprint | `5c08c3b817d1560a8329dedede5ef51f3037b96a9d0414a8574434e4516ea597` |
| Complete AU file-content fingerprint | `ec455f7a09f7d6f10522bb3e04bd2d7065b063c92ea826d54f6ab37f84231382` |
| New Live project | `f6d41e9ebe78c48e75a239c7cd8ef4bb5e1cfc3fa3c247c70383d0f8676ccba2` |
| First / reopened bounce PCM data, identical | `737ab43b9b358c899c0b1c4af4202cc2dcf22dcb4e24a01c5eb9ea5754b0699c` |

The existing eight validators/reports remain attached to these same bytes,
including the AU strict failure. A copied VST3 in an owned temporary custom
folder was fingerprinted before host use. lsof confirmed Live loaded that path;
the personal plugin directories were not modified. Ad-hoc seals are not release
signatures. Windows binaries/packages, native Intel/minimum macOS, clean fixtures,
Developer ID/Windows signing and prior signed upgrade version remain unavailable.

## Fresh results and limits

| Check | Observed result |
|---|---|
| Exact loaded VST3 | PASS exhaustive 201,894 points, seven stable IDs, six rates, offsets/blocks, rejection/replay, MIDI, lifecycle/editor/concurrent restore fixture; not a real-DAW offset proof |
| Exact AU callback | Sandbox registration FAIL; normal macOS framework access retry PASS, in-process only, no installation |
| Exact AU strict ordinary/scheduled offsets | **FAIL**, both collapse to offset zero; positioned CC22 passes. Accepted AU contract remains unmet |
| Source-matched native adapter / recall | PASS; positioned MIDI, rates/blocks/concurrency plus 72 reopens, four programs/six rates/three cycles, exact PCM, malformed state and empty-tape semantics. These are linked fixtures, not DAW projects or loaded AU/VST3 recall proof |
| Source-matched native editor | PASS 3,072 state/scale renders, 256 register words, native glyph extents, typing/gesture/undo and hidden/reopened timer assertions. 49 PNGs inventoried, five retained and representative 200% face/details inspected |
| Native offscreen UI timing | max updateView **0.3835 ms**, full snapshot **23.9760 ms**, mean **1.1094 ms**; snapshot exceeds 16.67 ms target. Includes snapshot allocation/raster work; not isolated paint, input response or declared-load host qualification. Earlier larger outliers remain |
| Live 12.4.6 / current macOS arm64 / exact VST3 | Scan/load PASS scoped; disabled-hardware-input generation and host FLUX/BIAS/domain edits observed; one real saved-project reopen retains values; two offline generation PCM payloads identical |
| Complete host/visual/input/AT | **PENDING**; floating editor absent from captured main window; host generic controls/AX values do not qualify native editor, VoiceOver or Windows reader |
| Musician / listening | **PENDING**, zero first-time participants or listening results; owner's availability is not a five-musician test |
| Eight-instance, eight-hour sustained DAW workload | **UNPERFORMED**; no shortened/offline replacement claimed. Dense one-instance diagnostic below fails budget |
| Real signed install/upgrade/uninstall/rollback | **UNPERFORMED** on all claimed cells; development custom-folder scan is not package lifecycle evidence |

Live used macOS **26.6.2 / 25G83**, M1 Pro / arm64 / 16 GB, full host build
**12.4.6 (2026-09-10_0de5c8fa9a)**, CoreAudio, no input device, system MacBook Pro
Speakers output, **48 kHz / 512 frames**. This differs from the agreed soak's
128-frame workload. The project selected cellular, FLUX/BIAS .507874012, DRIVE 1,
STATE 0, DOMAIN .5, OUTPUT .520547926, BYPASS 0; independent schema-1 CRC verifies.
The 32-bit float stereo bounce is **390,000 frames / 8.125 s**, including the
one-sixteenth remainder after four bars at 120 BPM. An attempted zero-remainder
edit did not persist. File size/frames take precedence over attempted UI input.
Peak L/R **.0647313222 / .0618666597**, RMS **.0142607993 / .0156998233**, zero
nonfinite samples. This is neither controlled listening nor MB-04 sound acceptance.

Both complete WAV files and the actual .als are retained; WAV file hashes differ
in metadata while PCM matches exactly. See
[host-artifacts.json](evidence/sprint-07/host-artifacts.json) and
[UI observations](evidence/sprint-07/live-ui-observations.md). CUA returned repeated
timeout/noWindowsAvailable errors around export/reopen, then recovered. Live
remained running; a one-second read-only stack sample mostly shows normal main
event-loop waiting. No crash/deadlock cause is established. Only the saved owned
test set was closed; final Live Untitled/empty device view, stopped transport and
silent peaks were observed. The custom VST3 source was verified Off again; its
remembered temporary path remains. Export preferences were restored to observed
original 44.1 kHz/16-bit values; the Cancel observation timed out. No other app
was installed and REAPER remains skipped.

## Callback observations; G01/G07 stay open

The existing probe-free loaded VST3 fixture ran twice on the exact candidate,
20,000 callbacks per condition, one dedicated ordinary std::thread, 48 kHz,
**224 ordinary automation points + 32 positioned CC21 events = 256 merged events**.
Lifecycle/editor/concurrent restore work occurs before the timing loop. Durations
are unadjusted and unpaced; this is not a CoreAudio driver run. It does not cover
eight instances, eight wall-clock hours, AU timing or sustained restore/UI churn.

| Run | Editor / frames | p50 / p95 / p99 / p99.9 (µs) | Observed max (µs) | Deadline misses / half-budget breaches |
|---|---|---|---:|---:|
| Initial sandbox | open / 128 | 34.834 / 124.833 / 389.125 / 1348.458 | 8485.792 | 4 / 20 |
| Initial sandbox | open / 64 | 24.708 / 82.417 / 230.792 / 815.792 | 2795.333 | 4 / 29 |
| Initial sandbox | closed / 128 | 34.791 / 115.416 / 283.417 / 1311.166 | 5785.708 | 6 / 19 |
| Initial sandbox | closed / 64 | 24.667 / 78.584 / 202.083 / 727.625 | 3178.500 | 6 / 22 |
| Native access retry | open / 128 | 32.625 / 35.375 / 47.875 / 75.583 | 114.000 | 0 / 0 |
| Native access retry | open / 64 | 23.333 / 25.459 / 37.583 / 68.375 | 129.625 | 0 / 0 |
| Native access retry | closed / 128 | 33.333 / 36.084 / 57.625 / 155.000 | **14314.625** | **2 / 6** |
| Native access retry | closed / 64 | 23.083 / 24.750 / 30.500 / 66.917 | 91.750 | 0 / 0 |

Clock-pair overhead initial p50/p99/max **.041/.042/19.208 µs**; native retry
**.000/.042/.084 µs** (clock quantization permits zero). Thresholds are unchanged:
128-frame deadline 2666.667 µs / half-budget 1333.333 µs; 64-frame deadline
1333.333 µs / half-budget 666.667 µs. Across these finite diagnostics, **22 misses,
96 half-budget breaches** are retained. Initial sandbox XPC warnings and overlap
with the Live launch attempt are recorded; background apps, power/thermal state
and scheduling were uncontrolled. No causal editor comparison is inferred.
Observed failures require diagnosis, not deletion of an outlier or a lower budget.
Finite testing cannot prove a hard realtime bound. Existing wrapper locks and
first-thread TLS/allocation/listener concerns remain independently open.

## Assignments, omissions and next actions

[issues.json](evidence/sprint-07/issues.json) assigns **S07-P1-01** AU offsets to the
format engineer, **S07-P1-02** observed timing breaches to audio/platform QA and
**S07-P1-03** retained complete-callback ownership/locks to the audio engineer.
These are accountable roles, not invented staff or spawned agents. No confirmed
defect was declared fixed. MB-04 DC/listening, snapshot timing and UI-tool failures
also retain explicit investigation owners. No speculative DSP/wrapper/UI change
was made to manufacture a pass.

[qualification-matrix.json](evidence/sprint-07/qualification-matrix.json) retains
all **16** original OS/CPU/DAW/format cells and every requested check. REAPER
cells remain unperformed under owner steering; they are not removed or passed.
No validated host/platform array is populated. Exact current-candidate transform,
recorded DAW automation and timed MIDI, internal bypass, all-study/multiple-cycle
reopen, full lifecycle, host-native visual/focus/keyboard/AT and complete native
callback/soak remain unqualified. Older Live AU/VST3 reports keep their original
hashes. No older binary evidence is relabeled for this candidate.

The [pending manual protocol](evidence/sprint-07/manual-protocol.md) supplies the
unchanged host/scale/keyboard/VoiceOver/Windows reader tasks, eight-hour workload,
five-musician completion/confusion/recall record requirements and three-cycle
signed package lifecycle criteria. All unavailable human/device tests are pending.
First-minute acceptance stays >=4/5 within 60 s without intervention; transform
>=4/5; all five must understand parameter-only recall after concise guidance.
No results or confusion/understanding were fabricated.

Resume: verify the immutable candidate and packet; diagnose assigned P1 issues
with their exact failing reports, preserve ownership proofs and apply focused
fixes only after diagnosis. Any rebuild/signing invalidates affected native/
host/validator/timing/UI evidence: retain prior bytes/reports, assign a new hash,
rerun affected checks before reusing the matrix. Recover actual host editor access
and complete current-Mac transform/automation/MIDI/bypass/all-study lifecycle and
AT first. Obtain missing musicians and clean external fixtures/credentials/prior
signed packages for remaining cells. Perform the actual sustained workload;
do not relabel this short diagnostic as a soak. Rights/source and sound gates also
remain open. Exit qualification only with agreed gates met and no P1 remaining.

## Commands, changed files and resumable checkpoint

[commands.json](evidence/sprint-07/commands.json) contains exact expanded argv,
cwd, UTC times, exit codes, tool/log hashes, including failed commands. Commands
ran from `/Users/lhommeduchili/algorhythmia`. Principal invocations:

```sh
python3 sys-01/docs/release-1.0.0/evidence/sprint-06/seal_packet.py --verify
python3 sys-01/docs/release-1.0.0/evidence/sprint-07/audit_prerequisites.py
/private/tmp/sys01-sprint06/isolated/build/apps/desktop/sys01_vst3_contract \
  'sys-01/dist/sys01-sprint06-macos-arm64/binaries/SYS 01.vst3' --profile
/private/tmp/sys01-sprint06/isolated/build/apps/desktop/sys01_au_contract \
  'sys-01/dist/sys01-sprint06-macos-arm64/binaries/SYS 01.component'
/private/tmp/sys01-sprint06/isolated/build/apps/desktop/sys01_au_offsets \
  'sys-01/dist/sys01-sprint06-macos-arm64/binaries/SYS 01.component' --require-positioned
/private/tmp/sys01-sprint06/isolated/build/apps/desktop/sys01_editor_tests /private/tmp/sys01-sprint07/editor-evidence
/private/tmp/sys01-sprint06/isolated/build/apps/desktop/sys01_adapter_tests
/private/tmp/sys01-sprint06/isolated/build/apps/desktop/sys01_recall_tests /private/tmp/sys01-sprint07/recall-evidence
python3 sys-01/docs/release-1.0.0/evidence/sprint-07/collect_host_artifacts.py
python3 sys-01/docs/release-1.0.0/evidence/sprint-07/verify.py
python3 sys-01/docs/release-1.0.0/evidence/sprint-07/verify.py --require-qualified
git diff --check -- sys-01
```

Loaded-binary commands were wrapped by existing validate_artifact.py; its five
new JSON reports bind unchanged artifact inventories before/after execution.
VST3 profiling commands exit 0 for conformance/collection while the independent
timing analysis **FAILS**; the strict AU command exits **1**. Initial sandbox AU
registration also exits **1**, authorized normal-framework retry exits **0**.
Other listed fixture/artifact/integrity commands pass. `--require-qualified`
exits **1** intentionally because the candidate does not meet gates. GUI actions,
failed setValue/observations, actual values and recovery are recorded separately
in live-ui-observations.md. No new sanitizer/full build was warranted by the
evidence/document-only boundary; prior sanitizer/validator scopes remain explicit.

Changed existing: `sys-01/docs/release-1.0.0/state.md`. Added: this sprint ledger
and `evidence/sprint-07/` (source helpers, baseline, exact reports/logs, hash
inventories, measurements, matrix, assignments, protocol, project/two WAVs and
representative source-fixture PNGs). `inventory.json` / `SHA256SUMS` enumerate
exact changed/added paths and hashes; all other baseline hashes are preserved.
Final `verify.py --seal` then `verify.py --checksums` run directly after logging,
avoiding a self-referential command/checksum log. Earlier packets are never resealed.

**Checkpoint: Sprint 07 PARTIAL; qualification/release BLOCKED.** Local exact-byte
evidence is retained; three P1 issues are assigned and open; unavailable human/
device/package/host/soak tests remain pending. Start with `verify.py --checksums`,
then the failing strict AU and timing reports. Do not publish or tag.
