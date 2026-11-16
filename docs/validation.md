# turing tape dsp validation // 2026-10-08

This is the historical DSP validation record. Current desktop implementation
and evidence are recorded in [implementation status](implementation-status.md).
The target limits and continuation state below describe the earlier session.

native platform: macos arm64, apple clang 21, c++20, strict warnings as errors, fast math disabled and floating-point contraction disabled. the dsp archive compiles without exceptions or rtti. tests use the existing pinned doctest checkout; optional build and analysis tools were cached outside the repository.

| check | result |
|---|---|
| release build and ctest | all six groups passed |
| addresssanitizer + undefinedbehaviorsanitizer | all six groups passed |
| unit suite | 27 cases, 662569 assertions passed |
| heap allocation/deallocation hooks | zero heap traffic during init, setters, raw gestures, telemetry, commit consumption, and stereo rendering |
| forbidden archive symbols | no direct allocator, blocking, or system i/o references |
| concurrent controls and readers | passed native stress and a separate threadsanitizer run |
| clang-format 18.1.8 | all first-party c/c++ sources passed |
| clang-tidy 22.1.8 | all native targets passed with project warnings as errors |
| wav smoke | stereo pcm16, 48000 hz, five seconds; header and length checks passed |
| firmware memory contract | 262144-byte tape, 262824-byte engine including tape, 3072-byte i/o, 265896-byte aggregate |

ctest groups cover wav output, firmware contract, unit behavior, realtime allocation/concurrency, runtime symbols, and a short stress run. unit coverage includes independent scalar verification of cellular rules, stereo azimuth and channel isolation, capstan braking, transport pause, deterministic reset and block partitioning, in-place processing, cubic seams and large read positions, reverse/freeze/scrub, feedback bounds, nonfinite input, supported rates and dial extremes, preview symmetry, midi mapping, and exact two-second commit timing.

## eight-hour offline simulations

each domain processed 1382400000 stereo frames at 48000 hz, totaling 24 simulated hours and 4147200000 stereo frames. these were full per-sample renders, not skipped-time state approximations. the script supplied stereo input and exercised forward/reverse rates, zero-hz freeze, bank steps, 120% feedback, transport stops, sustained brakes, long STATE holds, and nonfinite/subnormal input. every output sample was checked for finiteness, normal-or-zero classification, and magnitude no greater than one. all domains passed.

| domain | simulated hours | wall seconds | peak | rms |
|---|---:|---:|---:|---:|
| bytebeat | 8 | 65.683 | 0.867047 | 0.292369 |
| cellular fm | 8 | 99.827 | 0.711252 | 0.126479 |
| live transform | 8 | 104.291 | 0.892512 | 0.294233 |

raw results are available locally in `build-turing/stress-eight-hours.txt`. the full simulations ran in the release build. sanitizer ctest runs include the short stress mode, allocation/concurrency tests, and unit bounds. the eight-hour run is not a proof of arbitrary-input stability or audio quality; it records the tested scenarios. the tape helper subsequently gained safe modulo handling for arbitrary finite external read positions; normal in-range engine reads are unchanged and the final release/sanitizer suite covers that path.

## host throughput

a separate benchmark rendered 6144000 stereo frames per domain with maximum BIAS and recorded stereo input. final local measurements:

| domain | ns per stereo frame | multiple of realtime |
|---|---:|---:|
| bytebeat | 47.4 | 439.8 |
| cellular fm | 77.6 | 268.6 |
| live transform | 66.7 | 312.2 |

these are host throughput measurements, not worst-case callback deadlines or rp2350 profiling. scene transitions can execute multiple scenes; hardware needs measured headroom for transitions, codec transport, and persistence.

## reproduction

run from the `sys-01` directory. provide a local doctest source path through cmake's fetchcontent override if offline.

```sh
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release -DALGORHYTHMIA_BUILD_BENCHMARKS=ON
cmake --build build-release --parallel
ctest --test-dir build-release --output-on-failure
cmake --build build-release --target stress-eight-hours
./build-release/libs/algorhythmia_dsp/tests/algorhythmia_dsp_benchmark
cmake -S . -B build-sanitize -DCMAKE_BUILD_TYPE=Debug -DALGORHYTHMIA_ENABLE_SANITIZERS=ON -DALGORHYTHMIA_BUILD_BENCHMARKS=ON
cmake --build build-sanitize --parallel
ctest --test-dir build-sanitize --output-on-failure
```

extra race verification with a supporting native clang:

```sh
cmake -S . -B build-tsan -DCMAKE_BUILD_TYPE=Debug -DALGORHYTHMIA_ENABLE_TSAN=ON
cmake --build build-tsan --parallel
ctest --test-dir build-tsan -R dsp_realtime_tests --output-on-failure
```

clang-tidy 18 cannot parse this machine's newer macos sdk headers. the local analyzer was upgraded to 22.1.8 with explicit sdk and libc++ include paths; the repository's linux ci retains clang 18 with its matching platform headers. formatting remains pinned to 18.1.8.

## target limits

the current session verified the pure dsp, cli, host firmware contract, and native tests. the max adapter and starter patch have updated control messages; a previous bootstrap compiled the arm64 external, but its sdk checkout is no longer available locally and the current changes have not been rebuilt or loaded in max. vst/au packaging, host automation adapters, preset recall, bypass, and ui binding remain external integration work.

no rp2350 board, pico sdk, or arm bare-metal toolchain was supplied. device codec/pio/dma transport, linker map, stack watermarks, and worst-case render timing remain unverified. the core publishes asynchronous commit snapshots; the board worker must implement persistence and prevent flash/xip stalls. cubic interpolation reduces imaging but does not guarantee alias-free full-band pitch conversion. magnetic hysteresis is a bounded phenomenological memory model. see the [scene contract](scenes.md) for exact behaviors.

## continuation state

phases 1–5 of the shared dsp implementation are complete at the boundaries described above. the next development work belongs to the host or board adapter: map the four controls, deliver event-offset block splits, render telemetry, consume commit snapshots outside audio, and profile the actual device. the workspace has no git commits and all project directories were already untracked; this session created no commit or publication.
