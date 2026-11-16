# SYS 01 Windows ZIP route / Sprint 06

Owner selected: ZIP containing the x64 VST3, notices/source and PowerShell
install/rollback scripts; Authenticode-sign the plugin when authorized credentials
are available. The Windows binary, signature and clean-system/DAW qualification
remain unperformed. A macOS candidate also carries these tools for review; its
macOS VST3 cannot be installed with them.

These development scripts intentionally operate only in a **new, explicitly
initialized temporary test fixture**. They do not install into personal plugin
folders or Program Files. Use a disposable clean Windows account/VM and configure
its approved host's custom VST3 scan folder to `<fixture>\installed\VST3`.
Close the host before every filesystem transition and provide `-HostsClosed`.
No host settings are changed automatically. Record execution policy and downloaded-file
marking on the test account; its existing policy must permit the reviewed scripts.
These tools do not change execution policy or automatically unblock files. Standard user installation paths are
not enabled until qualification and the required test setup are complete.

Verify the downloaded ZIP against its separately trusted checksum and inspect
`candidate.json` before extraction/use. Scripts check all packet file hashes,
require a Windows-only VST3 packet and, by default, a trusted Authenticode
signature with a timestamp. Local unsigned fixture testing requires the explicit
`-AllowUnsignedFixture` switch; the receipt records that bypass. It is never a
signing/trust/clean-machine pass. No credential/password is accepted by these
install scripts.

```powershell
$fixture = Join-Path $env:TEMP 'sys01-clean-fixture-unique'
.\windows\New-Sys01TestFixture.ps1 -FixtureRoot $fixture
.\windows\Install-Sys01.ps1 -Candidate $PWD -FixtureRoot $fixture -HostsClosed
# Use the same Install command with the new verified packet for upgrade.
.\windows\Rollback-Sys01.ps1 -FixtureRoot $fixture -HostsClosed
.\windows\Uninstall-Sys01.ps1 -FixtureRoot $fixture -HostsClosed
# Rollback after uninstall restores the retained owned version.
.\windows\Rollback-Sys01.ps1 -FixtureRoot $fixture -HostsClosed
```

Receipts bind every installed file to a hash and the original candidate manifest.
The owned `installed` directory contains only VST3 and notices/source. Unexpected
or changed files cause refusal; scripts never remove them. Upgrades stage and
verify new bytes, move the old owned tree into `.sys01-state/history`, then rename
the new tree into place; a failed commit attempts to restore the old tree and
retains staging for inspection. Rollback verifies the retained version's receipt
and signature. Uninstall moves owned content out of the scan path, retaining a
rollback snapshot. Projects/preferences outside the owned tree are preserved.
Concurrent operations are excluded by a fixture lock. Reparse points, escaping
manifest paths and unmarked/existing fixture initialization are rejected.

Retained history consumes disk space; clear only the disposable fixture after
preserving needed evidence. This is not a power-loss journal, an atomic running-
host hot swap or proof of clean supported-system install/upgrade/rollback.
`Test-Sys01Install.ps1` creates synthetic, non-executable fixtures and tests the
filesystem/rollback controls on the Windows CI runner, including an injected
rename failure. Those tests do not replace three actual signed-package and host
scan/session cycles on each claimed Windows cell.
