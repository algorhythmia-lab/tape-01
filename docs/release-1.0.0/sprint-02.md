# Sprint 02 / formats and platform conformance

2026-10-14, America/Santiago. **PARTIAL. Release remains BLOCKED.** G02 is
PARTIAL for VST3 and BLOCKED for AU parameter timing; G03 remains PARTIAL.
Only this sprint was executed. No publication, release tag or accepted scope
change. Revision `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus preserved dirty
Sprint 01 sources and the changes listed below. No applicable AGENTS.md exists.

## Prerequisites and preserved foundations

Read implementation-status, ADR 0002/0003, manifest and release state before
implementation. All 130 immutable Sprint 00/01 evidence files verify; the
revision-based prerequisite verifier also passes (166 inputs/artifacts, 29 logs,
36 entries). The first whole-packet check encountered the intentionally mutable
`state.md`; that failed attempt is retained. Historical ledgers are not silently
rewritten to make their old working-tree hash manifests pass. JUCE checkout is
clean at `29396c22c93392d6738e021b83196283d6e4d850`, doctest at
`1da23a3e8119ec5cce4f9388e91b065e20bf06f5`. Existing ownership/realtime blockers
remain open; their replacement belongs to Sprint 01.

Processor/DSP, catalogue, codec and checksum-pinned adaptations were unchanged.
The desktop build now ad-hoc seals both final macOS plugin bundles after metadata
generation; this repairs real host scanning without changing DSP or release identity. `org.algorhythmia.sys01`, `Algr`, `Sys1`, all seven
parameter IDs, schema 1, factory study order, free full distribution, planned
macOS VST3/AU and Windows VST3 and deferred features remain intact.

## Implemented and exercised boundary

VST3 loads through its actual factory/IAudioProcessor. The fixture now asserts
literal component/controller IDs and all seven parameter IDs. It independently
schedules **201,894 ordinary parameter points**: every integer offset in
1/7/63/64/65/511/4096-frame blocks, all seven catalogue parameters, six rates
44.1/48/88.2/96/176.4/192 kHz. Each batch is at most 256 and is followed by a
point-free block to check no replay. The sweep keeps input asymmetric and
resets audible DRIVE/domain/bypass targets between parameters. Added exact
half-step thresholds and adjacent values for discrete controls, same-queue ties,
all CC20–23, invalid intermediate NaN/±Inf/out-of-range points with valid final
values, invalid endpoints -1/frames and merged automation/MIDI overflow.
Existing every-point, final-target, 256 capacity, cross-queue bypass overflow,
nonfinite final, automation/MIDI tie and lifecycle checks remain. Comparisons
check every rendered sample in both channels against separate shared DSP.

AU's independent native fixture verifies all seven IDs and compares real
AudioUnitSetParameter offsets and AudioUnitScheduleParameters immediate events
with both positioned and final-at-zero references. Both parameter APIs collapse
three DRIVE points (9/61/91) to the final value at offset zero. MIDI CC22 preserves
positioning; following blocks do not replay. Strict `--require-positioned` exits
1 on arm64 and x86_64/Rosetta. Default CTest passes **characterization only**.
[The explicit AU scope decision](au-timing-scope-decision.md) is PROPOSED and
awaits acceptance; the accepted timing contract is unchanged. Enabling upstream
SDK vector scheduling alone would not establish the bounded/realtime contract.

The declared macOS 11 deployment target exposed an existing test-only unguarded
MusicDeviceMIDIEventList call. AUContractTests now checks macOS 12 availability
and reports a skip on older OSs while retaining legacy MIDI tests. No wrapper
behavior was changed for this fix. Both thin binaries have `minos 11.0`; this
load-command check does not demonstrate execution on macOS 11.7.10.

## Results and exact binary bindings

Machine: MacBookPro18,1, M1 Pro, 16 GB, macOS 26.6.2 build 25G83. Apple Clang 21
`clang-2100.1.1.101`, Command Line Tools SDK 26.5, CMake 4.4.4. arm64 runs are
native. **x86_64 runs use Rosetta on this same machine**, not native Intel or a
minimum fixture. Release, C++20, no fast math/FP contraction; flags, generated
units and full file inventories are retained in `evidence/sprint-02/`.

| Final check | Actual result / evidence |
|---|---|
| arm64 and x86_64 builds | both exit 0 after final sealing change |
| arm64 full CTest | 15/15 passed, `ctest-sealed-arm64.log` |
| x86_64 full CTest | Rosetta 15/15 passed, `ctest-sealed-x86_64.log` |
| final bundle signatures | all four thin VST3/AU bundles pass codesign --verify --deep --strict |
| stock VST3 negative control | new OFF build; final fixture exit 1 for every-point/offset mismatch; also executed after retained-archive extraction |
| strict AU timing fixture, both final architectures | exit 1 each; parameter FAIL, MIDI PASS; `au-offset-strict-final-*.log` |
| official Steinberg validator | final arm64 and x86_64/Rosetta VST3: 47 passed / 0 failed, exit 0 each |
| pluginval 1.0.4 | final arm64 VST3 level 10 / seed 1012 / GUI enabled: SUCCESS, exit 0 |
| Apple auval 1.10.0 strict | final sealed arm64 AU passed, exit 0; earlier arm64/universal runs separately retained |
| x86_64 auval | attempted thin, retry and universal under Rosetta; exit 2: component not found; no pass claimed |
| formatting / diff / source export | final commands and results retained in commands.json |

Initial as-built seals failed for both formats: VST3 added moduleinfo.json after
JUCE's first seal, and AU had no final resource seal. Live's actual scanner
rejected the original VST3 with `a sealed resource is missing or invalid`.
A resealed test copy then scanned and loaded with stable class ID
`abcdef01-9182-faeb-416c-677253797331`. CMake now runs final ad-hoc sealing after
metadata generation for VST3/AU on macOS. Fresh builds, native tests, strict
signature checks and validators were rerun on their new executable hashes.
This repairs development-bundle integrity; distribution credentials/notarization
and clean-machine installation remain Sprint 06/07 gates.

Steinberg SDK validator source was obtained from the
[official repository](https://github.com/steinbergmedia/vst3sdk), commit
`3cdf9ca5d1f5b1b21e0a86832aa4abe55607bd96` (3.8.1); every submodule revision is
recorded in `dependency-identities.log`. This is a validator dependency only;
the plugin still uses pinned JUCE's SDK. Initial configuration required full
Xcode detection. The successful command uses the SDK's documented environment
hook plus `-DXCODE_VERSION=26.5` to skip that detection while compiling with the
actual **CLT** compiler; it does not claim a full Xcode installation. Initial
network denial, configuration/build errors, masked AU ID fixture correction,
sandbox AudioComponentRegister failures and scanner-registration failures are
retained, followed by their successful corrected runs. No validator was skipped
and relabeled a pass. pluginval's own optional Steinberg step was skipped; the
separate official validator runs above are the evidence.

| Final executable | SHA-256 |
|---|---|
| arm64 VST3 / actual Ableton VST3 target | `22b9eb22c4e636407eafdceba3137bfc9eb687bd9888d35616e4ff19aaa88f0e` |
| arm64 AU | `ebbf47693980a81851a33f3d7d82ab179c0024c779aa6c365a5ef4eee9d5e1d5` |
| x86_64 VST3 | `1854cbd567b3ba1dffe2d0e33dd54b5325f5763c4f752c266555ab154e2a21b6` |
| x86_64 AU | `49f6246b4b7cbb54d973227d1b6f7460ce07c6421dc86b6f2cc8dd110b981b2c` |
| stock negative VST3 | `1997a5317b0d0fd703afdc1ab5d6b9af5df93082f566dad0270012326f147241` |
| earlier local universal AU / Ableton AU transform target | `e6c921231f211c18ee8ffba529f209c1f7aae6be7a0b945d41d9927dfd28e254` |

Earlier pre-seal hashes and inventories are retained in
`historical-inventory-before-final-seal.json`; earlier native reports bind those
binaries, not the final candidates. The original AU generation session used
`65ed165d010cd268a01d5acf52c26949d917b64fe17242d96da9983d9b63fa2c`;
subsequent AU reopen/transform/bypass used the universal hash above. No earlier
AU DAW pass is transferred to the final thin AU hash. The final VST3 hash exactly
matches the resealed copy exercised in Ableton. Full bundle inventories, fixture
and validator executable hashes appear in inventory.json.

The local universal AU contains both already tested thin slices; lipo and ad-hoc
sealing commands are retained. It was constructed only to investigate scanner
architecture registration. No distribution package/hash, Developer ID signing,
notarization or clean-install qualification applies. Inventory binds validator,
fixture and installed Live executables by full hash. Temporary build artifacts
are under `/private/tmp/sys01-sprint02-{arm64,x86_64,negative,validator,validator-x86_64,universal}`;
reverify or rebuild when absent. Reports are permanently retained in this ledger.

All actual argv/cwd/UTC start/end/exit codes/log hashes are in
[commands.json](evidence/sprint-02/commands.json). Representative exact runs
(the bundle paths contain spaces):

```sh
ctest --test-dir /private/tmp/sys01-sprint02-arm64 --output-on-failure -V
ctest --test-dir /private/tmp/sys01-sprint02-x86_64 --output-on-failure -V
/private/tmp/sys01-sprint02-arm64/apps/desktop/sys01_vst3_contract '/private/tmp/sys01-sprint02-negative/apps/desktop/sys01_artefacts/Release/VST3/SYS 01.vst3'
/private/tmp/sys01-sprint02-arm64/apps/desktop/sys01_au_offsets '/private/tmp/sys01-sprint02-arm64/apps/desktop/sys01_artefacts/Release/AU/SYS 01.component' --require-positioned
/private/tmp/sys01-sprint02-validator/bin/Release/validator '/private/tmp/sys01-sprint02-arm64/apps/desktop/sys01_artefacts/Release/VST3/SYS 01.vst3'
/usr/bin/arch -x86_64 /private/tmp/sys01-sprint02-validator-x86_64/bin/Release/validator '/private/tmp/sys01-sprint02-x86_64/apps/desktop/sys01_artefacts/Release/VST3/SYS 01.vst3'
/usr/bin/auval -v aumf Sys1 Algr -strict
/usr/bin/arch -x86_64 /usr/bin/auval -v aumf Sys1 Algr -strict
/private/tmp/sys01-sprint00-pluginval/pluginval.app/Contents/MacOS/pluginval --strictness-level 10 --random-seed 1012 --output-dir /private/tmp/sys01-sprint02-pluginval-reports --validate '/private/tmp/sys01-sprint02-arm64/apps/desktop/sys01_artefacts/Release/VST3/SYS 01.vst3'
```

The ledger contains the absolute CMake/CTest paths actually used; the abbreviated
CTest commands above assume those binaries are on PATH. AU tests require access
to native audio services; successful fixtures ran outside the sandbox. auval
loads the temporary copied component via normal scanning, not the fixture's
process-local registration. Exact owned staging paths/actions are recorded.

## Named DAW evidence and unperformed checks

Live **12.4.6 [2026-09-10_0de5c8fa9a]**, native arm64, actual installed executable
hash retained; installer hash unavailable. CoreAudio / physical input **No Device**,
output **Use System: MacBook Pro Speakers (0 In, 2 Out)**, 44.1 kHz / 512 samples,
one SYS 01 AU on audio track 3; default empty project, returns had no sends.
Rescan found the AU, loading succeeded and exposed all seven host controls.
DRIVE On produced -3.58 dB peak at track/main with no clips and input disabled.
OUTPUT was reduced to normalized .375 (-30 dB) before saving/bouncing. This
observed metering and the bounce establish generation, not musician listening
or sample-accurate DAW automation.

Saved `SYS01-AU-generation.als`. Exported Main, start 1.1.1, length 2.0.0 bars at
120 BPM, stereo WAV, 44.1 kHz, 16-bit, No Dither, Normalize Off, Convert to Mono Off.
Actual file has 176,400 frames / 4 seconds, both channel peaks .023681640625 and
352,611 nonzero scalar samples. Saved project and WAV are retained here:

- [Live AU project](evidence/sprint-02/SYS01-AU-generation.als), SHA-256
  `0a454ab757584d54146accebdf13c0ae3c9a944969a32a2359de355b2d32144d`.
- [Stereo AU bounce](evidence/sprint-02/AU-generation.wav), SHA-256
  `d11cc70074746fdb548a939dba019df00d2e8d2f89c5e76d206cbf1bef7f3701`.

CUA intermittently returned noWindowsAvailable/timeouts around modal operations.
After recovery, the AU project reopened without a missing plugin and restored
BIAS .5, FLUX .5, DRIVE On, STATE Off, DOMAIN bytebeat, OUTPUT .375 and BYPASS Off.
The saved project's embedded AU descriptor has stable aumf/Sys1/Algr codes.
The subsequent AU session used the separately inventoried universal development
bundle, not the original thin AU. Source DSP code was unchanged, but evidence
remains bound to its exact executable hash.

Loaded the generated asymmetric stereo fixture (left 440 Hz / .125 peak, right
660 Hz / .0625 peak, 44.1 kHz / four seconds) into Session, then Arrangement at
beats 0–8; disabled warping. AU live transform used FLUX .75, BIAS .2, DRIVE On,
DOMAIN transform, OUTPUT .375. Wet render peaks were .00457763671875 / .002288818359375.
Internal BYPASS and host Device Activator renders are PCM-identical to each
other and exactly match the original stereo input away from 1,024 edge frames;
whole-clip differences are Live's fades. Dry bypass is intentionally untrimmed
(only wet processing uses trim). Wet processing recovered; recovered PCM equals
the first wet render. This is routing/recovery evidence, not a listening review.

Ableton's native CC Control device was configured for CC20=96, CC21=32, CC22=127,
CC23=64, routed to audio track 3 / 1-SYS 01, Send invoked with monitoring Auto
then In. Both static-send renders were PCM-identical to the baseline transform.
Those tests do **not establish actual MIDI delivery**: a manual send can be lost
when offline rendering reinitializes parameter-only DSP. A recorded timed CC
sequence remains unqualified; no plugin MIDI failure or pass is inferred. MIDI
file import attempts did not create an Arrangement MIDI clip; the Groove Pool's
Swing 16ths 66 template is unrelated. The configured source and incomplete
recording-session copy are retained to resume without installing a new DAW.

The resealed VST3 scanned in Live's normal startup scanner and loaded with the
explicit VST3 browser filter. Generation with physical input No Device produced
a four-second stereo render. The same asymmetric source then exercised transform
at FLUX .7519684, BIAS .2007875, DRIVE On, DOMAIN transform, OUTPUT .3720474.
Host bypass matched both dry channels exactly away from fades. Recovered and
**reopened** project renders are sample-identical to the transform render.
Visible parameter values restored on reopen. See `live-vst3-bounce-analysis.log`.
The host reports controller-state `not implemented`; component-state recall and
these observed reopen results passed, so that log line is not hidden or claimed
to be a controller-state implementation.

[UI observations](evidence/sprint-02/ui-observations.json) distinguish recorded
operations, hashes and limits. All projects, inputs and WAVs are retained in the
evidence packet with inventory hashes. A prepared `SYS01-VST3-midi.als` copy
was saved, but repeated native UI timeouts prevented the recorded-CC sequence.
A read-only sample showed Live's main event loop; no crash/deadlock diagnosis
is asserted. Failed scanner CLI path-syntax attempts scanned **zero candidates**
despite exit 0 and are not scan passes; the real startup scanner log is retained.

**Still unperformed:** recorded/routed CC delivery proof and ordinary DAW
sample-offset automation on each format; explicit VST3 internal bypass UI check
(the host panel hides it; native loaded fixture covers its positioned points);
repeat Ableton qualification on the final thin AU hash; all external matrix
cells. No assistive technology, sound review, stress budget or soak was added.

REAPER testing was **SKIPPED at the owner's explicit instruction** on
2026-10-14. Before that steering, the official 7.82 image was downloaded and
its EULA initially canceled; the owner then explicitly approved acceptance.
The isolated temporary app was prepared and launched to its initial scan/audio
setup, showing `7.82/macOS-arm64 rev 338578 (Oct 4 2026)`. No SYS 01 REAPER
session, routing, bounce or reopen test was performed. After the skip instruction,
the temporary host was quit, its exact app copy removed and its disk image
unmounted. No app was installed in /Applications. Do not install any other app
solely for testing, and do not resume REAPER tests without a new instruction.
Existing user preferences were not deleted. This supersedes the earlier pending
EULA checkpoint; the declared REAPER release cells remain unqualified, not passed.

`validated_hosts` and `validated_platforms` remain empty because no complete
promised host/platform cell, including automation/MIDI proof and final AU hash, has passed. Local fixture/validator passes and this
partial AU/VST3 sessions do not qualify Windows, native Intel, minimum macOS or full
AU timing. Manifest text records the observed boundaries without claiming parity.

## Exact external prerequisites and resume

1. Recover native Live UI control and record a timed CC20–23 sequence in the
   retained MIDI session. Measure ordinary DAW automation offsets independently.
   Repeat the AU session against the final sealed thin AU hash; earlier AU reports
   bind their named binaries. Preserve the successful VST3 reopen/PCM proof. Capture routing,
   driver/buffer/rate, automation/CC positions, bounce and reopening artifacts,
   full host build and installer/executable hashes. Test devices are development
   copies; reinstall exact builds at owned temporary paths when resuming.
2. Keep REAPER cells SKIPPED per owner. Use the installed Ableton host only
   for this sprint; do not install other apps for testing. The release owner
   must reconcile the original broader matrix before declaring support.
3. Native MAC-INTEL: MacBook Pro 13-inch 2017 four-TB3 / 3.1 GHz i5 / 8 GB,
   **macOS 11.7.10 x86_64**, Live 12.4.6 VST3/AU (REAPER cells skipped per latest owner instruction).
   Build both native format fixtures with `-DCMAKE_OSX_ARCHITECTURES=x86_64
   -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0`; execute, do not cross-build only. Run
   Steinberg validator and `/usr/bin/auval -v aumf Sys1 Algr -strict` on the exact
   candidates. Diagnose the x86_64 scanner failure on a native Intel machine.
4. Native MAC-ARM-MIN: Mac mini M1 2020 / 8 GB, **macOS 11.7.10 arm64**,
   the two Ableton format cells plus native fixture/validators; REAPER skipped. Modern UMP dispatch
   is unavailable on macOS 11; legacy MIDI is required and UMP omission explicit.
   CLT/compiler/SDK must be compatible with the pinned JUCE and that machine;
   record actual versions rather than promising local Clang 21 runs there.
5. WIN10-X64 and WIN11-X64: i5-8250U PC / 8 GB, **Windows 10 22H2 x64** and
   **Windows 11 24H2 x64**, respectively. Record exact OS patch/build, machine
   model, ASIO interface/driver, power mode and actual MSVC/Windows SDK/CMake
   versions. Provide VS 2022 C++ desktop tools, CMake >=3.22, Python 3 and Git,
   pinned JUCE/doctest checkouts and the complete dirty SYS 01 source inventory.
   No accessible Windows host/session was available here. Run natively:

   ```powershell
   cmake -S sys-01 -B sys-01/build-sprint02-win -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTING=ON -DALGORHYTHMIA_BUILD_DESKTOP=ON -DALGORHYTHMIA_POSITIONED_VST3=ON
   cmake --build sys-01/build-sprint02-win --config Release --parallel 4
   ctest --test-dir sys-01/build-sprint02-win -C Release --output-on-failure -V
   cmake -S sys-01 -B sys-01/build-sprint02-win-negative -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTING=ON -DALGORHYTHMIA_BUILD_DESKTOP=ON -DALGORHYTHMIA_POSITIONED_VST3=OFF
   cmake --build sys-01/build-sprint02-win-negative --config Release --target sys01_VST3 --parallel 4
   ```

   Run the ON fixture against the OFF target DLL; require the offset mismatch,
   not a loader failure. Build official Steinberg SDK validator at the recorded
   commit/submodules; run on the ON DLL/bundle and hash both binaries. Use an already installed
   Live 12.4.6 x64 host; do not install additional DAWs for testing. Exercise its VST3 cell's
   scan, silent/disabled-input stereo generation, asymmetric transform input,
   plugin/host bypass, CC20–23, automation, offline bounce and project reopen.
   Retain sessions, audio files, installer/candidate hashes and all actual output.
6. Resolve the AU timing proposal without silently downgrading ADR 0002. Retain
   strict failure until a reviewed bounded integration passes all required
   points/offsets/ties/quantization/overflow/nonfinite/replay cases and both DAWs.

Changed files in this sprint: `apps/desktop/{CMakeLists.txt,VST3ContractTests.cpp,
AUContractTests.cpp,AUOffsetTests.cpp}`, `docs/adr/0003-positioned-vst3-integration.md`,
`docs/implementation-status.md`, `release/manifest.json`, this `sprint-02.md`,
`state.md`, `au-timing-scope-decision.md` and the enumerated `evidence/sprint-02/`
packet. Existing user work elsewhere and Sprint 01 sources were preserved.
Inventory and SHA256SUMS bind final dirty inputs, changed documents, exact
commands/results and artifacts. No sanitizer rerun is claimed because shipping
code was unchanged; native Release, format-specific fixtures, validators,
formatting and source-package contracts cover the changed boundary.

The actual stock-wrapper bundle is retained as `negative-control-vst3.tar.gz`;
extraction reverified its executable hash and the fixture failed for the same
intended offset mismatch. This archive is a local test artifact, not a distribution
package. Owned temporary plugin copy cleanup is recorded in commands.json.

Compact checkpoint: Sprint 02 PARTIAL; both native suites pass; VST3 negative
control fails for its intended reason; AU strict timing fails; real validators, strict development seals and partial Live AU/VST3 session
evidence retained; CC delivery and ordinary DAW offsets unqualified. Recover Live, obtain native minimum/Intel/Windows machines, keep REAPER
skipped per owner, and resolve AU scope.
Do not execute downstream sprints, publish or tag from this checkpoint.


## Final cleanup and evidence verification

Only the hash-verified `SYS01-Sprint02.vst3` and `SYS01-Sprint02.component`
test copies were removed from the user's plugin folders after validation.
Other plugins/preferences were untouched. The REAPER temporary app/installer
were removed and its image unmounted at the owner's instruction. Build bundles,
retained negative bundle, sessions, source media and reports remain. No release
package or tag was created.

Reinstall exact development candidates from their temporary build roots to
those owned test names before resuming a DAW project; if temporary builds are
absent, rebuild and reverify hashes, then invalidate/rerun affected checks.
Do not infer that a saved project with a missing test copy is an install pass.
The stock negative archive is `evidence/sprint-02/negative-control-vst3.tar.gz`,
SHA-256 `6eb5d8a6d6bc3844ab9896c7ec088ce8c68ebd54e6ea349a95dbe839b7bf6cf4`.
It was extracted and executed with the final ON fixture, yielding the intended
positioned-point mismatch rather than a loader failure.

Nine protected Sprint 01 foundation inputs were rehashed unchanged, including
processor/editor/shared scheduler, catalogue, both adaptation scripts and VERSION.
The complete dirty source inventory is required on external machines: the
existing source-only export allowlist omits `.mm`, audio/session/test archives
and `.log` evidence. Its passing contract is not a complete native macOS source
or evidence package. Packaging correction remains Sprint 06; no exported
archive is represented as a shipping/native qualification packet here.

`commands.json` records final formatting, diff, evidence/document verification,
source-export contract results and snapshot capture. `inventory.json` binds final
source inputs and artifacts; the earlier inventory remains historical. SHA256SUMS
covers this final packet plus this sprint ledger, state and scope decision.

The DAW session/media archive `daw-session-fixtures.tar.gz` preserves the original
project-directory layout for resumption (extract under `/private/tmp` to recreate
the test fixture root). Its exact hash is in inventory.json and commands.json's
retention log. Flat copies are retained for direct review. Media references have
not been certified portable to a clean machine. The owned fixture folder may
remain in Live's Places and its test set in recents because native UI cleanup
was unavailable; remove only `sys01-sprint02-daw` from Places when resuming.
No existing preferences were deleted as part of the app/plugin-file cleanup.
