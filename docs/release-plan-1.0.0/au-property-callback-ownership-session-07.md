# Sprint 10 session 07: AU property callback access and acknowledgement

S10-AU-REGISTRY-02 is a bounded callback wait/order repair under S07-P1-03.
Sprint10 stays IN PROGRESS; release NO-GO/development-unqualified. This closes
one registry-induced cross-thread registration wait, while retaining the registry
access gate and prohibiting a whole realtime or arbitrary-reentry claim.

## Frozen loaded negative

Actual diagnostic Release AU fingerprint
`3870a2aa3a1b8ffb6c53e93dd2a1f59b11cae6ec417d5be1d959973319cb63ee`,
binary SHA256 `b35f36b4150b03923485aa63ab79cbf0bdf6612a00e3c40c3f39af2900c3f5d1`.
Real native BYPASS SetParameter is consumed in actual Render and reaches the
wrapper's regular listener, virtual PropertyChanged and host callback. The host
callback waits for another thread to add a trailing registration through the real
AudioUnit API. The recursive registry gate spans the callback, so add cannot
complete until the fixture watchdog releases the callback after two seconds.
Observed completed-before-return=0/calls=1, expected1/1. Existing reentrant order,
257 duplicates, removal acknowledgement and128 destruction cycles already pass.
This is a finite watchdog observation of the actual cyclic dependency, not an
invented sanitizer failure. The wait inside the host callback is fixture-owned;
the gate held across it is plugin-controlled.

Actual report, fixture, bundle, complete source export, generator, generated AU/
SDK/dispatch, cache and compile commands were frozen before repair.
The unchanged pinned SDK tree was also copied and hash-verified afterward.
The early sandbox load used the preceding executable while the baseline build
was still running and failed at factory registration; retain it as setup failure,
not the defect. Authorized in-process runs install nothing. Frozen bytes and all
historical inventories remain unchanged.

## Every registry participant

All AUBase virtual AddPropertyListener/RemovePropertyListener/PropertyChanged
still route to one instance AUPropertyRegistry. The SDK vector remains empty;
its const GetPropertyListeners accessor has no pinned caller. Every SDK path in
session06's map is retained: state/RestoreState, layout/stream/channel/input
connection/render callback, element/preset/property/name, LastRenderError,
AUEffectBase sample-rate ParameterList and conditional MIDI map/hot map. The
numbered ownership trace captures callers and exact source hashes. Conditional
MIDI maps have source coverage, without a new supported-MIDI-map runtime claim.

Registry and empty snapshot initialize before constructor body registration.
The constructor ContextName callback owns wrapper this; it reaches
updateTrackProperties and survives host-listener removal. Real SDK ContextName
nested SetProperty and NickName/native-bypass overlap exercise actual virtual
SDK dispatch. Off-message updater still only publishes atomic flags; fixed
message/timer delivery uses this same registry. Its lifetime and coalescing
contracts remain session06's. Bypass notifications stay synchronous outside
ingress, with existing parameter/framework locks. No shared processor, DSP,
editor relay or VST3 generator implementation changes.

## Publication and callback ownership

One ordinary mutex protects closed, current/retired publication, every Entry
active access and the complete intrusive frame list. Add builds a fresh structural
snapshot and appends in order, preserving all duplicates with no cap. It retains
old storage before replacing current. Remove completes throwing allocations and
retired retention before withdrawal; then marks matching entries inactive and
publishes a snapshot without them. This also avoids partial inactive mutation on
a failed allocation. Registration removal is identified by id/callback and exact
context for WithUserData, or all contexts for the legacy SDK selector.

Dispatch captures current under the gate, releases it, and visits the captured
ordered snapshot. For each matching active entry it inserts a stack Frame under
the gate, atomically with the active check. It then releases the gate before
calling host code. A concurrent remove therefore either prevents entry or sees
the already admitted frame. New adds are absent from the outer snapshot and
visible on the next/nested dispatch. Withdrawn tails are skipped on old snapshots.
The callback returns or unwinds through RAII Frame removal under the gate and
notifies waiters. Intrusive Frames have no allocation, TLS bootstrap, fixed table,
listener cap or retained host payload; their thread IDs identify live stack owners.

Remove waits on a condition variable with the access mutex released until all
matching frames on OTHER threads return. It checks the whole frame list, including
previously withdrawn entries, so a simultaneous second removal also acknowledges
in-flight contexts. Spurious wakeups recheck under the gate. Same-thread frames,
including nested ancestors, cannot be awaited by their own callback. Their host
contexts must remain alive until those frames return. Cross-thread callers may
destroy the removed contexts only after successful removal, excluding new matching
registrations/publications as part of their host context lifetime protocol.
A failed removal does not authorize destruction.

Snapshots retained by current/retired and active dispatch references keep Entry
storage alive even while callbacks are unlocked. Writer-only reclaim erases a
retired snapshot only when its use_count is1. Reader capture/reclaim is serialized
by the access gate; once retired it cannot acquire new reader owners. Normal
readers are never last owners and do no storage reclamation. Active Frame entry
pointers are covered by the enclosing snapshot. Callbacks touch host contexts
only inside admitted frames; traversal after removal reads immutable metadata and
active under gate, without dereferencing withdrawn context pointers.

Legal shutdown still requires host exclusion/join of ALL wrapper/processor
entries and complete dispatches, fixed-message-owner construction/destruction,
and no disposal from callbacks. Updater stops/clears before registry shutdown.
Shutdown can then withdraw/reclaim off audio while wrapper-this still lives.
The access gate alone no longer acknowledges whole dispatch teardown; do not
confuse per-registration removal acknowledgement with legal wrapper disposal.
Pinned processor listener removal alone does not acknowledge concurrent
updateHostDisplay. Arbitrary concurrent lifecycle remains unqualified.

## Limits and observable ordering

Per-dispatch order and duplicates are preserved. Concurrent property dispatches
can now overlap the same callback; host contexts must support that concurrent
use. There is no total ordering across independent simultaneous dispatches.
SDK state/layout storage, wrapper ContextName track updates, same-role RestoreState
and parameter listeners retain their independent caller/lifecycle contracts;
the registry does not make those writers concurrent-safe. The overlap fixture
uses independent NickName SDK storage and one native bypass Render owner.

The gate is retained, NOT replaced by a nonwaiting publication protocol. Reader
capture/admission/exit block; exit may notify a condition variable and traverse
host-sized active frames. Dispatch and writer traversal remain host-sized.
Reentrant writers can allocate/free on audio. Callback A waiting on another
thread that removes A must still wait for A's context safety; that cycle cannot
be solved by falsely acknowledging an active callback. Cross-thread callbacks
removing each other's active registrations and remaining parameter-lock/registry
orders remain unqualified. APVTS/regular/final listeners, CachedParamValues and
prepared resources/lifecycle still require Sprint10 ownership work. Do not remove
the access gate until every writer/reader/context uses complete replacement
publication and acknowledged reclamation.

Diagnostic hook defaults OFF and spans the actual complete virtual PropertyChanged
callback. Empty/zero-cost host callbacks distinguish plugin scaffolding from
host bodies. Fresh complete registry probe requires no reader alloc/free, while
retaining measured locks; it does not accept zero prohibited work. Finite separate
ASan/UBSan and TSan success cannot prove universal reclamation or realtime safety.

All seven keys/native IDs, four programs/order, schema1 exactly40 bytes,
parameter-only deterministic empty-history recall,10ms smoothing,shared256
whole-batch rejection/no replay,eight restores,single audio owner and stereo
input/generator/disabled-input/VST3 contracts remain. Stable editor relay uses
atomic dirty off-message and authoritative seven-word polling; synchronized
parameter removeListener lifetime stays intact. No parameter/gesture coalescing.

Affected AU callback/concurrency/host/lifecycle/performance/soak/signing evidence
is invalidated and renewed only at the named local boundary. Historical evidence
stays attached to its bytes; Sprint08 live-source remains intentionally invalid.
ActualDAW-origin/externalOS/architecture,signing/rights/accessibility/humans,
performance/soak and MB-04 remain unqualified. Final measurements and ON/OFF
identities are in session07 evidence and handoff. Remain Sprint10 without waiver.
