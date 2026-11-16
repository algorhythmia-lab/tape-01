# turing tape scene contract

the software delivery/recall decisions are recorded in [ADR 0002](adr/0002-desktop-control-state-and-release.md).

sys-01 and tape-01 share a static c++20 stereo engine. FLUX and BIAS default to 0.5; DRIVE and STATE default to zero; domain 00 starts selected. every scene owns its audio state. inactive scenes suspend after a 10 ms crossfade and resume their existing state when selected again. initialization clears all state and tape storage.

| domain | FLUX | BIAS | DRIVE hold | STATE tap | live word |
|---|---|---|---|---|---|
| 00 // bytebeat | 500–8000 integer ticks/s | three-expression morph, 0–64 tick stereo azimuth, magnetic drive | inertial capstan brake to sub-audio | alternate equation bank | low byte of the nearest active equation |
| 01 // cellular fm | 30–300 bpm, four generations/beat | rules 30, 90, 110, 150 in equal dial quarters, fm character | stop evolution; let voice envelopes decay | pentatonic octave register | eight-bit rule |
| 02 // live transform | -2x reverse to +2x forward; center 0 hz detent | tape wear, saturation, 0–120% recursion | stop writing captured history; FLUX scrubs playback | reverse direction through a smoothed zero-speed crossing | bilateral 4+4 output peak ladder |

## controls and gestures

normalized host parameters use the requested FLUX, BIAS, DRIVE, STATE enum members. DRIVE is latched run/pause. STATE is a single shared binary sub-register; domain changes keep its value. explicit scene selection carries the three-domain host register; midi cc 23 maps 0–42 to bytebeat, 43–85 to cellular, and 86–127 to transform. midi cc 20/21 map continuous FLUX/BIAS and cc 22 maps stop/play at 64. adapters map `param_flux`, `param_bias`, `transport_drive`, and `engine_state` to these boundaries. the three-domain `engine_state` host value must use scene selection, rather than the binary sub-register.

physical buttons use the separate raw-button API. a release before 300 ms toggles run/pause or steps the sub-register; a DRIVE hold at 300 ms engages the momentary brake and release restores playback without toggling the latch. holding STATE freezes the current audible BIAS ramp value, rather than its requested target. after 300 ms, rotating BIAS by at least 2.5% enters safe domain preview; release selects the preview with a 10 ms crossfade. returning the dial to its initial value still updates selection. ordinary BIAS control resumes on release.

holding STATE for exactly 2.0 s publishes one coherent commit snapshot (preview domain when selecting, frozen audible BIAS) without a flash call, lock, allocation, or audio restart. the non-audio worker consumes the latest snapshot; dials are quantized to 10 bits and repeated unconsumed requests coalesce. a long hold emits only one request until released. real flash integration must keep audio code and data available during the device's flash write; a mailbox alone cannot solve xip stalls.

button edges and automation are sampled at process boundaries. adapters must split a host block at event offsets for sample-accurate automation and must deliver each button edge. a latest-value mailbox does not queue presses that occur entirely between callbacks.

## domain 00

six unsigned equations form two banks of three. BIAS morphs adjacent decoded expressions while the right head evaluates a fractional offset from the left tick. independent channel magnetic memory feeds tanh saturation, a stable tape-head one-pole low-pass, and a sample-rate invariant 20 hz dc blocker. output headroom and denormal snapping bound recursive tails. the 160 ms capstan inertia approaches 0.0001 velocity during braking and recovers on release. transport pause stops tick evolution after the gain fade ends.

bytebeat is intentionally aliased mathematical synthesis. magnetic filtering shapes its discontinuities; it does not make its equations band-limited. telemetry reports an actual low-byte calculation word from the nearest expression, because a continuously morphed and saturated stereo signal has no single integer word.

## domain 01

a 32-cell periodic elementary automaton evolves with a bounded bit-parallel rule. both 0→1 and 1→0 cell flips excite barkhausen wall events. four adjacent cells share one of eight two-operator fm voices, with bounded event accumulation and fixed stereo pans. phases wrap every sample, envelopes decay to an exact snapped zero, and modulation is constrained at the lowest supported sample rate. a deterministic single-wall seed starts playback and reseeds an extinct lattice. normalized voice summation, dc rejection, and tanh limiting contain simultaneous events. rule changes apply to the next generation without resetting the lattice.

## domain 02

one 32768-frame stereo float circular history supports forward/reverse playback, a +/-0.5% center freeze detent, and four-point hermite interpolation. the head starts 2048 samples behind the write position. at +1x it behaves as a 2048-frame wet delay; the full captured loop covers about 0.683 s at 48 khz. all positions wrap in bounded arithmetic.

DRIVE hold freezes writing while the read head loops or scrubs with FLUX. zero capstan speed freezes both heads. playback dc rejection allows a stationary head to decay to silence instead of emitting a persistent dc level. independent channel wear filters and tanh limiters shape playback; input is finite-checked and clamped, feedback is dc-blocked and limited before storage. 120% feedback is a bounded regenerative region and can sustain a seeded loop; exact digital silence remains silent. the output is wet-only.

cubic interpolation reduces imaging and smooths fractional pitch motion. it cannot provide alias-free conversion for arbitrary full-band input at 2x speed; a future band-limited resampler would be needed for that guarantee. neither a constant-speed circular splice nor a frozen loop promises seamless content at its storage boundary.

## telemetry

bit 7 is the leftmost of eight micro-leds. domain preview words are `0xc0` (amber `#FFB000`), `0x18` (green `#00FF66`), and `0x03` (cyan `#54E5FF`), matching the locked 3+2+3 symmetry. live transform's left nibble grows from the center toward the left edge and the right nibble mirrors it. peak release is 150 ms. the audio owner publishes domain, word, preview, and transport status in one native 32-bit atomic word.
