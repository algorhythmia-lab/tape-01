# SYS 01 / 1.0.0 release decision

2026-10-15 (America/Santiago). **NO-GO. Sprint 08 PARTIAL; release BLOCKED.**
Final local review packaging is prepared; a qualified final release candidate
cannot be assembled while prerequisites fail. No tag, publication, release
approval request or payment gate is created. The full release stays at no charge.

Source baseline: `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus recorded dirty
inputs. The candidate manifest binds the complete source archive, original
build-input inventory/toolchain, dependency trees/notices, complete bundle file
hashes, exact-byte reports, qualification matrix and this decision. The source
snapshot precedes the final Sprint 08 ledger to avoid circular hashes; the
separate Sprint 08 evidence inventory binds the finished ledger and archive.
No clean Git revision or authenticated publisher provenance is claimed.

| Gate | Disposition / evidence and closure required |
|---|---|
| G00 authority and baseline | COMPLETE at its original boundary; ADR 0002/0003, version, stable catalogue, prerequisite hash verification. |
| G01 complete native realtime | BLOCKED; S07-P1-03, native-ownership.md and Sprint 01 instrumentation retain wrapper/lifecycle/listener/TLS concerns. Safe ownership proof, repairs and whole-format audit required. |
| G02 positioned automation | PARTIAL; exact VST3 conformance passes. S07-P1-01 AU ordinary/scheduled offsets fail; native Intel/Windows and DAW offsets remain unqualified. Repair and rerun all affected final-byte checks. |
| G03 validators and DAWs | PARTIAL; fresh local reports identify the exact bundle bytes. No complete supported cell; Sprint 07 retains one scoped Live 12.4.6 arm64 generation/reopen/bounce cycle. Historical auval is bound to earlier bytes; current installed-auval/Windows/native Intel remain unperformed. |
| G04 visual/input/accessibility | PARTIAL; source fixtures/rendering exist, but host scales/reachability, keyboard, VoiceOver/Windows reader and declared-load paint/input work remain pending. |
| G05 musical/session trust | PARTIAL; deterministic linked recall fixtures pass. MB-04 DC and controlled listening/transition acceptance remain open; all-study repeated actual projects remain incomplete. |
| G06 musician beta | BLOCKED; zero of five first-time participants. Original >=4/5 first-minute/transform and all-five recall-understanding criteria remain. |
| G07 performance/soak | BLOCKED; S07-P1-02 retains 22 deadline misses and 96 half-budget breaches across finite diagnostic runs. Eight instances/eight wall-clock hours per cell and minimum-machine evidence are missing. |
| G08 rights/source | BLOCKED; selected AGPL-3.0-only text and pinned notice bytes verified. First-party/transitive applicability and complete corresponding-source review need rights-owner acceptance. |
| G09 signing/install | BLOCKED; ad-hoc seals only. Developer ID/notary and Windows Authenticode/fixture prerequisites missing; three clean-system install/scan/upgrade/uninstall/rollback cycles per package unperformed. No prior signed qualified version. |
| G10 final qualification/decision | BLOCKED; three open P1s and required manual gates prevent GO. Publication/tag explicitly excluded from this sprint. |

`qualification/decision.json`, matrix.json and issues.json in the review packet
retain machine-readable gate owners, evidence references and next actions.
`qualification/evidence-index.json` binds retained external evidence without
claiming it was rerun. Fresh validators are in reports/. No earlier evidence is
transferred to a changed artifact. Native signing, stapling, rebuilding or metadata
changes invalidate affected evidence and require new inventories and reruns.

There are no validated supported hosts/platforms. Planned macOS VST3/AU and
Windows VST3 are retained; only a local macOS arm64 bundle pair is available.
The 16-cell original matrix is retained, including owner-skipped REAPER cells.
No new testing app is installed. Windows review scripts are not Windows binaries.

Accepted product limitations from ADR 0002 remain: parameter-only deterministic
empty-tape recall, no captured performance/phase recovery by undo, transient
performance MIDI, free clock, wet-only transform, uncompensated creative delay,
four stable studies and deferred formats/features. These are product scope,
not waivers for defects. **No lower-priority defect exception has been accepted**;
UI timing and tooling investigations remain open. Sound/DC is not accepted as
intentional merely because samples are finite.

The smallest next technical action is to reproduce and repair S07-P1-01 from the
bound strict AU report, with an ownership proof and focused regression; then
rerun affected native/validator/host checks on new exact bytes. Close the other
P1s and manual gates before requesting any release approval. Signing and clean
installation cannot be replaced by changing a manifest status. Publication/tag
requires a later explicit request, after qualification; no approval is requested now.
