# shared dsp architecture

`algorhythmia::dsp` is a dependency-free static c++20 library. a compile-time scene concept requires initialization, stereo processing, and an eight-bit telemetry word. a fixed tuple owns bytebeat, cellular fm, and live transform; a compile-time fold dispatches their render calls. there are no virtual calls, scene allocations, exceptions, or rtti. only scenes with nonzero crossfade gains execute.

## ownership and lifecycle

construct the engine before audio starts and initialize it exclusively. supported rates span 8–192 khz; invalid rates use 48 khz. initialization restores controls and clears history; it must not overlap rendering, setters, telemetry reads, or commit consumption. firmware keeps the engine in static storage, because its stereo tape uses 256 kib. desktop adapters may allocate an engine during lifecycle setup.

one audio thread owns all scene state, smoothing, gesture timers, and rendering. for a nonempty callback both output arrays must be valid and disjoint. optional input arrays contain the requested sample count; a missing channel supplies silence. corresponding-channel in-place operation is supported because both inputs are loaded before either output is written. partial or cross-channel aliases are outside the contract. zero frames touches no pointers or engine state. before initialization, valid outputs receive silence.

## parameter and event publication

each control and raw button uses a native `std::atomic<std::uint32_t>` mailbox. a static assertion rejects platforms without always-lock-free 32-bit atomics. relaxed loads/stores are sufficient because each word independently carries its whole value. finite parameters clamp to [0,1]; nonfinite values and unknown identifiers are ignored. continuous controls, sub-register changes, transport gain, and domain gains use bounded 10 ms ramps. recursive filters, envelopes, and final outputs snap tiny values above the denormal range independently of host ftz settings.

mailboxes are sampled once per process call. adapters must split blocks at midi or automation event offsets to provide sample-accurate delivery. intermediate writes can coalesce; parameters do not constitute an atomic preset transaction. raw buttons are distinct from latched host controls. see the [scene contract](scenes.md) for timing and mapping.

telemetry is a coherent packed atomic snapshot. commit publication uses a separate packed atomic word containing two 10-bit dials, domain, sub-register, transport, and an eight-bit generation. exactly one non-audio consumer owns its consumption cursor. latest snapshots coalesce; if 256 identical requests occur without consumption, generation wrap can hide a request. persistence code must not run on the audio owner. device integration must also prevent flash programming from stalling xip audio code or shared buses.

## stereo tape and realtime boundary

the engine owns a single stereo float history; capture/freeze reuses it without copying or clearing storage. hermite reads use four adjacent periodic samples and bounded head wrapping. every scene uses independent stereo signal state. the engine clamps mixed output to [-1,1] and sanitizes optional input before tape storage. inactive scene state suspends after its finite fade; transport pause stops computational clocks after its fade while recursive audio tails decay.

rendering and gesture handling perform no allocation, lock, system i/o, storage write, or sdk call. allocation hooks, archive symbol checks, concurrent setters/readers, sanitizers, and offline stress verify the host boundary. these checks do not certify a hardware deadline. device release needs the actual codec/pio/dma implementation, linker map, stack watermarks, and worst-case timing at the 64-frame 1.333 ms deadline.

## adapters

the cli renders planar float audio to stereo pcm16 wav outside the core. the max adapter converts arbitrary double vectors through fixed 64-frame arrays and forwards independently published control snapshots. its messages use `flux`, `bias`, `drive`, `state`, and `scene`. the pico stub owns static engine and i/o state and renders one probe block; its codec driver and flash worker await board integration.

`algorhythmia::host` owns the canonical desktop catalogue, bounded ordered events, trim/bypass and validated state encoding. one audio owner applies coherent restore transactions and splits rendering at event offsets. `apps/desktop` binds JUCE VST3/AU/development standalone and a native vector editor. the VST3 integration preserves ordinary automation points; AU timing and whole-format realtime behavior remain qualification gates. see [ADR 0002](adr/0002-desktop-control-state-and-release.md), [ADR 0003](adr/0003-positioned-vst3-integration.md) and [implementation status](implementation-status.md).
