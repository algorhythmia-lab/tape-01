# Sprint10 Session24: AU mode changes preserve prepared DSP

S10-AU-MODE-01 is locally verified at serialized native offline-mode continuity.
Sprint10 remains IN PROGRESS; release NO-GO/development-unqualified. Registry and
framework callback locks remain blocking. No architectural or external gate advances.

The unchanged Session23 shipping AU re-prepared SYS01's DSP whenever OfflineRender
changed. SYS01 has no mode-dependent DSP implementation: prepare deliberately clears
history and restores controls. Two loaded native instances of each study first matched
for 32 blocks. Then one instance toggled offline mode before each of 64 further blocks.
Both bytebeat and cellular FM differed on all 64 blocks; the silent-input live-transform
control remained equal. Every render ran on a fresh worker and its COMPLETE call joined
before mode changes or disposal. The negative reports, source export, generator, product
header/processor, fixture and executable, generated AU/SDK/dispatch/VST3, cache/compile
records, pinned SDK and both bundles were frozen BEFORE repair in
[Session24 evidence](evidence/sprint-10-session-24). This is actual history discontinuity
under serialized native calls, not a reproduced race, deadline miss or DAW result.

Only product [native_au_adapter.py](../../scripts/native_au_adapter.py) changes. A unique
pinned generation anchor removes the processor prepare call from the mode change.
The existing callback lock, setNonRealtime flag and truthful GetProperty readback remain.
Actual lifecycle prepare/sample-rate changes still reset history. No prepared buffer,
map, parameter listener, registry publication, control epoch or ingress slot is replaced.
There is no new allocation, queue, TLS, coalescing, retention or notification mechanism.

Release, separate ASan/UBSan and TSan ON/OFF bundles match sample-for-sample through
all 64 toggles in all three studies. A separate newly compiled fixture queues positioned
FLUX, OUTPUT and MIDI BIAS events before each toggle; all six matrices match the untoggled
instance. This verifies continuity of the observed smoothing/history and queued offsets.
It is finite serialized native verification, not arbitrary concurrent lifecycle safety.
Native strict AU, VST3, callback ownership/removal, first-worker entry, ContextName,
notifications/updater, editor, adapter/recall, format/pins/generator drift checks and
core16/16 pass. Exact reports, flags, tools, input/artifact hashes and failures are retained.
The inherited direct allocation-fault fixtures were not rerun: registry source is unchanged.

The complete module-wide registry protocol was reviewed and remains unchanged. Every
writer, capture, admission and exit holds the same module gate. Initial ALL-frame caller
origin recognition spans participating SYS01 AU instances; matches/collection filter owner.
Required other-thread callback-origin removal rejects before allocation or mutation. Every
matching add reserves EACH demand before ANY attachment/count/publication; collection marks
append first and clean under gate, including exceptions. Retained Entry liveFrames predicates,
withdrawn/newmatching frames, independent demands/spurious wakes and gate-held cleanup remain.
Ordered duplicates/survivors, outer/nested visibility, reader non-reclamation, writer-only
retired use_count1 reclaim and cross-instance constant incoming-link unlink remain. Final
Entry local notification decisions precede unlock/broadcast. Same-thread ancestors retain
context until return; errors authorize no release. COMPLETE owner/CV/wrapper/message-tail
joins remain required before teardown; foreign instance frames can remain live. Module unload
joins ALL participants. Updater stop/clear precedes registry shutdown while ContextName's
wrapper-this remains valid. This repair does not relax any lifetime exclusion.

Pinned SDK virtual Add/Remove/PropertyChanged routes, empty SDK vectors, uncalled accessor,
conditional MIDI source-only and GetMutex null remain. APVTS authority -> raw atomic mirror
-> recursive LockedListeners callback -> flags, regular/final parameter listenerLock and
synchronized editor removal remain. Processor size read outside lock, pointer under lock,
invocation unlocked and final forwarding unlocked still require lifetime work. VST3 separate
atomic floats/grouped flags/immutable IDs/shared storage remain. Prepared AU buffer maps,
scratch, MIDI storage and DSP history are still constructed under legal lifecycle exclusion;
the callback lock is not a publication/reclamation protocol. The SDK CanSetMaxFrames rejects
while initialized, so the investigated frame-limit mismatch was not established as a defect.
Caller-serialized same-role off-audio restore only; recursive/concurrent restore, arbitrary
owner migration, concurrent destruction and host dependency cycles remain unqualified.

Preserve org.algorhythmia.sys01/Algr/Sys1, seven keys/native IDs, four programs/order,
schema1 exactly40 bytes, parameter-only empty-history recall,10ms smoothing,shared256
whole-batch rejection/no replay,eight restores,single audio owner,formats and editor dirty/
seven packed FLOAT-GENERATION words/gestures/undo/recall/visibility.

Successful final reports contain no observed assertion/ASan/UBSan/TSan diagnostics.
The sandbox factory-registration failure, expected actual negative and fixture enum warning
remain recorded. All older failures and inventories remain immutable. Only two inherited
EXTERNAL bootstrap mutex shims exclude instrumentation in reused cross-audio fixtures;
product/generated/new continuity fixtures remain instrumented. Explicit8MiB SDK stacks in
inherited nested tests, fresh-worker joins and unpaced editor results are finite conditions.
Final probes observe alloc/free0/0 but AU fresh1 mutex9/hostile603 and VST3 fresh1024 mutex9.
Integrity is not acceptance; Sprint08's invalid live comparison is never resealed.
