# SYS 01 // SPRINT 03 // NATIVE DESIGN AND INTERACTION SYSTEM

2026-10-14 (America/Santiago). **COMPLETE at the design-contract boundary.**
native UI implementation and qualification remain pending. release remains
**BLOCKED / development-unqualified**; sprint 01/02 remain PARTIAL.

revision `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus preserved dirty work.
no applicable AGENTS.md exists in repository ancestors or SYS 01 descendants.
read implementation-status, ADR 0002/0003, manifest, prior state and sprint
ledgers, the brand aesthetic/tokens, native tokens, TAPE tokens/physical spec,
existing editor/catalogue/DSP helpers and immutable telemetry contract.
[design-inputs.json](evidence/sprint-03/design-inputs.json) binds exact design
sources. [baseline-inputs.json](evidence/sprint-03/baseline-inputs.json) retains
423 pre-existing SYS 01 input hashes; initial git status and pre-sprint state
are retained alongside it. no baseline, source foundation or historical evidence
packet was rewritten to erase another sprint's work.

## DELIVERED CONTRACT

[native-ux-spec.md](native-ux-spec.md) defines authority precedence, semantic
component states/colors, type/font-rights inventory, control geometry, four
uniform scales, direct mapped entry and keyboard/host gestures. it separates
seven saved host targets from transient CC20–23 and actual DSP telemetry;
preserves eight true register elements, fixed domains/shared binary STATE,
true transform detent and optional measured per-channel meters; limits DETAILS
to useful mapping/recovery information. independent status slots permit brake,
bypass, missing input, limiting, modified preset and rejection together. no
simulated transport, logo, sync, new parameter, DSP or schema is introduced.

[layout index](native-ux-layouts/README.md) provides three annotated SVG/PNG
boards for session, performance and signal, six total SVGs, twelve full-size
scale PNGs, authored scene/geometry JSON, reproducible generation/verification
scripts and an explicit 3072-case native acceptance CSV. every native case is
PENDING. the contract has 15 acceptance rows with separate design/native exits.
640×440 minimum is unchanged; the existing header accommodated a measured wider
DETAILS button without shrinking or hiding controls.

## PREREQUISITE AND BOUNDARY CHECKS

[prerequisite verification](evidence/sprint-03/prerequisite-verification.json):
sprint 00 35/36 hashes match, sprint 01 99/100 match; each sole mismatch is the
live state ledger intentionally superseded by sprint 02. sprint 02 187/187 match
before edits, including its live state. each of the three historical state hashes
also matches its next-sprint baseline copy; the full prerequisite chain verifies.
historical checksums remain unchanged.
this establishes retained evidence integrity, not new execution of native
validators or closure of AU/realtime/host gates.

all exact argv/cwd/UTC timestamps/exit codes and log hashes are retained in
[commands.json](evidence/sprint-03/commands.json). commands below run from repository
root; local node/CTEST paths in the evidence are fully expanded rather than
relying on PATH.

```sh
python3 sys-01/docs/release-1.0.0/native-ux-layouts/build_layouts.py
node sys-01/docs/release-1.0.0/native-ux-layouts/render_layouts.cjs
node sys-01/docs/release-1.0.0/native-ux-layouts/check_layouts.cjs
python3 sys-01/docs/release-1.0.0/native-ux-layouts/check_contract.py
python3 sys-01/scripts/export_theme.py --check
python3 sys-01/scripts/check_catalogue.py /private/tmp/sys01-sprint02-arm64/libs/algorhythmia_host/tests/host_catalogue sys-01/docs/parameters.md
git diff --check -- sys-01
python3 sys-01/docs/release-1.0.0/evidence/sprint-03/seal_packet.py
python3 sys-01/docs/release-1.0.0/evidence/sprint-03/seal_packet.py --verify
```

| check | result and limit |
|---|---|
| offline SVG generation | PASS; three faces + three annotated boards, exactly eight bits each; no external assets |
| raster dimensions | PASS; 12 renders at exact 100/125/150/200% dimensions |
| glyph fit | initial FAIL: six appearances of DETAILS had one raster pixel beyond declared inner padding; widened its header button from 64 to 76 units; final PASS 267/267 measured boxes, including annotations |
| accent budget | PASS; session .07244%, performance .61186%, signal .51989% of face pixels; below 5%; not an every-state native measurement |
| geometry/scenarios | PASS; 17 bounded nonoverlapping control targets; complete 3072 combinations, all native PENDING |
| contrast | PASS; 18 text/outline pairs; red excluded as ordinary text, bone redundant warning labels; final report retains ratios |
| prior input preservation | PASS; 422/423 baseline inputs unchanged, only live state ledger updated; runtime sources, manifest, tokens, ADRs and prior sprint ledgers unchanged |
| native theme / catalogue | PASS; existing token-generated theme and compiled catalogue documentation checks; no native binary rebuild needed or claimed |
| documentation links | initial check found a report not yet generated; generated reports before checking their links; final PASS, no unresolved links |
| diff hygiene | PASS; scoped git diff --check; no publication/tag |

[layout-verification.json](evidence/sprint-03/layout-verification.json) retains
actual offline glyph extents and scale dimensions.
[contrast-report.json](evidence/sprint-03/contrast-report.json) retains measured
color ratios. the reviewed PNGs use sharp 0.35.5 / librsvg 2.63.2, not JUCE;
renderer versions are recorded. three annotated previews were visually inspected
and the four-scale matrix checked numerically; native four-scale visual QA is
still unperformed. no synthetic design rendering is called a plugin screenshot.

an existing Inkscape CLI version probe exited 134 without output. an in-app
browser `file:` preview was rejected by its protocol policy; no bypass/retry or
local-server/browser workaround was attempted. offline sharp rasterization uses
only local SVG inputs and existing bundled tooling, with no network or new app
installation. these preview attempts are recorded in the tool observations file.
all required design checks completed through the offline renderer.

## CHANGED FILES AND ARTIFACT IDENTITIES

changed: `docs/release-1.0.0/state.md`; added `native-ux-spec.md`, this
`sprint-03.md`, the entire `native-ux-layouts/` directory and
`evidence/sprint-03/`. all are under SYS 01. the exact enumerated file list and
SHA-256 values are in [inventory.json](evidence/sprint-03/inventory.json) and
[SHA256SUMS](evidence/sprint-03/SHA256SUMS). the checksum root excludes itself to
avoid a recursive hash; the inventory excludes itself and that checksum root.
state and this ledger are sealed externally rather than self-hashed in prose.

| artifact | SHA-256 |
|---|---|
| native UX contract | `45be88122d512a54da4d55cf40410b4a5e993fc671a3763d1439a9b63e656c42` |
| session annotated vector | `d8d96f381ecda9631da2ebc955f22bdfe269f2bd3ae5d5567763be6bcadef2bf` |
| performance annotated vector | `53003f365b9711371f3430f40b1c30d9fb9576c5110604be082081d3283691a6` |
| signal annotated vector | `756641b9c0695df3c1539f01a1d3f397e9825f7e214dc0bfdb2b4e5034388183` |
| 3072-case native checklist | `b53416d4ad1cc0fe409addf911ef58bf10ed77d5f3b18a958d12c168e30a2605` |

## UNPERFORMED CHECKS AND NEXT ACTIONS

no runtime editor implementation, shared-helper implementation, new native build,
DAW gesture recording/scaling, VoiceOver/Windows screen reader, actual native
font resolution/license packaging, musician first-minute tasks, listening,
UI timing, optional meter implementation or host-window resize qualification
was performed. no runtime test, validator, sanitizer, realtime or release gate
pass is inferred from documentation checks. sprint 02 AU timing/CC/DAW/native
Intel/minimum OS/Windows gaps remain; REAPER stays skipped by owner and no
additional testing app is installed.

next authorized boundary for a future sprint 04 request: implement the shared
format/parse helper using existing mapping helpers, native component styling,
independent statuses and inspector over one telemetry consumer; cover unknown/
stale data, rejection divergence, true float detent, numeric edits under host
changes and balanced host gestures. run the native matrix and retain actual
font/scale/focus/accessibility/timing evidence. preserve accepted foundations.
sprint 06 owns packaging/rights, sprint 07 owns musician/host qualification.
the existing source-export suffix allowlist omits PNG/CSV/CJS (and earlier native
.mm/log/session evidence); retain this complete packet separately. expanding
that shipping allowlist is deferred to sprint 06, not silently fixed here.

compact checkpoint: sprint 03 design contract COMPLETE; spec + reviewable
layouts + explicit acceptance matrix sealed; native execution PENDING; release
BLOCKED. resume from native-ux-spec.md and acceptance-cases.csv on an explicit
next-sprint request. do not publish, tag or treat this as release approval.
