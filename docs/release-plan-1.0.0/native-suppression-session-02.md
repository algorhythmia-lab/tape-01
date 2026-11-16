# Sprint 10 session 02: AU native notification suppression

S10-AU-SUPPRESS-01 is a bounded repair under S07-P1-03. Sprint 10 remains
IN PROGRESS and the release remains NO-GO/development-unqualified. It does not
qualify callback locks, arbitrary listener work, host/OS callbacks or invalid
concurrent lifecycle/resource mutation.

## Failure retained before repair

The dedicated loaded-AU fixture observes the actual final AudioProcessor listener
through a default-OFF diagnostic hook. The frozen bundle fingerprint is
8ef83849f74ae5b3d9ab3c3ff8d5caaeb7d9aa1ba2d360df9a3e3ac96091637e;
executable SHA256 3c4a76ca73ea3386f9b6dbf090b045af52d8f934aab5e8d5ef84cda03a4a473b.
Both instances allocate once on their first fresh audio-thread Render, at
operator new inside JuceAU::Render. Native publication of FLUX reenters external
FLUX and BIAS notifications: only one final callback is suppressed (expected
three); four callbacks pass through (expected two independent thread/instance
callbacks). A failed publication CAS then leaves the consume/reset flag true
and suppresses an unrelated following notification. Legal Reset/inactive
publication already passed the simple lifecycle check.

Reports, bytes, generated wrapper, fixture source and probe library are frozen in
evidence/sprint-10-session-02. The first sandbox registration failure is retained;
subsequent explicitly authorized in-process runs reproduce the actual failure.
No AU installation occurs. DYLD_LIBRARY_PATH may point to the absolute retained
evidence directory if the frozen executable's build-directory rpath is absent;
never modify frozen bytes. Probe self-test verifies allocation/free/mutex/unfair
interposition. This is a dedicated fixture, not actual DAW-origin qualification.

## Participating owners and lifetime

| Writer/reader | Ownership |
|---|---|
| Constructor/destructor | Off audio, before publication/after all callbacks excluded. Atomic owner is an instance member, initially null; no thread registry, holder or dynamic cleanup. Wrapper lifetime still requires legal host exclusion. |
| Active SetParameter/Schedule/MIDI producers | Existing incomingMidiLock protects fixed ingress. They do not write suppression or call parameter listeners while holding ingress. |
| Render publication | One audio owner per instance. Each masked target enters a stack scope outside ingress around synchronous publishNativeParameter and all regular/final listeners. CAS failure also exits the scope. |
| Inactive publication | Legal lifecycle excludes Render and serializes native setters. Scope covers setValueNotifyingHost, including synchronous reentrant native setters on this thread. |
| Flush at prepare, Reset, Cleanup | Same legal lifecycle owner, no Render overlap. Capture under ingress, release ingress, scope conditional publication, restore prior owner on every return. |
| Editor/APVTS/undo/restore/arbitrary external setter | Does not mutate suppression. Final listener loads the atomic identity and compares current thread. Other-thread notifications pass even during native publication. Same-thread nested callbacks inherit active suppression for the complete native publication scope. |
| Final AudioProcessor listener | Reads suppression, never consumes/resets it. Reverse regular listeners may nest before the final callback; the outer scope stays active until complete dispatch returns. |
| Diagnostic hooks | Default OFF. Hooks installed while callbacks quiescent; immutable through worker completion. Fixture listener remains registered until synchronous removeListener returns. |

NativeParameterSuppression stores a lock-free atomic JUCE ThreadID (on this pinned
macOS path getCurrentThreadId is pthread_self). Scope exchanges current identity,
keeps the predecessor on the stack, and restores it on exit. Arbitrarily nested
same-thread native publication needs no heap, bounded thread table, global flag,
TLS bootstrap, traversal or reclamation. Thread IDs are compared only while a
live synchronous publisher scope exists, so reuse after thread exit has no stale
entry. Concurrent independent instances own different atomics. Concurrent native
publishers on one instance remain excluded by the existing single Render owner
and legal lifecycle contract; this is not a new concurrent lifecycle gate.

The final reader can linearize before scope start/after scope exit on another
thread and remains independent. Atomic ordering prevents races in identity access;
no pointer is dereferenced. No existing ingress/listener/callback/state/FL Studio
lock is removed. Native IDs, seven keys, parameter tickets, captured audio epochs,
state/schema/programs/smoothing/batch rejection and editor relay are unchanged.

## Local verification

The repaired dedicated Release probe observes alloc/free 0/0 for both fresh AU
instances; five mutexes remain. Nested external callbacks 3 suppressed/2
independent, invalidated CAS 0 suppressed/2 independent, lifecycle 2/1, native
inactive reentry 3/1, and concurrent two-instance 128/128 checks pass. The full
AU fresh 1-frame callback probe observes alloc0/mutex9, including a separate
probe executable on the final hook-OFF Release AU bundle; VST3 fresh1024 still
observes alloc1/mutex9 with calloc -> dyld -> tlv_get_addr. Mutex counts and host
costs are retained as unresolved whole-entry work, not accepted realtime behavior.

Separate Release, ASan/UBSan and TSan ON/OFF results and exact bundle identities
are recorded in the session handoff, reports and final-identities.json. These
are finite checks with generated AU/SDK/VST3 instrumentation; they do not prove
all possible lifecycle or hostile callback behavior. Strict AU uses an independent
loaded-binary oracle; editor/adapter/recall/core results retain their own boundaries.

## Remaining whole-entry paths

AU bypass regular listener still triggers PropertyChanged; AUEventListenerNotify
and SDK/input callbacks call host/OS implementations. Shared auEvent/restoringState
and arbitrary concurrent host notification semantics remain to be traced; this
repair does not certify them. APVTS and JUCE parameter/processor listener locks,
prepared maps/buffers and callback locks remain prohibited plugin-controlled work.

VST3 retains its native thread_local bool. First processParameterChanges uses
setValueAndNotifyIfChanged and InParameterChangedCallbackSetter; controller
paramChanged reads suppression; component construction resets it. Other reviewed
writers are controller Param::setNormalized when stopped, component setBypassed
and state/private-bypass decode; OwnedParameterListener forwards synthetic
parameters to paramChanged. State also independently scopes controller inSetState. The TLS is
translation-unit-wide, so same-thread cross-instance effects also need an explicit
replacement proof. Fresh-entry dyld calloc/TLV remains unresolved. Do not substitute
the AU per-instance field blindly: VST3 processor/controller writers and shared
lifetime must be mapped first. Prepared resources need nonwaiting acquisition,
off-audio configuration waits and acknowledged reclamation for every writer before
callback locks can be replaced. Remain Sprint 10 until its named exit is met.

AU wrapper/listener changes invalidate affected strict AU/callback, concurrency,
sanitation, validators, actual host, performance/soak and signing evidence. New
reports bind named new bytes only; historical evidence/seals are never rewritten.
External architecture/OS/DAW timing, humans/accessibility/rights/signing, performance/
soak and MB-04 remain unqualified. Finite successful tests cannot prove general
realtime safety or satisfy those gates.
