# Sprint 10 session 05: AU property registry and callback lifetime

S10-AU-REGISTRY-01 repairs one concrete reentrant notification defect under
S07-P1-03. Sprint10 remains IN PROGRESS; release remains NO-GO/development-unqualified.
This is a lifetime correctness boundary with blocking serialization, not a
nonwaiting registry or whole realtime qualification.

## Frozen loaded-binary negative

Session05 retained diagnostic Release AU fingerprint
`1a595176adc1a01f79a3519f78a43d5344b20c9b100a8cc7ad45121c3fb900a5`,
binary SHA256 `06aadbb404c388e8911a77a53f48809e106b7b9cfe73ce8602f4226919f4d55a`.
AURegistryTests registers a remover, victim and trailing callback through the
actual AudioUnit API. Native BYPASS SetParameter is consumed by real Render,
which calls the wrapper's regular bypass listener and SDK PropertyChanged.
The first callback removes the middle listener through the actual AU API. The
old vector erase shifts the trailing entry, but range traversal retains the
old end: trailing callback runs twice. Observed first/removed/tail/late is
1/0/2/0, expected1/0/1/0. This is actual host callback reentry in the loaded AU,
not an injected PropertyChanged hook or a claimed sanitizer race.

Fixture, actual original failure report/log, generator, generated wrapper/SDK,
cache/compile commands and exact bundle are frozen under
[evidence](evidence/sprint-10-session-05/negative-identities.json). Initial fixture
compile failures and sandbox registration failure are distinct retained tooling
failures. Authorized temporary in-process execution installed nothing. No frozen
bytes or historical reports were edited.

## Correction to prior guard interpretation

AUInstanceGuard constructs AUEntryGuard(GetMutex()). AUEntryGuard only locks
when the pointer is non-null. AUBase initializes mAUMutex=nullptr, and the pinned
wrapper never calls SetMutex. Thus the presence of AUInstanceGuard does **not**
establish host writer or RestoreState serialization for SYS01. Session04's
serialization attribution to that guard was incorrect. Its actual tests still
prove their forced serialized-restore/per-call-event boundary; concurrent same-role
RestoreState remains unqualified. The atomic lane scope assumes serialized
same-role ownership and cannot substitute for a lifecycle protocol. Historical
session04 evidence is unchanged; this correction supersedes the ownership claim.

## Registry writers, readers and publication

All three AUBase methods are virtual. The generated JuceAU now overrides
AddPropertyListener, RemovePropertyListener and PropertyChanged and routes them
to one instance-owned AUPropertyRegistry. No qualified base dispatch or pinned
SDK accessor caller bypasses the overrides. The original private SDK vector
remains empty. Its construction/destruction still occurs in AUBase, but never
stores published SYS01 callbacks. Pinned SDK/dependency bytes are unchanged.

| Participant | Ownership |
|---|---|
| Constructor | Registry/mutex/empty snapshot initialize before the constructor body. Constructor ContextName registration goes through the override before host publication. Context is wrapper this. |
| Host add | Both callback and context stored in an immutable Entry; under recursive gate a new snapshot copies existing entries and appends, preserving order and duplicates. No fixed listener cap. Newly added entries are visible to the next dispatch, including nested dispatch after add returns; they are absent from the already-running outer snapshot. |
| Host remove, both variants | Under gate, matches are marked inactive and excluded from a newly published snapshot. WithUserData matches exact id/callback/context; legacy variant matches id/callback across contexts. Old snapshots keep storage but skip withdrawn callbacks. |
| Native bypass | Real parameter regular callback calls virtual PropertyChanged, without holding parameter ingress over the host listeners. Still holds existing parameter listener/framework locks. Restore identity suppression unchanged. |
| AudioProcessorChangedUpdater | Latency/list/info/ClassInfo/preset notifications reach virtual override. AsyncUpdater flags, message posting, preset refresh and message lifetime remain unrepaired; no claim of zero off-message work. |
| SDK dispatch | AUBase.cpp161,693-896,1531,1556,1658,1881,1903 and AUBase.h515 use virtual PropertyChanged for max frames, properties, input/layout/state/preset/name/LastRenderError. AUMIDIBase.cpp117/131/180 and AUEffectBase.cpp228 also use virtual dispatch. Numbered trace retains all call sites, including unselected MIDI/effect paths. |
| Const accessor | GetPropertyListeners has no pinned SDK/wrapper caller. No external API exposes this internal C++ vector. |
| Destruction | Wrapper removes synchronized bypass and processor listeners, then shutdown takes registry gate, withdraws entries and reclaims snapshots while wrapper/context are alive. Host must exclude new entries and disposal inside a callback, and obey legal processor/editor/message lifecycle. Registry gate alone does not legitimize concurrent wrapper teardown/resources. |

One recursive mutex covers every registry entry/snapshot/active access and every
callback invocation. Cross-thread removal can acquire it only after in-flight
registry callbacks return; after successful removal returns no future dispatch
can call that registration. Cross-thread callers may destroy their context then.
Same-thread reentrant removal cannot wait for its own stack: its already-active
calling callback must retain its context until that frame returns. Removing a
later callback makes it inactive before the outer snapshot reaches it. Removing
another registration already active in an ancestor has the same stack-lifetime
requirement. Disposal inside a callback is invalid; no reclamation proof is
claimed for that operation. Callback-context ownership is host-owned, whereas
withdrawal/storage/scheduling scaffolding is plugin-controlled.

Every dispatch captures a shared snapshot under gate. Writers retain old storage
in retired **before** replacing current; published snapshots are structurally
immutable, Entry active is gate-protected. A retired snapshot is erased only by
a writer when its use_count is1; gate excludes other-thread capture and recursive
active frames retain references. The reader decrement never becomes last-owner
reclamation. No snapshot or node final delete occurs on normal PropertyChanged
return. A writer reentered on audio may allocate/free; that remains prohibited
work attributable to the plugin API scaffolding. Writer/shutdown reclamation
requires an off-audio caller for realtime qualification. No queue, gesture
coalescing, deferred payload, TLS, ingress extension or removed lock was added.

## Limits and next ownership step

This implementation deliberately retains blocking work. Arbitrary host-sized
traversal is unchanged, and the new recursive gate serializes callbacks. Host
callbacks that wait on another thread which reenters the registry can deadlock;
parameter-lock/registry-lock order across arbitrary parameter callback reentry
is not qualified. Host/OS callback bodies are separately owned but their plugin
submission and scaffolding remain measured. This session does not claim arbitrary
reentry, immutable prepared-resource ownership or safe invalid concurrent lifecycle.

Next session must reproduce the remaining registry wait/order or updater message
operation and choose a coherent nonwaiting publication/reclamation protocol.
Do not remove the gate without replacement for all writers/readers/context
acknowledgement. APVTS LockedListeners, regular/final processor listeners,
CachedParamValues and prepared-resource/lifecycle ownership remain open. The
stable editor relay, all seven parameter IDs/keys, factory programs/order, schema1
exactly40, parameter-only empty-history recall, smoothing/event/restore/audio-owner
and stereo/generator/disabled-input contracts remain unchanged.

Affected AU callback/concurrency/host/lifecycle/performance/soak/signing evidence
is invalidated. New local reports bind new bytes; historical inventories/seals
remain unchanged. External DAW-origin/OS/architecture/signing/rights/accessibility/
human/performance/soak and MB-04 remain unqualified. Sprint10 exit still requires
complete zero-prohibited plugin-controlled callbacks and concurrency/lifecycle
ownership checks. Finite sanitizer/probe passes do not establish that exit.
