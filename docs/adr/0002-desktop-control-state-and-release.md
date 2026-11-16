# ADR 0002 — SYS 01 desktop delivery contract

2026-10-10. status: product scope accepted by the owner; distribution qualification pending. supersedes contradictory software promises in the reference UI specification. retains ADR 0001's portable DSP boundary.

SYS 01 is the working public desktop name. `study 01` is the historical prototype; public `/sys-0` remains a route alias. tape 01 is the independent hardware twin. the full desktop release remains available at no charge. neither this implementation nor a version field constitutes release approval.

one stereo effect shell generates bytebeat/cellular with silent input and transforms an external stereo input. input-disabled stereo output is supported. separate instrument/FX identifiers are deferred until DAW routing qualification. stable identifiers are `org.algorhythmia.sys01`, manufacturer `Algr`, product `Sys1`; keep these stable once distributed.

VST3 on macOS/Windows and AU on macOS are the planned formats. standalone is a development harness. Max for Live follows separately; CLAP, AAX, public standalone and hardware qualification are deferred. supported OS/CPU/DAW versions are intentionally empty in the release manifest until exercised. locally compiling a binary establishes neither host compatibility nor an installed support promise.

native colors are generated from the shared brand token snapshot in `docs/design-tokens.json`; a contract check verifies generation and 4.5:1 normal-label contrast. desktop uses system fonts until a licensed condensed face is selected.

JUCE 8.0.12, pinned by full commit, supplies desktop lifecycle, format glue, vector controls and native accessibility interfaces. the dependency-free C++20 engine is retained. review the selected revision's `LICENSE.md`, bundled dependency notices and the applicable [JUCE legal terms](https://juce.com/legal/juce-8-licence/) before distribution. no proprietary or open-source license is assigned to pre-existing first-party material by this ADR. the rights owner's choice is a release gate.

## executable control contract

`libs/algorhythmia_host/include/algorhythmia/host/catalogue.hpp` owns stable IDs, defaults, normalized meanings and factory studies. desktop adapters and editor bind to it. the first four physical controls keep their engine enum meanings. `engine_state` is the three-domain selector; `subregister_state` is the shared binary STATE latch. switching domains does not remember independent STATE values. this explicitly supersedes descriptions of per-domain memory.

| capability | implementation or disposition |
|---|---|
| FLUX / BIAS / run / binary STATE | implemented; 10 ms audio ramps; direct numeric entry, default reset |
| direct domain selection | implemented, separate from binary STATE |
| raw DRIVE/STATE edges | shared scheduled-event contract; embedded tap/hold semantics retained |
| audible BIAS hold | freezes current ramp value; displacement measures requested dial from press |
| save during domain preview | embedded snapshot captures preview domain and frozen audible BIAS |
| direct brake | implemented independently of tap recognition; accessible desktop toggle |
| output trim | -60..+20 dB, default 0 dB; 10 ms gain ramp; output limiting |
| bypass | 10 ms dry/wet crossfade; dry input remains stereo; engine continues advancing |
| factory presets / reset | implemented; parameter-only deterministic restart; empty captured tape |
| host session state | schema 1 bounded binary, validation and checksum, coherent queued restore |
| MIDI CC 20/21/22/23 | sample-positioned FLUX/BIAS/run/domain performance events; not host parameter writes |
| undo/redo | desktop parameter edits via JUCE undo history; cannot recover captured audio |
| host sync / clock subdivisions | deferred; UI explicitly says free clock, no simulated phase |
| mix control | deferred; wet-only transform retained; bypass is recovery |
| captured audio / voices / phase persistence | deferred; never claimed by session recall |
| desktop long-hold save / file persistence | deferred; host session save is authoritative; embedded commit mailbox is not a desktop save protocol |
| physical chords / hardware clock and MIDI | hardware adapter milestone; no desktop support promise |
| reduced motion | editor has no decorative motion; register only reflects DSP telemetry |

implemented FLUX mappings remain bytebeat 500–8000 ticks/s, cellular 30–300 BPM, transform -2..+2x. the earlier hardware 20–360 BPM/1–2000 ms mappings are superseded for the shared engine. neither loop duration nor sample-rate-dependent storage is represented as a 2000 ms loop control.

## timing and ownership

one audio owner operates `host::Processor`; adapters pass immutable transactions and sample-positioned event spans. at most 256 events per block; ascending offsets in `[0, frames)`; same-offset ties preserve input order. a whole malformed/overflowing batch is rejected, with audio continuing from its last valid state. there is no unbounded retry, heap allocation, lock or I/O in the shared callback. simultaneous opposite raw edges have zero duration and are not a tap; an actual tap requires at least one intervening audio sample. UI brake edges are assigned successive sample positions when delivered together.

continuous event targets use step-at-offset semantics plus the existing 10 ms smoothing. no linear reconstruction of automation curves is claimed. smoothing and timing are distinct. discrete domain transitions retain the DSP's 10 ms crossfade; binary STATE retains its existing ramped musical transition.

When a native batch is rejected, the wrapper's already-published final host
targets are acknowledged but not applied to sound, including on following
blocks. This avoids replaying a partial rejected batch at offset zero. The
rejection counter is visible; a subsequent valid edit or reset restores an
explicit target. Invalid final VST3 values are not published to native state.

JUCE's stock VST3 wrapper keeps only the last ordinary parameter point per block. [ADR 0003](0003-positioned-vst3-integration.md) records the reviewed, checksum-pinned VST3 integration that preserves every point, with a loaded-binary test. AU retains its upstream wrapper and still requires offset qualification. no linear curve reconstruction is claimed; the accepted continuous contract is step-at-point plus smoothing.

restore calls are serialized outside audio and enqueue immutable states in eight usable SPSC slots. overflow rejects without altering host controls. an epoch protects the setter transaction: audio reads once and retains old controls if publication overlaps. a captured queue boundary excludes newer restores from an older snapshot. the callback drains at most eight accepted transactions and resets once to the newest state at that boundary. zero-length callbacks consume no controls or edges. parameter publication can be temporarily independent under ordinary DAW automation; preset/session restore is coherent.

session saving serializes declared host parameters, not transient MIDI modulation or live tape. MIDI parameters remain performance input; the primary dial displays host targets, the computation register displays actual DSP telemetry. mappings are fixed; learn/reassignment is deferred.

## recall and lifecycle

schema 1 stores seven normalized parameters as little-endian IEEE754 words, with `SYS1` magic, version and CRC32, exactly 40 bytes. decoding checks exact size, magic, version, checksum, finite ranges and discrete values before modifying the destination. there is no previously released schema to migrate: v0/future states fail safely. append-only evolution requires migration fixtures before release.

recall clears tape/voices/phases and initializes exactly the selected domain and controls; no default bytebeat leaks into transform initialization. reset/preset recall intentionally destroys captured performance. undo restores parameters only. this limitation is visible on RESET. graph prepare/sample-rate changes preserve desktop parameter targets and reset audio history deliberately. the Max prototype preserves history on same-rate graph rebuild and resets on rate change, applying its adapter controls on the next vector.

trim and bypass changes are smoothed. explicit reset/preset recall may introduce discontinuities and must pass sound qualification before release. no latency is reported for the creative 2048-frame wet delay. regeneration and feedback can sustain indefinitely; native adapters report an unbounded tail.

## release gates

`release/manifest.json` is the authority for qualification status. source version 1.0.0 is a development target. required manual gates include exact DAW/OS/CPU matrix, validators on actual binaries, UI/assistive technology, musician first-minute tasks, listening through transitions, a named minimum-machine/instance budget, editor-open profiling and realtime soak, rights/notices, signing/notarization and clean-machine installation. generated CI reports must stay attached to exact artifacts. distribution is not authorized merely by a passing core test suite.
