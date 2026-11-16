# Sprint 01 / native realtime ownership

2026-10-14 (America/Santiago). **PARTIAL. G01/G07 and release remain BLOCKED.**
Revision `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus the dirty SYS 01 inputs
and patch indexed in `evidence/sprint-01/inventory.json`. No commit, release tag,
publication or personal plugin installation was made. No other sprint was executed.

## Prerequisites and ownership

Read applicable instruction paths (no AGENTS.md), Git status, implementation-status,
ADR 0002/0003, manifest, release state and Sprint 00 evidence. The initial Sprint 00
verifier passed: 36 evidence files, 29 command logs and 166 inventory artifacts.
After edits, its old working-tree check correctly fails on changed source. The new
`verify-baseline.py` verifies those 100 source inputs through their recorded Git
revision and the remaining artifacts directly; it resolves the mutable state file
to the preserved `baseline-state.md`. Historical Sprint 00 files were not rehashed
to pretend the new source was its baseline. Pre-existing website, brand, TAPE and
release-plan work is preserved.

[Native ownership](native-ownership.md) was written before synchronization changes
and records actual VST3 process/processAudio and AU SDK/Render/processBlock entries,
parameter listeners, MIDI, restore epoch/eight-slot boundary and editor ownership.
**No lock was removed.** A safe replacement must cover lifecycle resource publication
and reclamation, host violations, parameter notification generations, restore/save
readback, editor destruction and AU ingress producers, not just DSP ownership.

## Implemented boundary

- Fixed VST3 audio-owned MIDI ingress accepts the declared CC20–23 only; no generic
  MidiMessage/SysEx copy or dynamic MidiBuffer growth. IEventList traversal caps at
  256 entries. Parameter traversal caps at 256 total points across ordinary and
  synthetic queues, rather than 256 points for each of up to 256 queues.
- Pinned build-only AU integration preallocates both alternating MIDI buffers to
  4096 bytes. Legacy ingress caps at 256 short messages between audio boundaries.
  UMP has 256-packet/1024-word bounds, at most 256 messages, complete-message and
  integer-offset checks. Only channel-voice packets reach the converter; SysEx is
  discarded before stateful dispatch/copy. Overflow rejects the entire musical
  batch, with bounded recovery. AU automation remains block-boundary.
- Merged native/MIDI/UI/scalar events retain stable ordering, 256-event capacity,
  whole-batch rejection and no rejected-final-target replay. Malformed MIDI data
  bytes and invalid scalar events now reject before consuming queued UI edges;
  the processor's rejection result is counted and exposed. Eight restore slots,
  captured queue boundary, single audio owner and zero-block rules are retained.
- Immutable eight-slot block telemetry includes actual per-channel input/output
  peaks, frames, rate, DSP flags/word, rejections, sequence and display drops.
  The editor reads one coherent bounded snapshot at 30 Hz. Full mailbox drops new
  snapshots; it never overwrites unread memory or waits. See the explicit staleness,
  ownership and no-RMS contract for [Sprint 04](telemetry-contract.md). The existing
  aggregate meter presentation is retained; no new UX is implemented.
- Native fixtures load actual VST3/AU binaries. AU uses process-local component
  registration, leaving personal plugin folders untouched. Tests cover dense
  automation/MIDI, stable ties, malformed/overflow/nonfinite values and recovery,
  zero/variable blocks, repeated prepare/release/rates, native concurrent state
  restore, editor attachment/automation and adapter suspend/lifecycle exclusion.
  Adapter tests exercise full restore queues, UI edge retention and asymmetric
  stereo telemetry. VST3 variable blocks cover 0/1/7/63/64/65/511/1024; AU and adapter
  include 4096. Native VST3 4096-frame matrix coverage remains to be added.

IDs, seven-parameter catalogue, schema 1 / 40 bytes, mappings, factory program order,
parameter-only empty-tape recall, free distribution and deferred scope remain intact.
Upstream JUCE checkout remains clean; both generated integrations retain its notices
and fail configuration on source hash/anchor drift.

## Whole-wrapper evidence / hazards are not waived

The macOS test-only dyld probe observes malloc/calloc/realloc/free,
pthread_mutex_lock and os_unfair_lock_lock on one selected thread between actual
native entry/return. Negative controls must detect allocation/free and both locks.
The shipping plugins and release timing fixture do not link the probe (otool log).
It can miss direct malloc-zone/aligned allocation, alternate lock APIs and I/O;
framework/system/host implementations are not exhaustively proved by these hooks.
Stack collection is bounded to 24 frames, diagnostic only, with its own unwinder
work excluded from counts. Probe timings are never used as release profiling.

| Workload / exact report | Observation |
|---|---|
| retained Sprint 00 AU, same overflow/UMP/64 KiB SysEx fixture, `baseline-au-negative-control.log` | first ingress/render group 6 allocation + 6 free calls; later groups 1 + 1; confirms the old allocation path |
| new AU, `ctest-probe-stack.log` | same group 0 allocation/free; 602 mutex calls including ingress; 256-message groups 258 mutex calls; Render retains ingress + lifecycle locks |
| native VST3 ordinary first conformance block | 0 allocation/free, 9 mutex calls; unchanged-target blocks typically 1 mutex |
| dedicated audio thread with editor open, 256 changing-target blocks, `ctest-probe-stack.log` | first block 1 calloc, stack: dyld ThreadLocalVariables::instantiateVariable / tlv_get_addr -> JuceVST3Component::processParameterChanges; source has `thread_local inParameterChangedCallback` at upstream line 829. Subsequent changing-target blocks typically 4 mutex calls, 0 allocation/free in this run |
| controlled PluginProcessor leaf, asymmetric dry render | zero observed allocation/free/mutex/unfair calls; proves only that leaf/workload |

The callback lock protects lifecycle/suspend access. Parameter notification enters
AudioProcessorParameter listenerLock, APVTS LockedListeners and the editor's
ParameterAttachment. On a real audio thread this calls AsyncUpdater -> macOS
MessageQueue::post -> ReferenceCountedArray::add and CFRunLoop wakeup. These have
locks and potential allocation; warmed observed zeros do not prove them safe under
other queue/editor load. Host output-parameter interfaces, AU SDK/input pulls and
FL Studio's conditional lifecycle mutex remain explicit additional paths.
Synthetic VST3 program changes still call non-audio restore/gestures/stateMutex
on the caller; audio-thread program/state misuse is unqualified, not made safe by
the musical ingress bounds. No global/TLS flag was replaced with a racy shared flag.

Early editor probe reports processed on the message thread and therefore exercised
synchronous UI updates/allocations. They are retained but **not attributed to normal
DAW audio threads**. The corrected dedicated-thread fixture and stack report above
are authoritative. AU SDK caches render timestamps; the variable-block fixture gives
its zero-block probe a distinct timestamp so the following call actually enters
Render. Same-timestamp zero/nonzero DAW behavior remains unqualified.

## Checks and exact replay

All argv, cwd, UTC starts, exit status and log hashes are in
[evidence/sprint-01/commands.json](evidence/sprint-01/commands.json). Final native
and validator runs bind executable SHA-256 before and after execution. Full expanded
CMake configure/build commands are there; do not infer flags from a binary version.
The build recipe uses the same pinned JUCE/doctest sources, Apple Clang 21, SDK 26.5,
macOS 26.6.2/25G83 arm64 on M1 Pro / 16 GB (not an agreed minimum fixture).

| Final check | Result / evidence |
|---|---|
| Release CTest | 14/14; `ctest-final-release.log`; corrected VST3 audio-owner follow-up 1/1 in `ctest-release-audio-owner.log` |
| diagnostic CTest | 14/14; final `ctest-probe-stack.log`, including negative controls, actual wrapper counts and first-allocation stack |
| ASan + UBSan Debug | 4/4 targeted native/host; `ctest-final-asan.log`; corrected audio-owner follow-up 1/1; generated VST3/AU and AU SDK TU instrumented |
| TSan Debug | 3/3 targeted native; halt_on_error=1, no suppressions; `ctest-final-tsan.log`; corrected audio-owner follow-up 1/1 |
| pluginval 1.0.4 | level 10 / seed 1011 / editors enabled, SUCCESS on exact Release VST3; `pluginval-final.log` + copied native report; Steinberg validator explicitly skipped, no auval pass claimed |
| format / source / catalogue | C++ formatting, native theme, catalogue, source export reproducibility, WAV, core symbols and DSP/host tests passed; source export excludes .log files, so keep this evidence sidecar |

Representative exact commands (full tool prefixes/arguments in commands.json):

```sh
/Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin/ctest --test-dir /private/tmp/sys01-sprint01-release --output-on-failure -V
/Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin/ctest --test-dir /private/tmp/sys01-sprint01-probe --output-on-failure -V
/usr/bin/env TSAN_OPTIONS=halt_on_error=1 /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin/ctest --test-dir /private/tmp/sys01-sprint01-tsan --output-on-failure -V -R 'desktop_adapter|vst3_positioned_automation|au_native_callback'
/private/tmp/sys01-sprint01-release/apps/desktop/sys01_vst3_contract '/private/tmp/sys01-sprint01-release/apps/desktop/sys01_artefacts/Release/VST3/SYS 01.vst3' --profile
```

Failed attempts remain in the evidence: build before configure completed; AU fixture
SDK enum type errors and generated include order, repaired; probe startup aborts
(in sandbox and GUI retry, cause not independently diagnosed), then failed negative
controls until the observer avoided its own TLS and the control moved out of the
interposition image. Two early command records were reconstructed from exact session
argv and retained logs after overlapping recorder writes; their UTC start is null,
not invented. The recorder now rereads before append. The old prerequisite verifier
fails after intentional edits; revision-based verification passes. C++-only format
configuration cannot parse ObjC; explicit LLVM/4-space/left-pointer/96-column formatting
was applied and checked for the test-only NativeEditorHost.mm. No failure is hidden
by renaming a later pass or transferring historical evidence to new bytes.

## Local timing diagnostic / qualification still missing

Single instance, native loaded VST3, fake host, 48 kHz, 224 ordinary automation points
plus 32 positioned CCs per block, changing final FLUX targets, all-domain/drive/bypass
transitions; no audio driver, DAW, realtime thread priority or paced scheduling.
One real native editor is attached/open, then removed/closed; its message loop runs
on the message thread, callbacks on a dedicated worker. Each cell has 20,000 calls.
Timer-pair overhead in final-local-profile.log: p50 0, p99 0.042 us, max 0.083 us;
reported callback durations are unadjusted. This is a finite offline diagnostic,
not eight instances, realtime soak or minimum-machine evidence.

| Final diagnostic | p50 us | p95 us | p99 us | p99.9 us | max us | deadline misses | half-deadline budget breaches |
|---|---:|---:|---:|---:|---:|---:|---:|
| editor open / 128 | 32.959 | 35.667 | 44.709 | 71.834 | 132.125 | 0 | 0 |
| editor open / 64 | 23.333 | 25.209 | 30.083 | 45.792 | 126.792 | 0 | 0 |
| editor closed / 128 | 32.917 | 35.834 | 44.750 | 73.708 | 977.583 | 0 | 0 |
| editor closed / 64 | 23.583 | 25.709 | 33.959 | 116.500 | 988.167 | 0 | 1 |

The first local run (`local-profile.log`) observed open/128 max **10,352.291 us**,
1 deadline miss; closed/128 max 1,296.792 us, 0 misses. A subsequent run
(`local-profile-64.log`) observed no misses/breaches. All reports are retained.
The first run overlapped other validation activity; external load/power/priority
were not controlled. Neither editor causality nor a lock-wait cause was established.
Later lower maxima do not erase the outlier or establish a hard realtime bound.
Minimum fixtures in state.md were unavailable; their editor-open/closed profiling,
eight-instance host cycle and eight-hour driver-backed soak are **UNPERFORMED**.

## Changed files, hashes and resume

Tracked changes: PluginProcessor.hpp/.cpp, PluginEditor.cpp, AdapterTests.cpp,
VST3ContractTests.cpp and CMakeLists.txt under apps/desktop; host processor.hpp/.cpp;
native_vst3_adapter.py; implementation-status.md; ADR 0003; release/manifest.json.
Added: AUContractTests.cpp, NativeEditorHost.hpp/.mm, RealtimeProbe.hpp/.cpp,
native_au_adapter.py, this sprint document, native-ownership.md, telemetry-contract.md
and evidence/sprint-01/. state.md is updated. The source patch, exact changed-file
inventory, source hashes, generated units, CMake cache/compile flags and executable
hashes are in inventory.json; the complete packet is indexed by SHA256SUMS.

| Release executable | SHA-256 |
|---|---|
| VST3 | `8b0edb1f2a05e398a1822051e1470e80e11bf972456e8802e8ab50c9d73b1e63` |
| AU | `41765d2fb6b5146129f9c83abf8384743f2dbbf2402d70b8d6970178a9791724` |

They remain temporary development builds under `/private/tmp/sys01-sprint01-release/`.
No new distribution archive was made; package hash is not applicable. Ad-hoc bundle
signatures remain development-only; no signing/notarization or clean-install gate
was promoted. Probe and sanitizer artifact hashes are separate in inventory.json.

Resume **Sprint 01**, not an automatic downstream sprint: verify SHA256SUMS and
inventory against available binaries; replace the TLS/parameter-notification paths
with a proved lifecycle/restore/editor publication design; retain compatibility and
rerun affected native fixtures/sanitizers. Capture lock wait causes under declared
load. Add VST3 4096 and same-timestamp AU zero/nonzero fixtures. Obtain agreed minimum
machines and actual DAWs/drivers for editor-open/closed 64/128 and eight-instance
profiling. AU offsets/DAW matrix/auval/Steinberg/Windows, accessibility, listening,
eight-hour soak, rights/source packaging, signing and clean installs remain manual
or downstream omissions, not passed checks. Do not tag, publish or mark G01/G07 complete.


Final ledger checks (each exit 0): `test_source_package.py` produced two matching
128-file temporary exports; revision-based baseline recheck passed; `git diff --check`
passed; final status shows only the enumerated SYS 01 tracked changes plus preserved
initial untracked work and this evidence. Final packet commands:

```sh
/usr/local/bin/python3 sys-01/docs/release-1.0.0/evidence/sprint-01/capture.py
/usr/local/bin/python3 sys-01/docs/release-1.0.0/evidence/sprint-01/verify.py --write-manifest
/usr/local/bin/python3 sys-01/docs/release-1.0.0/evidence/sprint-01/verify.py
```

The packet verifier checks every command-log hash, current source/artifact hashes,
unchanged native-run before/after bindings, exact latest fixture/validator images,
local document links and the complete Sprint 01 file index. Temporary binaries may
later disappear; absence invalidates a fresh recheck, not a historical result.
