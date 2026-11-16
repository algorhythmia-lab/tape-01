# SYS 01 / local release packet preparation

Sprint 06 packets are development-unqualified. No public release/tag, support
claim or signed-bundle bit-identity claim follows from packaging. Source archive
paths, modes, ordering and gzip/tar timestamps are normalized. Reports retain
actual run times; signing/notarization timestamps are deliberately real.

Build from a source export in an empty directory with CMake 3.22+, C++20 compiler,
Python 3.9+, platform SDK and network access to the two pinned archive URLs in
`cmake/dependencies.lock.json`. Native macOS additionally requires Apple command
line tools; Windows uses MSVC x64 and a Windows SDK. No sibling brand directory,
existing dependency checkout or shared cache is required. Local dependency
overrides must match every pinned file; they are optional development inputs.
Export archives intentionally omit historical evidence trees; preserve those in
the separate release evidence packet. Max/Pico remain outside desktop packaging.

```sh
python3 scripts/package_source.py --output /tmp/sys01-source.tar.gz
# Extract into a new directory; configure there, with no dependency overrides.
cmake -S /tmp/sys01-source -B /tmp/sys01-build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON -DALGORHYTHMIA_BUILD_DESKTOP=ON
cmake --build /tmp/sys01-build --config Release --parallel 4
ctest --test-dir /tmp/sys01-build -C Release --output-on-failure
cmake --build /tmp/sys01-build --config Release --target candidate-provenance
# Run scripts/ci_native_evidence.py on this build for fresh bound reports.
# package_candidate.py --help describes explicit artifact/provenance/source inputs.
python3 scripts/verify_candidate.py /tmp/local-packet
```

The packet includes the exact source archive, dependency tree/archive inventory,
compiler/SDK/build settings, revision and dirty status, complete artifact hashes,
validator commands/tool/log hashes, source license/notices and prerequisites.
Each report is checked against its entire named bundle and log. Packaging rejects
unbound reports, stale source inputs and changed artifacts. Failed reports remain
FAIL in a development packet. Signing, stapling, metadata or executable changes
require new provenance and validators. Earlier host reports cannot be relabeled.
A clean revision is preferable for the eventual candidate; dirty work is recorded
honestly using source-file hashes. Unsigned manifest hashes give local integrity,
not third-party provenance authentication.

macOS preparation: `scripts/prepare_macos.py --plan` prints commands. Execution
copies inputs to a new staging root, uses explicit existing Developer ID Application
and Installer Keychain identities, enables hardened runtime and secure timestamps,
constructs a signed PKG containing VST3/AU and notices/source under
`/Library/Application Support/algorhythmia/SYS 01/<version>`, submits via an existing
notarytool Keychain profile, requires Accepted, staples and assesses the PKG.
No password/private-key argument or export is supported. No Developer ID Application/Installer identities
were visible in Sprint 06; unrelated self-signed identities are not used. Profile availability is not inferred or secrets searched.
Validate the final signed staged bundles, then retain final stapled PKG hashes and
Gatekeeper results on a quarantined clean-system download. Do not re-sign/staple
after recording the final validator identity. No installer is run on the personal
machine by this sprint.

Windows preparation: the owner selected **ZIP containing the x64 VST3, notices/source
and PowerShell install/rollback scripts; Authenticode-sign the plugin when authorized
credentials are available**. `scripts/prepare_windows.py --plan` prepares explicit
certificate-store signing commands without secrets. `scripts/archive_candidate.py`
creates a deterministic ZIP after packet verification. Candidate packets carry the
PowerShell tools under `windows/`; the source archive carries their implementation
and synthetic tests under `scripts/windows/`. A macOS packet contains review copies
of the tools and its macOS bundles; it is not a Windows executable distribution.

On the clean Windows fixture use the existing authorized certificate
store/hardware-backed key with `signtool sign /sha1 <selected-public-thumbprint>
/fd SHA256 /tr <approved-RFC3161-URL> /td SHA256 <plugin-executable>`, followed by
`signtool verify /pa /all /v <plugin-executable>`. No PFX password belongs in
arguments, repository or diagnostics. Require the selected signer and a timestamp
certificate, then retain signature verification and rerun final-byte native
conformance/validators before packaging. Credentials/provider, timestamp endpoint
and a Windows machine remain missing locally; no Windows signing pass is claimed.

The development install/rollback/uninstall tools require a **new marked temporary
fixture on Windows**, closed hosts and explicit `-HostsClosed`. Default installation
requires trusted timestamped Authenticode; unsigned testing requires the explicit
`-AllowUnsignedFixture` switch and is recorded as such. They check all candidate
file hashes, reject macOS bundles/reparse paths/changed or unexpected owned files,
stage verified copies and retain exact owned versions for rollback. Rename failure
attempts to restore the previous owned deployment. Uninstall removes it from the
fixture scan path while retaining recovery content; projects/preferences outside
the owned deployment are preserved. `windows/README.md` describes commands,
receipt/history locations and recovery limits. No personal plugin directories or
Program Files paths are enabled in Sprint 06, and no execution-policy/unblock
change is made. Use an authorized disposable account with a policy permitting the
reviewed scripts and configure its host's custom VST3 folder explicitly.

`Test-Sys01Install.ps1` covers parsing, synthetic install/upgrade/rollback/uninstall/
reinstall, missing setup/signature/host-close guards, tamper, user-file preservation
and injected rename-failure recovery on Windows CI. PowerShell is unavailable on
the local Mac, and the new CI job has not run. These script tests and all actual
Windows clean-system/host lifecycle cycles remain **UNPERFORMED locally**.

Clean-system qualification remains UNPERFORMED on each declared OS/CPU/host cell.
Use disposable test accounts/VM snapshots and explicitly authorized locations;
never replace personal plugin folders. Retain OS build/CPU, host installer/hash,
audio driver/rate/buffer and source/package/executable hashes. Perform **three**
cycles per package: snapshot -> install -> host scan -> stereo generation/transform
-> save/reopen -> upgrade from prior version -> reopen/compare parameters and PCM
-> uninstall owned binaries/notices only -> confirm projects/preferences preserved
-> rollback to prior signed package -> reopen saved project. Test failed upgrade
recovery, missing credentials, tampered package and unexpected pre-existing files.
For macOS use the system installer/uninstaller receipt and owned-file inventory;
for Windows use the selected ZIP/install route in the dedicated fixture only.
No prior qualified signed version exists here; upgrade/rollback cannot be claimed
using two labels on identical bytes. Directory-copy tests only prove staging logic.

Preserve local diagnostics via `scripts/collect_diagnostics.py --output <new-file>
--binary <executable> --host <DAW-version>`. It collects OS/architecture, supplied
host and optional executable hash, no home path, environment, credential, machine
identifier, project or audio. Review supplied free text before sharing; nothing is
sent automatically.

[Apple notarization documentation](https://developer.apple.com/documentation/security/notarizing-macos-software-before-distribution)
and [Microsoft SignTool documentation](https://learn.microsoft.com/en-us/windows/win32/seccrypto/signtool)
are the external signing references. The user-selected AGPL-3.0-only text comes
from [GNU](https://www.gnu.org/licenses/agpl-3.0.txt); notice provenance is in
`licenses/inventory.json`. Rights and corresponding-source review remain blocked.
