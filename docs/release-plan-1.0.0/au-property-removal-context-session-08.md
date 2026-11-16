# Sprint 10 session 08: callback-origin removal acknowledgement

S10-AU-REGISTRY-03 repairs one loaded-AU callback/removal wait cycle. Sprint10
stays IN PROGRESS; release NO-GO/development-unqualified. Registry access gate
remains BLOCKING. This is a per-instance error/acknowledgement boundary, not
nonwaiting admission/exit or arbitrary callback dependency qualification.

## Frozen negative and change

Session07 final default Release AU fingerprint
`e37e3b2e5e3b85ee0c33a9bd03efb5771323c6ba9f35d7d7238c44aff25c3f55`, binary
SHA256 `18f43aad137a2d75a49fe17e2cd643eb46834300c5e6157df14200b63bbc8196`.
Real native bypass Render enters callback A. Independent real SDK NickName
SetProperty enters callback B. A requests removal of B; B waits for A to return.
Registry removal waits for B's frame, so neither returns until fixture watchdog
releases B. Both public WithUserData and actual loaded SDK legacy Lookup selector
observe returned-before-release0/status0/target-calls1, expected1/-10863/2.
Host waiting is fixture-owned; plugin admission of the removal wait creates the
cycle. No race/sanitizer failure is invented. Initial sandbox registration failure
is separate; approved in-process run installs nothing.

Exact loaded failure/report/bundle, fixture executable/source, pre-repair complete
source export, generator/generated wrapper/SDK/cache/compile commands and pinned
SDK tree are frozen in evidence/sprint-10-session-08. All149source+15reference
inputs matched session07 before edits. Historical evidence is unchanged.

Under the retained registry gate, remove scans live frames before allocations or
mutation. If this caller owns ANY callback frame in this instance and a matching
OTHER-thread frame exists, remove returns kAudioUnitErr_CannotDoInCurrentContext
(-10863). No registration is withdrawn/published, and failure never authorizes
host context destruction. Host retains context and retries outside callbacks after
owners are joined, excluding new matching registrations. The check includes
already-withdrawn live frames. It conservatively rejects even if the host knows
that the other callback will not depend on this caller.

If no other-thread matching frame exists, original same-thread reentrant removal
continues, with active/ancestor contexts kept alive until their frames return.
If caller owns no callback frame in this registry, original successful removal
withdraws and waits with gate released for ALL matching other-thread frames,
including simultaneous second removers. Because check and withdrawal use the
same gate, a matching callback cannot be admitted between them. Cross-thread
successful return remains context acknowledgement; errors do not acknowledge.

## Every participant and retained protocol

All SDK virtual AddPropertyListener/RemovePropertyListener/PropertyChanged callers
still route through one registry; generated wrapper and SDK routing are unchanged.
SDK vector remains empty; const GetPropertyListeners has no pinned caller. State,
RestoreState, stream/layout/channel/input/render callback, preset/name/element,
MaximumFramesPerSlice, LastRenderError, AUEffect sample-rate ParameterList and
conditional MIDI mapping/hot-map calls are captured in ownership-trace.json.
Conditional MIDI paths have source coverage only. Constructor ContextName
registration owns wrapper this; real nested64 dispatch/removal and surviving
wrapper registration are verified. Bypass and message/timer updater use the same
virtual dispatcher. Legacy selector is called on actual loaded instance because
public no-context function is unavailable arm64. No SDK/pin/generator change.

One mutex still protects closed/current/retired/active/frame publication, reader
snapshot capture/admission/exit and removal decisions. Immutable ordered snapshots
preserve duplicates/no cap and outer-vs-nested visibility. Stack intrusive Frames
admit with active check under gate, callbacks run unlocked, RAII exit unlinks and
notifies. Current/retired retain snapshot storage; only writers reclaim retired
use_count1, so normal reader never last-owner deletes. This repair changes neither
publication nor reclamation. Do not remove gate until EVERY writer, reader and
context follows replacement publication and acknowledged reclamation.

Legal teardown excludes/joins ALL complete wrapper and processor dispatches,
runs on fixed message owner, and never disposes inside callbacks. Updater stops/
clears before registry shutdown while wrapper this lives. Processor removeListener
alone does not acknowledge updateHostDisplay or final listener traversal. Access
gate does not acknowledge complete wrapper lifetime. Concurrent resources,
RestoreState same-role/recursive stateMutex and owner migration remain unqualified.
Optional AUEntryGuard only locks nonnull GetMutex; SYS01 leaves it null.

## Next listener/resource trace and limits

Pinned APVTS ParameterAdapter reads authoritative virtual getValue, publishes raw
atomic mirror/needsUpdate and invokes LockedListeners under recursive CriticalSection.
add/remove use that same APVTS lock. Parameter regular reverse listeners and final
processor listener dispatch stay under parameter listenerLock; stable editor relay
lives through synchronized removal and off-message only publishes atomic dirty.
AudioProcessor ParameterChangeForwarder traverses processor listeners without
listenerLock; updateHostDisplay indexes list then obtains each pointer with lock,
releases it before invoking callback. Processor listener removal alone cannot
acknowledge those complete dispatches. Frozen numbered excerpts retain this exact
qualification rather than repeating an unsynchronized-all-paths shorthand.

VST3 shared JuceAudioProcessor constructs CachedParamValues/maps before publication;
component/controller own shared references. CachedParamValues uses FlaggedFloatCache
separate atomic float values and grouped atomic flags, immutable parameter-ID vector; cache updates/readout can
overlap but storage setup/destruction still need lifecycle exclusion. Every writer/
reader and notification/reclamation must be reviewed before changing its ownership.
Prepared AU mapper/audio scratch and VST3 maps/buffers, DSP prepare/release and
callback/FL locks remain separate Sprint10 blockers.

Registry capture/admission/exit still lock; exit traverses host-sized frames and
notifies a condition variable. Reentrant writers may allocate/free. Traversal is
host-sized. Cross-instance cycles, callback waiting on an off-callback remover of
its own active context, and parameter/registry lock orders remain unqualified.
Rejecting this local callback-origin wait cannot solve arbitrary host dependencies.
It introduces no listener cap, notification loss, gesture coalescing, queue/TLS/
retained payload or audio reclamation. Whole G01 needs zero prohibited plugin work
and immutable prepared resources/nonwaiting audio/off-audio waits/acknowledged
reclamation/safe invalid-lifecycle silence or error in shipping format checks.

All seven keys/nativeIDs, org.algorhythmia.sys01/Algr/Sys1, four programs/order,
schema1exactly40, parameter-only deterministic empty-history recall,10ms smoothing,
shared256 whole-batch rejection/no replay,eight restores,single audio owner and
stereo/generator/disabled-input/VST3 contracts remain. Stable editor relay and
message gestures/undo/recall/visibility remain. Only registry header and fixture
implementation change; shared processor/DSP/editor/VST3 generator unchanged.

Affected native AU callbacks/concurrency/lifecycle/host/performance/soak/signing
proof is invalidated; new finite checks bind their exact bytes only. Historical
packets stay frozen; Sprint08 live-source remains intentionally invalid. Actual
DAW-origin/externalOS/architecture,signing/rights/accessibility/humans/budgets/soak
and MB-04 remain open. No Sprint11 advancement or waiver.
