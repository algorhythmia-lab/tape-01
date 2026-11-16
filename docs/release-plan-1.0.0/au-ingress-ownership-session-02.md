# Session 02 ownership working notes

S09-OWN-01 failing loaded-AU reproduction retained before repair. Restore and
editor both changed newer FLUX 0.8 back to 0.2; host readback, schema40 and following
audio failed. Conditional atomic publication repaired the forced boundary.

Each catalogue parameter retains JUCE type metadata but overrides virtual
getValue/setValue with a packed atomic uint64: 32-bit unique external writer
ticket and float bits. All notifying and direct host setters, restore/programs,
APVTS attachments, undo and ValueTree replay dispatch through that virtual setter.
An external writer allocates one ticket and exchanges one word. Exchange is its
linearization; unique tickets, rather than monotonic exchange order, prevent ABA.
AU pending ingress records the external writer word at acceptance. Render copies
fixed values/tickets, releases ingress, and attempts one strong CAS per target.
A newer external writer makes the CAS fail. Earlier AU publications retain the
same external generation, so a new accepted AU epoch can advance its target even
if the preceding AU render changed float bits. At 2^32-1 external writes per
parameter an irreversible exhaustion flag disables conditional publication; it
never silently wraps. Audio performs no CAS loop, wait, allocation or reclamation.
Objects live until processor teardown under existing lifecycle exclusion.

The processor's existing restore epoch protects coherent scalar acknowledgement;
restores remain eight slots serialized by stateMutex off audio. Atomic parameter
storage does not remove that mutex, ingress lock, framework callback/listener
locks or TLS. Parameter-only restore resets on the audio owner. The AU captured
block keeps its captured points even when a later editor wins host publication;
following blocks observe that editor. A later restore drains coherently when its
epoch can be acknowledged. No G01 closure claim.

Successful CAS sends listeners outside ingress and without calling unconditional
setValueNotifyingHost. Pinned APVTS listener reads current virtual getValue, rather
than trusting its notification payload. Processor controls/save use authoritative
virtual getValue, not delayed APVTS raw mirrors. General listener/UI callback
ordering, APVTS raw-mirror timing, TLS and host notifications remain Sprint 10
complete-callback work; finite sanitizer passes will not qualify them.

Inactive ordinary/scheduled setters now publish on legal lifecycle owner outside
ingress too. Scheduled calls validate at most 256 entries before any write.
No lock was removed. Active setters continue retained serialized MPSC ingress.

S09-BUS-01: pinned AU rejected disabled input via ElementCount=0 (-10865).
ASBD zero-channel safety validation also rejects zero-channel streams. Repair
allows only input element count 0/1 before initialization, keeps one processor
layout slot, assigns disabled/stereo layout, and delegates SDK resources. Output
stays fixed stereo. This uses existing host lifecycle exclusion, no concurrent
resource change/reclamation. Exact loaded-AU generator/transform coverage added.

Verification: Release and separate ASan/UBSan and TSan fixture builds pass both
forced capture-to-publication interleavings. All default-hook-OFF builds pass
1,274 strict independent AU comparisons, callbacks, VST3 exact-bundle conformance,
adapter/recall 3/3 and ad-hoc seals. Generated wrappers carry sanitizer flags.
Default binaries contain no exported fixture symbols. Native editor/undo fixture
passes. Core 15 default tests plus separately enabled stress smoke pass 16/16.
No observed sanitizer/race failures in these finite runs.

Sprint 09 remains locally verified at this checkpoint, not externally qualified.
Next review must exercise callback reentry and acceptance-before-capture editor
ordering through loaded AU and confirm ownership/acknowledgement for all writers.
Current fixture pauses only after capture, tests a real restore and the exact
virtual setter used by editor attachments; it is not a real DAW-origin edit or
an exhaustive listener interleaving proof. No auval/pluginval/final-platform or
Ableton-origin timing result qualifies these bytes. General whole-callback locks,
listener notification payload ordering and TLS remain G01/Sprint 10 work; Sprint
10 has not been selected. No AU timing waiver.
