# Install SYS 01

The first public release is **v1.0.0-beta.1**, a free macOS testing build with a
standalone app, VST3 and Audio Unit. It contains both Apple silicon and Intel
code, targets macOS 12+, and is not yet production-qualified. Intel code is
tested through Rosetta locally; native Intel and older OS testing remain open.
See [release limitations](../release/public-beta.json).

## Recommended: Terminal download

Close SYS 01 and your DAW before installing or updating. Paste this in Terminal:

```sh
sys01_tmp="$(mktemp -d)" &&
curl --fail --location --proto '=https' --tlsv1.2 \
  https://github.com/algorhythmia-lab/tape-01/releases/download/v1.0.0-beta.1/install-macos.sh \
  --output "$sys01_tmp/install-macos.sh" &&
/bin/bash "$sys01_tmp/install-macos.sh"
```

The versioned installer downloads the release archive over HTTPS, checks its
embedded SHA-256 and verifies the bundles' ad-hoc signatures before installation.
It installs for your account without `sudo`:

| Format | Installed location |
| --- | --- |
| App | `~/Applications/SYS 01.app` |
| VST3 | `~/Library/Audio/Plug-Ins/VST3/SYS 01.vst3` |
| AU | `~/Library/Audio/Plug-Ins/Components/SYS 01.component` |

Existing SYS 01 bundles at these paths are replaced. Other products and
symlinks are rejected. If installation fails partway through, the installer
restores the previous bundles. It does not open the app or change security
settings. Rescan plugins or restart your DAW after installation.

To inspect the script before running, open the downloaded file in a text editor.
Run it with `--dry-run` to download and verify without installing, or `--help`
for offline archive and custom-prefix options. For example, appending
`--dry-run` to the last line above leaves your applications and plugins alone.

## Why curl helps on macOS

Browsers normally attach `com.apple.quarantine` to downloads. Apple's developer
documentation states that Unix networking tools such as `curl` do not set that
attribute. Downloading and extracting this archive in Terminal therefore avoids
that browser quarantine path. This is useful for testing builds, and the same
versioned-download, checksum and per-user-install pattern can be reused for
other projects.

It does **not** turn an ad-hoc signature into a Developer ID signature or
notarize an app. macOS can invoke Gatekeeper in other circumstances and still
checks for malicious software. Managed Macs and DAW policies may impose further
restrictions. A Developer ID signature plus notarization is the durable path
for normal browser downloads. This installer does not run `xattr`, disable
Gatekeeper, or modify system security preferences.

References: [Apple: Resolving Trusted Execution Problems](https://developer.apple.com/forums/thread/706442),
[Apple: Gatekeeper and runtime protection](https://support.apple.com/guide/security/gatekeeper-and-runtime-protection-sec5599b66df/web).

## Manual download and removal

The [GitHub release](https://github.com/algorhythmia-lab/tape-01/releases/tag/v1.0.0-beta.1)
also contains the archive, corresponding source, manifest and `SHA256SUMS.txt`.
The default Terminal route above is recommended for this unnotarized beta.
If you download in a browser and macOS blocks opening the app, follow
[Apple's Open Anyway instructions](https://support.apple.com/en-us/102445)
only after confirming you trust the download.

To remove SYS 01, quit the app and DAW, then move the three installed bundles
listed above to the Trash. Saved standalone preferences may remain in your
Library; removing a plugin does not change your DAW projects.

## First sound and feedback

Lower monitoring volume. Open the app, select **01 / magnetic pulse**, enable
DRIVE/RUN, and move FLUX and BIAS. Generators work without input; a DAW may need
an active audio track to process a generator effect. **03 / forward tape** needs
stereo input, RUN enabled, BRAKE released and FLUX around .75 for forward tape.
Read the [musical guide](musical-guide.md) for the controls and recall behavior.

[Report a bug](https://github.com/algorhythmia-lab/tape-01/issues/new/choose)
with the release tag, Mac/CPU, macOS, DAW and format (if applicable), sample
rate/buffer, routing, steps to reproduce and expected/actual behavior. Please
exclude private session audio and personal information from public reports.
