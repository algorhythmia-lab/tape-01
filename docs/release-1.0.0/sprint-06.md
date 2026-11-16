# SYS 01 / Sprint 06 / build, evidence and packaging

2026-10-15 (America/Santiago). **PARTIAL. Release remains BLOCKED /
development-unqualified.** Local build/evidence automation and a concrete macOS
arm64 development packet are implemented and exercised. Developer ID credentials,
Windows machine/signing credentials, rights approval and clean-system lifecycle
qualification are missing. No public release, tag, push or personal plugin-folder
installation was performed. No other sprint's product behavior was implemented.

Revision: `5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` plus preserved dirty inputs.
The candidate records `source_dirty: true`, the revision, the exported source
manifest and complete compilation-input hashes. Initial git status/input hashes
and the prior state are in `evidence/sprint-06/baseline.json` and
`baseline-state.md`. No applicable AGENTS.md was found in ancestors/SYS 01.
Implementation status, ADR 0002/0003, manifest and the prior sprint ledgers were
read. Historical retained evidence verifies 34/96/184/35/214/143 files for
Sprints 00/01/02/03/04/05 respectively; source/state entries intentionally
superseded by later sprints are not relabeled or resealed.

## Implemented boundary

* CMake now reads a single dependency lock with full revision URL, archive SHA-256,
  full source-tree SHA-256 and file count. Both fresh downloads and local JUCE/
  doctest overrides must match. Overrides are checked before use, populated trees
  at configuration, and trees again when producing build provenance after native
  targets finish. Negative controls reject a changed header and an extra file.
  Dependency retrieval has TLS verification and a timeout. Core builds without
  tests/desktop retain the no-Python/no-downloaded-dependency path.
* Workspace and standalone-export CI retain pinned action SHAs, cross-platform
  core/native builds, format/tidy/analyzer jobs and separate sanitizer trees.
  TSan now covers host concurrency as well as realtime tests. A native macOS
  ASan/UBSan job covers generated wrappers; native conformance/seals/pluginval
  reports identify exact bundle contents. Pluginval uses the existing verified
  1.0.4 archive and level 10/seed 1012, with GUI tests explicitly skipped in CI.
  Strict auval preparation runs only on a disposable GitHub-hosted macOS runner,
  refuses an existing test bundle, checks the copied bytes and removes only its
  unchanged owned copy. The local guard was exercised and refused installation.
  No remote CI execution is claimed; both workflow YAML files parse locally.
* Formatting covers Objective-C++ and the shared config permits it. Only whitespace
  changed in NativeEditorHost.mm. Tidy applies to project targets rather than
  upstream targets. The intentionally stable host/control enum widths retain
  their ABI; the size-shrinking advisory is explicitly excluded. Actual fixture
  multiplication warnings were fixed by widening before multiplication, without
  changing supported-rate results. Native static analysis includes .mm and
  rejects a zero-translation-unit run.
* Source export now includes Objective-C++ and extensionless dependency notices.
  It excludes historical evidence trees from source-only archives; those remain
  in this separate evidence packet. The archive binds revision/dirty origin in
  SOURCE_PROVENANCE.json and every file in SOURCE_MANIFEST.sha256. Ordering, modes,
  tar and gzip timestamps are normalized. Fresh retrieval/native compilation in
  `/private/tmp/sys01-sprint06/isolated` succeeded with **all dependency overrides
  empty**, no Git checkout or sibling brand directories, and no shared dependency
  cache. Compiler/CMake/Python/SDK installations are toolchain prerequisites.
* `candidate-provenance` depends on the built VST3/AU targets. It records source
  inputs, revision/dirty status, compiler executable hash/version, SDK, OS/CPU,
  CMake cache/settings, compilation commands, verified dependency inventory and
  complete binary/bundle hashes. Compiler metadata falls back to CMake's generated
  compiler file for generators that omit it from the cache; that fallback was
  tested with a cache copy, not represented as Windows execution.
* Validator execution fingerprints the artifact before/after, hashes the tool,
  saves exact argv/time/exit/output and checks an optional success expression.
  Assembly rejects a changed artifact, stale/incomplete source inventory, missing
  dependency/toolchain provenance, mismatched log, report for another artifact,
  unbound report file or artifact lacking a report. Failed reports remain FAIL in
  development packets. Verification works using only the exported Python tools.
  Packet negative controls also reject log substitution even after recalculating
  the outer file inventory, wrong report subjects and stale source provenance.

## Rights, signing and installation

The owner-selected **AGPL-3.0-only** text is retained unmodified in LICENSE.
GNU text SHA-256: `0d96a4ff68ad6d4b6f1f30f713b18d5184912ba8dd389f86aa7710db079abcb0`.
NOTICE records the selection without asserting ownership of every pre-existing
file. Eleven selected dependency notices were copied byte-for-byte from trees
verified against fresh pinned archives: JUCE/framework, VST3, AudioUnitSDK,
HarfBuzz, SheenBidi, PNG, JPEG, zlib, FLAC, Ogg Vorbis and test-only doctest.
`licenses/inventory.json` binds their upstream paths/revisions/hashes. No external
font asset is bundled; system fonts are requested at runtime. First-party/brand
provenance, complete compiled transitive applicability and corresponding source
including used dependency sources still require rights review. **G08 stays BLOCKED.**

`prepare_macos.py --plan` produced a concrete Developer ID Application/Installer
PKG/notarytool/Accepted/stapler/Gatekeeper recipe, using only explicit existing
Keychain identities/profile and staged copies. Notices/source install with the
payload. The sandbox saw zero identities; an authorized normal-Keychain read
found two unrelated self-signed identities and **no Developer ID identities**.
Neither was used. notarytool exists, but no profile was supplied or credential
contents searched. Actual signing, submission, stapling, Gatekeeper assessment
and downloaded-quarantine validation are **UNPERFORMED**. Ad-hoc integrity passes
below are not Developer ID signing/notarization.

The owner answered the pending question after the initial checkpoint and selected
**ZIP containing VST3, notices and PowerShell install/rollback scripts;
Authenticode-sign the plugin when credentials are available**. This resolves the
route decision only. `release/windows-route.json` and the candidate manifest record
it. `prepare_windows.py --plan` prepares certificate-store SHA-256/RFC3161 signing
and checks the signer/timestamp. Candidate packets include `windows/` with fixture
initialization, install, rollback and uninstall scripts and their common module.
The source includes the synthetic lifecycle/failure-control test; both workflows
add a Windows job to parse and execute it. Scripts operate only in explicit marked
temporary Windows fixtures, require closed-host acknowledgement, verify hashes,
require trusted timestamped signatures by default, retain owned rollback snapshots
and refuse unknown/changed files or reparse paths. `-AllowUnsignedFixture` is an
explicit recorded test bypass, never signing qualification. No execution policy
or personal plugin-folder configuration is changed. PowerShell/Windows are absent
locally: parsing/runtime/lifecycle tests have **not run locally or in remote CI**.
Actual Windows compilation, certificate/provider, timestamp endpoint, approved
policy/test setup and clean host/system cycles remain prerequisites.

Clean supported macOS Intel/arm64 and Windows 10/11 x64 fixtures, approved Ableton
builds and a prior signed version were unavailable. **Install, DAW scan, upgrade,
uninstall and rollback on clean supported systems are all UNPERFORMED.** No copy
into a personal plugin directory was made. A local direct-load/pluginval result
does not qualify these cycles. The three-cycle protocol and prerequisites are in
[packaging-06.md](packaging-06.md) and `release/prerequisites.json`, included in the
candidate. REAPER/no-new-testing-app steering is preserved. **G09 stays BLOCKED.**

`collect_diagnostics.py` remains an exclusive-create, local-only standard-library
path: OS/architecture, supplied host and optional executable hash; no environment,
username/home path, project/audio, machine ID, credential scan or upload. Fresh
plain and binary-bound JSON examples are retained. Review supplied host free text
before sharing. No optional diagnostic collection is treated as host qualification.

## Exact checks and limitations

[commands.json](evidence/sprint-06/commands.json) is the exact argv/cwd/UTC time/
exit/log-hash ledger, including failures. `tested-binary-inventory.json` retains
fixture/plugin and cache hashes for each build. `isolation-and-preservation.json`
records source/override/ZIP/report checks. Native Release uses macOS 26.6.2 build
25G83 / arm64, Apple Clang 21.0.0.21000101, SDK 26.5, deployment target 11.0;
that target flag is not execution on an older OS. CMake 4.4 is installed under the
recorded uv toolchain path; Python 3.13.7 is `/usr/local/bin/python3`.

| Check | Result / scope |
|---|---|
| Final isolated exported native build + CTest | **21/21 PASS**, including editor, recall, loaded VST3/AU and new dependency/report/source contracts; normal macOS framework access, no plugin installation |
| Core ASan/UBSan | Initial **14/14 PASS**; after fixture-only lint casts/new pin test, targeted final **4/4 PASS**, including musical behavior. Earlier musical result is superseded; other core test executables unchanged |
| Native ASan/UBSan | **5/5 PASS**: adapter, loaded VST3, AU callback/characterization and recall; reused upstream objects after full pinned tree verification |
| Core TSan / native TSan | **2/2 PASS** realtime + host concurrency; **1/1 PASS** native adapter. No zero-lock/realtime or whole-DAW qualification inferred |
| Static analysis / tidy / formatting | Analyzer **12 first-party units PASS**; clang-tidy **18.1.8 PASS** on SDK 15.2/core build; widened-fixture musical rerun PASS; final C/C++/ObjC++ format PASS |
| Exact final VST3 validators | Steinberg **47 passed / 0 failed**; pluginval **level 10, seed 1012, headless PASS**; final ad-hoc strict seal PASS |
| Exact final AU reports | Native callback PASS, characterization PASS, strict seal PASS; **strict parameter offset contract FAIL** (ordinary/scheduled collapse at zero; positioned MIDI passes) |
| Source/package controls | Source archive repetition PASS, full notice hashes PASS, changed dependency/source/binary/log/subject controls PASS, both workflow YAMLs parse, Python syntax and diff checks PASS |
| Local packet | Eight reports individually verified against named bundle bytes and log hashes; source/compile inputs match, ZIP contents and repeated ZIP bytes match |

Initial network retrieval in the sandbox failed DNS (curl exit 6); authorized
retrieval succeeded with pinned hashes. One sandboxed configure was interrupted
while fetching and emitted no complete configure report. The first native export
configure failed because .mm was omitted, and notice inspection found missing
extensionless licenses; both were fixed before the successful isolated build.
Formatting first failed on C++-only config, then on .mm whitespace; repaired.
LLVM tidy 18 initially could not discover Apple's libc++ headers; SDK 26.5 remains
incompatible with that older tool. The final tidy run uses the installed SDK15.2,
with explicit libc++ discovery. Stable enum-size advisories were scoped out;
fixture widening warnings were corrected. An intermediate mechanical cast edit
failed compilation and was repaired; failed logs are retained. Mixed ASan/TSan
configuration intentionally fails (exit 1), as does the local AU-install guard.

During iterative source-overlay testing, normalized archive mtimes prevented
CMake from noticing changed inputs in an existing build directory. Extracted
source mtimes were refreshed, first-party targets and generated wrappers rebuilt,
and the **final 21-test suite/validators/provenance** were run afterward. Extract
release source into a new directory; do not overlay a previous source tree and
assume timestamps imply freshness. The final archive/provenance code additions
only affect packaging scripts; bundle bytes remained unchanged and each report
binding was reverified. Earlier local packet iterations were moved to temporary
storage, not presented as final artifacts.

No fresh auval installation on this personal system, actual DAW scan/session
cycle, minimum-machine/native Intel/Windows, independent listening, assistive
technology, musician beta, performance soak, Developer ID or remote CI pass is
claimed. Earlier AU timing, realtime, sound/DC (MB-04), UI and external-machine
gates remain open. The strict AU failure is retained in the packet, not waived.

## Concrete local packet and hashes

Final packet: `sys-01/dist/sys01-sprint06-macos-arm64/` and
`sys-01/dist/sys01-sprint06-macos-arm64.zip` (6,639,341 bytes). These ignored local
outputs are preserved in the workspace. SOURCE_MANIFEST binds 172 selected source
files plus origin metadata; the archive has 174 entries including its manifest.
The source snapshot precedes final ledger sealing; this ledger/evidence remains
separate to avoid recursive archive/checksum hashes. Final compilation inputs
match the current working source. No source commit or release tag was created.

| Artifact | SHA-256 |
|---|---|
| Final local packet ZIP | `cf4f2515a81c35751c7dcfc2a37d867f8eee9353f40e62e938724146cde7ecd4` |
| Included source.tar.gz | `4850c00082b539303e832235b79c4c2c37d960afbcb072c61c156b684ab7475d` |
| candidate.json | `0b348fb85761e2bab4267247c50b93d4dfde02ced4b2cbbf6e9b020a26dba21f` |
| build-provenance.json | `3079113e9ac3e23041b815f79ad7c327b9a9485486dc81bcf7e0ebe7f346156c` |
| VST3 executable | `bdf225221f23bd28c75394f814cc212d0c922bc563b2c08a281fa5b176dbd968` |
| AU executable | `709acf92683d65c5de0ed712cca7cf6173d1c20114615d4e1b98aef3ce334996` |
| VST3 complete file-content fingerprint | `5c08c3b817d1560a8329dedede5ef51f3037b96a9d0414a8574434e4516ea597` |
| AU complete file-content fingerprint | `ec455f7a09f7d6f10522bb3e04bd2d7065b063c92ea826d54f6ab37f84231382` |

ZIP repetition proves deterministic compression/metadata for the **same existing
input files**. Source archive repetition is also checked. Neither proves an
independently rebuilt native binary, cross-toolchain output or bit-identical
Developer ID signed/notarized bundles. Actual report and signing times are not
fabricated or normalized into historical evidence.

Main commands (full absolute executables, arguments/results and failed attempts
are in commands.json):

```sh
python3 sys-01/scripts/package_source.py --output /private/tmp/sys01-sprint06/source-sealed-code.tar.gz
cmake -S /private/tmp/sys01-sprint06/isolated/source -B /private/tmp/sys01-sprint06/isolated/build \
  -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON -DALGORHYTHMIA_BUILD_DESKTOP=ON \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0 -DPython3_EXECUTABLE=/usr/local/bin/python3
cmake --build /private/tmp/sys01-sprint06/isolated/build --parallel 4
ctest --test-dir /private/tmp/sys01-sprint06/isolated/build --output-on-failure
cmake --build /private/tmp/sys01-sprint06/isolated/build --target candidate-provenance
python3 sys-01/scripts/ci_native_evidence.py --build /private/tmp/sys01-sprint06/isolated/build \
  --reports sys-01/docs/release-1.0.0/evidence/sprint-06/bound-reports \
  --pluginval /private/tmp/sys01-sprint00-pluginval/pluginval.app/Contents/MacOS/pluginval
python3 sys-01/scripts/verify_candidate.py sys-01/dist/sys01-sprint06-macos-arm64
python3 sys-01/scripts/archive_candidate.py sys-01/dist/sys01-sprint06-macos-arm64 \
  --output /private/tmp/new-local-packet.zip
```

These creation commands refuse existing outputs/reports. Use new paths on resume;
do not overwrite the retained packet to make old reports appear fresh.

Changed existing files: root and SYS 01 .clang-format/.clang-tidy, workspace and
standalone CI, root/desktop CMake and ProjectOptions, NOTICE/release manifest,
format/analyzer/source/export-test/candidate scripts, NativeEditorHost.mm
(whitespace), musical_tests.cpp (fixture casts), implementation-status and state.
Added: dependency lock/helper, LICENSE and verified licenses tree, prerequisites,
report/provenance/verification/CI/signing/archive/negative-control Python scripts,
packaging-06.md, this ledger and `evidence/sprint-06/`. Exact paths/hashes and local
artifact inventories are enumerated in the final `inventory.json`/SHA256SUMS.
Production DSP, processor/wrappers, stable IDs, state schema, factory values,
accepted formats, free distribution and deferred scope remain unchanged from the
pre-sprint dirty baseline. Prior work/evidence and unrelated website/brand work
were preserved.

## Resumable checkpoint

Sprint 06 **PARTIAL**. Verify the local packet with `verify_candidate.py` and this
packet's `seal_packet.py --verify`. Review source/provenance/report prerequisites
before changing artifacts. Use the owner-selected Windows ZIP/PowerShell route; obtain an authorized
Windows certificate/provider/timestamp endpoint and clean fixtures; obtain existing
Developer ID Application/Installer credentials and notarytool profile on macOS.
Complete first-party/transitive notice/corresponding-source rights review. Execute
signing preparation on new staging paths, then fresh final-byte validators and
three clean install/scan/upgrade/uninstall/rollback cycles per claimed cell,
retaining prior signed packages and host/project evidence. Do not reuse the
ad-hoc bundle reports after signing. Preserve every earlier blocker; no release
status, support arrays, tag or publication is authorized by this checkpoint.

Final sealing commands (run directly after command logging is complete, avoiding
a self-referential log/checksum), both exit **0**:

```sh
python3 sys-01/docs/release-1.0.0/evidence/sprint-06/seal_packet.py
python3 sys-01/docs/release-1.0.0/evidence/sprint-06/seal_packet.py --verify
```

`release-packet-index.json` binds the final ZIP, source, candidate/provenance,
ledger and command registry. SHA256SUMS additionally covers every enumerated
changed source/document, retained evidence and local packet file. Earlier packet
checksum files are preserved as historical evidence.


## Windows route follow-up / owner response

The direct owner response supersedes only the earlier unresolved Windows packaging
choice. `evidence/sprint-06/windows-route-followup/owner-decision.json` records it;
prior ledger/index/inventory/checksum/commands copies preserve the initial checkpoint.
The native executable bytes and eight validator report bindings remain unchanged.
Only route metadata, PowerShell/packaging source and CI/docs changed; full DSP/UI
and sanitizer suites are not repeated for this follow-up. Source export/repetition,
packaging/provenance, packet tamper controls, workflow YAML and checksum verification
are rerun. PowerShell parsing and runtime controls are explicitly **UNPERFORMED**
until the Windows job/fixture is available; no local parser/runtime or clean-system
pass is inferred from source review. The existing no-new-testing-app preference is
preserved; no PowerShell app/runtime was installed. Sprint 06 remains **PARTIAL**,
with credentials, complete rights review, clean target systems and earlier AU/
realtime/sound/UI/host gates still open. No public release/tag or personal plugin
installation is authorized by the route selection.

Follow-up verification: `windows-route-*` entries in commands.json record source
archive/repetition, packaging/provenance, all script-copy hashes, unchanged native
binaries/eight report bindings, packet tamper rejection, same-input ZIP repetition,
Python syntax, nine-job workflow YAML parsing and diff checks, all exit **0**.
`windows-powershell-prerequisite` exits **1** because no PowerShell executable is
available; parsing and synthetic script execution are explicitly UNPERFORMED.
The updated packet ZIP is 6,639,341 bytes, SHA-256
`cf4f2515a81c35751c7dcfc2a37d867f8eee9353f40e62e938724146cde7ecd4`.
The earlier packet is preserved at
`sys-01/dist/sys01-sprint06-macos-arm64-before-windows-route/` and its matching ZIP.
Run the Windows CI parser/synthetic filesystem controls first, then actual signed
plugin/host lifecycle qualification on the clean fixtures; neither is waived by
the selected route or local archive checks. Final `seal_packet.py` and `--verify`
are rerun after this follow-up; earlier sprint evidence is not resealed.
