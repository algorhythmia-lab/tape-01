# Sprint 10 session 03: VST3 notification suppression ownership

S10-VST3-SUPPRESS-01 is a bounded repair under S07-P1-03. Sprint 10 remains
IN PROGRESS and release remains NO-GO/development-unqualified. It does not
qualify listener/callback/FL locks, resource mutation, host implementations,
arbitrary listener work or invalid concurrent lifecycle.

## Actual failure retained before repair

The exact diagnostic Release bundle fingerprint is
76cf1dc7ca8b42cea1f56a0fa76629a8861591159d8547fe41442a87f5d7eb15;
executable SHA256 3dd4788184c634dce661894ba7b74d583cde99758e2051cc593928e1f1381a68.
Two fresh audio threads each allocate once: calloc -> dyld ThreadLocalVariables
-> tlv_get_addr -> JuceVST3Component::processParameterChanges -> process.
The diagnostic hooks only observe the original decisions. Bundle, fixture,
generator, generated wrapper and probe were frozen before changing suppression.

Stopped controller FLUX publication reenters BIAS controller publication and an
external same-instance FLUX writer. Upstream setter always restores false, so
only two final callbacks are suppressed instead of three; two leak through.
Moving an independent same-thread second-instance write before inner publication
reproduces translation-unit TLS interference: zero independent callbacks instead
of one. Both fixture versions and actual reports remain. The second case converts
the prior cross-instance hypothesis into an observed failure. It does not depend
on a claimed TSan failure or a warm allocation measurement.

Frozen reports/identities are in evidence/sprint-10-session-03. The frozen fixture
LC_RPATH references /private/tmp/sys01-s09-repair/apps/desktop; if absent use
DYLD_LIBRARY_PATH pointing to the absolute evidence directory. Never edit frozen
bytes. Probe SHA256 59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.
The alloc/free/mutex/unfair negative control verifies interposition. Mutex3 remains
in the focused complete64-frame entry; the existing full1024-frame probe had9.

## Every suppression writer and shared lifetime

References are to pinned juce_audio_plugin_client_VST3.cpp, normalized line numbers.

| Path | Participating owner and lifetime |
|---|---|
| JuceAudioProcessor396 / component constructor2677 | Component creates one reference-counted shared processor before host publication. Its owning unique_ptr contains SYS01 processor/parameters. Component and connected controller retain VSTComSmartPtr references. Suppression is a member of that shared object, initially empty. Constructor no longer resets translation-unit storage. |
| Controller connection1238 / installAudioProcessor1715 | Direct query and message fallback install the same shared object; parameter wrappers retain references to its parameters. Controller terminate removes the processor listener before releasing its shared reference. Component disconnect/destruction and controller teardown still require legal callback exclusion; no concurrent lifetime protocol is claimed. |
| processParameterChanges3657 | Single process owner per component publishes native points. It uses the shared process lane for synchronous setValueNotifyingHost through all regular/final listeners. A stopped transport still receives process calls; vst3IsPlaying is derived from ProcessContext, not setProcessing. |
| Controller Param::setNormalized967 | VST3 IEditController::setParamNormalized is UI-thread-only in pinned ivsteditcontroller.h527. When transport is stopped it can publish while process still runs. Uses the shared control lane. This is why the AU single-native-publisher field cannot be copied here. |
| paramChanged1472 / final listener | Reads both lanes for this shared processor and compares current thread identity. Never consumes suppression. External other-thread or other-instance setters pass. Message-thread independent forwarding may call EditController::setParamNormalized and therefore Param::setNormalized; any resulting native publication also uses control. Other threads update CachedParamValues only. |
| OwnedParameterListener1647 | Wrapper-owned bypass/program listeners forward to the same controller paramChanged and shared state. Catalogue bypass is regular in SYS01; the owned bypass fallback is not selected. Synthetic program and its UI restore behavior remain unchanged and are outside the off-audio restore contract when invoked on audio. |
| setBypassed2850 / private-state decode2870 | Only caller is setJucePrivateStateInformation from component state decode. IComponent::setState is UI-thread-only, including Processing state (ivstcomponent.h199). Uses shared control lane, can nest within state. SYS01 supplies regular bypass, so private wrapper bypass path is unselected but still repaired. |
| setStateInformation2903 / inSetState | UI state can legally overlap audio process. The old non-atomic controller bool was read in paramChanged on audio. It is now a controller-owned atomic identity lane scoped on stack; gesture reads also compare identity. State suppresses only synchronous notifications on its publishing thread. Nested state scope restores predecessor; recursive SYS01 restore under stateMutex remains unqualified. Unconnected component uses a local empty lane. |
| Controller setComponentState1193 | Reads authority and calls setParamNormalized on serialized UI owner; resulting stopped native publications use control lane. Controller base setState has no additional suppression writer. |
| Editor/APVTS/undo/direct/external writers | Do not mutate lanes; synchronous final callback only reads them. Message-thread final forwarding can enter the reviewed controller path. Off-message forwarding does not acquire a lane. Existing framework synchronization/lifetime is retained. |

## Fixed storage and nesting proof

NativeParameterSuppression contains exactly two lock-free atomic ThreadID lanes:
process and control. Each lane has one concurrently active native owner under the
single process owner / serialized VST3 UI contract. They can overlap each other.
A Lane::Scope exchanges current identity, retains predecessor in its own stack
frame, then restores predecessor on every exit. Same-thread nesting is unlimited
by storage capacity; all live frames follow synchronous stack unwinding. Readers
never dereference the identity or retain a frame address. No historical thread
entries, thread registry, heap, TLS/dyld bootstrap, global suppression flag,
traversal, retry loop or audio reclamation exists. ID reuse after a publisher
returns cannot leave a stale record. Atomic ThreadID has a compile-time lock-free
requirement; pinned macOS accessor is pthread_self.

The separate state lane follows the same UI owner contract. Process/control
identity comparison prevents UI state from suppressing unrelated audio-thread
external writes. Invalid multiple process owners or concurrent UI writers on one
instance are not legitimized by atomic storage: stack restoration requires role
ownership. The fixture forces legal stopped audio/UI publication overlap on
separate keys, plus independent external writers. It does not use listener
waiting as shipping behavior; barriers live only in fixture callbacks outside
measured audio segments.

No ingress is held across listeners and no listener/callback/state/FL lock is
removed. Existing stable editor dirty relay, seven authoritative packed words,
undo/recall/visibility and synchronized relay removal remain unchanged. Identity,
keys/native IDs, four programs/order, schema1 exactly40, deterministic empty-history
recall,10ms smoothing, shared256 whole-batch rejection/no replay,eight coherent
restore slots,single audio owner and stereo effect/generator/disabled input remain.

## Remaining ownership work

APVTS ParameterAdapter::parameterValueChanged still updates atomic raw mirrors
and invokes LockedListeners::call under CriticalSection185. Regular parameter
listenerLock serializes reverse regular callbacks and final processor listener.
AudioProcessor listener dispatch and CachedParamValues must also be traced before
removing locks. Arbitrary public listeners can allocate/reenter and must have a
replacement lifetime/publication contract; the editor relay alone does not solve it.

Generated AU sendAUEvent writes shared auEvent fields1457-1459 before invoking
AUEventListenerNotify; restoringState is non-atomic at RestoreState955 and read
in sendAUEvent1454 and bypass parameterValueChanged1498. Bypass calls SDK
PropertyChanged1499 even during native publication. These are concrete trace
findings, not yet reproduced new defects. AudioProcessorChangedUpdater explicitly
notes PropertyChanged may allocate and uses AsyncUpdater; pinned SDK listener
registry mutation/dispatch and OS event behavior need complete instrumentation.

Prepared audio maps/scratch/DSP lifecycle still use existing locks. Sprint10 needs
immutable prepared resources, nonwaiting audio acquisition, off-audio configuration
waits and acknowledged reclamation for every writer; invalid lifecycle requires
safe silence/error. Whole-entry zero prohibited operations is not achieved by
this suppression repair. Actual DAW-origin/external OS/architecture qualification,
production signing/rights/accessibility/humans/performance/soak and MB-04 remain open.
Historical evidence/seals stay unchanged; new VST3 listener/state changes invalidate
affected native/host/lifecycle/performance/soak/signing proof. New reports name new
bytes and finite sanitizer checks do not prove universal realtime safety.

## Verified local boundary

Focused Release probe: fresh instances allocation/free0/0,mutex3; nested3/3,
cross-instance1/1,overlapping process/control native2/external1/instance1,
concurrent two-instance native128/128 and external128/128,state native7/external2/
other-instance1 PASS. Expanded state fixture holds a UI restore regular callback
while another thread writes a different key; no recursive restore is asserted safe.
Release, separate ASan/UBSan and TSan hook-ON matrices pass. Final default Release,
ASan/UBSan and TSan matrices pass, including strict1274/callback/VST3/seals and
adapter/recall/VST3 3/3. Notifications pass seven-key direct readback, nested
ordering and64 attach/detach cycles in all trees. Release editor3072/four scales
and separate sanitizer gesture/undo/recall/close checks pass. Core16/16,format
and three pinned generator drift rejections pass. No observed final assertion,
sanitizer or race failure in these finite runs. Generated AU/SDK/VST3 and both
suppression fixture flags verify; hooks/probe linkage absent final defaults.
Full default Release VST3 fresh1024 alloc/free0/0,mutex9; default AU fresh1
alloc/free0/0,mutex9. Later AU hostile traffic remains mutex603 in the retained
probe. These are measurements, not whole realtime acceptance.

Initial Release/TSan AU checks failed at sandbox factory registration, before
plugin tests; authorized temporary in-process matrices pass. First repair build
failed because suppression member was private; public accessor fixed generation,
then successful rebuild. Both frozen negative rechecks actually reproduce TLS/
nesting/cross-instance failures. The second authorized recheck was redundant,
requested after incorrectly inferring an exit1 meant sandbox loading failure;
its actual log again reproduces the original defect. No frozen bytes changed.
