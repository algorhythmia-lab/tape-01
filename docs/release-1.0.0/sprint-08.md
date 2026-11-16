# Sprint 08 / final candidate and release decision

2026-10-15 (America/Santiago). **PARTIAL; release BLOCKED; NO-GO.** Reversible
local review preparation is complete. A qualified final 1.0.0 candidate cannot
be declared: three assigned P1 issues and required technical/manual gates remain
open. No status promotion, scope waiver, supported host/platform, public release,
tag, commit, installation, new testing app or approval request was created.
The full release remains at no charge.

Revision `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus preserved dirty work.
No applicable ancestor or SYS 01 AGENTS.md was found. Required implementation
status, ADR 0002/0003, manifest, previous state, Sprint 07 qualification/issues and
Sprint 06 packaging/provenance were read. Initial status, 1,183 baseline file
hashes and previous state are retained in [baseline.json](evidence/sprint-08/baseline.json)
and baseline-state.md. Work remains under SYS 01 only.

## Prerequisites and final review

Before editing the live ledger, Sprint 07's verifier passed all **64** sealed
hashes, five exact-candidate report bindings and baseline preservation checks.
Retained internal evidence Sprints 00–07 verified **34/96/184/35/214/143/154/62**
hashes. Historical live-state checksums remain historical; the Sprint 07 live
state is now captured in this sprint's baseline-state.md. No earlier packet was
resealed. All original candidate build inputs still match build-provenance.json.

[Diff review](evidence/sprint-08/diff-review.md) and the full inherited tracked
SYS 01 diff log document the release-boundary review. The prior 27-file tracked
diff and untracked foundations are preserved. The sole pre-existing file edited
by this sprint is state.md; the other **1,182** baseline files, including unrelated
dirty website/brand/TAPE/root work, remain byte-identical. No production behavior,
catalogue, parameter ID, schema, factory program, wrapper, accepted format,
dependency, license term or release manifest was changed.

## Exact local artifacts

The concrete packet is
`sys-01/dist/sys01-1.0.0-sprint08-macos-arm64-review/`.
Its manifest binds the current documentation/source snapshot to the **unchanged
Sprint 06 build-input inventory, toolchain and exact original macOS arm64 binaries**.
Original build-provenance.json is retained unchanged. No rebuilding, signing,
stapling or metadata mutation of the bundles occurred. Dirty source is identified
by the archive and per-file hashes; no clean-revision or publisher-authentication
claim is made. The source snapshot precedes this final ledger to avoid circular
hashes; this sprint's separate inventory binds the finished ledger and artifacts.

| Artifact / repository-relative path | Bytes | SHA-256 |
|---|---:|---|
| `sys-01/dist/sys01-1.0.0-sprint08-macos-arm64-review.zip` | 6,741,398 | `43970a6e244d3bd6033c5682d09659611dd693194ad3c2eb3179fd1fda5d8aaf` |
| `sys-01/dist/sys01-1.0.0-sprint08-source.tar.gz` | 330,177 | `fbbebefb53e7cc0890be7d5471ed346953302cab1914e5fa53c042033e955999` |
| packet `candidate.json` | 24,045 | `510c3501e02b6e70d6f0e1d68717338044e5293c717df0634dc860c1e44da252` |
| packet VST3 `Contents/MacOS/SYS 01` | 8,840,848 | `bdf225221f23bd28c75394f814cc212d0c922bc563b2c08a281fa5b176dbd968` |
| packet AU `Contents/MacOS/SYS 01` | 8,566,752 | `709acf92683d65c5de0ed712cca7cf6173d1c20114615d4e1b98aef3ce334996` |

Full bundle file-content fingerprints remain VST3
`5c08c3b817d1560a8329dedede5ef51f3037b96a9d0414a8574434e4516ea597` and AU
`ec455f7a09f7d6f10522bb3e04bd2d7065b063c92ea826d54f6ab37f84231382`.
All archive members, source manifests, file sizes and executable permissions
verified after fresh ZIP extraction. Repeated source and same-input ZIP bytes
match exactly; this does not prove independently rebuilt signed-bundle identity.
[artifact-index.json](evidence/sprint-08/artifact-index.json) and
ARTIFACT_SHA256SUMS provide exact additional paths/hashes/sizes.

Naming/version checks confirm SYS 01, 1.0.0 in source/manifests/bundle versions,
`org.algorhythmia.sys01`, `Algr`, `Sys1`, AU `aumf`/65536, all seven catalogue keys
and their order. Loaded conformance checks literal native IDs. Schema 1 remains
40-byte parameter-only deterministic empty-tape recall; audio history/phase is
not recovered by undo/reopen. Both binaries are arm64/ad-hoc, with no Team ID.
No validated hosts/platforms exist. Planned macOS VST3/AU and Windows VST3 remain;
Windows review scripts are not a Windows package.

## Fresh checks on final bytes

| Check | Result / limit |
|---|---|
| Loaded VST3 conformance | PASS, 201,894 points/seven parameters/six rates; exact staged final bytes |
| Steinberg validator | PASS, 47 passed / 0 failed |
| Pinned pluginval | PASS level 10/seed 1012/headless with normal macOS framework access; initial sandbox abort (-6), empty output retained as FAIL |
| Native AU callback / offset characterization | PASS with normal framework access; initial sandbox registration failures retained. Characterization is not timing parity |
| Strict AU accepted ordinary/scheduled offsets | **FAIL**, both collapse to offset zero; positioned CC22 passes. Normal-framework report confirms open P1 |
| Complete VST3/AU seals | PASS strict ad-hoc integrity, not Developer ID/notarization |
| Source export / notices | PASS two identical final exports, manifests and selected pinned notice hashes/no bundled fonts; rights audit remains pending |
| Fresh extracted core source Release build | PASS, **15/15** CTest; empty directory, no Git/brand siblings/local dependency overrides, fresh declared checksum-pinned doctest download |
| Final packet + compressed ZIP | PASS 83 exact members/modes/hashes, duplicate/path checks, extracted verifier and same-input ZIP repeat; 20 report bindings (8 retained + 12 fresh) |
| Independent release gate | **exit 1 / NO-GO** after integrity passes; three P1s and required manual gaps remain |

Fresh report commands name the final staged bundle paths; full inventories prove
the compressed/extracted bytes are identical. Earlier validator tools were
hash-checked before reuse. Initial CMake was absent from PATH (spawn failure,
reconstructed command row explicitly marked); the existing absolute tool path
was used. Sandbox DNS then failed; the authorized network retry fetched the
pinned dependency and configured successfully. A helper incorrectly replaced a
versioned path suffix during initial archive verification; the failed archive
and consequent missing-index gate audit are retained, the explicit full path was
repaired, and verification reran successfully. These are not hidden green runs.

No fresh current installed AU auval, new DAW/UI/AT session, Windows/native Intel/
minimum-OS validator, native rebuild/sanitizer, callback timing/soak, listening,
musician or clean-system install test is claimed. Retained source-matched native
and sanitizer results keep their original scope. The changed boundary is release
packaging/documentation/evidence; core tests do not establish native realtime.

## Gate packet, manuals and next action

Packet GO-NO-GO.md and `qualification/decision.json` identify **G00–G10**, owners,
evidence and closure actions. G00 is complete at its original boundary. G01–G09
remain partial/blocked, so G10 is blocked. All **16** original matrix cells and
three assigned open P1s are copied verbatim and hash-bound. The external retained
evidence index records repository paths/hashes; it does not pretend all historical
media is inside the source archive or rerun. Source ledgers remain available in
source.tar.gz. Accepted lower-priority defect exceptions: **none**.

G01/S07-P1-03 requires safe complete-callback ownership/lock/allocation work.
G02/S07-P1-01 requires AU offset repair. G07/S07-P1-02 requires diagnosis and
declared-workload timing evidence; the retained 22 misses/96 half-budget breaches
are not discarded. G04 host/scale/keyboard/screen readers, G05 DC/listening/
complete real projects, G06 five musicians, G07 eight-instance/eight-hour soak,
G08 rights/corresponding-source approval and G09 Developer ID/notary/Windows
signing plus three clean-system lifecycle cycles remain incomplete. Only the
owner/current computer are available; REAPER remains skipped, without deleting
its unqualified cells. No missing participant, fixture or credential was invented.

Finished review documents are changelog-08.md, first-minute-08.md, install-08.md,
troubleshooting-08.md and decision-08.md. Exact copies are packet CHANGELOG.md,
FIRST-MINUTE.md, INSTALL.md, TROUBLESHOOTING.md and GO-NO-GO.md. The historical root
CHANGELOG.md is preserved as an original build input; the final changelog's source
location is explicit in candidate.json. Installation/rollback describes the
temporary development route and missing signed lifecycle evidence; no personal
plugin directory was modified. No previous qualified signed package exists.

## Commands and checkpoint

[commands.json](evidence/sprint-08/commands.json) records exact expanded argv,
cwd `/Users/lhommeduchili/algorhythmia`, timestamps, exit results and log hashes,
including failures. Bound validator reports additionally retain exact commands,
tool hashes, before/after complete artifact identity and logs. Principal entrypoints:

```sh
python3 sys-01/docs/release-1.0.0/evidence/sprint-07/verify.py --checksums
python3 sys-01/docs/release-1.0.0/evidence/sprint-08/audit.py
python3 sys-01/scripts/test_source_package.py
python3 sys-01/scripts/package_source.py --output sys-01/dist/sys01-1.0.0-sprint08-source.tar.gz
python3 sys-01/docs/release-1.0.0/evidence/sprint-08/validators.py
python3 sys-01/docs/release-1.0.0/evidence/sprint-08/validators.py --native-retry
python3 sys-01/docs/release-1.0.0/evidence/sprint-08/packet.py bind
python3 sys-01/docs/release-1.0.0/evidence/sprint-08/check_candidate.py
python3 sys-01/docs/release-1.0.0/evidence/sprint-08/check_archive.py
python3 sys-01/docs/release-1.0.0/evidence/sprint-08/verify.py --require-go
git diff --check -- sys-01
```

Exact configure/build/CTest, assembly, archive and repeat commands (including
the existing absolute CMake/CTest and validator paths) are in commands.json.
Changed paths/hashes, artifact sizes and preservation proof are in inventory.json
and SHA256SUMS. Seal/checksum verification runs directly after command logging to
avoid a self-referential log hash; earlier sprint seals are never regenerated.


Final document review changed BRAKE from “momentary performance state” to
“performance control” to avoid implying a momentary button. Source/doc packaging
was regenerated from the original extracted snapshot; original build inputs and
all binary bytes remain identical. The artifact table/index above supersedes
the earlier packaging log hashes. Initial Sprint 08 pre-correction artifacts are
retained in `/private/tmp/sys01-sprint08/pre-document-review/`; no earlier-sprint
artifact was moved. New source/ZIP repeats and extracted-packet checks pass.

**Checkpoint: Sprint 08 PARTIAL; NO-GO / release BLOCKED.** Start with
`python3 sys-01/docs/release-1.0.0/evidence/sprint-08/verify.py --checksums`.
The smallest next technical action is a focused S07-P1-01 AU offset repair with
ownership proof and regression, followed by affected final-byte evidence on the
new candidate. Close the other P1s and manual gates before a GO decision. Changes
to bundles/signing/stapling require new provenance/hashes and affected reruns.
Do not relabel this packet as released, publish or tag. Final owner approval is
not requested because publication/tagging is explicitly excluded.
