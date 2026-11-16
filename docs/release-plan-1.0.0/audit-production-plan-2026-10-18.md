# SYS 01 production audit and version 1.0.0 completion plan

Audit date: 2026-10-18, America/Santiago. Repository: `/Users/lhommeduchili/algorhythmia/sys-01`. Git repository root: `/Users/lhommeduchili/algorhythmia`. Revision: `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus the current working tree.

SYS 01 has a substantial working desktop implementation, but version 1.0.0 is **NO-GO**. The remaining work includes product defects, native realtime architecture, cross-platform evidence, human acceptance, and production distribution. Repackaging the existing candidate cannot close these gaps. Begin with a focused AU automation repair, follow it with safe callback ownership, then qualify the resulting candidate through the plan below.

This plan continues the completed October 13–15 work as Sprints 09–17. Earlier Sprints 00–08 and their sealed evidence remain historical records. [rolling-state.md](rolling-state.md) is the compact entry point for subsequent sessions; [sprint-09-kickoff.txt](sprint-09-kickoff.txt) is the first complete prompt. This audit changes documentation only and does not promote the release manifest.

## Release scope and authority

The accepted product contract remains [ADR 0002](../adr/0002-desktop-control-state-and-release.md), with generated wrapper boundaries defined in [ADR 0003](../adr/0003-positioned-vst3-integration.md). [release/manifest.json](../../release/manifest.json) remains the machine-readable qualification authority. Existing source version `1.0.0` identifies a development target, not a released artifact.

Deliver SYS 01 as a stereo effect/generator: macOS VST3 and AU, Windows x64 VST3. Generation must work with silent or disabled input; transform uses external stereo input. Keep the seven stable parameter keys, literal native IDs, bundle/manufacturer/plugin IDs, four existing programs and their order, schema 1 with its exact 40-byte encoding, and parameter-only deterministic recall with empty audio history. Continuous automation steps at its sample position and uses the existing 10 ms smoothing. Preserve 256 merged events, whole-batch rejection, stable ties, no rejected-target replay, eight restore slots and one audio owner.

The full release remains free, with optional donations. AGPL-3.0-only is the selected first-party license; dependency/provenance and corresponding-source acceptance remain required. Standalone is a development harness. Max for Live, hardware shipment, CLAP/AAX, public standalone, host sync, MIDI learn, captured-audio recall, desktop hold-save and a mix control remain later milestones. They are not missing desktop 1.0.0 features.

Historical owner steering permits only the installed Ableton host and skips REAPER; do not install other testing apps. External target machines, participants and credentials must be supplied through permitted arrangements. An audit or sprint plan does not authorize contacting people, publishing, tagging, or changing personal plugin directories.

## Audit results and confidence

### Checks performed on the current source

The [audit evidence directory](evidence/audit-2026-10-18/) records expanded commands, tool hashes, logs, the captured source inventory, and JUnit output. Fresh core configuration/build used Apple Clang 21, CMake 4.4.4, Release mode, benchmarks enabled, desktop disabled and the existing doctest tree. All 270 dependency files passed the pinned tree verification before use. This was a fresh build directory, with a verified local dependency override; it was not a fresh network dependency retrieval or a native plugin build.

| Check | Observed result | Meaning |
|---|---|---|
| Fresh Release configure and build | PASS | Current portable sources build under the named local toolchain. |
| Core CTest | **16/16 PASS** | DSP, scheduler/state, recall/musical bounds, dependency pins/notices, WAV/memory contract, source export and catalogue checks pass. |
| Retained Sprint 08 integrity | **182 sealed hashes PASS**, candidate/report bindings PASS | The previous development packet and baseline inputs remain intact. |
| Independent release decision | **Exit 1, NO-GO** | Three retained P1s and required technical/manual gates remain open. |
| Fresh MB-04 single-case render | PASS render; exact historical WAV SHA-256 reproduced | The DC episode remains reproducible from current source. |
| Independent MB-04 PCM oracle | **FAIL**, settled right-channel DC `-0.011571761286181901`; high-gain final second `-0.11665187326634631` | The existing absolute mean threshold of 0.01 is exceeded. This requires diagnosis and musical judgment. |
| Source packaging, evidence negative controls, notice hashes, theme and Max patch checks | PASS | Existing infrastructure works at these particular boundaries. Rights and native accessibility remain separate gates. |
| Existing tracked diff whitespace | PASS | No whitespace errors found in the inherited SYS 01 diff. |

There was no fresh native build, native sanitizer run, validator execution, DAW operation, screen-reader session, listening session, beta, signing, Windows run or soak in this audit. Native findings below come from source inspection and the intact named historical packet. A passing core suite does not test AU timing parity; current CTest deliberately characterizes the defect instead of rejecting it.

### Engineering already delivered

The portable DSP implements deterministic bytebeat, cellular FM and live stereo transform. The shared host layer implements canonical controls, ordered bounded events, smoothed trim/bypass, transport/STATE/brake behavior, telemetry and validated recall. JUCE VST3/AU wrappers and a native vector editor are implemented. VST3 has substantial independent loaded-binary offset coverage, including seven parameters and six rates. Host-neutral and native restore/lifecycle fixtures, malformed state cases and factory-program compatibility checks exist.

Historical native work includes Steinberg 47/47, pluginval level 10, strict arm64 auval on specific earlier bytes, arm64 and Rosetta fixture runs, separate ASan/UBSan and TSan coverage, and limited actual Live scan/generation/transform/bounce/reopen evidence. These results retain their original binary and platform scope. Rosetta execution does not establish native Intel or old-OS compatibility.

Dependency archives and complete trees are pinned. Source export is reproducible; candidate reports bind entire bundle content, logs and source/toolchain manifests. Local bundles have working ad-hoc integrity seals. macOS signing plans and transactional Windows fixture scripts exist. These are useful foundations to finish, not reasons to restart the product or rebuild the editor from scratch.

### Gaps that prevent production

| Finding and priority | Current evidence | Required closure |
|---|---|---|
| **S07-P1-01: AU automation offsets collapse to zero** | `AUOffsetTests.cpp` independently drives SetParameter and ScheduleParameters; strict mode fails. `native_au_adapter.py` only bounds MIDI. | Bounded, correctly owned AU parameter ingress; strict parity and hostile/lifecycle cases on rebuilt bytes. |
| **S07-P1-03: complete native callback retains locks/allocation paths** | [native-ownership.md](../release-1.0.0/native-ownership.md) traces wrapper callback/MIDI/listener locks, async UI posting and first-thread TLS allocation. | Replace affected ownership/publication paths safely, then instrument the complete shipping entry. Deleting locks alone is unsafe. |
| **S07-P1-02: diagnostic callback deadline misses** | Eight finite 20,000-block conditions retain 22 misses and 96 half-budget breaches; normal-framework closed/128 maximum 14.314625 ms. Runs were unpaced ordinary threads. | Separate scheduler/instrumentation effects from callback work; repair measured work and meet declared Core Audio/ASIO/DAW workload budgets. No inferred editor causality. |
| **MB-04 / S07-INV-01: DC and listening unresolved** | Fresh oracle fails identically; listening, transitions, clipping/loudness acceptance and repeated actual projects incomplete. | Root-cause analysis, focused repair or named intentional-behavior acceptance, six-rate regressions and controlled listening. |
| **S07-INV-02: UI responsiveness and accessibility unqualified** | Native source/state fixtures exist. 23.976 ms source-matched snapshot maximum exceeds UI target but includes raster/allocation work. Historical larger outliers retained. | Actual paint/timer/input measurements under audio load; all scales reachable in host windows; keyboard and native reader tasks. |
| **S07-TOOL-01: Live UI test reliability** | Export/reopen automation sometimes reports timeout/noWindowsAvailable while PCM export succeeds; cause unconfirmed. | Reproducible operator-assisted lifecycle workflow and evidence capture. Do not call this a plugin crash without proof. |
| **No complete supported matrix cell** | Manifest validated arrays are empty. Final packet has 16 original cells; only current-arm64 Live VST3 is partial. | Explicit viable matrix, exact final candidate and all mandatory checks per claimed cell. |
| **No verified remote CI run in retained evidence** | Active root workflow and standalone-export workflow exist; the latter is nested and not active in this monorepo. AU strict parity and full Windows validators are absent. | Run root CI on reviewed source, add enforceable strict gates, retain run IDs and final-byte reports. |
| **Source baseline is not committed** | 27 modified tracked SYS 01 files, 2,063 additions/345 deletions, plus substantial untracked foundations, tests, licenses and sprint evidence. Sibling work is also dirty. | Review and capture all intended SYS 01 inputs; use scoped commits/export and isolated builds. Never bulk-stage the monorepo. |
| **Signing, production installation and rights incomplete** | Ad-hoc macOS seals only. Windows scripts accept development packets and temporary fixtures only. `verify_candidate.py` intentionally rejects release-qualified status. | Production package/schema/installation contract, corresponding source, signing/notary, supported-system lifecycle tests and final release decision tooling. |

CMake is absent from the default shell PATH. An existing absolute CMake/CTest path remains usable and is recorded in the audit commands; old `/private/tmp/sys01-sprint*` work directories are absent. New sessions must locate tools and dependency inputs rather than assume old temporary paths survive. The historical release state is 67,888 bytes and mixes current and superseded tables; repeated full-ledger loading is a context-management gap.

## Gate ownership and acceptance

Owners below are accountable roles, not claims that additional people have been hired. The user is the product/rights/release owner. One engineering implementer can take sequential roles; human beta and external-machine evidence cannot be synthesized.

| Gate | Accountable role | Production exit |
|---|---|---|
| G00 baseline | Engineering lead | Reviewed source snapshot/commit, exact pins, decisions and reproducible bootstrap; no unrelated files included. |
| G01 whole native realtime | Audio engineer | Zero allocation/deallocation, blocking locks, I/O and unbounded plugin-controlled work across shipping callback paths; ownership/reclamation and concurrency proof. Host-owned callbacks are identified separately. |
| G02 automation | Format engineer | Independent AU/VST3 loaded-binary parity for all parameters, six rates, event limits, offsets/ties/quantization and rejection/no replay; DAW-origin automation/MIDI behavior verified. |
| G03 validators and DAWs | Platform QA | Required validators and complete host operations for every active supported cell, bound to final signed bytes. |
| G04 native UI | UI engineer and accessibility QA | Complete reachable states/scales, keyboard/name/role/value/actions, native readers and declared-load responsiveness. |
| G05 sound and session trust | Audio engineer and musician reviewer | No unresolved unintended DC/click/loudness defect; exact within-build recall, repeated real project cycles and accepted transitions. |
| G06 first-time musicians | Product owner and QA | Five actual first-time musicians; at least four complete first-minute and transform tasks independently; all five understand parameter-only recall after concise guidance. |
| G07 performance and soak | Audio engineer and platform QA | Named minimum-machine budgets, eight simultaneous stereo instances and eight wall-clock hours per active matrix cell, with zero observed misses/underruns. |
| G08 rights and source | Rights owner and release engineer | First-party provenance, compiled transitive dependency applicability, notices and complete corresponding-source packet accepted. |
| G09 signing and installation | Release engineer and platform QA | Developer ID/notary macOS package and Authenticode Windows route, final signatures/hashes; three lifecycle cycles per supported package and host checks per supported cell. |
| G10 decision and delivery | Release owner | No open P1 or required gate, approved exact packet and explicit publication/tag authorization. |

Retain the existing quantitative criteria. Single-instance maximum must be below half the block deadline: below 1.333 ms at 48 kHz/128 and below 0.667 ms at 48 kHz/64. The eight-instance 48 kHz/128 DAW cycle must stay below 80% of 2.667 ms, approximately 2.133 ms. Report p50/p95/p99/p99.9/max, sample count, miss counts, driver/CPU/power/load and clock overhead. No eight-instance/64-frame promise is implied.

Fixture rates are 44.1/48/88.2/96/176.4/192 kHz and blocks 0/1/7/63/64/65/511/4096; add 128 for budget tests. UI remains 640×440 minimum with uniform 100/125/150/200% scale, normal-label contrast at least 4.5:1, large text at least 3:1, feedback at most 33.33 ms and timer plus actual paint maximum below 16.67 ms under declared load. Closed editors do no timer/repaint work.

Sound investigation triggers remain absolute DC over 0.01, transition delta over 0.1 and unintended RMS jump over 3 dB, using ten seconds after one-second settling. These triggers are not automatic rejection of deliberate bytebeat edges. Intentional exceptions require a named fixture, monitoring level, reviewer and product acceptance. Never raise thresholds merely to clear the current case.

## Matrix and prerequisite work

The original target machines were an Intel MacBook Pro 2017/i5/8 GB on macOS 11.7.10, M1 Mac mini/8 GB on macOS 11.7.10, the current M1 Pro/16 GB Mac on the recorded macOS 26.6.2 build 25G83, and i5-8250U/8 GB Windows 10 22H2 and Windows 11 24H2 targets. Treat these as historical qualification targets. Record the actual current OS/build when testing; verify host/dependency/deployment-target feasibility before committing to old-OS support.

There are eight Live format cells across those five targets: two formats for each of three Macs and one VST3 format for each Windows target. The original matrix doubles that to 16 through REAPER. The current instruction to skip REAPER conflicts with the old requirement for every original cell to be complete. Recommended resolution: keep all 16 historical rows immutable and create a versioned active matrix with eight Live cells, explicitly recording REAPER as excluded from 1.0.0 qualification under the retained owner steering. Obtain product acceptance of the support wording before claiming coverage. If a target OS is infeasible, prepare an evidenced proposal; retain NO-GO until any scope change is accepted. Do not silently reduce planned formats or accept an AU timing exception.

Release prerequisites should be prepared early alongside local repairs, but only authorized operations may execute:

| Input | Preparation that can proceed now | Dependency for final closure |
|---|---|---|
| External machines and CI | Fixture manifest, scripts, exact workload projects and request list; verify runner architectures and OS support | Authorized access to Intel/arm64 minimum targets and Windows x64; run IDs, OS patch, driver, CPU, power mode and host build/installer hash. |
| Signing | Review command plans and credential selectors; isolate staging; define expected Team ID/publisher and timestamp policy | Existing authorized Developer ID Application/Installer identities, notary Keychain profile and Windows signer/provider. No secret copied into logs. |
| Rights | File provenance and compiled-component inventory; prepare source and notice packet | Rights-owner acceptance of applicability, licenses and corresponding source. Public archive URLs alone do not settle completeness. |
| Human acceptance | Five-participant task script, consent/data boundaries, listening set and results sheet | Actual first-time participants and musician review; contact only with explicit authorization. |
| Upgrade/rollback | Produce two distinct signed test candidates from known source, with unchanged IDs/schema; mark them internal fixtures | Three install/scan/save/reopen/upgrade/uninstall/rollback cycles. No previously released signed version exists; internal predecessor fixtures must not be described as historical releases. |

With eight active Live cells, a single eight-hour soak per cell totals at least **64 machine-hours**, excluding retries and lifecycle tests. Keeping the original 16 cells would require at least 128 machine-hours and a revised testing-app instruction. Offline simulated DSP hours do not satisfy this workload.

## Sprint sequence

Each sprint is a bounded delivery unit that can span several fresh sessions through checkpoints. Estimates are implementer-days, not automatic release promises. External availability and measurements determine the calendar. Finish a coherent change plus relevant checks before transitioning; a blocker never authorizes quietly starting a different release scope.

| Sprint | Main output | Prerequisites | Indicative effort |
|---|---|---|---|
| 09 | Strict AU automation repair and independent evidence | Audit baseline and AU ownership map | 3–6 days |
| 10 | Safe complete-callback ownership and notification repair | 09 ingress semantics | 6–12 days |
| 11 | DC/transition and session-trust closure | Stable audio/control behavior | 2–5 days plus listening |
| 12 | Measured callback and native UI budget closure | 10; accepted sound behavior from 11 | 3–6 days |
| 13 | Active support matrix, remote CI and native platform regression | Stable shipping source; external fixtures | 3–6 days plus runner access |
| 14 | Rights-complete source and production package/signing tools | Matrix contract, compiled inventory; credentials | 3–6 days plus review |
| 15 | Complete DAW routing/session and accessibility qualification | Final staged/signed candidate and fixtures | 3–6 days plus operator access |
| 16 | First-time musician beta and eight-instance realtime soak | 15 and release-identity freeze | 2–4 operator days plus ≥64 machine-hours |
| 17 | Final evidence decision, approved release and verification | All gates closed on unchanged bytes | 1–2 days plus release approval |

Nominal sequential engineering effort is 26–53 implementer-days, with no credible fixed release date until machines, signing, rights and participants are available. These ranges are planning allowances; wrapper architecture may require more work. Prerequisite preparation can overlap code work without requiring multiple agents. There is no automatic delegation in these prompts.

### Sprint 09 AU automation and source checkpoint

Start with the retained failing strict fixture. Locate and hash current tools/dependencies, capture tracked and untracked source, and preserve the working tree. Create new build/report directories; do not reuse disappearing historical temporary paths. Complete a reviewed source checkpoint suitable for scoped local commits or exact exports. The existing audit inventory identifies the inherited input boundary.

Inspect pinned JUCE AU SetParameter, ScheduleParameter, render dispatch/cache, Initialize/Cleanup/Reset, property changes and MIDI producer paths. Specify whether setters run on audio or control threads and how events belong to one render epoch. Replace ordinary/scheduled immediate parameter ingress with bounded storage, literal-ID mapping, discrete quantization, stable order and coherent host readback. Define ramp-event behavior openly against the accepted step-target contract. Do not enable SDK vector/split-render scheduling without bringing it inside the bounded ownership proof.

Cover zero render, repeated timestamps/cache reuse, varying lengths, stereo buses, bypass, inactive control updates, eight restores, rejected batches and next-block recovery. The AU producer model must be valid for all participating threads; do not claim SPSC if MIDI/setters can have multiple producers. Preserve the 256 merged-event cap across parameters/MIDI/UI and reject a whole invalid batch without replaying final host targets.

Expand `AUOffsetTests.cpp` from its current DRIVE characterization to all seven parameters, six rates, edge offsets, exact capacity and overflow, invalid points, quantization, lifecycle and reference sample comparisons. Preserve the failing stock/previous behavior as a negative control. Use `scripts/native_au_adapter.py` checksum gates and generated source; keep upstream trees/notices intact. Add an explicit strict qualification test/report that exits nonzero while parity fails. Green characterization must never imply strict conformance.

Exit: strict independent AU fixtures pass on the rebuilt locally available architecture; native VST3/adapter/recall regressions and relevant sanitizer coverage pass; before/after artifact hashes and ownership decisions are retained. Run auval/pluginval where permitted and bind their exact bytes; unavailable external cells remain open. G01 does not close just because AU offsets pass. A valid partial checkpoint records the precise failing case and smallest next implementation step rather than an AU scope waiver.

### Sprint 10 Native realtime ownership and notifications

Trace every shipping AU/VST3 callback entry through framework glue and plugin-controlled work. Cover buffer mapping, MIDI/SysEx/UMP, parameter listener/host callbacks, programs, state, lifecycle, bypass, UI attachments and first entry on a new audio thread. Distinguish legal host lifecycle contracts from invalid concurrent calls and define a safe silence/error path where needed.

Implement immutable prepared resources, nonwaiting audio acquisition, off-audio reconfiguration waits and acknowledged reclamation. Replace callback locks only after every participating writer follows that protocol. Defer audio-to-editor notifications through bounded generation-aware publication; avoid allocating message posts in audio, preserve truthful parameter readback/save and safe attachment teardown. Resolve the TLS notification path without a shared global flag that breaks thread-local nesting. Bound all ingress traversal before host-sized MIDI work occurs.

Keep format changes generated and pinned unless an explicit dependency-upgrade decision proves the old approach infeasible. Any upgrade needs a new dependency/license inventory and old negative controls. Run dedicated first-audio-thread and warm probes with editor open/closed and hostile traffic; separate instrumented and timing builds. ASan/UBSan and TSan require separate trees and generated wrapper coverage. Stress parameter/editor attach-detach, prepare/release, restore, rejected queues and resource reclamation.

Exit: reviewed ownership map, complete plugin-controlled callback instrumentation with zero prohibited operations and passing concurrency/lifecycle tests on available shipping formats. Record host/OS-owned work and finite instrumentation limits precisely. Both existing P1-03 and the new AU path must be addressed; external-platform proof continues in Sprint 13.

### Sprint 11 Musical behavior and session trust

Reproduce `performance-d2` at 88.2 kHz with the independent raw PCM oracle. Diagnose whether gain, clipping asymmetry, feedback, interpolation or transition timing causes the mean; those are investigation hypotheses, not established causes. Choose the smallest sound-preserving correction, or obtain named acceptance of intentional behavior. Consider DC filtering only after evaluating its effect on bytebeat character, transients, bounds and determinism.

Repeat the existing 44 cases at six rates, analyze raw stereo peak/RMS/DC/delta/rails before attenuation, compare partition invariance and within-build recall, and retain old waveforms. Check domain/reset/preset/brake/STATE/bypass/trim transitions, transform input asymmetry, feedback extremes, empty input and sample-rate rebuild. Keep factory mappings and IDs stable unless a specific defect requires an explicit compatibility decision.

Perform controlled listening at documented fixed level with the prepared audition set; distinguish the existing −24 dB audition attenuation from the normalized −18 dBFS input test requirement. Retain session-reopen/parameter-only undo tests, malformed/future state behavior and all-study repeated projects. Real DAW project cycles can finish in Sprint 15, but G05 remains partial until they do.

Exit: MB-04 disposition supported by measurements and named musical review; no unresolved unintended transition/DC/loudness defect; regression and recall results attached. An unperformed listening session is never reported as acceptance.

### Sprint 12 Audio and native UI performance

Measure a paced callback/actual audio device path on named hardware, then the DAW graph. Preserve the old unpaced outliers and explain the measurement boundary rather than subtracting them away. Separate clock overhead, OS scheduling, plugin processing and telemetry/UI work. Evaluate 256-event sorting/reset costs, editor transitions and eight-instance load only through measurements; optimize demonstrated bottlenecks.

Measure actual native timer and paint duration separately from snapshot creation; measure pointer/key response and ensure closed editors stop work. Use fixed-capacity audio-side sampling with off-audio aggregation. Verify focus/gestures/undo while throttling only visual work; do not reduce automation resolution to meet a UI target. Test all three domains and status combinations at four scales, including fully onscreen 200% windows and modal inspector recovery.

Exit: declared local callback and UI budgets pass with counts/distributions and controlled load; causes or remaining exceptions are explicit. Minimum-machine and all-cell soak proof remain separate until their fixtures execute. A fast average cannot close maximum-duration failures.

### Sprint 13 CI and supported platform contract

Create a versioned active matrix that resolves the REAPER conflict and records exact target viability. Verify JUCE/compiler deployment targets, native x86_64 and arm64 slices, Windows toolchain and plugin layout. Select universal macOS packaging or separately labelled architecture packages based on tested installation; verify actual slices and minimum load compatibility. Rosetta results remain separate.

Update the active monorepo workflow at `../.github/workflows/ci.yml` and the standalone export workflow together. The root workflow is outside the current writable root and requires appropriate workspace access or escalation before editing. Run core/native builds, strict AU/VST3 conformance, wrapper sanitizers/race checks, format checks/static analysis, isolated core and native source-export builds, Windows script tests and pinned external validators. Require strict AU parity rather than `au_offset_characterization` as the release signal. Add Windows Steinberg/pluginval evidence; headless GUI skips remain explicit.

Retain workflow run URLs/IDs, runner architecture/OS/toolchain, exact artifact inventory and JUnit/validator logs. Enforce required checks through the actual repository configuration when authorized. Keep local candidate-integrity success distinct from release qualification. Validate common failures: wrong source revision, missing untracked build input, dependency hash mismatch, stale report, failed validator, no native artifact and unsupported architecture.

Exit: reviewed source runs required remote jobs successfully; active cells and compatibility claims are explicit. A server CI build does not replace old desktop OS, device driver, DAW or musician evidence. Missing remote access produces a concrete resume package, not a repeated local packaging sprint.

### Sprint 14 Rights and production delivery tooling

Audit first-party file ownership and assets, compiled JUCE/transitive components, modified generated wrappers and notices. Establish the applicable corresponding-source contents, dependency source provision, build scripts and modifications for the chosen distribution. Verify an extracted packet builds without Git, sibling brand files, private paths or mutable external overrides. The existing notice hash checker is a component of this review, not its conclusion.

Extend development-only packaging with a separately defined release-candidate/release state contract. `package_candidate.py` currently copies Sprint 06 instructions, allows failed bound reports and labels development; `verify_candidate.py` rejects non-development status; Windows installation accepts only development packets and temporary fixtures. Implement a release verifier/assembler that requires complete gates, the active matrix, required report kinds, signatures and source identity. Keep the development path able to retain failures. Test missing/failed/stale evidence and corrupted/path-colliding archives as meaningful negative controls; do not just relabel the packet.

Finish the selected macOS PKG and Windows ZIP/PowerShell routes. Define production install targets, privilege needs, receipts, backups, removal and rollback separately from temporary fixture mode. Exercise fixtures before any personal/system path mutation. macOS requires uninstall/rollback tooling and owned-file policy in addition to PKG creation. Windows production scripts must work in PowerShell 5.1 with trusted timestamped signatures; synthetic scripts are not a released installer.

Use authorized existing identities only. Sequence build/metadata → sign bundles → verify/extract identical shipping payload → final-byte validators → create/sign package → notarize/staple → verify package → hash/download packet. Capture notarization acceptance, Team ID/publisher, timestamp and public credential selectors. Signed artifacts are not expected to be byte-identical across independently timed builds; source reproducibility and exact distributed artifact identity are separate claims.

Exit: rights-owner accepted source/notices, production tooling with meaningful failure tests, signed staged packages and immutable evidence. If credentials are unavailable, finish and validate the plans/tools and list exact missing inputs; G09 remains open.

### Sprint 15 Actual DAW and accessibility qualification

On every active OS/CPU/host/format cell, load exact signed candidate bytes and complete scan, disabled-input generation, asymmetric stereo transform, timed ordinary automation and CC20–23, stable ties/dense events, internal/host bypass, offline bounce, all-program save/reopen and repeated lifecycle. Bind project and raw PCM hashes, sample rate/buffer, driver, host full build and plugin identity. Do not substitute UI host edits for recorded automation/position evidence. Where a DAW coalesces automation, characterize the host delivery separately from the plugin fixture contract.

Recover unreliable UI automation with a permitted human operator; save reproducible projects and observation steps. Record timeout/host/plugin distinctions. Avoid repeated blind export attempts that lose state. Complete keyboard/pointer tasks, 100/125/150/200% reachability, inspector close/focus restoration, native screen-reader names/roles/values/actions and redundant status information. Use built-in VoiceOver and the approved Windows reader available on the fixture; ask for missing external setup only when dependent execution needs it.

Perform three clean-system install/scan/save/reopen/upgrade/uninstall/rollback cycles per supported package, then cell-level scan/load/recall checks. Use signed internal predecessor fixtures with preserved schema/IDs; retain receipts, installed file hashes and project checks. Do not delete unknown user files or claim a historical 0.x signed upgrade path.

Exit: complete active matrix operations, G04 accessibility and G05 real session evidence; zero unexplained scan/restore/routing failures. Any fix changes candidate identity and invalidates affected checks before Sprint 16.

### Sprint 16 Human beta and realtime soak

Freeze a candidate after routing, sound, UI and packaging regressions. Run five actual first-time musician sessions with independent task results, elapsed time, interventions/confusion and parameter-only recall understanding. Obtain the accepted first-minute/transform thresholds; fix reproducible failures and rerun the affected tasks without pretending a coached participant is first-time.

Run eight simultaneous stereo instances at 48 kHz/128 on every active cell for eight wall-clock hours, with declared domain transitions, dense automation/MIDI, restores, editor closed/all-open periods, audio device/driver and controlled external load. Collect DAW cycle and instance measurements off audio; report observed underruns and full duration. Run additional single-instance 64-frame stress. An unattended job can survive session handoff through PID/job/run IDs and a resume command; do not restart it simply because a session ends.

Exit: five-participant acceptance, every required soak completed without observed deadline misses/underruns, retained distributions and artifact identity. Any functional fix triggers the relevant regression, signing and affected cell/soak reruns.

### Sprint 17 Final decision and delivery

Reconcile manifest, active matrix, issue ledger, rights decisions, source revision, signatures, reports and final packet. Verify no open P1, unresolved investigation treated as waived, missing manual gate or untested supported claim. Finish current changelog, first-minute/install/uninstall/rollback/troubleshooting documentation and downloadable corresponding source; do not leave Sprint 06 instructions in the public package.

Run the production verifier from a freshly extracted package. Present a concrete GO/NO-GO record naming package/source hashes, supported cells, known accepted limitations and rollback process. Request explicit owner authorization for publishing/tagging only when the exact packet is complete and reviewable. Until then retain NO-GO. Website/download changes outside SYS 01 require their own authorized destination and workspace access.

After approval, publish the approved immutable artifacts and source/tag matching the reviewed revision. Download through the actual release endpoint, verify hashes and signatures, exercise a clean smoke install/scan/reopen, and retain publication links and the removal/rollback procedure. No telemetry service, account requirement or paywall is added.

## Evidence invalidation and issue management

Use stable issue IDs, adding new ones when evidence reveals a new defect. The initial issue set is S07-P1-01/02/03, S07-INV-01/02 and S07-TOOL-01. Assign one active technical objective per session. Track code-fixed, locally-verified, externally-qualified and gate-complete as distinct states; fixing one format does not close another.

| Change | Evidence that must be renewed |
|---|---|
| AU wrapper/ingress | Strict AU offsets/callback, concurrency/sanitizers, auval, all affected AU host and performance evidence. |
| Shared processor/DSP/state | Core/musical/recall and both formats, affected sound/session/performance/soak evidence. |
| Listener/lifecycle/telemetry | Whole callback probes/races, editor/host lifecycle, UI/audio budgets and affected soak. |
| Editor/input | Native state/scale/keyboard/reader, response/audio contention, musician tasks affected by interaction. |
| Dependency/compiler/platform | Pin/notices/source build plus native conformance/validators and all affected target results. |
| Signing/stapling/package metadata | New complete artifact identity and signature/package verification; rerun validators on shipping bundle bytes and verify extracted payload. |
| Documentation only | Source/package manifests and instruction review; binary results retain their named bytes if build inputs and binaries are unchanged. |

Preserve original failures and historical seals. Never rewrite old `SHA256SUMS`, run a historical `--seal`, or edit old decision JSON to make a new candidate pass. The historical Sprint 08 verifier compares live files and will intentionally stop matching after source repairs; capture its initial successful baseline, then verify historical packets against their frozen inventories/captured inputs. Add new evidence for new source. Integrity, test success and release approval are three separate results.

## Rolling handoff protocol

The next session must not depend on prior chat memory. Read `rolling-state.md`, the current sprint brief and only the relevant source/evidence. Consult the historical ledger by section/search as needed. Keep the rolling file around 1,200 words maximum; link full logs and old ledgers. The immutable detailed audit is a reference, not an ever-growing session diary.

At session start, record current revision/status, compare active source to the previous input hashes and resolve user changes before edits. Confirm tools/dependencies and pending background jobs. Do not discard, stash or bulk-stage dirty work to create a clean appearance. When isolation is needed, use a complete source snapshot including untracked inputs; a HEAD-only worktree would lose the implemented sprint foundations.

At each coherent checkpoint, update the compact state and create `handoffs/sprint-NN-session-MM.md` from [handoff-template.md](handoff-template.md). Record objective, changed paths, actual commands/results, artifact hashes, decisions, failures, external prerequisites and exact next action. Evidence directories are unique per session. Use log files and command registries, not long pasted test output. Hash the files the next task actually depends on; avoid a circular checksum of a file that embeds its own checksum.

Checkpoint after a coherent change/test boundary, before a large new investigation, before leaving a long job, or after roughly 30–45 minutes of substantial work. This is a context-management cadence, not a fixed execution timeout. If unfinished, the next prompt resumes the same sprint and failing test; if exit criteria are met, it selects the next dependency-ready sprint. Do not spend sessions endlessly verifying every old ledger or making new review ZIPs while product blockers remain unchanged.

Every final sprint response provides the achieved outcome, relevant test results, remaining blocker and complete ready-to-paste next-session prompt. Missing machines/participants/signers do not stop independent local implementation. Required decisions remain pending until explicitly answered; elapsed time is not approval. No sub-agents are implied by this handoff process.

## Definition of version 1.0.0 complete

Version 1.0.0 is complete when the accepted AU/VST3 behavior and complete callback gates pass, all active supported cells meet sound/session/UI/accessibility/performance requirements, five musicians pass, signed packages survive lifecycle tests, rights and corresponding source are accepted, and the owner approves the exact release packet. The release is then published with matching source/tag, verified from its real download endpoint and accompanied by working rollback and support instructions. Anything earlier remains a development candidate.
