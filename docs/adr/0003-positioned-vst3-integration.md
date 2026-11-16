# ADR 0003 — preserve native VST3 automation points

2026-10-10. status: implemented and checked against the loaded macOS VST3; other platform/DAW qualification remains pending.

ADR 0002 established that stock JUCE 8.0.12 discards ordinary intermediate VST3 points and applies each parameter's final value at the block boundary. the shared scheduler alone could not repair that format boundary. SYS 01 uses a small explicit build-only integration in `scripts/native_vst3_adapter.py`.

the script hashes the upstream translation unit against the reviewed commit, checks unique integration anchors, and generates a translation unit inside the build tree, retaining upstream code and notices. it does not edit the supplied/fetched checkout. CMake excludes only the stock VST3 translation unit in this directory and compiles the reviewed generated one. any upstream drift fails configuration; updating JUCE requires reviewing these anchors and rerunning binary conformance and validators. AU and standalone retain their upstream format wrappers.

before each VST3 block, the integration clears its fixed ingress buffer. it forwards every ordinary catalogue parameter point (index, offset, normalized target) into the audio-owned adapter. the framework still updates the final host parameter value, preserving its editor/controller behavior. synthetic program parameters remain lifecycle operations, not position-automatable musical controls. zero-block flushes update host targets and are applied at the next nonempty block.

native ingress validates offsets, finite ranges and capacity; discrete points quantize to the catalogue's steps. native points suppress duplicate block-boundary application of those host targets, including bypass. the shared stable scheduler merges them with positioned MIDI and queued UI edges. parameter ties preserve format queue/point order. continuous curves use step-at-point plus 10 ms smoothing, not implicit interpolation between points. overflow rejects the musical batch and renders from the previous state; final host targets are acknowledged without following-block replay. queued UI edges are retained until a batch is accepted.

`VST3ContractTests.cpp` loads the built module through Steinberg's actual factory and `IAudioProcessor::process`. it supplies multiple DRIVE/FLUX points, BIAS/domain/bypass points, checks every rendered sample against independently scheduled DSP, verifies the next block retains final values, and checks overflow/recovery. `ALGORHYTHMIA_POSITIONED_VST3=OFF` exists only to reproduce the original defect; this same loaded-binary test must fail in that configuration. production/default configuration is ON.

this closes VST3 point delivery in the tested binary; it does not establish AU offset handling, real DAW automation interpolation, cross-architecture exactness, or arbitrary host behavior. AU precision remains a release gate.

## remaining whole-format realtime gate

our shared DSP/control callback has allocation hooks and forbidden-symbol checks. JUCE's broader format glue still has callback/lifecycle synchronization, parameter-listener locks and MIDI-buffer infrastructure. those upstream paths are outside that proof. a core allocation pass or pluginval result does not establish zero locks/allocations across the entire native wrapper. retain this explicit release blocker until auditing/replacing those paths and profiling the actual binary under the declared host workload. do not remove lifecycle synchronization without a documented replacement ownership proof.

Rejected batches acknowledge final host targets without applying them to DSP, so
following blocks cannot replay a partial rejected batch as scalar changes. The
loaded fixture covers overflow across queues (including bypass) and nonfinite
last points. Rejected host targets remain visible; use a subsequent valid edit
or coherent reset/preset to recover.


## Sprint 01 native ownership extension / 2026-10-14

The original delivery evidence above is historical. Sprint 01 bounds total VST3
parameter-queue traversal to 256 points across all ordinary/synthetic queues,
replaces generic input MIDI conversion with fixed audio-owned CC20–23 ingress,
and bounds IEventList traversal. Ignored SysEx/notes are not copied. The merged
musical batch still caps at 256 with stable ties and whole-batch rejection.

AU now also has a checksum-pinned, build-only integration in
`scripts/native_au_adapter.py` (reviewed AU source SHA-256
`16a5246f380a93c1f33f9afeef810702748039720ea094208765c0c8c916fe86`).
It preallocates both alternating MIDI buffers for 256 short messages, bounds
legacy/UMP ingress and rejects whole overflowing batches, drops undeclared SysEx
before copy/dispatch, and preserves pending ingress on zero Render calls. AU
parameter scheduling remains upstream/block-boundary: this is not offset parity.
Upstream dependency files/notices remain intact. Source drift fails configuration;
review both wrappers and rerun their loaded fixtures on dependency upgrades.

No synchronization lock was removed. [Native ownership](../release-1.0.0/native-ownership.md)
records the actual lifecycle, parameter-listener, restore, editor and host-owned
paths and replacement prerequisites. [Sprint 01](../release-1.0.0/sprint-01.md)
records whole-wrapper measurements and remaining blockers. Passing native
conformance does not assert zero locks, a callback bound or DAW qualification.


## Sprint 02 format qualification / 2026-10-14

The loaded VST3 fixture now asserts component/controller and all seven parameter
IDs, exercises every integer offset in 1/7/63/64/65/511/4096-frame blocks at all
six declared rates, exact discrete half-step thresholds, stable ties, all fixed
CCs, 256-event capacity, cross-queue and merged MIDI overflow, invalid/nonfinite
intermediate/final points and following-block non-replay. The checksum-pinned
VST3 adaptation is unchanged. A newly built stock-wrapper negative control
fails the same fixture. macOS x86_64 execution here uses Rosetta, not a native
Intel or minimum-OS machine.

The independent AU fixture demonstrates that both direct offset setters and
scheduled immediate events collapse at offset zero, while positioned MIDI
works. AU strict timing mode fails; default CTest only characterizes the gap.
[The explicit scope decision](../release-1.0.0/au-timing-scope-decision.md) awaits
owner acceptance without changing ADR 0002. Validator and partial named-DAW
results are bound to binaries in [Sprint 02](../release-1.0.0/sprint-02.md).
