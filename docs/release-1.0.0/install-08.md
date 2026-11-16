# SYS 01 / installation and rollback

**NO-GO: local development packet only.** Full release at no charge; no
qualified hosts/platforms, signed installer or supported public install exists
yet. This macOS arm64 ZIP contains VST3/AU, source, notices and review documents.
Windows PowerShell files are review copies; no Windows executable is included.

Before any approved fixture test, verify the ZIP SHA-256 and byte size against
the separate artifact index, extract into a new directory preserving executable
permissions, and run `python3 scripts/verify_candidate.py /absolute/packet` using
the script from the extracted source archive. It checks candidate.json inventories
and report bindings. Keep that index with the original ZIP. An unsigned checksum
provides local integrity, not publisher authentication. Never edit candidate.json
to bypass a mismatch. Do not disable OS security or remove quarantine to obtain
a passing test.

For local VST3 inspection, close the host, keep the supplied bundle intact in an
owned temporary test folder, and explicitly select that custom VST3 scan folder
in the existing approved host. Record its prior setting, scan/load the exact
bundle, and restore the setting afterwards. A custom-folder development scan
does not qualify installation. Do not replace personal plugin directories.
The AU requires a dedicated disposable registered-component fixture; its
in-process native tests do not constitute an auval or installed-host pass.

The eventual macOS route requires Developer ID Application signing of staged
bundles, final-byte validators, Developer ID Installer PKG, Accepted notarization,
stapling/Gatekeeper evidence and three clean-system lifecycle cycles. The intended
standard locations are `/Library/Audio/Plug-Ins/VST3/SYS 01.vst3` and
`/Library/Audio/Plug-Ins/Components/SYS 01.component`; notices/source are retained
under `/Library/Application Support/algorhythmia/SYS 01/<version>`. These are
future fixture destinations, not permission to install this unqualified ZIP.
See packaging-06.md in the source for the prepared credential-free recipes.

The selected Windows route is x64 VST3 ZIP with notices/source and PowerShell
install/rollback, Authenticode using an authorized existing identity and approved
timestamp endpoint. `windows/README.md` specifies the marked temporary fixture,
`-HostsClosed`, trusted timestamped signatures and receipt/backup checks. No
PowerShell execution-policy change is needed or authorized by this packet.
Windows runtime, signing and all real host/lifecycle checks remain unperformed.

## Rollback and removal

Save the project under a new name and render needed audio before changing a
plugin: SYS 01 never restores captured tape. Close the host. Retain the exact
previous package, its checksum, owned-file inventory/receipt and compatible
project copy. Verify that current files match the installed receipt; stop and
preserve unexpected files instead of deleting them.

For the temporary macOS custom-folder test, restore the recorded host scan
setting, remove only the owned staged bundle after checking its inventory, and
rescan/reopen the preserved project with the previous verified build. Keep
projects, recordings and preferences. No system installer or personal-directory
removal was executed here. For a future signed system deployment, removal and
rollback must follow its tested receipt/owned-file procedure; that evidence is
still missing. Never broadly delete a plugin or Application Support directory.

On a marked Windows fixture, use the shipped Rollback-Sys01.ps1 or
Uninstall-Sys01.ps1 with the exact fixture/receipt and closed-host arguments from
windows/README.md. Retain owned recovery versions; signature/tamper/unexpected-file
errors require investigation, not an unsigned override on a real installation.
These fixture tools are not a qualified Program Files installer.

No prior qualified signed SYS 01 package exists locally. Actual signed upgrade,
uninstall and rollback have **not** passed on any platform; relabeling identical
bytes as two versions cannot establish recovery. Do not discard a working version
or project backup while this gate is open.
