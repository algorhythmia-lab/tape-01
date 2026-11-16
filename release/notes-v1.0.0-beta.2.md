# SYS 01 — public beta, corrected macOS packaging

The first public testing series of SYS 01: a free cassette-shaped
instrument and stereo effect with magnetic bytebeat, cellular FM and live tape
displacement.

**macOS 12+ · Apple silicon + Intel universal · standalone app, VST3 and AU**

## Download and install

Close SYS 01 and your DAW, then paste this in Terminal:

```sh
sys01_tmp="$(mktemp -d)" &&
curl --fail --location --proto '=https' --tlsv1.2 \
  https://github.com/algorhythmia-lab/tape-01/releases/download/v1.0.0-beta.2/install-macos.sh \
  --output "$sys01_tmp/install-macos.sh" &&
/bin/bash "$sys01_tmp/install-macos.sh"
```

The installer verifies the archive's pinned SHA-256 and bundle signatures, then
installs the app and plugins for your account. No `sudo` is required. Add
`--dry-run` to the last line to verify without installing.

These bundles have **ad-hoc integrity signatures, without Developer ID or Apple
notarization**. `curl` avoids the quarantine flag normally added by a browser;
it does not guarantee Gatekeeper acceptance. The installer changes no security
settings. Read the [installation guide](https://github.com/algorhythmia-lab/tape-01/blob/main/docs/install.md)
for paths, removal, browser downloads and the reusable technique.

This build adds the required macOS microphone permission description for
standalone audio input and includes the repository test portability fixes.
It supersedes beta.1.

## Start playing

Lower monitoring volume. Select **01 / magnetic pulse**, enable DRIVE/RUN, and
move FLUX/BIAS. Try **02 / cellular walls**. For **03 / forward tape**, supply
stereo audio, release BRAKE and set FLUX around .75. Generators use an audio
effect slot in a DAW; keep the track active even with silent input.

## Beta scope

This is an early testing release with incomplete production qualification.
Realtime/lifecycle work remains open; host-specific issues or audio dropouts
may occur. An extreme transform setting has a known DC-offset issue: keep
monitoring low and avoid high output gain. State recall saves parameters and
restarts empty tape; it does not save recorded audio. Broad DAW compatibility,
native Intel hardware, old macOS versions, accessibility and long soaks remain
unqualified. Windows, Max for Live and hardware builds are not included.

The source archive includes the first-party source, exact pinned JUCE/doctest
sources, fonts, license notices and offline build instructions. SYS 01 is
AGPL-3.0-only; bundled components retain their respective licenses.

[Report a problem](https://github.com/algorhythmia-lab/tape-01/issues/new/choose)
with the tag, system, host/format, rate/buffer, routing and reproduction steps.

## Assets

- `SYS-01-1.0.0-beta.2-macOS-universal.tar.gz`: app, plugins, guides and notices.
- `install-macos.sh`: version-pinned installer with the archive hash embedded.
- `SYS-01-1.0.0-beta.2-source.tar.gz`: corresponding source including dependencies.
- `manifest.json`: source revision, dependency pins and per-bundle file hashes.
- `SHA256SUMS.txt`: checksums for all assets above.
