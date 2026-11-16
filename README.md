# SYS 01

A free cassette-shaped instrument and stereo effect from algorhythmia.
Three deterministic sound domains: **magnetic bytebeat**, **cellular FM**, and
**live tape displacement**.

**First public beta: v1.0.0-beta.1 · macOS 12+ · Apple silicon + Intel**

## Download and test

The release includes a standalone app, VST3 and Audio Unit. Close SYS 01 and
your DAW, then paste this in Terminal:

```sh
sys01_tmp="$(mktemp -d)" &&
curl --fail --location --proto '=https' --tlsv1.2 \
  https://github.com/algorhythmia-lab/tape-01/releases/download/v1.0.0-beta.1/install-macos.sh \
  --output "$sys01_tmp/install-macos.sh" &&
/bin/bash "$sys01_tmp/install-macos.sh"
```

The installer verifies the archive's pinned SHA-256 and bundle signatures, then
installs into your user Applications and Audio Plug-Ins folders. No `sudo` is
required. Add `--dry-run` to the last line to verify without installing.

[All downloads and release notes](https://github.com/algorhythmia-lab/tape-01/releases/tag/v1.0.0-beta.1)
· [Installation, removal and how curl helps](docs/install.md)
· [Report a problem](https://github.com/algorhythmia-lab/tape-01/issues/new/choose)

These are **ad-hoc signed, unnotarized beta builds**. Terminal downloads avoid
the browser's quarantine flag; they do not guarantee Gatekeeper acceptance or
replace Developer ID signing and notarization. See the installation guide for
the mechanism and how to reuse the pattern for other projects.

## First minute

Lower monitoring volume. Select **01 / magnetic pulse**, enable DRIVE/RUN,
and move FLUX and BIAS. Try **02 / cellular walls**. For **03 / forward tape**,
feed stereo input, enable RUN, release BRAKE and set FLUX around .75.
The VST3 and AU use a stereo audio effect slot; some DAWs need an active track
to process a generator with silent input.

FLUX controls time/rate; BIAS changes equation or character. STATE selects an
alternate register. The register window toggles BRAKE. BYPASS returns dry input.
Preset recall, RESET, session reopen and sample-rate preparation restore
parameters with empty tape and restarted phases. Record the output to preserve
a performance. Read the [musical guide](docs/musical-guide.md) for details.

## Testing scope

This first release is for public testing. Whole-wrapper realtime/lifecycle work,
broad host compatibility, native Intel hardware, old macOS versions,
accessibility, musician acceptance and long soaks remain unqualified. An extreme
transform setting has a recorded DC-offset issue; keep monitoring low and avoid
high output gain. See [the release scope](release/public-beta.json) and
[qualification record](release/manifest.json). The code reports base version
`1.0.0`; use the full release tag `v1.0.0-beta.1` when reporting an issue.

Windows, Max for Live and hardware binaries are not part of this beta. Their
source adapters remain in the repository for development.

## Build from source

Use CMake 3.22+, a C++20 compiler and Python 3.9+. Desktop builds fetch the exact
JUCE and doctest revisions declared in [dependencies.lock.json](cmake/dependencies.lock.json).
The portable DSP does not depend on JUCE.

```sh
cmake --preset desktop
cmake --build --preset desktop --parallel 4
ctest --preset desktop
```

macOS bundles appear under `build/desktop/apps/desktop/sys01_artefacts/Release`.
For a universal build, add `'-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64'
-DCMAKE_OSX_DEPLOYMENT_TARGET=12.0` to configuration.

The release's **source archive includes the complete pinned dependencies**.
After extracting it, configure offline with:

```sh
cmake -S . -B build/desktop -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_TESTING=ON -DALGORHYTHMIA_BUILD_DESKTOP=ON \
  -DFETCHCONTENT_SOURCE_DIR_JUCE="$PWD/dependencies/JUCE" \
  -DFETCHCONTENT_SOURCE_DIR_DOCTEST="$PWD/dependencies/doctest"
cmake --build build/desktop --parallel 4
ctest --test-dir build/desktop --output-on-failure
```

For the portable library and WAV renderer, omit desktop configuration:

```sh
cmake -S . -B build/core -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build/core --parallel 4
ctest --test-dir build/core --output-on-failure
./build/core/apps/cli_harness/algorhythmia_render output.wav
```

[Desktop development guide](docs/desktop-guide.md) · [Architecture](docs/architecture.md)
· [Source and release scripts](scripts/README.md)

## License

SYS 01 first-party source is **AGPL-3.0-only**. The instrument is free of charge.
JUCE and other bundled components retain their respective licenses. See
[LICENSE](LICENSE), [NOTICE](NOTICE) and [dependency notices](licenses/).
Internal sprint records describe their historical development and qualification
scope; the public beta scope above applies to this release.
