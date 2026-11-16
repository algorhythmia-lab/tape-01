# Sprint 00 / authority and release baseline

2026-10-14 (America/Santiago). **COMPLETE** at the requested reviewable-ledger and
fresh-baseline boundary. The release remains **BLOCKED**, with manual and rights
gates pending in [state.md](state.md). Revision:
`5c45cf6bed431317a1b412b1fc12ee62b66cfdd5`; no commit, tag or publication created.

## Scope and prerequisite verification

Read ancestor/SYS 01 instruction paths (no applicable AGENTS.md), Git status,
implementation-status, ADR 0002/0003, release manifest, old audit, reference UI,
validation history, catalogue/codec, build/tests/scripts, NOTICE, CI and retained
candidate reports. No previous `docs/release-1.0.0/state.md` existed. Existing
untracked release-plan, website, brand and TAPE work was preserved. Inspection
found no tracked runtime diff; fresh build inputs are indexed by SHA-256.

The owner delegated hardware/matrix/instance decisions, selected AGPLv3 for
first-party SYS 01 code and accepted optional donations alongside free downloads.
The selected minimum fixtures, exact qualification targets, instance count,
budgets and remaining rights review are recorded in state.md. This sprint records
the license decision; Sprint 06 implements the license/source/notice packet after
file provenance review. No rights status was promoted in the release manifest.

## Historical reconciliation (history remains intact)

| Historical assertion | Current authority / disposition |
|---|---|
| Oct 09 audit: no native shell/UI/control state, no Git baseline, versions at 0.1.0, broken Max selectors | later implementation-status and fresh tests establish these repairs. Central VERSION is 1.0.0 development; old archives and findings remain historical. Max structural repair is not a finished .amxd or host qualification. |
| reference UI: CLAP/Max, exact hardware facsimile, spinning/host-clock halos, SYNC HOST/ports and full callback zero-allocation | ADR 0002 keeps VST3/AU planned scope, real DSP telemetry, restrained vector geometry and free clock; whole-format realtime still blocked. Website preview is silent and fixed renders are not native evidence. |
| reference UI: binary STATE conflated with three-domain `engine_state`; `engine_bypass` with inverted meaning; MMC/CC24 | catalogue owns separate `subregister_state`/`engine_state`, `bypass` 0 wet/1 dry; fixed CC20–23 only. No ID change or invented MIDI support. |
| per-domain STATE, captured tape recall, long-hold desktop save, sync/subdivisions, gain chord/mix | shared binary STATE and parameter-only deterministic empty-tape restart; direct trim/brake/presets/bypass implemented; other promises deferred by ADR 0002. |
| hardware cellular 20–360 BPM / 1–2000 ms loop | shared engine 30–300 BPM / -2..+2x; 32768-frame transform history and 2048-frame wet offset; no fixed-ms loop or latency compensation promise. |
| cassette envelope called golden; 1:1 physical controls and global Tab/Space shortcuts | 100.4:63.8 ~=1.574; only central 20:12.36 window ~=golden. Preserve geometry cues, numeric/keyboard/native controls; Tab focus and host transport are not captured globally. |
| ADR 0003 earlier paragraph: rejected final targets may apply next block | ADR 0002 and ADR 0003's later clarification plus fixture establish no replay; new valid edit/coherent reset recovers. Earlier paragraph is not a new feature request. |
| earlier 8-hour simulations, throughput, screenshot and validator passes | retained evidence only; no equivalent real-time soak, musical quality, accessibility, DAW or installation pass implied. |

## Inventory

Local: macOS **26.6.2 / 25G83**, native **arm64**, MacBookPro18,1 / M1 Pro / 16 GB.
Apple Clang **21.0.0 (clang-2100.1.1.101)**, Command Line Tools SDK **26.5**,
CMake **4.4.4**, GNU Make **3.81**, Python **3.13.7**, clang-format **18.1.8**.
Full Xcode is unavailable (`xcodebuild -version` reports Command Line Tools);
basic `sysctl` hardware inspection was sandbox-denied, so filtered
`system_profiler` supplied model/chip/RAM without serial data. Ableton Live Suite
**12.4.6 (2026-09-10_0de5c8fa9a)** is installed, not exercised. No local pluginval
was initially on PATH; AU `auval` exists but no test component was registered.

Build: C++20, strict warnings as errors, no fast math or FP contraction, Release,
tests/benchmarks enabled. Portable DSP remains dependency-free; doctest is test
only. Native code links JUCE audio_utils and uses system fonts, generated brand
theme and APVTS/catalogue bindings. Formats/IDs/schema remain as recorded in state.

| Pin / identifier | Verified value |
|---|---|
| JUCE 8.0.12 | `29396c22c93392d6738e021b83196283d6e4d850`, clean temporary checkout |
| doctest 2.4.12 | `1da23a3e8119ec5cce4f9388e91b065e20bf06f5`, clean existing checkout |
| reviewed JUCE VST3 translation unit SHA-256 | `16031325b31be2e32b23689f07338bd6c631941475b9bbf87d176d1f2565c202` |
| bundle / manufacturer / product | `org.algorhythmia.sys01` / `Algr` / `Sys1` |
| catalogue / state | seven ordered IDs, schema 1 / 40 bytes; state.md lists defaults and semantics |
| source version | `sys-01/VERSION`: 1.0.0; development target only |
| CI checkout / upload action pins | `11bd71901bbe5b1630ceea73d27597364c9af683` / `ea165f8d65b6e75b540449e92b4886f43607fa02` |
| CI validator pin | pluginval 1.0.4 macOS ZIP SHA-256 `3c4c533bda0c5059eea3ddaea752d757ee2025041f0f47e6bcb0e87f6082b29f` |

Pinned JUCE LICENSE.md lists AGPLv3/commercial options and bundled dependencies.
Notice inputs for VST3 SDK (MIT), AudioUnitSDK (Apache 2.0), doctest (MIT), ASIO
(GPLv3/proprietary alternatives), text shaping, image codecs and compression are
hashed in inventory.json; this is an input inventory, not an exhaustive legal
clearance. AAX/Max/Pico remain outside the shipping scope. Do not apply AGPL labels
over upstream licenses or brand/hardware assets. System fonts have no newly
bundled font asset. The accepted route requires corresponding source, build
instructions and preserved notices; donations are compatible with the AGPL's
permission to charge for copies. [Pinned JUCE license](https://github.com/juce-framework/JUCE/blob/29396c22c93392d6738e021b83196283d6e4d850/LICENSE.md),
[GNU AGPLv3](https://www.gnu.org/licenses/agpl-3.0.en.html).

## Commands and results

All commands ran from `/Users/lhommeduchili/algorhythmia` unless explicitly `git -C`.
[commands.json](evidence/sprint-00/commands.json) retains full executable paths,
argv, cwd, UTC start, exit status and log SHA-256. Below, `CMAKE_BIN` means
`/Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin`;
`ROOT` means the workspace above. These abbreviations are explanatory; the
recorded commands use absolute expanded paths.

```sh
git clone --no-checkout --filter=blob:none https://github.com/juce-framework/JUCE.git /private/tmp/sys01-sprint00-juce
git -C /private/tmp/sys01-sprint00-juce checkout --detach 29396c22c93392d6738e021b83196283d6e4d850
```

Initial sandboxed clone exited 128 (DNS/network unavailable); the approved
network retry and checkout exited 0. Dependencies were retrieved into temporary
storage; the checkout remains clean and supplied source was not rewritten.

```sh
"$CMAKE_BIN/cmake" -S sys-01 -B /private/tmp/sys01-sprint00-core-release -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON -DALGORHYTHMIA_BUILD_BENCHMARKS=ON -DALGORHYTHMIA_BUILD_DESKTOP=OFF -DFETCHCONTENT_SOURCE_DIR_DOCTEST="$ROOT/sys-01/build/_deps/doctest-src" -DPython3_EXECUTABLE=/usr/local/bin/python3
"$CMAKE_BIN/cmake" --build /private/tmp/sys01-sprint00-core-release --parallel 4
"$CMAKE_BIN/ctest" --test-dir /private/tmp/sys01-sprint00-core-release -C Release --output-on-failure -V
"$CMAKE_BIN/cmake" -S sys-01 -B /private/tmp/sys01-sprint00-native-release -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON -DALGORHYTHMIA_BUILD_BENCHMARKS=ON -DALGORHYTHMIA_BUILD_DESKTOP=ON -DALGORHYTHMIA_POSITIONED_VST3=ON -DALGORHYTHMIA_JUCE_SOURCE=/private/tmp/sys01-sprint00-juce -DFETCHCONTENT_SOURCE_DIR_DOCTEST="$ROOT/sys-01/build/_deps/doctest-src" -DPython3_EXECUTABLE=/usr/local/bin/python3
"$CMAKE_BIN/cmake" --build /private/tmp/sys01-sprint00-native-release --parallel 4
"$CMAKE_BIN/ctest" --test-dir /private/tmp/sys01-sprint00-native-release -C Release --output-on-failure -V
/private/tmp/sys01-sprint00-native-release/apps/desktop/sys01_vst3_contract "$ROOT/sys-01/dist/sys01-development-macos-arm64/binaries/VST3/SYS 01.vst3"
/usr/local/bin/python3 sys-01/scripts/check_format.py --check --clang-format /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/clang_format/data/bin/clang-format
```

All above configure/build/test/format commands exited 0. The doctest CMake
deprecation warning is retained; no warning was suppressed to claim a pass.

| Fresh check | Result / limit |
|---|---|
| core Release | 12/12, 4.06 s; 28 DSP cases / 662,575 assertions, 7 host cases / 10,605 assertions |
| native Release | 14/14, 4.19 s; includes native restore/MIDI/rate/block/editor construction/concurrency and loaded VST3 point/offset/capacity/overflow/nonfinite/recovery fixture |
| retained VST3 loaded fixture | passed against the original packaged executable; not inferred from the rebuild |
| theme/catalogue/Max/source export/WAV/memory contract | included in CTest; all passed; firmware contract is host storage only |
| core runtime hooks / symbols / short stress | passed; excludes complete framework callback and musical quality |
| formatting | first-party C/C++ passed, clang-format 18.1.8 |
| retained candidate manifest | 36/36 entries hash-match, including reports and screenshots |
| fresh pluginval 1.0.4 | level 10, seed 1010, GUI/editor tests enabled, SUCCESS on exact fresh VST3; sandboxed startup exited -6 (SIGABRT), approved GUI-access retry exited 0 |
| strict development-bundle signatures | VST3 exit 1: `moduleinfo.json` added after seal; AU exit 1: code has no resources but signature expects them. Baseline defects assigned to Sprint 06, not repaired in Sprint 00. |

Retained reports: Oct 10 ASan/UBSan 12/12; core/native TSan; Clang analyzer ten
units; workflow lint; extracted source 11/11; stock-wrapper negative control;
four scale images; earlier pluginval levels 5/10 and exact-candidate final level
10. Oct 13 core 12/12 is also retained. They were not regenerated by these fresh
checks. No remote CI execution is available. AU compiled, but its native timing,
auval registration and DAW behavior are still unperformed.

Additional exact commands (full paths/exit codes in commands.json and
retrieval.json):

```sh
curl -L --fail https://github.com/Tracktion/pluginval/releases/download/v1.0.4/pluginval_macOS.zip -o /private/tmp/sys01-sprint00-pluginval.zip
shasum -a 256 /private/tmp/sys01-sprint00-pluginval.zip
unzip -q /private/tmp/sys01-sprint00-pluginval.zip -d /private/tmp/sys01-sprint00-pluginval
/private/tmp/sys01-sprint00-pluginval/pluginval.app/Contents/MacOS/pluginval --strictness-level 10 --random-seed 1010 --output-dir /private/tmp/sys01-sprint00-pluginval-reports --validate "/private/tmp/sys01-sprint00-native-release/apps/desktop/sys01_artefacts/Release/VST3/SYS 01.vst3"
codesign --verify --deep --strict --verbose=2 "/private/tmp/sys01-sprint00-native-release/apps/desktop/sys01_artefacts/Release/VST3/SYS 01.vst3"
codesign --verify --deep --strict --verbose=2 "/private/tmp/sys01-sprint00-native-release/apps/desktop/sys01_artefacts/Release/AU/SYS 01.component"
```

Archive pin was verified before execution. pluginval's log includes named auval
and VST3-validator stages, but this run selects VST3 and explicitly skips the
Steinberg validator because no validator path is set. Do not count those headings
as independent format-validator passes. The successful native report is retained
as `pluginval-fresh-native-report.log`; its executable/report/target hashes are in
inventory.json. Startup failure cause is not independently diagnosed; successful
retry does not erase it. Signature verification and validator success answer
different questions and both outcomes remain recorded.

## Artifact hashes

SHA-256 below is the executable, not a claim that entire bundles are identical.
Full file sizes/hashes, core archives/test executables, source inputs and retained
candidate entries are in [inventory.json](evidence/sprint-00/inventory.json).

| Artifact | SHA-256 |
|---|---|
| retained development VST3 executable | `8160afe70413fee6bfae57b20ff6e84becd6132ccbedfbdf8ce5bb8b71a64d21` |
| retained development AU executable | `8b449fbb5b1f87c10ff182ad591ef37ce395a70a3c1f9c002c0def867a546fb9` |
| retained development standalone executable | `16aef1e255f0e21249721278ba5b0840427a10b6e8ab9286b69ad72902e3581f` |
| retained source 1.0.0-development archive | `e86d2bd9ed67962ef7ec9554f61d1bbb890426ddc0cdbbb05801a95ba73db915` |
| fresh native VST3 executable | `f2618e5036de3c00f3ce447415751bfe4b5e34be5972faa9b6df9d3efdb58209` |
| fresh native AU executable | `70490925bfc0ab51ff82a0e248963395a2dd1f54d51a73b80dd25619957d626e` |
| fresh native standalone executable | `fb7bc11bc5ff1cd0d427be6fe1e2410019c0c9f28a1a6509a3481f619a453cea` |

Retained binaries are under `sys-01/dist/sys01-development-macos-arm64/binaries/`;
fresh binaries under `/private/tmp/sys01-sprint00-native-release/apps/desktop/sys01_artefacts/Release/`.
The changed bytes have different hashes; retained reports do not qualify the
fresh build. Generated source is confined to its build tree.

## Changed files, final verification and checkpoint

Added `sys-01/docs/release-1.0.0/state.md`, `sprint-00.md` and evidence files under
`evidence/sprint-00/`. The final evidence manifest enumerates every added file;
it excludes itself to avoid a self-hash cycle. No existing tracked source,
manifest, ADR, schema, IDs, UI or historical audit was edited. Dependency/build
directories and capture helpers in `/private/tmp/sys01-sprint00-*` are local
temporary artifacts, not release packages.

Final documentation verification checks exact command-log hashes, 166 inventory
artifacts (including 100 tracked source inputs), local document links and the
complete evidence manifest. Source export reproducibility/hash checks passed
again after the new ledger was added; final commands/results are retained below
and in commands.json. `git diff --exit-code -- sys-01` passed and Git status
shows only this newly added directory beyond the preserved initial work.

```sh
/usr/local/bin/python3 sys-01/docs/release-1.0.0/evidence/sprint-00/verify.py
/usr/local/bin/python3 sys-01/scripts/test_source_package.py
git status --short
git diff --exit-code -- sys-01
/usr/local/bin/python3 sys-01/docs/release-1.0.0/evidence/sprint-00/verify.py --write-manifest
/usr/local/bin/python3 sys-01/docs/release-1.0.0/evidence/sprint-00/verify.py
```

Each final command exited 0. Evidence hashes and changed-file enumeration are in
`evidence/sprint-00/SHA256SUMS`, verified independently after writing. Manual omissions and next
actions remain the gate ledger in state.md: next separately requested sprint is
01; no automatic implementation of 01–08. If interrupted, keep the ledger and
logs, finish only final evidence verification, and rehash artifacts before reuse.
