# Sprint 10 session 01: editor notification ownership

S10-NOTIFY-01 is the bounded editor attachment repair under S07-P1-03. Sprint 10
and G01 remain IN PROGRESS. Release remains NO-GO/development-unqualified.

The dedicated fresh-audio-thread fixture uses the real PluginEditor and seven
shipping parameters. It registers a real reverse-order regular listener after
the FLUX attachment. Outer FLUX0.2 notification reenters FLUX0.8/BIAS0.7 and
then resumes; a late obsolete0.2 notification follows. Frozen executable
`evidence/sprint-10-session-01/notification-negative` SHA256
`343e1904a5200b329191d647dcc8333c21bbf36854100ebca1057b882c9d3085`
fails: authority/raw mirror0.8, editor0.2. Command s10-notification-baseline,
log and original default AU/VST3 bundles were retained before repair. Their
fingerprints match the session03 checkpoint. This is a dedicated first-thread
fixture, not a DAW-origin automation claim.

## Bounded repair and lifetime

`ControlAttachments.hpp` replaces only SYS01's APVTS Slider/Button/ComboBox
editor attachments. APVTS parameter adapters and host listeners remain. An
editor-owned final relay publishes a lock-free atomic dirty bit on other threads.
It never stores notification payloads, posts messages, accesses controls, allocates
or traverses a queue. The message-thread editor update reads each atomic packed
authoritative float/writer generation, compares the entire word, and updates only
changed controls. Polling also covers direct nonnotifying writes and float changes
within one AU external generation. Exhausted generation still includes float bits;
no independent wrapping UI sequence or queue capacity exists. An old outer
notification can mark dirty but cannot replace the authoritative word.

Poll clears dirty before the authoritative read. A later write remains marked
for the next tick; the existing visible-editor timer polls all seven parameters
even without a notification, so no pending bit can lose a direct write. Dirty is
a coalesced wake request, not a stored value or historical event. There is no
automation-resolution reduction: only the UI visual target is coalesced. The
audio scheduler/targets are unchanged. Message-thread notifying setters synchronously
poll using the constructor-captured JUCE thread ID, preserving immediate gestures,
undo grouping, discrete controls and recall behavior. Construction/destruction
remain message-thread-only. The existing editor timer stops when hidden/closed;
showing an editor refreshes authority. No new message or timer is posted on audio.

The relay is a separate final object with an unchanging vtable while its owning
derived attachment is constructed/destroyed. Other threads touch only relay dirty
and immutable thread ID, never dereferencing owner. JUCE parameter listenerLock
serializes every dispatch against removeListener. The relay remains alive until
removeListener returns, then members are destroyed. Message-thread synchronous
owner polling cannot overlap message-thread construction/destruction. Control
listeners detach on message thread; held slider gestures finish in PluginEditor
destructor while attachments still exist. No framework listener lock is removed,
and processor teardown still requires legal lifecycle exclusion.

The new fixture checks nested/late ordering, all seven direct writes and 64 real
editor attach/detach cycles during dense seven-key notifying production. Separate
Release diagnostic, ASan/UBSan and TSan fixtures pass. Existing gesture/undo/recall
checks remain the compatibility oracle. Finite checks do not prove general
nonblocking dispatch, all native UI budgets or actual accessibility.

## Complete callback trace and remaining ownership

| Path | Current owner/boundary; remaining Sprint10 work |
|---|---|
| AU Render/capture/publication | SDK validates host buffers and pulls input through host callbacks. Prepared audio maps/scratch belong to legal lifecycle; single audio owner renders. Fixed ingress is captured under incomingMidiLock, listeners run after unlock, conditional CAS uses accepted external generation. Cleanup reacquires ingress. No replacement ingress/resource reclamation yet. |
| JUCE parameter dispatch | Recursive parameter listenerLock, reverse regular listeners, final processor listener. APVTS reads authoritative getValue, updates raw mirror and calls locked APVTS listeners. Editor relays now do bounded atomic publication; arbitrary listeners and APVTS locks remain. |
| AU final processor/bypass listeners | Per-instance ThreadLocalValue bool is assigned before native publication and consumed/reset by final listener. New thread allocates ObjectHolder and list traversal grows with thread count. Nested notification may consume suppression before outer final callback. AUEventListenerNotify/property notifications enter OS/host code. This repair does not qualify suppression or their payload order. |
| VST3 ingress/controller | Generated fixed256 point/event limits and CC20–23 mapping precede host-sized MIDI conversion. Virtual notifying writes publish authority. Upstream thread_local suppression triggers dyld TLV bootstrap; host parameter queues/controller callbacks and processor listener locks remain. |
| Callback resources | AU processBlock and VST3 processAudio retain processor callbackLock; VST3 retains conditional FL Studio lifecycle workaround. prepare/release/layout/offline mutation is serialized by existing legal lifecycle and locks. No immutable prepared-resource acquisition/reclamation or invalid-concurrent lifecycle silence/error protocol has been implemented. |
| MIDI/buffers | AU preallocated4096 alternating MIDI buffers; bounded legacy short events, ignored SysEx, bounded UMP dispatch/packets/words. VST3 fixed queues/events before plugin traversal. Buffer mapping/copy costs bounded by negotiated channel/block resources; host pull/queue virtual methods are host-owned implementations. All entry locks still instrumented. |
| State/program/bypass | Off-audio stateMutex and eight coherent restore slots/epoch remain. Audio drains a captured boundary and owns DSP reset. Native synthetic program can enter restore on calling thread and remains outside the off-audio contract if host invokes it from audio. Bypass scheduling remains format-specific/covered by exact fixtures. Recursive restore while stateMutex is held remains unqualified. |
| Editor teardown | New stable relay plus retained listener synchronization; 64 concurrent attach/detach cycles and held-gesture close verified. No pointer/slot reclamation on audio. Native window, accessibility and OS paints remain message/OS work and need later performance/human fixtures. |

The complete rebuilt Release entry probes retain **AU fresh1-frame allocation1,
mutex9** with new/operator allocation stack inside JuceAU::Render; generated source
traces the first-thread ThreadLocalValue holder path. **VST3 fresh1024-frame
allocation1, mutex9** stack resolves calloc → dyld ThreadLocalVariables →
tlv_get_addr → processParameterChanges. Warm zeros cannot accept first-thread
or arbitrary callback load. Focused attachment dispatch observes first mutex5
(baseline14), warm mutex1 (baseline2), allocations0 in both finite cases; these
are whole dispatch counts, not proof all locks belong to the replaced attachment.
Negative probe self-test observes alloc/free/mutex/unfair, proving interposition.

Host/OS callback implementations are outside plugin ownership, but plugin-triggered
ThreadLocalValue/TLV, listener locks, async posts and resource locks are plugin
work and remain gate blockers. No global suppression flag, extended ingress over
listeners, lock deletion, timing waiver or inferred whole-realtime pass is used.

All native IDs, seven keys/ranges, four programs/order, schema1 exactly40,
parameter-only deterministic empty-history recall,10ms smoothing, shared256
whole-batch rejection/no replay, eight restores, single audio owner and
stereo effect/generator/disabled-input contracts remain. Generated upstream pins
are untouched. Fixture hooks remain defaultOFF and final default binaries must
be scanned for absence. Editor/shared source changes invalidate affected native,
DAW/UI/performance/soak/signing evidence; new finite checks bind new bytes only.
Historical Sprint08 comparison stays invalidated and all old evidence stays frozen.
