# Max for Live scaffold

`external/algorhythmia_tilde.cpp` wraps the same `algorhythmia::dsp` static library as the CLI and firmware. The Max SDK stays outside the DSP boundary. This repository supplies an editable audio-effect patch, not a finished `.amxd` release.

## Build the external

Supply an official [max-sdk-base checkout](https://github.com/Cycling74/max-sdk-base) outside this repository. The native macOS build was compile-checked with SDK commit `c03a2922a2a8ff149165a1e2ea134e9321d6e202`; loading in Max/Live and the Windows build still need validation.

```sh
cmake -S . -B build-max -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF \
  -DALGORHYTHMIA_BUILD_MAX=ON \
  -DALGORHYTHMIA_MAX_SDK_BASE=/absolute/path/to/max-sdk-base
cmake --build build-max --parallel
```

The macOS bundle appears at `build-max/max/externals/algorhythmia~.mxo`. Windows produces `algorhythmia~.mxe64` (a multi-configuration generator may append `Release/` to the output directory). Build for the architecture used by the host; this bootstrap's local build is arm64, not a signed universal distribution. You can also configure `apps/max_for_live` directly as the source directory.

## Patch and control contract

Add the external's directory to Max's search path. Create a Max Audio Effect in Live, open its editor, and copy the contents of `patchers/study01.maxpat` into the device patch. The `plugin~` and `plugout~` objects route stereo audio through `algorhythmia~`. enable DRIVE to hear the selected stereo domain. scene 0 is bytebeat, scene 1 is cellular fm, and scene 2 transforms live stereo input. The plain number/toggle controls are integration controls; Live automation exposure and persistent parameter recall are still pending.

| Message | Values | Purpose |
|---|---|---|
| `flux <float>` | 0–1 | scene clock or capstan rate |
| `bias <float>` | 0–1 | scene morph, rule, saturation, feedback |
| `drive <float>` | threshold 0.5 | Muted/running output |
| `state <float>` | threshold 0.5 | shared binary sub-register |
| `scene <int>` | 0, 1, 2 | Bytebeat, Cellular FM, Live Transform |

Max's double signal vectors convert through fixed 64-frame float buffers, including nonmultiples of 64. Message handlers publish atomic values; the audio callback forwards a snapshot into Engine. Memory allocation and destruction occur only during object lifecycle, and `dsp64` initializes the core while the object's perform routine is stopped. same-rate DSP graph rebuilds preserve audio history; rate changes reset it deliberately, with adapter controls applied on the next vector. See the [official MSP lifecycle](https://sdk.cdn.cycling74.com/max-sdk-7.1.0/chapter_msp_anatomy.html).

The [architecture](../../docs/architecture.md) explains threading and pointer ownership. `ui/` holds future visual assets and `art/j-card/` holds future cassette artwork. Before release, implement Live parameter names/ranges, save/restore behavior, host bypass policy and scene UI; verify sample rates, vector sizes, transport restarts, offline rendering, multiple instances, packaging and host loading. Save and package the tested device from Max for Live as `.amxd`.
