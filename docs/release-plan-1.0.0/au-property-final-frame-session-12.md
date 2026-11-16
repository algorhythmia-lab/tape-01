# Sprint10 session12: final matching Entry frame notification

S10-AU-REGISTRY-07 suppresses intermediate exits of the same Entry while a remover
must still acknowledge its remaining frames. Sprint10 IN PROGRESS; release NO-GO/
development-unqualified. Registry gate BLOCKING. This is a bounded notification
repair, not nonwaiting capture/admission/exit or whole G01 acceptance.

The unchanged session11 final ON Release AU fingerprint
5a03506118c7951a682594f3a090a778a60f580bf7ebd0704cd8f148926db8be, binary
d0786d36d6addb4af55dc9413021da5f53a38a0e6bd3ab6429998af2ea52de97,
reproduces real SDK NickName nesting1/8/64/257. Both real removal selectors wait
and acknowledge2; exits broadcast1/8/64/257 instead1/1/1/1 for the single Entry.
Original-call external pthread_cond_broadcast interposition captures changed ->
generated PropertyChanged -> SDK DispatchSetProperty -> AudioUnitSetProperty.
No synthetic PropertyChanged/product negative observer was added. Host barriers
and stack unwinding are separate diagnostic ownership. This proves unnecessary
plugin-triggered OS notification work, not deadlock/race/deadline miss. Complete
source export, fixture sources/executables/interposer, generated AU/SDK/VST3,
cache/compile commands, 29-file pinned SDK, bundles and original reports were
frozen BEFORE repair. Sandbox registration setup failure is retained separately.

Only product implementation changed: AUPropertyRegistry.hpp. Each Entry now
owns a gate-protected liveFrames count, initially0. Admission increments exactly
once after the active check and intrusive publication under gate. Linked RAII exit
unlinks constant work, decrements exactly once, then broadcasts only on transition
to0 with waitingRemovals nonzero. Unadmitted inactive frames do neither. Callback
exception unwinding uses the same destructor; a supplemental direct test covers
it without asserting an OS callback exception contract. The Frame references its
immutable snapshot shared_ptr slot; no reader allocation/retention/reclamation
operation was added. The Entry remains retained by that snapshot during exit.

Why final Entry exit suffices: a waiting removal originates outside this registry's
callbacks. A callback-origin caller with any matching other-thread frame is
rejected BEFORE allocations/mutation; a successful callback-origin immediate
removal never releases gate with a live waiting demand. Therefore every waiting
predicate includes every live matching Entry frame, regardless of its thread.
If liveFrames remains positive, that predicate cannot yet acknowledge. Its final
exit broadcasts to all independent demands. An off-callback remover's own thread
cannot simultaneously own a callback frame. No demand can require acknowledgement
of a proper subset of an Entry's remaining frames under this retained protocol.

All add publishers initialize new Entry count0 and retain/reserve/attach each
matching pending WaitDemand before publication. Existing demand count ownership,
withdrawn Entry retention, independent second-remover scopes and exceptional wait
cleanup remain unchanged. Remove still scans ALL matching other-thread frames,
including withdrawn and newly matching Entries. Different Entries can still
broadcast individually while another matching Entry remains; shared CV can wake
unrelated predicates. Every demand retains entries until acknowledgement and
unlinks/decrements under gate. Failed allocation mutates capacity only before
logical publication/withdrawal. Counters do not legitimize host context destruction
while newly matching registrations remain published. Errors authorize no context
release; same-thread active/ancestor contexts live until return.

Current/retired snapshots, ordered duplicates/no cap, outer/nested visibility,
writer-only use_count1 reclamation and reader never-last-owner deletion remain.
All SDK virtual Add/Remove/PropertyChanged paths route to the registry, SDK vector
empty/accessor uncalled, conditional MIDI source-only. Gate remains until EVERY
writer/reader/context has replacement publication and acknowledged reclamation.
Legal wrapper teardown excludes/joins complete wrapper/processor entries, fixed
message owner, never callback disposal; updater stop/clear precedes registry
shutdown while ContextName wrapper-this lives. Processor removeListener alone
acknowledges neither complete updateHostDisplay nor final forwarding. GetMutexnull.
Restore remains caller-serialized same-role offaudio, recursive/concurrent same-role
restore and owner migration unqualified.

Continued pinned-source review: ParameterAdapter virtual authority -> atomic raw
mirror -> recursive LockedListeners call -> listenersNeedCalling=false/needsUpdate
true. Regular/final parameter callbacks retain listenerLock and synchronized editor
removal. Processor updateHostDisplay reads size outside lock, obtains each pointer
under lock, calls unlocked; ParameterChangeForwarder traverses without that lock.
VST3 CachedParamValues uses separate atomic floats and grouped atomic flags, immutable
IDs after shared construction, component/controller retained storage. Flag exchange
and latest scalar loads are not packed words/event history. Prepared maps/scratch/
DSP/callback/FL locks and invalid concurrent lifecycle remain open; no replacement
without complete every-writer/reader/lifetime ownership.

Preserve identity/7keys+nativeIDs/4programs/schema1exactly40/parameter-only empty-history
recall/10ms smoothing/shared256 wholebatch rejection+noreplay/eightrestores/singleaudio
owner/stereo generator disabledinput/formats/stable editor atomicdirty/7authoritative
packed float-generation words/synchronized removal/message gestures/undo/visibility.
Capture/admission/exit waits, final matching broadcasts, host-sized dispatch/remove/
reclaim/add-demand work and allocating audio-reentered writers remain blockers.
Host/cross-instance cycles and parameter lock orders remain unqualified. Retained
session09 ASan default-stack overflow257 and session10 allocator/leak failures remain;
all new nested257 fixtures use explicit8MiB SDK stacks+legal joins. Separate sanitizer
condition tests do not prove allocator interposition; Release proof is separate.
Finite successful logs qualify only exact bytes and named local boundaries. Actual
DAW-origin/external OS/architecture/signing/rights/accessibility/humans/budgets/soak/
MB-04 remain open. No gate waiver or advancement.
