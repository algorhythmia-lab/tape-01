# Local build and packaging tools

- `./scripts/build.sh [build-directory] [CMake arguments...]` configures a Debug
  host build with tests, builds it, and runs CTest. For example:
  `./scripts/build.sh build-sanitize -DALGORHYTHMIA_ENABLE_SANITIZERS=ON`.
- `python3 scripts/check_format.py --check --clang-format clang-format-18`
  checks first-party C/C++ files. Substitute `--fix` to format them.
- `python3 scripts/check_wav.py build/apps/cli_harness/algorhythmia_render`
  runs the renderer in a temporary directory and checks the WAV contract and
  output error handling; CTest runs this automatically.
- `python3 scripts/check_rt_symbols.py --nm nm build/libs/algorhythmia_dsp/libalgorhythmia_dsp.a`
  rejects direct allocator, blocking and I/O references in the DSP archive.
  CTest runs this on Unix alongside the C++ allocation hooks. It supplements
  source review and sanitizers; it does not prove bounded callback timing or
  audit the internals of arbitrary external libraries.
- `python3 scripts/package_source.py` reads the canonical `VERSION` and creates
  `dist/algorhythmia-source-1.0.0.tar.gz`. Use `--list` to inspect the file list,
  or `--output /path/to/new-archive.tar.gz` to choose another destination.

Source packaging requires Python 3.9+ and no Git checkout. It preserves the
first-party source hierarchy and includes a SHA-256 manifest. Fixed archive
metadata makes identical source files produce identical archives. Existing
outputs are never overwritten; source symlinks are rejected. The archive uses
an explicit first-party path and source-extension allowlist, excluding strategy
files, builds, generated WAVs/binaries, and conventional SDK/vendor directories.
Keep local SDKs outside the repository as documented by the adapter READMEs.

`check_max_contract.py` verifies patch-to-selector wiring; `test_source_package.py`
checks byte reproducibility and all export hashes. CTest also compares
`docs/parameters.md` with the compiled `host_catalogue` executable. Regenerate
that page by running `host_catalogue > docs/parameters.md` after a catalogue edit.

`collect_diagnostics.py --output report.json --host "DAW version"` writes local
version/OS/architecture diagnostics without project/audio data. Optionally use
`--binary` to attach the SHA-256 of an explicitly selected plugin executable.

`run_clang_analyzer.py build/desktop/compile_commands.json --clang clang++`
analyzes first-party library, renderer and native adapter translation units.
Upstream includes are treated as system headers; this is not dependency analysis
or a replacement for the whole-format realtime audit.

`package_candidate.py --artifacts build/desktop/apps/desktop/sys01_artefacts/Release
--reports /path/to/reports --output dist/new-development-candidate` assembles
native bundles, local evidence, instructions, revision metadata and SHA-256
hashes. It labels every output unqualified and refuses an existing destination.
It does not sign, notarize or publish a release. Keep reports tied to the exact
executables being packaged; see `docs/implementation-status.md`.

This is a source snapshot. Packaging a distributable `.amxd`, signed Max
external, or hardware firmware release requires the corresponding SDK build,
host/hardware validation, assets, and release/legal decisions. The script
includes an existing top-level license or notice if supplied; it does not choose
a license or grant redistribution rights.
# Public macOS beta packaging

`package_public_release.py` packages the clean committed checkout as a universal
macOS public beta, with full pinned dependency source, bundle file hashes,
SHA-256 checksums and a generated version-pinned installer. It reads the public
scope from `release/public-beta.json` and refuses a dirty checkout, mismatched
dependencies, non-universal binaries or invalid bundle signatures.

```sh
python3 scripts/package_public_release.py --build build/desktop --output dist/public-beta \
  --juce-source build/desktop/_deps/juce-src \
  --doctest-source build/desktop/_deps/doctest-src
python3 scripts/test_macos_installer.py dist/public-beta
```

The installer tests use disposable prefixes only, including a real partial
update failure and rollback. Release notes are in
`release/notes-v1.0.0-beta.1.md`. For another project, adapt the versioned asset
URL, identifiers, bundle paths and release metadata; keep checksum/signature
verification and rollback. [The installation guide](../docs/install.md) explains
the macOS quarantine behavior and its limits.
