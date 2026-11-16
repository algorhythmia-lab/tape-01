# Sprint 02 / AU parameter timing scope decision

2026-10-14. **PROPOSED; owner acceptance pending. G02 BLOCKED for AU.**
The accepted ADR 0002 contract is unchanged. AU remains a planned format; this
record neither removes it nor grants a timing exception.

The independent `sys01_au_offsets` fixture loads the exact component factory and
uses AudioUnitSetParameter with nonzero buffer offsets, AudioUnitScheduleParameters
with immediate events, MusicDeviceMIDIEvent, and AudioUnitRender. It compares every
sample in both channels with separately scheduled host::Processor references.
DRIVE points at samples 9, 61 and 91 are deliberately audible. An independent
final-target-at-zero reference distinguishes a collapsed block from positioned
steps. A following block checks replay. All seven exported AU IDs/names are
asserted as literal values. No calls to PluginProcessor's native ingress create
this evidence.

On both locally built arm64 and x86_64 (Rosetta execution) components, ordinary
setters and scheduled immediate points returned noErr but matched only the
collapsed reference. MIDI CC22 matched positioned steps. Strict fixture mode
`--require-positioned` exited 1 on each architecture. Default CTest mode is
**characterization**, and its pass is not AU timing conformance. Apple's strict
auval structural pass on arm64 did not detect this product-specific defect.
The exact binary bindings and commands are in [Sprint 02](sprint-02.md).

Inspected JUCE 8.0.12 paths (dependency commit and source hash remain pinned):

- `juce_audio_plugin_client_AU_1.mm`, SetParameter: catalogue values are published
  immediately; `inBufferOffsetInFrames` is unused on that path.
- The same wrapper's CanScheduleParameters returns false.
- `AU/AudioUnitSDK/AUBase.cpp`, ScheduleParameter: immediate events call that
  setter in input order; ramp events are retained only when scheduling is enabled.
- SYS 01's generated AU changes bound MIDI ingress. They do not adapt parameters.

Recommended disposition: keep AU as an internal development artifact until a
reviewed bounded parameter ingress preserves offsets, stable ties, discrete
quantization and whole-batch rejection without next-block replay. Keep the
shared 256-event cap across parameter/MIDI/UI ingress and define zero-render,
same-timestamp AU cache and lifecycle/restore ownership before changing the
wrapper. Do not merely enable upstream scheduling: the SDK event vector and
split-render machinery are outside the existing realtime proof. Preserve the
checksum gate, upstream source/notices, IDs and state schema. A parameter ingress
change must rerun strict offset fixtures, hostile/overflow fixtures, auval and
both named DAWs, invalidating current AU binary evidence.

The alternative is an explicit owner-approved AU timing exception or revised
format scope in ADR 0002 and the manifest, with the changed user promise stated
openly. No exception has been accepted. Engineering has prepared this decision
instead of claiming parity. VST3 timing remains qualified only at its exercised
loaded-binary boundary; real DAW automation remains a separate open check.
# Sprint 09 current checkpoint / 2026-10-18

The earlier failures below remain historical and sealed. No AU timing exception
was accepted. Sprint 09 now implements bounded direct/scheduled immediate ingress
and pinned SDK dispatch. Local arm64 Release/ASan/UBSan/TSan runs pass 1,249
comparisons. The added lifecycle check reproduced accepted pending FLUX lost
across Cleanup/Initialize, then a legal lifecycle target flush repaired it; the
latest Release strict run passes 1,251 comparisons. Publication versus concurrent
restore ownership remains unresolved. Sprint 09 and G02 remain incomplete.
See [ownership checkpoint](../release-plan-1.0.0/au-ingress-ownership-session-01.md)
and [session handoff](../release-plan-1.0.0/handoffs/sprint-09-session-01.md).

