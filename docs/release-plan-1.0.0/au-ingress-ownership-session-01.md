# Sprint 09 AU ingress ownership checkpoint

2026-10-18. Implemented and locally exercised, **ownership exit incomplete**.
This augments the historical native ownership document; it does not replace or
close G01, G02, S07-P1-01 or S07-P1-03.

The supplied JUCE 8.0.12 tree is unchanged. `native_au_adapter.py` generates
`Sys01AU.mm`; `native_au_dispatch.py` generates `Sys01AUSDK.mm` and
`Sys01AUDispatch.cpp`. All three upstream inputs are individually checksum
gated, and CMake verifies the complete dependency tree. Original notices remain
in generated files. SDK dispatch prevalidation is removed only in this generated
AU compilation: it previously scanned arbitrary scheduled arrays and rejected
NaN before the wrapper could reject the pending musical epoch.

## Producers, capture and bounds

Ordinary setters, scheduled immediate calls, legacy MIDI and UMP can originate
on different host threads. They serialize with the **retained** `incomingMidiLock`;
this is MPSC under a mutex, not SPSC or a realtime-safe lock-free claim. The new
paths add use of that existing lock and no new lock object. The audio consumer
captures at most 256 parameter points plus the bounded alternating MIDI buffer
under the same lock. Parameter targets, masks and monotonically increasing
per-instance publication serials are copied with the epoch. New producers write
the next epoch; captured storage is an audio-stack value with no reclamation.

Literal AU IDs map through a fixed seven-entry array. Raw DOMAIN [0,2] and bool
[0,1] values round to the nearest integer and normalize; other values are [0,1].
Invalid IDs/scopes/elements/nonfinite/range/events reject the pending musical
epoch. Immediate schedule calls inspect at most 256 events in two bounded
passes. Huge counts fail before dereferencing. Ramps return an error and reject
the pending epoch: no SDK vector or split-render interpolation is enabled.
Offsets are checked against the actual nonzero render length, not the maximum
slice. Targets of valid but overflowed/out-of-block calls remain readable, and
are acknowledged without next-block musical replay. UI overflow merges in the
shared processor, retaining its existing queue semantics and 256 total slots.

Parameter points merge before UI then MIDI, with stable insertion sort at equal
offsets. Inactive setters update initial controls directly. Active GetParameter
overlays pending targets until captured publication completes; serial comparison
prevents an older capture clearing a newer pending target. ClassInfo preserves
the existing state serialization then overlays pending AU targets into schema 1
(40 bytes). Native bypass property updates now enqueue an offset-zero parameter
step and do not clear other parameter points. Bypass property readback uses the
same pending target view. No VST3 integration or shared DSP/state format changed.

Zero RenderBus calls consume neither ingress, restores nor DSP and leave the
SDK cache untouched. A repeated nonzero timestamp and identical length reuses
the SDK cache without consuming next-epoch input. A repeated timestamp with a
different length returns `kAudio_ParamError`, avoiding stale differently-sized
output. The supported output is a single stereo bus; input is stereo or disabled.
Initialize/Cleanup/Reset keep upstream resource synchronization and cache reset;
property/program restores clear pending points before the coherent restore.

## Proven checks and precise remaining ownership gap

Strict external API AU comparisons pass 1,249 cases on local arm64 Release,
ASan/UBSan and TSan builds. The previous generated wrapper fails the same strict
entry. Multiple setter producers and MIDI, eight restores, pending saves,
zero/cache, bypass and rejection/no-replay cases pass. Full loaded AU callback
tests with concurrent ClassInfo restores also passed before the final bypass-only
change. These finite tests do not establish every interleaving.

**S09-OWN-01 remains open, identified by source review; no observed TSan failure
is claimed.** `Render` captures parameters and `controlEpoch`, releases ingress,
then calls `publishParameter` (APVTS listeners/TLS/host notifications) before
`PluginProcessor::process`. A control-thread `restore()` can publish newer controls
between capture and that publication. The old captured target can then overwrite
those controls. The snapshot generation check prevents acknowledgement of the
old scalar snapshot while generations differ, but cannot atomically protect the
separate APVTS write. After the restore drains, a later scalar read may replay the
older target. Per-parameter AU serials protect pending AU producers, not the
processor's restore generation or editor writers.

Smallest next deliverable: create a deterministic barrier-controlled regression
for restore/editor publication between AU capture and `publishParameter`; assert
newer host readback, saved schema state and next-block audio remain coherent. Then
implement generation-aware publication with participating restore/editor writers
and acknowledged ownership. Do not solve this by holding ingress across listener
callbacks or deleting locks; host listeners can reenter APIs. The notification
system remains Sprint 10's larger objective, but Sprint 09's new path must have a
safe ownership boundary before its exit is met.

**S09-LIFE-01 is independently reproduced and repaired locally.** The strict oracle sets FLUX to
0.2 at offset 7 while active, calls Uninitialize before render, then Initialize
without new setters. GetParameter no longer returns 0.2 because Cleanup clears
pending masks without transferring host targets to persistent controls. The final
`au-lifecycle-strict.json` exits 1 on exactly this named case after all earlier
1,249 comparisons pass. The subsequent `flushParameterTargets()` captures pending
targets under ingress, clears positioned storage, then publishes those targets
outside ingress on the legal lifecycle owner before teardown/rebuild. Cleanup,
Reset and preparation use this flush; ClassInfo/program restore intentionally
discard prior pending targets. The rebuilt Release strict run passes 1,251 cases
and preserves that named initial target. A setter racing lifecycle still relies on the legal SDK
lifecycle exclusion contract; this checkpoint adds no invalid-host protocol.
Complete stereo-transform/disabled-input exact AU comparisons, restore queue
overflow and cache-with-next-pending cases before calling the matrix complete.

External auval/pluginval/DAW/platform results remain unqualified for these bytes.
`auval` selects registered components by ID and cannot directly validate this
temporary bundle; personal plugin directories may not be modified. Historical
validator binaries were not found, and no other testing app was installed.
