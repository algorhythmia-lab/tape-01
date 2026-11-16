# SYS 01 / 1.0.0 release state

Updated 2026-10-15 (America/Santiago). **Sprint 08: PARTIAL; decision NO-GO;
release BLOCKED; three assigned P1 issues and required manual gates remain open.**
Sprint 07: PARTIAL; Sprint 06: PARTIAL (details appended below).
**Sprint 00: COMPLETE** at its documentation
and baseline boundary. **Sprint 01: PARTIAL. Sprint 02: PARTIAL. Sprint 03: COMPLETE
at the design-contract boundary. Release: BLOCKED / development-unqualified.**
Bounded native MIDI, rejection recovery, immutable stereo telemetry and whole-wrapper
fixtures/probes are implemented. Wrapper locks, first-audio-thread TLS allocation,
observed local timing breaches and missing minimum-machine evidence keep G01/G07 open.
Sprint 02 added conformance fixtures, actual validators and a partial Live AU session.
Through Sprint 02 no tag/publication or downstream sprint execution occurred; temporary development
plugin copies were used for validators/DAW scanning.

Revision: `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus dirty SYS 01 changes indexed
in [Sprint 01](sprint-01.md), [Sprint 02](sprint-02.md) and
[Sprint 03](sprint-03.md), with their evidence inventories. Initial untracked website,
brand, TAPE and release-plan work is preserved. No applicable AGENTS.md was found.
Sprint 00's initial verifier passed before edits; its original state is captured in
`evidence/sprint-01/baseline-state.md`. Use the revision-based baseline verifier after
intentional source edits; the old working-tree manifest is historical, not rewritten.

## Authority and preserved scope

`release/manifest.json` owns machine-readable qualification. Its status remains
`development-unqualified`; `validated_platforms` and `validated_hosts` remain empty.
[ADR 0002](../adr/0002-desktop-control-state-and-release.md) owns accepted product
behavior; [ADR 0003](../adr/0003-positioned-vst3-integration.md) owns the positioned
VST3 boundary. This ledger records Sprint 00 decisions, Sprint 01/02 evidence and the Sprint 03
design contract without
silently changing either schema or declaring a platform supported.

SYS 01 is the desktop name; study 01 is historical and `/sys-0` remains an alias.
Keep `org.algorhythmia.sys01`, `Algr`, `Sys1`. Deliver the full release at no charge;
optional donations were accepted by the owner in this session. Planned formats
remain macOS VST3/AU and Windows VST3 in one stereo effect/generator shell, with
silent/disabled-input generation and external stereo transform input. Separate
instrument/FX IDs remain deferred. Standalone is a development harness; Max for
Live is a following milestone. CLAP/AAX, public standalone, hardware shipping,
host sync, MIDI learn, desktop hold-save, captured-audio recall and a mix control
remain deferred. Do not restart completed source, catalogue, state, VST3 or editor
foundations recorded in [implementation status](../implementation-status.md).

Schema 1 remains exactly 40 bytes: `SYS1`, LE version 1, seven LE IEEE754 normalized
words in catalogue order, CRC32. Validate size/magic/version/checksum/finite ranges
and discrete values before mutation; v0/future schemas fail safely. Recall and
presets restart deterministically with empty tape/voices/phases; undo restores
parameters, never audio history. Cross-CPU bit identity is not promised.

| Stable parameter in schema order | Default | Accepted meaning |
|---|---:|---|
| `param_flux` | 0.5 | continuous; 500–8000 ticks/s, 30–300 BPM, -2..+2x by domain |
| `param_bias` | 0.5 | continuous domain macro |
| `transport_drive` | 0 | binary run |
| `subregister_state` | 0 | binary STATE shared across domains |
| `engine_state` | 0 | domain at normalized 0 / 0.5 / 1 |
| `output_trim` | 0.75 | -60..+20 dB, default 0 dB |
| `bypass` | 0 | 0 wet / 1 dry, smoothed; engine keeps advancing |

The existing four factory programs and their order remain stable. Continuous
targets step at event offset with 10 ms smoothing. Events cap at 256 per block,
ascending offsets in `[0, frames)`, stable ties, whole-batch rejection and no
following-block replay of rejected final host targets. Restore has eight usable
slots, immutable transactions and a single audio owner. Zero blocks consume no
controls/edges. MIDI CC20/21/22/23 is positioned performance input, not saved host
parameter writes. AU currently has block-boundary automation; parity is pending.

## Decision record and qualification matrix

The owner delegated minimum-machine/coverage/instance choices to engineering on
2026-10-14, then selected **AGPLv3** for SYS 01 first-party code, free downloads and
optional donations. The planned license identifier is `AGPL-3.0-only` (version 3;
no later-version grant inferred). Sprint 06 must implement license text/notices
after provenance review. This session does not assert rights to every existing
file or treat the license choice as a completed rights audit.

Engineering selects these deliberately modest **qualification targets**, with
8 GB RAM and a host-compatible Core Audio/ASIO configuration. They are not measured
minimum requirements. Use Live **12.4.6** and REAPER **7.82** separately in every
listed format; record their installer hashes and full build IDs when exercised.
Latest owner steering (Sprint 02): use the installed Ableton host only and skip
REAPER tests; do not install other apps for testing. The original REAPER matrix
cells below remain unqualified and are not silently marked passed or removed
from historical release targets.


| Target | Exact OS target / native CPU | Minimum-machine test fixture | DAWs / format cells | Evidence now |
|---|---|---|---|---|
| MAC-INTEL | macOS 11.7.10 / x86_64 | MacBook Pro 13-inch 2017, four TB3 ports, 3.1 GHz dual-core i5, 8 GB | Live 12.4.6 + REAPER 7.82, each VST3 and AU | PENDING |
| MAC-ARM-MIN | macOS 11.7.10 / arm64 | Mac mini M1 2020, 8 GB | same four host/format cells | PENDING |
| MAC-ARM-CURRENT | macOS 26.6.2 build 25G83 / arm64 | local MacBookPro18,1, M1 Pro, 16 GB; current-OS coverage, not minimum fixture | same four host/format cells | fresh core/native fixture/pluginval only; DAWs PENDING |
| WIN10-X64 | Windows 10 22H2 / x64 | Intel i5-8250U PC, 8 GB | Live 12.4.6 + REAPER 7.82, each VST3 | PENDING; legacy reach target |
| WIN11-X64 | Windows 11 24H2 / x64 | Intel i5-8250U PC, 8 GB | same two host/format cells | PENDING |

Windows full build/patch number, machine model, interface/driver version and power
mode are evidence prerequisites to testing WIN10-X64/WIN11-X64, not invented measurements.
No interpolation of support to untested patch releases, Intel on macOS 26,
Rosetta, Windows ARM, Linux, other DAWs or newer DAW releases. Intel deployment
target and universal bundle packaging still need Sprint 02/06 verification.
If an old-OS cell cannot meet the pinned dependency/host contract, report failure
and revise the target openly; do not populate validated arrays with a build alone.

Live's published minimums include 8 GB RAM and macOS 11.7.10 / Windows 10 22H2;
REAPER 7.82 provides native x64/arm64 macOS and Windows x64 builds. These informed
reach targets, not SYS 01 compatibility claims. [Ableton requirements](https://help.ableton.com/hc/en-us/articles/115001663530-Live-Minimum-System-Requirements),
[REAPER download/version](https://www.reaper.fm/download.php),
[Intel Mac fixture specifications](https://support.apple.com/en-us/111972).

The RP2350 has 150 MHz cores and 520 KB SRAM. Fresh host memory-contract evidence
is 262,832-byte engine / 265,904 bytes with I/O; it does not measure board stack,
codec/DMA, persistence stalls, native UI or DAW overhead. Hardware qualification
remains separate. [Pico 2 specifications](https://www.raspberrypi.com/products/raspberry-pi-pico-2/).

| Decision | Owner / disposition |
|---|---|
| minimum machines, exact OS/CPU/DAW targets | engineering lead; selected above under owner delegation; measured viability PENDING |
| instance workload | audio engineer; eight simultaneous stereo instances, 48 kHz / 128 frames; acceptance below |
| first-party terms / donations | product/rights owner (user); AGPLv3 selected; implementation and file provenance PENDING |
| approval of dependency rights and distribution packet | product/rights owner accountable; release engineer prepares notice/source inventory; final review PENDING |

## Gate ledger

Owners below are accountable roles for subsequent sprint implementers, not claims
that a team or external reviewer has been hired. The product/rights owner is the
user. COMPLETE means evidence meets the named boundary, PARTIAL means some evidence
exists, BLOCKED means a required release prerequisite is missing. PENDING is used
for individual unperformed tests/decisions. No required gate is waived.

| Gate / manifest mapping | Owner | Status and exit evidence | Next sprint |
|---|---|---|---|
| G00 authority, inventory, baseline | engineering lead | COMPLETE; this state, Sprint 00, commands + hashes | 00 |
| G01 whole_format_realtime | audio engineer | BLOCKED; Sprint 01 PARTIAL: fixed bounded MIDI/telemetry, actual wrappers tested; callback/listener/ingress locks and dyld TLS first-thread calloc observed. Safe replacement and whole-format zero allocation/lock/I/O evidence still required | 01 |
| G02 sample_offset_host_automation | format engineer | PARTIAL; Sprint 02 loaded VST3 exhaustive offsets/parameters/six rates on arm64 and x86_64/Rosetta passed. AU ordinary/scheduled offsets FAIL; proposed scope decision pending. Native Intel/Windows and DAW automation remain PENDING | 02 |
| G03 format_validators + DAW_qualification | platform QA | PARTIAL; final sealed VST3 Steinberg 47/47 on arm64 + x86_64/Rosetta; final arm64 AU strict auval and VST3 pluginval level 10 passed. Actual Live AU/VST3 generation/transform/bypass/bounce/reopen exercised on recorded hashes; timed CC/DAW offsets/final thin AU host rerun/external cells pending; REAPER skipped by owner | 02, 07 |
| G04 accessibility / visual | native UI engineer + accessibility QA | PARTIAL; Sprint 03 design contract COMPLETE, annotated layouts/contrast/scale math and 3072-case native checklist retained; native implementation, full state/scale captures, keyboard, VoiceOver and Windows screen reader remain required | 03, 04, 07 |
| G05 sound / session trust | audio engineer + musician reviewer | PARTIAL Sprint 05; 264 musical renders, 72 native saved-state reopens, destructive UI undo and fixed study fixtures pass. Independent 88.2 kHz high-trim transform DC investigation gate FAIL (MB-04); controlled listening and new actual DAW project cycles UNPERFORMED | 05 |
| G06 musician_beta | product owner + QA | BLOCKED; five participants absent; at least 4/5 complete first-minute and transform tasks without intervention | 07 |
| G07 performance_soak | audio engineer + platform QA | BLOCKED; local native editor-open/closed diagnostics retained, including 10.35 ms deadline outlier and 64-frame half-budget breach. Agreed minimum-machine/DAW/eight-instance/soak evidence unperformed | 01, 07 |
| G08 rights_review | rights owner + release engineer | BLOCKED; Sprint 06 retains owner-selected AGPL-3.0-only text and verified selected dependency notices; first-party/transitive applicability and complete corresponding-source review unfinished | 06 |
| G09 signed_clean_machine_install | release engineer + platform QA | BLOCKED; Sprint 06 prepares Developer ID PKG/notary and owner-selected Windows Authenticode/ZIP/PowerShell route, with exact-artifact local packet. No Developer ID identity, Windows/clean-machine or lifecycle-cycle evidence; local ad-hoc seals only | 06, 07 |
| G10 tagged_source / final decision | release owner | BLOCKED; approved qualification packet and no P1 defects required; no tag/publication authorized in this sprint | 08 |

## Acceptance budgets handed downstream

These are Sprint 00 engineering criteria under the delegated decision, not passed
measurements. Do not lower them to turn missing evidence into a pass.

| Boundary | Budget / reproducible acceptance | Owner |
|---|---|---|
| callback | zero allocation/deallocation, blocking locks, I/O and unbounded work in the complete shipping callback. Measure entry-to-exit duration with instrument overhead characterized separately. Single-instance maximum <50% of `frames/rate`: <1.333 ms at 48 kHz/128; <0.667 ms at 48 kHz/64. Report count, p50/p95/p99/p99.9/max and deadline misses, not throughput alone. | audio engineer |
| instance / soak | eight stereo instances at 48 kHz/128, all domains and repeated transitions, 256-event blocks plus MIDI/UI merge stress, restore/lifecycle churn, editors closed then all open. Whole DAW audio cycle <80% of 2.667 ms and zero observed underruns/misses over eight wall-clock hours per matrix cell. Record audio driver, graph, external load and instrumentation overhead. Offline simulated hours cannot substitute. Single-instance 64-frame run is additional stress; no eight-instance/64 promise. | platform QA + audio engineer |
| rates / blocks | fixture sweep 44.1/48/88.2/96/176.4/192 kHz; 0/1/7/63/64/65/511/4096 frames, malformed events and repeated prepare/release. Host rate claims require actual matrix-cell evidence; eight-instance budget applies to 48 kHz/128 only. | format engineer |
| visual / input | preserve 640x440 minimum and uniform 100/125/150/200% scaling until a measured alternative is accepted. Zero clipped essential labels, overlaps or unreachable controls in every domain/status combination. Normal-label contrast >=4.5:1, large text >=3:1; color is redundant. Native focus/name/role/value/actions work with keyboard and screen reader. | UI engineer + accessibility QA |
| UI work | bounded 30 Hz telemetry (33.33 ms period); target pointer/key feedback <=33.33 ms and timer+paint maximum <16.67 ms under declared normal load. Measure separately from audio; no decorative motion or closed-editor timer/repaint work. UI target failure requires diagnosis; screenshots alone do not pass it. | UI engineer |
| musical / recall | zero nonfinite samples for finite and hostile-input fixtures; bounded [-1,1] output for normalized test inputs. Characterize peak/RMS/DC and sample deltas over 10-second fixtures after one-second settling. Flag abs DC mean >0.01, transition delta >0.1 or unintended RMS jump >3 dB for listening/review; these are investigation triggers, not a blanket ban on deliberate bytebeat edges. No unresolved unintended click/DC/loudness defect; intentional exceptions need named fixture + musician/product acceptance. Within-build deterministic recall is exact with empty tape; no cross-CPU exactness claim. Listen at fixed documented monitoring level with normalized -18 dBFS input. | audio engineer + musician reviewer |
| first minute | >=4 of 5 first-time musicians independently hear a pattern, change FLUX/BIAS, switch domain and recover within 60 seconds. Separate transform input/freeze/reverse/recovery task; all five must understand parameter-only recall after concise guidance. Record failures/confusion, no fabricated participants. | QA + product owner |
| installation | zero scan/load failures, missing notices or undocumented manual workarounds on each claimed clean OS/CPU/DAW/format cell. Three recorded install/scan/save/reopen/upgrade/uninstall/rollback cycles per package; restore opens correctly after reinstall. Verify downloaded package and executable hashes, signing/notarization where required; uninstall removes owned files without deleting projects/preferences. Rights-approved source accompanies binaries. | release engineer + platform QA |

## Evidence index and invalidation

| ID | Evidence | Fresh / retained and limit |
|---|---|---|
| E00 | [commands.json](evidence/sprint-00/commands.json) + named `.log` files | fresh exact argv/cwd/UTC time/exit/hash; local toolchain and Release checks |
| E01 | [inventory.json](evidence/sprint-00/inventory.json) | fresh tracked-input, dependency-license, archive, retained candidate and fresh executable hashes; 36/36 retained entries verified |
| E02 | [core log](evidence/sprint-00/ctest-core.log) | fresh 12/12; core Release, 4.06 s; 28 DSP cases/662,575 assertions and 7 host cases/10,605 assertions |
| E03 | [native log](evidence/sprint-00/ctest-native.log) | fresh 14/14, 4.19 s; includes adapter and actual loaded VST3; no DAW, AU offset or whole-wrapper realtime proof |
| E04 | [retained binary rerun](evidence/sprint-00/ctest-retained-vst3.log) | fresh positioned fixture against retained VST3 executable `8160afe7…64d21`; executable unchanged |
| E05 | `../../dist/sys01-development-macos-arm64/reports/summary.json` and `candidate.json` | retained 2026-10-10; hashes reverified, historical ASan/UBSan/TSan/analyzer/source extraction/negative control/screenshots, not rerun |
| E06 | `../release-plan-1.0.0/evidence/audit-provenance.json`, `core-release-ctest.log` | retained 2026-10-13 core 12/12 (2.95 s); native evidence then retained, not fresh |
| E07 | [Sprint 00 final verification](sprint-00.md) | docs/link/hash/package checks and any additional native-validator outcomes |
| E08 | [fresh pluginval report](evidence/sprint-00/pluginval-fresh-native-report.log) and retry log | fresh level 10, seed 1010, GUI tests enabled, SUCCESS; initial sandboxed startup SIGABRT, approved retry exit 0; fresh VST3 hash in E01; Steinberg validator skipped, no AU validation |
| E09 | [VST3 signature check](evidence/sprint-00/codesign-fresh-vst3.log), [AU signature check](evidence/sprint-00/codesign-fresh-au.log) | fresh failures, exit 1 each; development bundle seals do not pass strict verification; assigned to G09 / Sprint 06 |

Baseline artifact SHA-256 (full bundle file inventories/sizes are in E01):

| Artifact | SHA-256 |
|---|---|
| retained VST3 executable | `8160afe70413fee6bfae57b20ff6e84becd6132ccbedfbdf8ce5bb8b71a64d21` |
| fresh VST3 executable / E03 + E08 target | `f2618e5036de3c00f3ce447415751bfe4b5e34be5972faa9b6df9d3efdb58209` |
| fresh AU executable / compiled only | `70490925bfc0ab51ff82a0e248963395a2dd1f54d51a73b80dd25619957d626e` |
| retained source development archive | `e86d2bd9ed67962ef7ec9554f61d1bbb890426ddc0cdbbb05801a95ba73db915` |

Changed files: this `state.md`, `sprint-00.md`, and the complete enumerated
`evidence/sprint-00/` packet in `SHA256SUMS`. E00 records exact commands/results;
E07 contains the expanded build recipe and final verification commands. Fresh
core/native configure, build and CTest exited 0; pluginval startup -6/retry 0;
strict signatures each 1; document/source-package/diff verification each 0.

The candidate summary explicitly identifies level 5 and earlier level 10 pluginval
as earlier development binaries; only `pluginval-vst3-level10-final.txt` binds the
retained candidate executable. Earlier negative-control binaries are absent;
retained failure log demonstrates history, not a new negative-control rerun.
No sanitizer, static analyzer, eight-hour DSP simulation or remote CI execution
is newly claimed. All fresh native artifacts live in a temporary build tree and
may disappear; their hashes and reports remain in this ledger.

Evidence records must identify source revision/dirty inputs, exact executable
and package SHA-256, OS build/CPU, dependency pins, compiler/SDK/flags, host build,
driver/rate/buffer/instance count, command/seed/workload/duration, measured results,
owner and manual omissions. Rebuilds, signing or source changes invalidate
applicable binary/host/validator/timing evidence even if version remains 1.0.0.
Reverify hashes before reuse; do not transfer retained reports to fresh binaries.
Finite timing runs report observed maxima and cannot establish a hard realtime
bound. The source export's existing allowlist omits `.log` files; retain the full
evidence directory alongside it. Final release evidence packaging belongs to 06.

## Downstream input contracts / checkpoint

Sprint 01 receives the 256-event/eight-restore ownership contract, G01/G07 budgets
and known wrapper blockers; it must return call paths, lifecycle ownership,
whole-wrapper instrumentation and bounded immutable telemetry. Sprint 01 now publishes actual per-channel peaks through its immutable telemetry
contract; Sprint 04 must handle initial/stale snapshots and display drops.
Sprint 02 receives the matrix and pinned generated VST3 boundary; it must return
exact binary/host validation and resolve AU timing without changing IDs. Sprint
03 receives ADR precedence, visual budgets and all simultaneous status states;
Sprint 04 consumes its UX spec plus Sprint 01 telemetry. Sprint 05 consumes
parameter-only recall, stable studies and musical review triggers. Sprint 06
consumes AGPLv3 intent, notice/source inputs and artifact evidence schema; Sprint
07 consumes the exact candidates and every budget. Sprint 08 cannot authorize
release until required gates pass and the owner approves the release action.

Unperformed manual work: DAW scans/routing/offline bounce/reopen, AU validation
and positioned timing, native assistive technology, complete visual/state capture
review, listening/musician tasks, minimum-machine editor-open/closed timing,
realtime soak, Windows execution, signing/notarization and clean installs. The
current machine is above the selected minimum; its passes cannot close G07.

Resume from [sprint-00.md](sprint-00.md) and verify E00/E01 hashes first. Rights
owner/provenance approval and access to target machines/hosts remain PENDING.
Resume Sprint 01's unresolved realtime ownership and minimum-machine checks from
[sprint-01.md](sprint-01.md); do not automatically execute downstream sprints or create
a release tag.


## Sprint 01 evidence and checkpoint

Implementation status is **PARTIAL**, with G01/G07 **BLOCKED**. Both generated native
wrappers retain lifecycle/listener synchronization. VST3 uses fixed CC ingress and
256 total queue-point bounds; AU preallocates/bounds legacy and UMP MIDI, drops
undeclared SysEx before copy and rejects overflow atomically. UI edges recover after
malformed MIDI. Eight immutable restore slots and stable IDs/schema/recall remain.
[Ownership](native-ownership.md) records the inspected paths and safe-replacement
prerequisites; [telemetry](telemetry-contract.md) hands bounded immutable snapshots
and real per-channel peaks to Sprint 04 without implementing its UI.

Fresh Release and diagnostic CTest: 14/14; ASan/UBSan 4/4 targeted; TSan 3/3 targeted
with halt_on_error and no suppressions; corrected dedicated-audio-thread VST3 follow-ups
pass in all builds. pluginval 1.0.4 level 10 / seed 1011 / editor enabled passed on the
exact release VST3. AU loaded via process-local registration; no auval/Steinberg/DAW
qualification is claimed. Full commands/results, failed attempts, exact dirty inputs
and hash bindings are in [Sprint 01](sprint-01.md) and `evidence/sprint-01/commands.json`.

Probe negative controls detect allocation/free/mutex/unfair locks. The same retained
AU workload allocated on SysEx/overflow (6 + 6 allocation/free first group); the new
AU group shows zero such calls but 602 mutex calls. Corrected VST3 editor automation
on a dedicated audio owner records one first-entry calloc, stack-attributed to dyld
TLS initialization in processParameterChanges (upstream thread-local notification
flag); subsequent changing-target blocks typically take four mutex locks. Older
message-thread editor counts are retained with their scope correction. No core-only
pass is used to certify wrappers, and alternate allocator/lock/I/O paths remain outside
the probe's proof. Release profiling does not link the probe.

Local M1 Pro native timing is a short, unpaced, one-instance diagnostic with 256
merged automation/MIDI events per block, editors open then closed, 48 kHz/64 and 128.
The first open/128 run observed max 10,352.291 us and one deadline miss under concurrent
validation load. Final diagnostic closed/64 max 988.167 us breached the 666.667 us
single-instance half-deadline budget once. Later lower maxima do not waive either
observation. No driver/DAW/eight-instance cycle, controlled power/external load or
minimum fixture was exercised. See all percentiles/counts/clock overhead in sprint-01.

Release VST3 executable SHA-256:
`8b0edb1f2a05e398a1822051e1470e80e11bf972456e8802e8ab50c9d73b1e63`.
Release AU executable SHA-256:
`41765d2fb6b5146129f9c83abf8384743f2dbbf2402d70b8d6970178a9791724`.
Temporary artifact roots are `/private/tmp/sys01-sprint01-{release,probe,asan,tsan}`;
separate hashes, generated units and flags are in inventory.json. No new package or
package hash applies. The original development candidate and reports remain intact.

Changed files are enumerated in sprint-01 and inventory.json, with the complete
packet indexed by `evidence/sprint-01/SHA256SUMS`. Next actions: verify that packet;
replace TLS and notification/lifecycle/ingress hazards with an ownership proof;
resolve timing causes under declared load; add native VST3 4096 and AU same-timestamp
zero/nonzero cases; obtain agreed minimum machines/DAWs/drivers for required profiling.
Manual omissions remain minimum-machine/eight-instance profiling, realtime soak,
Windows, actual DAW routing/offline/reopen, AU offsets/auval/Steinberg, accessibility,
listening/musicians, rights, signed clean installs and publication approval. Preserve
all existing work and resume only Sprint 01 unless separately instructed.


## Sprint 02 evidence and checkpoint

**PARTIAL; AU timing G02 BLOCKED, G03 PARTIAL, release BLOCKED.** Shipping
adaptations remain checksum-pinned and unchanged. Expanded loaded VST3 tests
cover 201,894 catalogue points (every integer offset of declared block sizes,
all seven parameters, six rates), exact discrete quantization thresholds,
stable component/controller/parameter IDs, ties, 256 capacity, cross-queue and
merged MIDI overflow, nonfinite intermediate/final values and no following-block
replay. A fresh stock-wrapper OFF build fails the same fixture for the intended
offset mismatch. Its actual bundle is retained in the evidence archive and was
executed after extraction. AU has an independent native dual-oracle fixture:
direct and scheduled parameter points collapse at zero; CC22 preserves offsets.
Strict mode exits 1 on both final architecture builds; default CTest only
characterizes this defect. [AU scope decision](au-timing-scope-decision.md) is
PROPOSED, not an accepted contract change.

Both native bundles initially failed strict integrity checks. Actual Live scanning
rejected the original VST3 seal after moduleinfo generation. CMake now ad-hoc
seals the final VST3/AU bundles after metadata generation; all four thin plugin
bundles pass strict codesign verification. This local development repair does
not establish Developer ID signing, notarization or clean-machine qualification.
Fresh final Release CTest **15/15** on arm64 and **15/15** on x86_64 via Rosetta;
both have macOS 11.0 deployment load commands, not minimum-OS/native Intel proof.
Official Steinberg validator at `3cdf9ca5d1f5b1b21e0a86832aa4abe55607bd96`
passes 47/47 on both final VST3s. Apple strict auval passes final arm64 AU;
x86_64 auval attempts could not find the component and remain unqualified.
pluginval 1.0.4 level 10 / seed 1012 / GUI enabled passes final arm64 VST3.
All argv/cwd/times/exits and failed attempts are retained in
[evidence/sprint-02/commands.json](evidence/sprint-02/commands.json).

Live 12.4.6 [2026-09-10_0de5c8fa9a], arm64, CoreAudio, input No Device,
MacBook Pro Speakers, 44.1 kHz/512, one plugin instance: AU/VST3 scan/load,
disabled-hardware-input generation, asymmetric stereo transform, bypass,
offline render and project reopen were exercised. Final VST3 hash matches the
resealed candidate exercised in Live: recovered and reopened renders are
sample-identical to its transform render. Bypass preserves stereo dry input
exactly away from Live's clip fades. AU generation used an earlier thin bundle;
AU reopen/transform used the named universal bundle. **Those AU host reports
are not transferred to the final thin AU hash**, which still needs a host rerun.

CC Control was configured/routed for CC20–23 and its static Send exercised;
resulting offline PCM was unchanged, so actual recorded MIDI delivery remains
UNQUALIFIED. Do not infer a plugin MIDI failure or pass from static sends that
may be discarded on offline reinitialization. Recorded CC and ordinary DAW
sample-offset automation remain pending. Native UI access intermittently timed
out and recovered, then timed out during MIDI recording preparation; a saved
`SYS01-VST3-midi.als` checkpoint is retained. A read-only host sample showed its
main event loop; no crash/deadlock diagnosis is claimed. The host scanner's
zero-candidate CLI attempts are not pass evidence; its actual startup scan is.

The owner explicitly approved the REAPER EULA, then instructed skipping REAPER
tests and installing no other apps for testing. The temporary REAPER host was
stopped, its isolated app and installer removed, and its image unmounted. No app
was installed into /Applications; no SYS 01 REAPER qualification was performed.
Do not resume REAPER tests or install other testing apps without new instruction.
Both support arrays remain empty. External Windows/native Intel/minimum macOS
cells remain unperformed; their exact prerequisites are in Sprint 02.

| Final candidate | Executable SHA-256 |
|---|---|
| arm64 VST3 / actual Live target | `22b9eb22c4e636407eafdceba3137bfc9eb687bd9888d35616e4ff19aaa88f0e` |
| arm64 AU | `ebbf47693980a81851a33f3d7d82ab179c0024c779aa6c365a5ef4eee9d5e1d5` |
| x86_64 VST3 | `1854cbd567b3ba1dffe2d0e33dd54b5325f5763c4f752c266555ab154e2a21b6` |
| x86_64 AU | `49f6246b4b7cbb54d973227d1b6f7460ce07c6421dc86b6f2cc8dd110b981b2c` |
| stock negative VST3 | `1997a5317b0d0fd703afdc1ab5d6b9af5df93082f566dad0270012326f147241` |
| earlier universal AU / Live transform target | `e6c921231f211c18ee8ffba529f209c1f7aae6be7a0b945d41d9927dfd28e254` |

Changed files: `apps/desktop/CMakeLists.txt`, `VST3ContractTests.cpp`,
`AUContractTests.cpp`, new `AUOffsetTests.cpp`, ADR 0003, implementation-status,
manifest, this state, [Sprint 02](sprint-02.md), the AU scope decision and its
complete evidence packet indexed by SHA256SUMS. Processor/DSP, parameter
catalogue, state schema, native adaptation scripts, free distribution,
planned formats/deferred scope and unrelated dirty user work were preserved.
No sanitizer rerun is claimed; processing code was unchanged. Source export,
formatting, final native fixtures, integrity checks and validators cover the
changed build/format boundary. Earlier hashes/reports remain explicitly historical.

Resume only Sprint 02: verify its inventory/log/SHA256SUMS, recover Ableton UI,
record CC20–23 and independently measure DAW automation offsets, rerun AU hosts
on the final thin AU hash, and obtain exact native Intel/minimum macOS/Windows
fixtures. Keep REAPER skipped and install no other testing apps. Resolve the AU
scope proposal without silently downgrading ADR 0002. No tag or publication is
authorized by this checkpoint. Rights, release signing/notarization, soak,
listening/accessibility and final release approval remain downstream gates.


Final local cleanup removed only the two hash-verified SYS01-Sprint02 plugin
scan copies. Exact build candidates and retained negative/session/media archives
remain; reinstall the exact candidate copies before reopening the test projects.
The owned Live Places folder may remain until native UI access recovers. Other
plugin files and existing preferences were not deleted. Full cleanup commands,
archive hashes, changed-file inventory and final verification are in the packet.


## SPRINT 03 // DESIGN CONTRACT CHECKPOINT // 2026-10-14

**COMPLETE for the requested design-contract sprint.** this explicitly authorized
sprint supersedes the historical sprint 02 resume-only checkpoint for this turn.
no downstream native implementation was executed. release remains BLOCKED;
manifest/support arrays, prior partial sprint statuses and all accepted/deferred
scope remain unchanged.

revision `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus the preserved dirty inputs.
read applicable guidance (no AGENTS.md found), implementation status, ADR 0002/0003,
manifest, prior state/evidence, brand/native/TAPE tokens and physical spec through
ADR precedence, current editor, catalogue, mapping helpers and telemetry contract.
pre-edit prerequisite verification: sprint 00 35/36 and sprint 01 99/100 hashes
match, with only each superseded live state.md differing; sprint 02 187/187 match.
all three historical state hashes also match their retained next-sprint baseline
copies. original records were preserved, not silently rehashed.

new [native-ux-spec.md](native-ux-spec.md) and [annotated layout index](native-ux-layouts/README.md)
define the session strip, twin spools/eight-bit register, signal strip and useful
secondary inspector. all three domains, run/pause, shared binary STATE, brake,
bypass, known missing vs quiet input, limiting, modified targets and rejection
can coexist. shared helper requirements preserve the actual transform detent;
host targets stay separate from transient MIDI. 640×440 minimum and uniform
100/125/150/200% remain; fonts stay system-resolved with no bundled font files.
keyboard, direct entry, focus/host gestures, color semantics and optional real
stereo peaks are specified without adding runtime features or false sync/motion.

changed files: this state ledger; added native-ux-spec.md, sprint-03.md, the entire
native-ux-layouts/ directory (6 SVGs, 15 PNGs including 3 annotated boards,
geometry/scene JSON, 3072-case CSV and 4 generation/verification scripts plus
README) and evidence/sprint-03/. exact file paths/hashes are enumerated in
[inventory.json](evidence/sprint-03/inventory.json) and
[SHA256SUMS](evidence/sprint-03/SHA256SUMS). all prior 422 other inventoried SYS 01
inputs are unchanged, including runtime sources, manifest, tokens, catalogue,
ADRs and sprint ledgers. state itself is the 423rd baseline input.

| artifact | SHA-256 |
|---|---|
| native UX contract | `45be88122d512a54da4d55cf40410b4a5e993fc671a3763d1439a9b63e656c42` |
| session annotated vector | `d8d96f381ecda9631da2ebc955f22bdfe269f2bd3ae5d5567763be6bcadef2bf` |
| performance annotated vector | `53003f365b9711371f3430f40b1c30d9fb9576c5110604be082081d3283691a6` |
| signal annotated vector | `756641b9c0695df3c1539f01a1d3f397e9825f7e214dc0bfdb2b4e5034388183` |
| 3072-case native checklist | `b53416d4ad1cc0fe409addf911ef58bf10ed77d5f3b18a958d12c168e30a2605` |

exact commands/results/cwd/timestamps/log hashes:
[evidence/sprint-03/commands.json](evidence/sprint-03/commands.json); expanded
recipes and omissions: [sprint-03.md](sprint-03.md). boundary checks:

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

all final checks exit 0: 267/267 offline glyph boxes fit; 17 control rectangles
bounded/nonoverlapping; 12 scale images have exact dimensions; 18 semantic
contrast pairs pass; accent area .07244/.61186/.51989% across three face fixtures;
3072 complete domain/state/scale scenarios explicitly native PENDING. initial
DETAILS inner-padding fit check failed and was repaired by widening that header
button to 76 units; its initial failure is retained. annotated PNGs were
visually reviewed; these are design renders, not plugin screenshots. existing
theme/catalogue checks pass. final preservation/link/diff/checksum checks pass.

manual checks unperformed: native UI/helper implementation, native all-state
screenshots and resolved-font QA, DAW scaling/gesture recordings, keyboard and
screen readers, musician tasks/listening, UI timing, optional meters and resize
refusal handling. no binary rebuild/validator/sanitizer/realtime rerun is claimed
or required by this documentation-only boundary. G04 and all other release gates
remain open. original AU/host/realtime blockers are not repaired by a design spec.

next: on a separate authorized sprint 04, implement this contract with existing
attachments, coherent restore, mapping helpers and one immutable telemetry
consumer; test numeric edits under automation, detent boundaries, independent
faults and host gestures, then native accessibility/scales/timing. preserve
sprint 02 follow-ups without executing them here; REAPER remains skipped and no
additional testing apps may be installed. sprint 06 rights/packaging and sprint
07 qualification remain downstream.

compact resumable checkpoint: sprint 03 COMPLETE at design boundary; spec/layouts/
acceptance matrix plus exact commands and hashes retained. start at native-ux-spec.md;
all native acceptance cases remain PENDING. current source exports omit PNG/CSV/CJS;
retain the complete Sprint 03 packet separately until sprint 06 packaging work.
no release tag/publication.


## SPRINT 04 // NATIVE INSTRUMENT INTERFACE // 2026-10-14

**PARTIAL; release remains BLOCKED / development-unqualified.** This explicit
request supersedes the historical Sprint 03 next-sprint checkpoint. Implemented
the existing native UX contract with planar three-band layout, vector spools,
eight true bits, adjacent FLUX units, coupled BIAS macro, strict mapped numeric
entry, keyboard/fine/undo/host gestures, uniform 100/125/150/200% transform and
actual fitted scale text. APVTS bindings, native roles, IDs/catalogue/schema/DSP,
free full distribution, formats and deferred scope remain unchanged.

Independent input UNKNOWN/observed quiet, limiting hold, brake/pending, cumulative
rejection/history and telemetry health coexist. DETAILS explains host targets vs
DSP, performance-only CCs, recall/undo consequences and recovery; four real sampled
L/R peak lanes use existing per-channel telemetry. Single bounded 30 Hz consumer,
reopen/stale/drop/wrap handling, limited repaint regions and hidden-editor timer
stop are implemented. Immediate undo exposed a deferred APVTS tree-flush problem;
user gesture/click/recall boundaries now flush that existing tree outside audio.
No DSP/state/undo framework replacement or new saved parameter was introduced.

Revision `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus preserved dirty work.
No applicable AGENTS.md. Prerequisite hashes: Sprint 03 66/66; prior Sprint
00/01/02 sole mismatch each was superseded live state.md. All four historical
state hashes match retained next-sprint baselines. 806 pre-edit input hashes,
initial status and state retained in `evidence/sprint-04/`.

Final local Release checks: **15/15** regression tests outside sandbox plus the
separate **native editor fixture PASS**. 3072 state/scale renders + all 256 words,
49 representative component PNGs, 3328 raw image hashes, 9216 measured native
font extents, parser/detent/gesture/typing/undo/lifecycle/telemetry/meter checks.
The first build/test failures, AU sandbox registration failure and interrupted
build are retained, not hidden. AU characterization remains a known offset gap.
Exact commands/cwd/times/results/log hashes: `evidence/sprint-04/commands.json`;
expanded recipes, artifact identities, limitations and next actions:
[sprint-04.md](sprint-04.md). Source formatting, strict local bundle seals,
source/theme/catalogue/contract checks and packet/preservation checks are recorded.

| Final local artifact | Executable SHA-256 |
|---|---|
| VST3 | `865127bf08ed8ddb5a3236741c81fc4733f42f18a73e33d9ee516613fa8fd27c` |
| AU | `e4642ab2ddcc24f2b7b2dab18b6213dd6b79cbeb61db513781b3ebd72b2800a0` |
| Native development harness | `c83a5fa2574fd1db9c68f4e6423f33ae7bafd9218ae380e3a5e1a24c81341456` |

Actual native harness checks cover mapped value/undo, real Tab/Enter numeric entry,
keyboard steps, invalid/cancel, domain/STATE/brake/bypass coexistence, reset, four
scale indicators and inspector modal/focus return on explicitly named candidates.
Final guard binary separately repeats mapped value/undo, typed entry, scales and
reset. Live 12.4.6 scanned/loaded the earlier named accessibility VST3; its floating
editor was not exposed to the tool. No DAW UI/automation qualification transfers
to the final wheel-guard binary. Only owned empty Untitled test sets were discarded;
the hash-verified temporary Library plugin copy was moved back to /private/tmp.
No other testing app was installed; REAPER remains skipped.

**Important limits:** actual 200% native window capture is partly offscreen/white
at its desktop position; complete deterministic component renders do not prove
real host reachability. Pointer/fine drag/wheel/double-click, all-scale keyboard
matrix, VoiceOver/Windows readers and DAW gesture recording remain unqualified.
System font measured Lucida Grande / Regular; file/version/fallback unresolved.
Earlier snapshot maxima **51.2571 / 75.4727 ms** exceed the stated frame budget.
Final run max **5.4364 ms**, mean .8105 ms, update max .0492 ms does not waive
those observations. Snapshot allocation/raster timing is not isolated native paint,
end-to-end response or declared minimum-machine/audio-load profiling. G04 and
performance/accessibility gates remain open; no sanitizer/validator, sound/beta,
platform/host support or whole-format realtime pass is inferred.

Changed existing files: desktop CMakeLists, PluginEditor.cpp/.hpp,
implementation-status and this state; added EditorControls.cpp, EditorValues.hpp,
EditorTests.cpp, sprint-04.md and evidence/sprint-04/. Inventory/SHA256SUMS enumerate
all paths and hashes, final and earlier bundle archives, logs and native captures.
All other baseline inputs, including dirty processor/wrappers/DSP, ADRs, manifest,
prior sprint packets and unrelated work, are preserved.

Checkpoint: **Sprint 04 PARTIAL**; implementation and local tests delivered.
Resume UX-03/08..14: obtain DAW floating editor access, fix/qualify 200% window
placement/reachability, complete pointer/keyboard/recorded gestures and native
screen readers, then separately measure timer/paint/response under declared audio
and instance load. Keep AU/realtime, rights/packaging and musician work in their
existing boundaries. No tag, release publication or scope exception.


## SPRINT 05 // MUSICAL BEHAVIOR AND RECALL TRUST // 2026-10-15

**PARTIAL; release remains BLOCKED / development-unqualified.** This request
supersedes earlier resume-only checkpoints. Repeatable musical/recall/undo fixtures,
measured review notes and a [concise user guide](../musical-guide.md) are delivered;
no production DSP/mapping/codec/preset changes are claimed. Stable IDs/schema,
free full distribution, planned formats, parameter-only empty-tape recall and
deferred scope remain unchanged. No tag/publication or new app installation.

Revision `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus preserved dirty work.
No applicable AGENTS.md. Required implementation-status/ADRs/manifest/state read;
prerequisite packet entries 36/100/187/66/223 verify, with superseded historical
state hashes checked against retained next-sprint baseline copies. Initial status,
state and 661 scoped input hashes retained in `evidence/sprint-05/`.

New host musical fixture renders 44 cases × six declared rates: extreme FLUX/BIAS/
STATE, reverse/detent/near-zero speed, brake, pause/run, gain/bypass, domain changes,
reset/preset, factory programs and silent/hostile input. Raw float measurements
include stereo peak/RMS/mean DC/deltas/rails/nonfinite, settled ten-second windows
after one second, per-second and 20 ms transition windows. 511/127-frame partitions
match exactly; 48 kHz repeat matches 90/90 raw artifacts. Four factory names/order/
values and independent 40-byte schema fixtures are locked without migration.

Native adapter fixtures pass 72 disk saved-state reopens across four programs/six
rates/three cycles, before/after prepare, exact PCM, rate changes, 48 malformed/
future cases preserving running history, input-disabled generation and performance
MIDI exclusion. Actual UI RESET/study/UNDO callbacks restore parameters, never
cleared tape. The first undo oracle mistook a departing-domain .0283604395 peak
inside the accepted 10 ms ramp for recovered tape; the corrected later-silence
assertion passes and the failed log remains. No audio-history recovery is promised.

**Unresolved MB-04:** 88.2 kHz `performance-d2`, right, mean DC over seconds 1–11
is **−.011571761286181901**, beyond the .01 investigation trigger. The +20 dB
clipped second averages **−.11665187326634631**. Independent PCM oracle and strict
review both exit **1**, intentionally retained as open findings outside the green
regression count. One-case replay/source hash, recipe and compressed raw diagnostic
excerpt are retained. High-trim clipping, hard recall deltas (recall-1 .536378),
bypass/study energy changes and feedback/detent/brake transitions still require
listening and intended/unintended classification. No speculative gain/filter/map
change or fabricated fix is presented as sound qualification.

Eleven fixed **−24 dB / 48 kHz PCM16** audition copies, hashes and review sheet are
prepared. **Listening, monitoring calibration and musician participation are
UNPERFORMED**. The Ableton UI selection call timed out with error -10005 after
1028.1775 s; no new DAW project save/reopen/rate change succeeded. Native saved-state
files are not DAW projects. Historical Live reports retain their original exact
hash scope; no support/host/timing gate is promoted. REAPER stays skipped and no
other testing app was installed.

Final checks: core CTest **12/12**, native CTest **17/17 outside sandbox** excluding
the separate **native editor PASS** (new destructive undo plus inherited all-state
checks). The initial sandbox suite failed two AU factory registrations; authorized
rerun passed. Six-rate bounded/nonfinite/partition checks, repeat hashes, independent
schema fixtures, formatting, strict local VST3/AU seals, source/theme/catalogue and
preservation/checksum checks pass. **Strict DC investigation FAIL** remains separate;
finite samples never establish good sound. Initial target/configure and one-case
CLI compile/stale-binary failures are retained with successful repairs. No sanitizer,
validator, actual DAW, Windows, realtime/soak or musician pass is newly claimed.

| Artifact | SHA-256 |
|---|---|
| VST3 executable; unchanged Sprint 04 identity | `865127bf08ed8ddb5a3236741c81fc4733f42f18a73e33d9ee516613fa8fd27c` |
| AU executable; unchanged Sprint 04 identity | `e4642ab2ddcc24f2b7b2dab18b6213dd6b79cbeb61db513781b3ebd72b2800a0` |
| MB-04 complete raw stereo float WAV | `9808db814008b27d54d7ec69f1b43a0c00e1f961381ae113ddedb438c209b0ec` |
| Program 1 canonical saved state | `31e6acb42f70d34e60cfef21dca93d447464dba6dbbe5de62d857ee2518b2975` |
| Program 3 canonical saved state | `0447fbace9b34d96027d1241fb9697bd2c313e375872ab3680acc4458299fdfb` |

Changed existing files: desktop CMakeLists/EditorTests, host tests CMakeLists/
host_tests, implementation-status, desktop-guide and this ledger. Added musical/
native recall fixtures, review_musical.py, musical-guide, listening notes,
[sprint-05.md](sprint-05.md) and the exact evidence packet. Commands, cwd, timestamps,
results and log hashes: [commands.json](evidence/sprint-05/commands.json). Full changed
paths/source/artifact hashes: inventory.json and SHA256SUMS there. All other
baseline inputs, prior evidence, production processor/DSP/catalogue/schema/
wrappers/ADRs/manifest and unrelated work are preserved. Full raw float renders
and fixture executables live in /private/tmp; retain/regenerate them by recorded
recipes if that directory is cleared. The compact DC excerpt/auditions/metrics/
saved states remain in the repository packet. Evidence packaging remains Sprint 06.

Checkpoint: Sprint 05 **PARTIAL**. Reproduce MB-04 with the one-case CLI and failing
independent oracle first; diagnose limiter/DC behavior and evaluate a focused fix
without changing accepted mappings or recall/bypass semantics. Close it with a
regression and renewed six-rate/listening evidence. Perform controlled listening
MB-01..04 with named reviewer/device/level and actual stereo music; recover Ableton
UI and run three real project reopen cycles per study plus sample-rate/reset/undo,
retaining exact candidate/project/PCM hashes. Keep prior AU/realtime/UI/rights/
packaging blockers and no-new-apps/REAPER steering. Do not publish or tag.


## Sprint 06 / 2026-10-15 / PARTIAL

Revision `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus preserved dirty inputs.
[Sprint 06](sprint-06.md) records the full scope, exact commands/results, changed
files, failures, artifact hashes and resume actions. Release stays **BLOCKED /
development-unqualified**; validated platform/host arrays remain empty. No tag,
publication, push, personal plugin-directory installation or other sprint's
product behavior was performed.

CMake now uses archive and full-tree dependency pins, including drift rejection
for local overrides and final provenance rechecks. CI covers separate core/native
ASan/UBSan and TSan, first-party formatting/tidy/analyzer work and exact-bundle
native validators; remote execution remains unperformed. Source export omissions
for Objective-C++ and extensionless licenses were repaired. The exported native
source built in an isolated directory with fresh declared dependency retrieval,
no Git/brand siblings and empty dependency overrides, then passed **21/21** tests.
Native ASan/UBSan passed **5/5**, core initial **14/14** plus final changed-fixture/
contract **4/4**, core TSan **2/2**, native TSan **1/1**. Native analyzer covered
12 first-party units; tidy18 passed with compatible SDK15.2; format passed.
The enum ABI is retained; only .mm whitespace and fixture integer-widening casts
changed in C++/Objective-C++ sources. Production runtime behavior is preserved.

Concrete local packet: `sys-01/dist/sys01-sprint06-macos-arm64/` and its **6,639,341
byte ZIP**. ZIP SHA-256 `cf4f2515a81c35751c7dcfc2a37d867f8eee9353f40e62e938724146cde7ecd4`;
source archive `4850c00082b539303e832235b79c4c2c37d960afbcb072c61c156b684ab7475d`;
candidate manifest `0b348fb85761e2bab4267247c50b93d4dfde02ced4b2cbbf6e9b020a26dba21f`.
The manifest binds revision/dirty source inputs, toolchain/SDK/settings/commands,
verified dependency inventory, executable/bundle hashes, reports and prerequisites.
Eight reports match their exact named bundle bytes/logs. Final VST3 executable:
`bdf225221f23bd28c75394f814cc212d0c922bc563b2c08a281fa5b176dbd968`;
AU executable `709acf92683d65c5de0ed712cca7cf6173d1c20114615d4e1b98aef3ce334996`.
Steinberg **47/47**, pluginval **level10/seed1012 headless**, native VST3/AU and
strict ad-hoc seals pass. **Strict AU offset contract still FAILS** and is retained
as FAIL. No fresh local auval/DAW installation is claimed. Source and same-input
ZIP repetition pass; independently rebuilt or bit-identical signed/notarized
bundles are not proved. Final ledger/evidence is separate from the source snapshot
to avoid self-referential hashes.

Owner-selected **AGPL-3.0-only** text and eleven selected verified dependency
notices are retained; no external font asset is bundled. **G08 remains BLOCKED**
for first-party/transitive applicability and complete corresponding-source review.
Developer ID Application/Installer identities are absent; unrelated self-signed
identities found outside the sandbox were not used. Signing/notarization recipes
were tested in plan mode only. The owner selected Windows ZIP containing VST3, notices/source and PowerShell
install/rollback scripts, with Authenticode when credentials are available.
Fixture-only install/rollback/uninstall scripts and a Windows CI synthetic-control
job are prepared. Windows credentials/provider, approved timestamp endpoint/policy
and machine remain prerequisites; PowerShell parsing/runtime checks are unperformed
locally and remote CI has not run.
**G09 remains BLOCKED**: clean-system install, actual host scan, upgrade, uninstall
and rollback are all UNPERFORMED. Dedicated clean target fixtures/approved host
builds and a prior signed version are prerequisites. Local no-secret diagnostics
remain available and emit only OS/architecture, supplied host and optional binary
hash, without scanning credentials or uploading anything.

Changed file paths/hashes are in `evidence/sprint-06/inventory.json`; full exact
argv/results/log hashes in `commands.json`; packet verification, source isolation,
negative controls and prior retained-evidence checks in the same folder. Preserve
all earlier realtime/AU/sound-DC/UI/machine/beta/soak blockers. Resume from
Sprint 06's checkpoint: verify packet, use the selected Windows ZIP/PowerShell route, resolve credentials/rights,
sign new staged artifacts, rerun validators on final bytes and execute the three
clean-system lifecycle cycles per claimed cell. No release gate is waived.


Windows route follow-up: direct owner reply selected ZIP/VST3/notices/PowerShell
install and rollback, with Authenticode signing when authorized credentials exist.
The choice is now recorded in `release/windows-route.json`; it is no longer an
owner-decision blocker. Scripts enforce a marked temporary Windows fixture,
closed hosts, candidate/receipt hashes and signatures by default; preserve owned
backups/projects and reject reparse/escaping paths or unexpected changes. Source
export/packaging carries the tools; Windows CI adds parser/lifecycle/failure controls.
No PowerShell runtime was available or installed locally, so those controls and
all real Windows install/host/upgrade/uninstall/rollback cycles remain UNPERFORMED.
See Sprint 06's follow-up and `windows-route-followup/owner-decision.json`.
The original checkpoint is preserved under that evidence subdirectory; updated
packet hashes below supersede its archive metadata, never its native validator bytes.


## Sprint 07 / 2026-10-15 / PARTIAL

**Release BLOCKED / development-unqualified; qualification exit not achieved.**
The owner confirmed “only myself and my computer”. Five-musician, external clean/
minimum-machine and Windows tests remain **PENDING**. The complete 16-cell matrix,
native screen-reader/host visual/keyboard work, eight-instance/eight-hour sustained
workload and real signed install/upgrade/uninstall/rollback are unperformed or
incomplete. No requirement, accepted format or deferred scope was changed; no
validated host/platform was added. No release/tag/publication/commit.

Revision `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus preserved dirty work.
No applicable AGENTS.md; required status/ADRs/manifest/previous state read. Initial
Git status, 1,121 input hashes and prior state are retained in
`evidence/sprint-07/`. Before editing this state, Sprint 06's complete **268**
hashes/report bindings verified; retained internal Sprints 00–06 evidence counts
**34/96/184/35/214/143/154** verified. The earlier live-state checksum is now
historical and matches Sprint 07's baseline-state.md. Earlier packets were not
resealed. Candidate compilation inputs still match current source/provenance.

Reused the exact Sprint 06 macOS arm64 packet without rebuilding or signing:
VST3 executable `bdf225221f23bd28c75394f814cc212d0c922bc563b2c08a281fa5b176dbd968`,
AU executable `709acf92683d65c5de0ed712cca7cf6173d1c20114615d4e1b98aef3ce334996`.
Complete bundle fingerprints remain VST3
`5c08c3b817d1560a8329dedede5ef51f3037b96a9d0414a8574434e4516ea597`, AU
`ec455f7a09f7d6f10522bb3e04bd2d7065b063c92ea826d54f6ab37f84231382`.
Loaded VST3 conformance passes 201,894 points/seven IDs/six rates, and exact AU
callback passes with normal framework access after sandbox registration failed.
**Strict AU ordinary/scheduled offset contract FAILS** on the exact candidate;
positioned CC22 passes. Five new bound reports retain both successes and failures.

Probe-free local VST3 diagnostics measured 20,000 callbacks per editor-open/
closed and 128/64-frame condition, twice, at 48 kHz, 224 ordinary points + 32
positioned CCs, one unpaced dedicated thread. Initial sandbox maxima open/closed
128 **8.485792 / 5.785708 ms**, 64 **2.795333 / 3.178500 ms**. Normal-framework
retry open/closed 128 **.114000 / 14.314625 ms**, 64 **.129625 / .091750 ms**.
Across both finite runs **22 deadline misses / 96 half-budget breaches** are
retained, with percentiles and clock overhead in timing-observations.json.
This is neither an eight-instance/driver/host soak nor hard realtime proof;
background scheduling/load was uncontrolled and no causal editor effect is claimed.
G01/G07 stay open, including retained wrapper locks/TLS/listener ownership issues.

Source-matched linked native adapter/recall fixtures pass, including 72 reopens,
four studies/six rates/three cycles and malformed/empty-tape behavior. These are
not DAW or loaded-format recall passes. Source-matched editor passes 3,072 state/
scale renders, 256 register values, glyph extents and existing gesture/typing/
undo/hidden timer assertions. Full snapshot max **23.9760 ms**, mean **1.1094 ms**,
updateView max **.3835 ms**; snapshot exceeds the UI target and is not isolated
native paint/input/declared-load qualification. Representative 200% face/details
images were inspected; host reachability, keyboard and assistive technology remain
unqualified. Existing larger observations remain retained.

Actual installed Live **12.4.6 (2026-09-10_0de5c8fa9a)** on macOS **26.6.2 /
25G83 arm64** scanned/loaded the fingerprint-verified temporary VST3 copy; lsof
confirmed that path. CoreAudio, hardware input **No Device**, system MacBook Pro
Speakers, **48 kHz / 512 frames**. Generation and host FLUX/BIAS/domain edits were
observed. A new saved cellular project reopened once with matching targets; its
two offline 32-bit float stereo bounces are exactly equal in PCM, **390,000 frames /
8.125 s**, peak L/R **.0647313222/.0618666597**, zero nonfinite samples. PCM SHA-256
`737ab43b9b358c899c0b1c4af4202cc2dcf22dcb4e24a01c5eb9ea5754b0699c`; project
`f6d41e9ebe78c48e75a239c7cd8ef4bb5e1cfc3fa3c247c70383d0f8676ccba2`.
The one-sixteenth render remainder persisted despite an attempted edit; actual
frames are authoritative. No listening/MB-04 acceptance is claimed.

CUA repeatedly timed out around export/reopen, then recovered. Live remained
running and a short stack sample mostly shows normal event-loop waiting; no
confirmed crash or root cause is inferred. The main-window capture omits the
floating native editor. Generic host AX roles/values/actions do not qualify the
native editor or VoiceOver. Final Untitled/empty device view/stopped transport/
silent peaks were observed after closing only the saved owned test session.
Custom VST3 source was verified Off; a remembered temporary folder path remains.
Original 44.1 kHz/16-bit export values were restored and observed; Cancel's state
read timed out. No personal plugin folder change or new testing app; REAPER skipped.

Three P1 issues remain assigned/open in `evidence/sprint-07/issues.json`: AU offsets
to format engineering; observed callback budget/deadline failures to audio/platform
QA; retained complete-callback ownership/locks to audio engineering. Roles are
accountability assignments, not hired participants or spawned agents. MB-04 DC/
listening, snapshot timing and UI-tool behavior retain investigation owners. No
speculative production fix or closed defect is claimed. Signing/rebuilds invalidate
affected evidence and require new hashes/affected native/host/validator/UI/timing
checks; this sprint did not change binary bytes or transfer old host results.

[Sprint 07](sprint-07.md) contains full exact commands/results, hashes, changed
files, failures, limitations and next actions. Machine records: commands.json,
qualification-matrix.json, host-artifacts.json, timing-observations.json and
inventory.json/SHA256SUMS under `evidence/sprint-07/`. Loaded commands run through
the existing immutable-artifact wrapper; strict AU exits **1**, sandbox AU first
exits **1** / normal access retry **0**, fixture/integrity checks pass. Independent
`verify.py --require-qualified` exits **1** because gates remain unmet. No new
build/sanitizer run is claimed for the evidence/document-only boundary. Existing
state.md is the only pre-existing file changed; all other baseline file hashes,
including unrelated dirty work, are preserved. The sprint ledger and exact evidence
packet are added. No implementation foundation was reworked.

Checkpoint: **Sprint 07 PARTIAL; release BLOCKED.** Verify `evidence/sprint-07/verify.py
--checksums`, then reproduce assigned AU and timing failures. Complete focused
repairs with ownership proofs and affected reruns on new exact bytes. Recover
actual host editor/AT access and finish local transform/recorded automation/MIDI/
internal bypass/all-study/lifecycle checks. Obtain five first-time musicians,
external clean fixtures, credentials and previous signed packages; execute the
unchanged eight-hour/eight-instance and three-cycle installation requirements.
The pending manual protocol specifies completion/confusion/recall capture without
invented participants. Preserve all earlier sound/DC, rights/source and platform
blockers. Do not mark qualification complete with an open P1; do not publish or tag.


## Sprint 08 / 2026-10-15 / PARTIAL / NO-GO

**Release BLOCKED / development-unqualified.** The local final review packet and
user documentation are prepared. Three assigned P1 issues and required manual
gates remain open; a qualified final candidate cannot be declared. No tag,
publication, commit, support promise, scope exception, installation, approval
request or new testing app. Full release at no charge remains unchanged.

Revision `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus preserved dirty inputs.
Required status/ADRs/manifest/state/qualification and packaging evidence read;
no applicable AGENTS.md. Before editing state, Sprint 07's **64** hashes and five
report bindings passed; retained internal evidence Sprints 00–07 verified
**34/96/184/35/214/143/154/62** hashes. Initial 1,183 file hashes/status and prior
state are retained. Earlier live-state hashes are historical; no packet resealed.
All original build inputs and bundle bytes still match Sprint 06 provenance.

Concrete local artifacts under `sys-01/dist/`:

| Artifact | Bytes | SHA-256 |
|---|---:|---|
| `sys01-1.0.0-sprint08-macos-arm64-review.zip` | 6,741,398 | `43970a6e244d3bd6033c5682d09659611dd693194ad3c2eb3179fd1fda5d8aaf` |
| `sys01-1.0.0-sprint08-source.tar.gz` | 330,177 | `fbbebefb53e7cc0890be7d5471ed346953302cab1914e5fa53c042033e955999` |
| review directory `candidate.json` | 24,045 | `510c3501e02b6e70d6f0e1d68717338044e5293c717df0634dc860c1e44da252` |

Packet directory: `sys-01/dist/sys01-1.0.0-sprint08-macos-arm64-review/`.
Unchanged VST3 executable SHA-256
`bdf225221f23bd28c75394f814cc212d0c922bc563b2c08a281fa5b176dbd968`;
AU `709acf92683d65c5de0ed712cca7cf6173d1c20114615d4e1b98aef3ce334996`.
Both are arm64/ad-hoc, not Developer ID/notarized. Exact source archive, original
build/toolchain/dependency inventory, selected notices, 16-cell matrix, open issues,
G00–G10 decision record and external retained-evidence index are bound in the
manifest. Source freeze precedes this final ledger; the separate final evidence
inventory binds both. Dirty source revision plus per-file hashes is explicit.

Fresh exact staged-byte checks: loaded VST3 **201,894 points/seven parameters/six
rates PASS**, Steinberg **47/47**, pluginval **level10/seed1012/headless PASS** with
normal framework access, AU callback/characterization PASS, strict ad-hoc seals
PASS. **Strict AU ordinary/scheduled offsets FAIL** with normal framework access;
positioned CC22 passes. Sandbox AU registration failures and pluginval abort are
retained alongside retries. Twenty reports bind exact unchanged bundles/logs
(eight retained/twelve fresh). Current installed AU auval remains UNPERFORMED;
historical earlier-byte auval evidence is not rebound. No new timing/native
sanitizer/full host/AT/listening/beta/soak/signing/install result is claimed.

Fresh source export is reproducible, manifests/notices verified; clean extracted
core Release build passes **15/15**, no Git/brand sibling or dependency override,
fresh declared pinned doctest download. Initial missing CMake PATH and sandbox
DNS failed; absolute existing tool/network retry passed. Archive helper initially
truncated a versioned path, then was repaired; failed archive/missing-index gate
attempts remain in the command ledger. Final ZIP repetition, all **83** members,
exact bytes/modes/sizes and fresh extracted packet verification pass. Independent
`verify.py --require-go` passes integrity then exits **1 / NO-GO** for open gates.

[Sprint 08](sprint-08.md) and [decision-08.md](decision-08.md) record all gates,
manual omissions, commands/results, hashes, limitations and next actions.
Changelog, first-minute guide, troubleshooting and install/rollback are finalized
as `*-08.md` source documents and exact top-level packet copies. Historical root
CHANGELOG.md remains an unchanged build input. No lower-priority defect exception
was accepted. ADR 0002 product limitations remain; DC/listening, UI timing and
whole-callback findings are not waived. No supported hosts/platforms are declared.

S07-P1-01 AU offsets, S07-P1-02 timing failures and S07-P1-03 native ownership/locks
remain assigned/open. Required host/scale/keyboard/readers, MB-04 DC/listening,
all-study real project cycles, five musicians, eight-instance/eight-hour/minimum-
machine workload, rights/corresponding-source review and three signed clean-system
install/scan/upgrade/uninstall/rollback cycles remain pending. Windows/native Intel/
minimum OS/clean fixtures, signing credentials and prior signed packages are
missing. REAPER stays skipped without deleting its unqualified matrix cells.

Changed existing: state.md only. Added: sprint-08.md, five user/decision documents,
`evidence/sprint-08/` and the local dist artifacts. All other **1,182** baseline
file hashes, including unrelated work, remain unchanged. Exact argv/results/log
hashes are in `evidence/sprint-08/commands.json`; complete changed/added file and
artifact inventories are inventory.json/SHA256SUMS and artifact-index.json there.
No production/code/schema/catalogue/format/manifest/dependency foundation reworked.


Final document review changed BRAKE from “momentary performance state” to
“performance control” to avoid implying a momentary button. Source/doc packaging
was regenerated from the original extracted snapshot; original build inputs and
all binary bytes remain identical. The artifact table/index above supersedes
the earlier packaging log hashes. Initial Sprint 08 pre-correction artifacts are
retained in `/private/tmp/sys01-sprint08/pre-document-review/`; no earlier-sprint
artifact was moved. New source/ZIP repeats and extracted-packet checks pass.

Checkpoint: **PARTIAL / NO-GO; release BLOCKED.** Verify
`python3 sys-01/docs/release-1.0.0/evidence/sprint-08/verify.py --checksums` first.
Next implement a focused S07-P1-01 AU offset repair with safe ownership proof and
regression, then rerun affected checks on the new exact bytes. Close all other
P1/manual gates before GO; rebuilding/signing/stapling invalidates affected
identity/evidence. No publication/tag is authorized or performed here, so no final
owner approval is requested. Do not release by changing a status string.
