# Sprint 10 session 10: AU callback exit notification demand

S10-AU-REGISTRY-05 suppresses idle callback-exit condition broadcasts. Sprint10
remains IN PROGRESS; release NO-GO/development-unqualified. Capture/admission/exit
still take the BLOCKING registry gate. Pending removers still require broadcasts;
this repair does not establish nonwaiting exit or whole G01 acceptance.

## Actual loaded negative, frozen before repair

Unchanged session09 diagnostic Release AU fingerprint
c74530ef1f986775c3dbdb73300cad9af1ca7357f3116c58426c65325aa7f3b7,
binary SHA256 64f2fd1e6843e00cee785e445e6986d2c7ee4457fe3423de711c55315a565c59.
The fixture registers 1/8/64/257 duplicates through the actual AU API, sends native
bypass SetParameter and consumes it in real Render on a fresh worker. All callbacks
return, with no removal pending. Complete virtual registry scope observes exactly
1/8/64/257 pthread_cond_broadcast calls, expected zero. Actual captured stack:
interposer -> libc++ condition_variable::notify_all -> AUPropertyRegistry::changed
-> JuceAU::parameterValueChanged -> parameter listeners -> publishNativeParameter
-> JuceAU::Render -> SDK -> AudioUnitRender. This proves unnecessary plugin-triggered
OS notification work; it does not reproduce a deadlock, race or deadline miss.

The condition interposer calls the original function and counts only the observed
thread. First-call stack unwinding is diagnostic work excluded from the existing
allocation/lock probe. Host callbacks only increment atomics; barriers in inherited
ownership tests are fixture-owned. No synthetic PropertyChanged, product observer,
admission change or shipping hook was added for this negative. Existing diagnostics
only delimit the complete real registry path. Historical default and ON bundles
stay separately frozen. A real condition control later observes exactly one call.

Negative exact complete source export, header/fixture, fixture source/executable,
interposer source/library, generated AU/SDK/VST3, generator, cache, compile commands,
loaded bundles, pinned29-file SDK copy and actual report were frozen before repair
in evidence/sprint-10-session-10. Unprivileged first run failed AU registration
before plugin tests; authorized in-process run reproduced the defect, installing
nothing. No frozen bytes were changed. All151source+19reference inputs matched
session09 before editing, HEAD unchanged, no applicable AGENTS.

## Every participant and no missed acknowledgement

The only product change is AUPropertyRegistry.hpp. A gate-owned size_t counts
removal wait scopes. After existing throwing allocations, withdrawal, publication
and writer reclamation, each successful remover constructs stack WaitDemand under
the retained gate before condition_variable::wait can atomically release it.
The predicate still scans ALL matching other-thread frames, including withdrawn
frames and second removers. Demand persists across wakeups and is decremented by
RAII while the lock is still owned, including exceptional wait exit.

Frame exit still holds that same gate, performs the session09 constant-work
incoming-link unlink, then calls notify_all only when demand is nonzero. If exit
wins the gate first, the later remover sees the absent frame in its predicate and
need not wait. If removal wins first, demand is published before the wait releases
the gate; exit sees demand and broadcasts. A woken remover must reacquire the gate
before testing and withdrawing demand. A second remover independently retains
demand until its own acknowledgement. There is no interval in which an admitted
matching frame can disappear without the required wakeup. Immediate/no-match and
same-thread removal scopes do not expose demand to another gate-held exit.

Callback-origin other-thread wait rejection still returns -10863 BEFORE allocation,
withdrawal or demand. An error never acknowledges context release. Same-thread
active/ancestor removal is legal only with contexts alive until those frames return.
Successful off-callback removers withdraw and wait gate-released for every matching
other-thread frame, excluding new matching registration in the host lifetime
protocol. Idle exit adds one gate-held scalar test, no allocation, TLS, cap, table,
queue, listener loss or gesture coalescing. Pending exit broadcasts remain prohibited
and can wake unrelated removal predicates; dispatch/remove/reclaim remain host-sized.

All SDK virtual callers, native bypass, ContextName wrapper-this and fixed updater
still route through the same registry. SDK vector empty, const accessor uncalled,
conditional MIDI source-only. Publication/current/retired, active checks and stack
frames remain gate-owned. Ordered immutable snapshots preserve duplicates and
outer-versus-nested visibility; writer-only retired use_count1 reclamation ensures
normal readers never last-owner delete. Do not remove the gate before EVERY writer,
reader and context has replacement publication and acknowledged reclamation.

Legal wrapper teardown excludes/joins ALL complete wrapper/processor entries, fixed
message owner, never disposal inside callbacks. Updater stop/clear precedes registry
shutdown while wrapper-this lives. Processor removeListener alone acknowledges
neither full updateHostDisplay nor final forwarding. Restore lanes still require
serialized same-role off-audio publishers; recursive stateMutex/concurrent same-role
restore and arbitrary lifecycle remain unqualified. GetMutex stays null.

## Continued listener/cache/resource review

Unchanged pinned ParameterAdapter reads virtual authority, stores atomic raw mirror,
then invokes LockedListeners under recursive CriticalSection, then clears
listenersNeedCalling and sets needsUpdate. Add/remove use the same lock. The flag
stores actually follow listeners.call; raw mirror precedes it. Reverse regular and
final parameter listeners remain under listenerLock, including synchronized stable
editor removal. Processor updateHostDisplay reads list size outside that lock,
gets each pointer under lock and invokes unlocked; final ParameterChangeForwarder
traverses processor listeners without processor listenerLock. None is replaced;
every writer and complete callback lifetime must be acknowledged first.

VST3 cache has separate atomic floats and grouped atomic change flags. Float exchange
is relaxed, followed by flag fetch_or(acq_rel); readout exchanges flag words(acq_rel)
then loads float(relaxed). It reports latest scalar targets, not an event history.
IDs/storage are constructed before shared processor publication; component/controller
references retain storage. Off-message paramChanged writes cache; process drains
flags into host output queues. Setup, destruction, map changes and prepared resources
still require legal lifecycle exclusion. No packed float/flag claim or arbitrary
lifecycle acceptance. Prepared AU mapper/scratch, VST3 maps/buffers, DSP prepare/
release, callback/FL locks and safe invalid-concurrent silence/error remain open.

## Verification boundaries

Final exact results and hashes are in checkpoint.json, reports and handoff.
Focused Release verifies zero idle broadcasts1/8/64/257, condition control1,
0/0/3/0 complete single-listener registry probe, older unlink zero visits and
pending broadcasts1/8/64/257 while both selectors wait then acknowledge2. Inherited
active/withdrawn callback-origin errors, ordered257 duplicates, nested64 ContextName,
overlap2/2, second remover, cross-add and context destruction128 remain included.
Separate sanitizer bundles include generated AU/SDK/VST3 and fixture instrumentation.
Session09's genuine default-worker ASan stack overflow remains historical; all new
nested257 fixtures retain explicit8MiB SDK worker stack and join before destruction.
Finite successes do not accept arbitrary stack depth, lifecycle or realtime work.

Preserve identity,seven IDs/keys,four programs/order,schema1exactly40,parameter-only
empty-history recall,10ms smoothing,shared256whole-batch rejection/no replay,eight
restores,single audio owner,stereo/generator/disabledinput/formats,stable editor
atomicdirty/seven authoritative packed float-generation words/synchronized removal/
message gestures/undo/recall/visibility. Generator/SDK/pins/shared processor/DSP/
editor/VST3 implementation unchanged. Affected AU callback/lifecycle/host/validator/
performance/soak/signing proofs need renewal on exact new bytes. Historical inventories
stay frozen, Sprint08 live comparison intentionally invalid. Actual DAW/external
OS/architecture/signing/rights/accessibility/humans/budgets/soak/MB-04 remain open.
No Sprint11 advancement or waiver.

Session10 retained sanitizer tooling failure: the initial ASan condition fixture
used the Release malloc interposer control. ASan allocator interceptors superseded
those bindings (alloc/free0/0,mutex1,unfair3), so the control failed before focused
condition checks; early std::exit left loaded instances alive and emitted actual
JUCE leak assertions. Original executable/library/log/report remain retained. The
condition-only separate sanitized fixture retains real broadcast control1, full
loaded registry/nesting/both selectors/idle counts/legal teardown on SAME AU bytes;
Release separately retains allocation/free/mutex/unfair controls and0/0/3/0. Do not
claim sanitizer allocation interposition or no assertion failure ever occurred.
