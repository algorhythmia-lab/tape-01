# Sprint 09 session 03 ownership acceptance

S09-OWN-01 covers generation-aware AU target publication and acknowledgement.
It does not accept whole-callback realtime behavior (G01), DAW-origin timing,
notification payload ordering for every observer, or external platform cells.
The final v2 ON/OFF Release, ASan/UBSan and TSan matrices, editor checks,
core16 and exact-byte integrity checks pass (session-03 handoff). S09-OWN-01
and Sprint 09 are COMPLETE AT NAMED LOCAL BOUNDARY. External AU/DAW
qualification and G01 remain open; no scope/timing gate is waived.

## Reproduced boundary and repair

The loaded AU accepted FLUX 0.2, then a fixture editor setter published FLUX 0.8
before Render capture. A second case installed a real parameter Listener at the
capture hook; the FLUX 0.2 notification reentered notifying setters, publishing
FLUX 0.8 and BIAS 0.7 before the later BIAS CAS. Nested callbacks are guarded by
the fixture, not by shipping behavior. The fixture reads all seven authoritative
values, APVTS raw mirrors (DOMAIN denormalised), native GetParameter and saved
schema 1 exactly40 bytes inside the callback and after Render/following blocks.
An independent processor checks captured points and following audio exactly.

Frozen negative bundle fingerprint
`c3c415836e9d9bf2d7c56580f4cd662cf14241168436251c657f71be2cd34427`:
authoritative values/raw mirrors and audio passed; GetParameter and ClassInfo
save failed inside both newer-writer callbacks. Their unconditional pending-AU
overlay masked the newer external value. Reports and bundle/executable were
retained before repair. The initial sandbox attempt failed factory registration,
not ownership; the explicitly authorized in-process registration reproduced the
actual defect without installation.

The overlay now compares the pending target's accepted external generation with
the current authoritative generation. An invalidated target is not overlaid on
native readback/save; its already accepted points still belong to the captured
audio epoch. Lifecycle flush also uses conditional publication, outside ingress,
rather than an unconditional notifying setter. A legal Reset after newer editor
publication retains the editor target and exact following audio. At permanent
ticket exhaustion capture exposes the invalid sentinel generation, including if
a delayed pre-exhaustion writer exchanges later; both overlay and CAS fail closed.
Scalar virtual getValue remains the real float value, and external writes continue.

## Writer and reader ownership map

| Participating path | Publication/ownership and acknowledgement |
|---|---|
| AU active ordinary/scheduled setters | Under existing incomingMidiLock, accept fixed points/final targets, per-target serial and external ticket. Multiple parameter/MIDI producers remain serialized MPSC. No parameter listener runs under this ingress lock. Schedule validates whole call before mutation; rejected epochs do not replay points. |
| AU Render capture | Single audio owner takes one fixed ingress snapshot, clears consumed point count/rejection, captures authoritative scalar words and restore epoch. Positioned points are forwarded once. Cache hits/zero renders do not consume another epoch. |
| AU conditional publication | After unlocking ingress, one strong CAS per masked target. Same external generation permits later AU epochs to advance values. External exchange invalidates old publication. No audio CAS retry, wait, allocation or reclamation was added. Success sends listeners without unconditional setValue. |
| AU pending readback/ClassInfo save | Ingress protects pending data. Only a non-exhausted matching external generation overlays authoritative virtual getValue/encoded controls. A concurrent external write can linearize after a read; after the write completes, an old pending overlay cannot win. Save retains stateMutex then ingress order. No lock held over notification. |
| AU target cleanup | Audio reacquires ingress and clears a mask only when its serial equals the captured serial. A reentrant AU setter can retain a next-epoch target; an external editor setter invalidates via its ticket. |
| Inactive ordinary/scheduled AU setters | Legal host lifecycle serializes setters/resources; virtual notifying writer executes outside ingress. Scheduled calls prevalidate bounded entries. No new concurrent lifecycle guarantee. |
| Initialize/Cleanup/Reset/prepare flush | Legal lifecycle excludes Render. Capture/clear pending under ingress; conditional target publication and listeners outside ingress. Newer external generations survive. Offsets are discarded across lifecycle, persistent targets retained. |
| Native direct/notifying setters | Custom Parameter overrides virtual setValue/getValue. Each setValue allocates a unique ticket and atomic exchanges float/ticket. Pinned setValueNotifyingHost calls that virtual setter then listeners. Direct setters do not promise APVTS notification until their caller sends it; controls/save still read authority. |
| Editor sliders/toggles/domain/accessibility | SYS 01 uses pinned APVTS Slider/Button/ComboBox attachments. ParameterAttachment gesture writers call virtual setValueNotifyingHost after range normalization. Editor commit/accessibility flows reach these attachments. No SYS 01 concrete base get()/assignment shortcut or raw-mirror write is used. Native editor fixture covers gestures, undo/redo, recalls and scales. |
| APVTS ValueTree/undo replay | ValueTree property change -> setNewState -> ParameterAdapter setDenormalisedValue -> setNormalisedValue -> virtual notifying setter. Undo/redo runs off audio; APVTS ignore-callback guard prevents flush-to-tree feedback. Existing state/undo ownership remains. |
| Restore/program setters | stateMutex serializes producers, eight restore slots published before controls. Odd/even epoch brackets virtual notifying setters. Full queue leaves controls intact. Audio owns DSP restoration/reset and bounded queue consumption. Program selection uses same restore. No stateMutex removed. |
| APVTS ParameterAdapter listener | Pinned listener reads current virtual getValue rather than notification payload, denormalises, stores atomic raw mirror then calls APVTS listeners. In the forced nested case, later completion reads current authority and does not revert mirror. Arbitrary concurrent listener invocation can delay mirrors; raw mirrors are not shipping controls/save authority. |
| Parameter listener dispatch | Pinned listenerLock surrounds reverse-order regular listeners then final AudioProcessor listener. It is recursive, enabling same-thread setter reentry; fixture registration lifetime is quiescent, removed before teardown. Production attachment attach/detach remains under framework listener lifetime synchronization. The new test does not establish lock-free or allocation-free dispatch. |
| AU processor/bypass listeners | Final processor listener consumes the existing thread-local suppression boolean or sends AU events. Bypass listener reports property change. Neither is an unconditional musical scalar writer. Host reentry can accept a new ingress epoch; ingress is free during notifications. Nested suppression/event payload ordering is separate Sprint 10 work. |
| VST3 controller/audio writers | Pinned generated controller notifying setter uses the virtual publication word; positioned native points remain on single audio owner. Existing controller/listener caches and suppression remain, exact VST3/adapter/recall results are renewed. AU-only pending target overlay is not used by VST3. |
| nativeControlsSnapshot/process | Audio-only snapshot carries captured controls/restore epoch. Stable matching even restore epoch permits drain/acknowledgement. Positioned masks prevent scalar final targets replacing positions; cached captured final targets prevent rejected replay. A newer external value after capture becomes next-block scalar change; it does not contaminate the captured block. Restore epoch mismatch defers restore consumption. |

Parameter objects live until processor teardown under legal lifecycle exclusion.
Atomic float/ticket exchange is writer linearization; ticket allocation order need
not be exchange order, and unique tickets prevent ABA before exhaustion. The
64-bit allocation counter is not the generation word; the irreversible exhaustion
flag makes subsequent counter wrap irrelevant to conditional publication.
Existing incomingMidiLock, stateMutex, listener/framework locks, TLS and all queues
remain. Capture is bounded by seven targets and at most256 events; sanitation
and single audio ownership remain as described in sessions 01/02.

## Explicit limits

Pinned ParameterAttachment consumes notification payloads and can schedule async
UI work; reverse-order listeners can receive an older outer payload after nested
notification. APVTS raw timing/flags, general listener concurrency, AU event
suppression across nested callbacks, TLS first-thread allocation/list traversal,
framework callback/resource locks and host notification costs require Sprint 10
whole-callback ownership work. Finite ASan/UBSan/TSan passes do not prove their
realtime safety. No ingress lock was extended over a listener and no lock was
removed. Restore reentry into a currently held stateMutex is not exercised or
newly authorized as safe. The fixture uses same-thread parameter setter reentry,
not recursive restore nor actual DAW-origin editor/automation.

ALGORHYTHMIA_AU_OWNERSHIP_FIXTURE defaults OFF. Added read/listener/before-capture
hooks exist only in ON generated AU glue. Frozen ON evidence is for deterministic
verification; final OFF binaries require complete symbol/string absence checks.
Historical evidence remains attached to original bytes and is never resealed.

Diagnostic repairs retained in session 03: sanitizer strict tests exposed three
JUCE layout assertions already present in session-02 logs. Disabled input tag zero
means UseChannelDescriptions, not a standalone disabled layout tag; generated
initialization now explicitly uses AudioChannelSet::disabled() for zero channels
instead of calling the tag-only converter. SDK upstream remains pinned/unmodified.
The full sanitizer editor fixture emitted nine ASCII-constructor assertions for
its Arabic invalid-entry test; explicit String::fromUTF8 fixes this fixture-only
oracle input. A controls-only CLI option permits sanitizer gesture/recall/teardown
verification without claiming full instrumented image or timing acceptance. Release
retains the complete 3072 render/four-scale fixture. An earlier ASan full-image
run was deliberately terminated after retaining its log/partial images; it is
incomplete, not a pass. Final-source renewals are required after these repairs.
