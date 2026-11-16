# SYS 01 Sprint 10 session 11 handoff

## Checkpoint

2026-10-20 17:49 America/Santiago (20:49 UTC). Sprint/session10/11.
S10-AU-REGISTRY-06 LOCALLY VERIFIED at matching-entry exit notification demand /
acknowledged waiting remover boundary. Sprint10 IN PROGRESS, release NO-GO/
development-unqualified. Registry gate BLOCKING; G01/actual DAW/external AU/MB-04
remain open. No Sprint11 advancement or gate waiver.

Git root /Users/lhommeduchili/algorhythmia; SYS01 root
/Users/lhommeduchili/algorhythmia/sys-01. HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5
unchanged. No applicable AGENTS. ALL152 source+21 reference inputs matched
session10 BEFORE editing. Dirty/untracked SYS01/unrelated work preserved. No
subagents/install/stage/reset/stash/discard/commit/tag/contact/publication or
personal plugin directory changes. Final source153/reference23 inputs; source
export SHA256 3bd1ef595e97439a86ab3396c828776bfdc1477efb78f4d8473b05a6a28cd354; resume-inputs
SHA256 1238550c47109fc0e6e025fd69060bb635aaf32fe6f79c542afa41cc984899bb. All jobs completed, none to resume.

## Changes and decisions

Only product implementation: apps/desktop/AUPropertyRegistry.hpp. New ownership
record ../au-property-matching-demand-session-11.md and session-owned fixtures.

UNCHANGED session10 diagnostic Release AU fingerprint
56422d1f8b5277374ba3b1c38d2fd2b72f5a95c579c0565d3221767ee58d9f7a,
binaryb13fbb54fc40d9be1cbab781f005dadda9b1a4290be228c3045d453b628445f5.
Off-callback remover waits for held actual SDK NickName frame. Unrelated native
bypass Render callback with same function exits and broadcasts1 instead0, both
real removal selectors. Actual original-call condition interposer observes wait
and broadcast, stack registry->bypass->Render->SDK. Fixture barriers/unwinding
are host-owned. No synthetic PropertyChanged or product negative observer.
This is unnecessary plugin-triggered OS work, not deadlock/race/deadline miss.
Complete negative source/fixture/executable/interposer/generated/cache/SDK/bundle/
report frozen BEFORE repair; never edit frozen bytes. Loader/rpath and sandbox
registration failures retained. Authorized in-process execution installs nothing.

Entry now owns gate-held matching demand count. Remove retains each unique
matching other-thread Entry from ALL live frames, including withdrawn frames,
before mutation. Frame references immutable enclosing snapshot shared_ptr slot;
writer can retain without extra reader shared_ptr operation. Stack WaitDemand
joins intrusive gate-owned writer list, increments matching counts and retains
owners until predicate acknowledgement; destructor unlinks/decrements gate-held,
including exception exit. Add reserves every matching owner before publication;
after ALL throwing work it attaches/counts the new Entry under gate. Newly matching
frames cannot miss wakeup. Failed reserve changes capacity only, no logical mutation.
Exit still constant-work unlink/gate-held, then broadcasts only own entry demand.
Matching broadcasts can still wake unrelated predicates through shared CV.

Both selectors/second removers/withdrawn/new matching frames retain acknowledgement.
Callback-origin rejection remains before allocations/mutation. Errors authorize
NO context destruction; same-thread active/ancestor contexts live until return;
new matching add excluded by host destruction protocol. Gate/current/retired/active/
Frames retained, ordered duplicate snapshots/no cap/nested visibility/writer-only
retired use_count1 reclaim/normal reader never last-owner deletion unchanged.
Retain gate until EVERY writer/reader/context has replacement publication and
acknowledged reclamation. No TLS/table/queue/payload/loss/gesture coalescing.

ALL SDK virtual routing remains; SDK vector empty,const accessor uncalled,MIDI
source-only. ContextName wrapper-this lives through legal shutdown. Teardown joins/
excludes ALL wrapper/processor entries,fixed message owner,never callback disposal;
updater stop/clear precedes registry shutdown. Processor removeListener alone
acknowledges neither complete updateHostDisplay nor final forwarding. GetMutexnull,
restore same-role caller-serialized offaudio/recursive stateMutex limits retained.

APVTS authority->rawmirror->lockedlisteners->flags,regular/final listenerLock,
processor callback lifetime,separate atomicfloat/groupedflags VST3cache/immutableIDs/
sharedcomponent-controllerstorage and prepared maps/scratch/DSP/callback/FLlocks
remain unchanged/open. Preserve identity/7keys+IDs/4programs/schema1exactly40/
parameter-only deterministicempty-historyrecall/10ms smoothing/256wholebatch
rejection+noreplay/8restores/singleaudioowner/stereo/generator/disabledinput/formats
and stable editor relay/synchronized removal/message gestures/undo/recall/visibility.

## Verification

|Check|Exact command registry reference|Result|Boundary|
|---|---|---|---|
|Live comparison|initial-state.json|152+21 match|BEFORE edits|
|Loaded negative|s10s11-negative-authorized|exit1,unrelatedbroadcasts1 bothselectors|Frozen unchanged session10 ON AU|
|Focused condition|s10s11-condition-repair/asan/tsan|control1,unrelated0,idle0,matching1/8/64/257,ack2,unlink0 PASS|Final exact bundles,separate sanitizers|
|New matching publication|s10s11-publication-repair/asan/tsan|sameIDcontext0;originalexit then newframe wait+ack2callbacks PASS|Supplemental direct registry protocol,not loaded negative|
|Native ON/OFF|s10s11-*-final-{on,default}-native|strictAU1274/callback/registry/ownership/suppression/VST3/seals/adapter/recall3of3 PASS|Release/separateASan-UBSan/TSan|
|Editor/updater|s10s11-*-editor/notifications,*-on-updater|nested/direct7/64detach;3072/fourscales;sanitizercontrols;updater128fresh/4x256/64teardown PASS|Finite offscreen CPU/controls-only budgets excluded|
|Core/pins/format/generation|s10s11-core/juce-pin/doctest-pin/format-final/generation|16of16,4377/270,format,3driftrejections PASS|Unchanged shared/pin sources|
|Release registry probe|s10s11-release-registry-probe|alloc/free/mutex/unfair0/0/3/0;larger17/129/515 PASS|Separate allocation/mutex interposer|
|Default whole probes|s10s11-default-au-probe/default-vst3-probe|alloc/free0/0,mutexremain|G01 open|
|Final integrity|s10s11-checkpoint,finalization.json|PASS|OFFhooks/probeabsent,separateflags,frozeninventories|

No observed assertion/ASan/UBSan/TSan diagnostic in final successful logs. Session09
real default-stack ASan overflow257 and session10 allocator-control/leakassertions
stay frozen; nested257 fixtures explicit8MiB SDK stack+join. Never claim no sanitizer
failureever/arbitrary stackdepth/general lifecycle/whole realtime. New sanitizer
condition fixture omits allocator qualification; Release proof separate. Capture/
admission/exit still lock,matchingbroadcasts remain,dispatch/remove/reclaim/adddemand
traversal host-sized,reentered writers allocate/free. Hostdependency cycles and
parameterlockorders remain unqualified. Release editor is unpaced offscreen CPU,
not DAW/driver/actualUIbudget. External validators/platform/DAW/signing/rights/
accessibility/humans/budgets/soak/MB-04 unperformed,retain NO-GO.

Twelve historical inventories291/505/357/364/409/366/397/422/492/470/546/561 unchanged;
Sprint08 live comparison intentionally invalid,neverreseal. CMake/CTest4.4.4 recorded
uvpath,format18.1.8 sibling,AppleClang21/macOS26.6.2/25G83arm64,JUCE8.0.12/doctest2.4.12
pinslock4377/270 unchanged. Probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.
Exact commands/tool/dependency/source/generated/fixture/artifact hashes in evidence.

Final diagnosticsOFF Release AU 7a88104cbb80efd214d3f92a49d0c2e5e9b46e45d692d90d89a8c0763b9cb7bd,
VST3 b74647a2fdb6272dcf9a3381793670a863c0fd0814ec910bc973aea73d924c85; all ON/OFF binary hashes
in final-identities.json. All native temporary trees finalOFF. Frozen rpaths use
absoluteDYLD_LIBRARY_PATH if needed,nevermodifybytes.

## Resume

All jobs complete,none to resume. Immediate session12 action: compare LIVE153+23
against session11 resume inventories FIRST. Read registry gate/capture/admission/
exit/conditionalnotify/WaitDemand/remove/reclaim,EVERYadd publisher and generated
routing/ownership trace. Reproduce/freeze ONE remaining actual loaded-AU wait/order
or callback ownership defect BEFORE ONE bounded repair. Bothselectors,active/
withdrawn/newmatching/secondremovers,nested/overlap,firstthread,ContextName/legal
teardown. Then APVTS/regular+final listener/cache/immutable resource/lifecycle work.
Sprint10 exit needs nonwaitingaudio/offaudiowaits/EVERYwriteracknowledgedreclaim/
safeinvalidsilence-error/completezero-prohibitedshippingcallback+concurrencytests.
Only installedAbletonLive12Suite permitted;REAPERskipped/pluginvalabsent/auvaltemp
requires forbiddeninstall. No advancement/waiver.

## Complete next session prompt

Complete ready-to-paste prompt (includes all inherited detailed requirements):
[session12 prompt](../evidence/sprint-10-session-11/next-session-prompt.txt).

```text
Work in /Users/lhommeduchili/algorhythmia/sys-01.

Continue SYS 01 v1.0.0 Sprint10, session12. This is an implementation session.
Read docs/release-plan-1.0.0/rolling-state.md and handoffs/sprint-10-session-11.md FIRST, then this COMPLETE prompt and au-property-matching-demand-session-11.md. Follow the inherited detailed requirements below with SESSION11 as the current resume authority. Do not resume session10 or session11 jobs: all are complete according to finalization.json.

Before editing, check Git status and all applicable AGENTS. Compare LIVE against evidence/sprint-10-session-11/resume-inputs.json and resume-reference-inputs.json (153 source and23 reference inputs). Consult checkpoint.json/finalization.json for exact hashes and final identities. Session11 began with ALL152source+21reference inputs matching session10; HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5 unchanged, no applicable AGENTS. Preserve dirty/untracked SYS01 and unrelated monorepo work and frozen evidence. No subagents, installation, staging, reset, stash, discard, commits, tags, contact or publication. No personal plugin directory changes. Remain Sprint10, release NO-GO/development-unqualified; no advancement or gate waiver.

Read session11 checkpoint.json, final-identities.json, initial-state.json, negative-freeze.json, negative-identities.json, ownership-trace.json/provenance, sanitizer-flags.json, fixture-identities.json, failures.json, invalidation.md, historical-integrity.json, finalization.json, commands.json and actual logs. Session11 S10-AU-REGISTRY-06 LOCALLY VERIFIED at matching-entry exit notification demand / acknowledged waiting remover boundary. This does not qualify nonwaiting capture/admission/exit or whole G01.

Actual loaded negative uses UNCHANGED session10 final ON Release AU fingerprint56422d1f8b5277374ba3b1c38d2fd2b72f5a95c579c0565d3221767ee58d9f7a, binaryb13fbb54fc40d9be1cbab781f005dadda9b1a4290be228c3045d453b628445f5. Off-callback removal waits for a held actual SDK NickName frame. A different registration with the SAME callback pointer exits in actual native bypass Render and broadcasts once, expected0. Both real WithUserData and SDK Lookup legacy selectors reproduce broadcasts1/waited1/calls1 instead0/1/1. External original-call interposer observes real pthread_cond_wait and pthread_cond_broadcast, with actual stack registry changed -> bypass listener -> Render -> SDK. No synthetic PropertyChanged or product negative observer. Host barriers/unwinding separately owned. This is unnecessary plugin-triggered OS notification work, not reproduced deadlock/race/deadline miss. Complete negative source/generated/SDK/cache/bundle/report/fixture/probe frozen BEFORE product repair. Never edit frozen bytes. Missing rpath and sandbox registration failures are retained separately; authorized in-process execution installs nothing.

Only product implementation change is AUPropertyRegistry.hpp. Each Entry has a gate-owned waitingRemovals count. Before withdrawal remove allocates a UNIQUE retained list of matching other-thread entries from ALL live frames, including withdrawn frames. Frame references its immutable enclosing snapshot shared_ptr slot so writer retention adds no reader shared_ptr ownership operation. Stack WaitDemand counts matching entries, joins an intrusive gate-owned writer list and retains entries until acknowledgement; destructor unlinks constant-work and decrements gate-held, including exceptional wait exit. Every ADD participates: reserve all matching pending owners BEFORE publication and after all throwing work attach/count new entry under gate. Newly matching frames therefore wake the existing predicate; no reader demand-list scan or callback cap. Failed reserve changes capacity only, no logical mutation. Remove predicate remains ALL matching other-thread frames, including new registrations; host destruction still excludes new matching add. Callback-origin required other-thread wait rejection remains BEFORE allocations/mutation. No failed removal authorizes context release. Retired use_count1 writer reclamation and normal reader never-last-owner deletion remain.

Frame exit retains constant-work incoming-link unlink under same blocking gate, then checks its own entry count. Idle/unrelated exits broadcast0. Matching pending exits still broadcast1/8/64/257; global CV may still wake unrelated predicates when a matching exit requires notification. Multiple remover scopes retain independent counts; second removers retain withdrawn entries. Exit-first leaves no matching frame for later predicate; remover-first publishes demand before condition wait releases gate. Gate/capture/admission/exit/current/retired/active/frame ownership unchanged. DO NOT remove gate until EVERY writer, reader and context has replacement publication and acknowledged reclamation.

Final Release and SEPARATE ASan-UBSan/TSan ON/OFF matrices PASS on exact checkpoint bytes. Focused condition control1, unrelated bothselectors broadcasts0/waited1/calls1, idle0 at1/8/64/257, matching pending broadcasts1/8/64/257 and both removers acknowledged2, older unlink0 predecessor visits PASS. Supplemental DIRECT registry protocol test (not loaded negative) passes same-ID/different-context broadcasts0 and new matching publication: original frame exits, remover waits again for new frame, then acknowledges after new frame returns, bothselectors. Inherited active/withdrawn callback-origin rejection, overlap2/2, order257, nestedContextName64, first-thread and context destruction128 PASS. Updater128fresh/4x256/64teardown, editor nested/direct7/64detach, Release3072/four scales, sanitizercontrols/gesture/undo/recall/close, strictAU1274/callback/ownership/suppression/VST3/seals/adapter/recall3of3/core16/16/pins4377/270/format/3generator drift rejections PASS. Final defaultsOFF hook/probe absence and separate generated AU/SDK/VST3/fixture sanitizer flags verified. Separate Release registry probe alloc/free/mutex/unfair0/0/3/0 with1/8/64/257 mutex3/17/129/515; whole default AU/VST3 alloc/free0/0 but locks remain (AUfresh1mutex9/hostile603,VST3fresh1024mutex9). Never interpret finite zeros/sanitizers as whole realtime acceptance.

All12historical inventories unchanged:291/505/357/364/409/366/397/422/492/470/546/561. Sprint08 live-source intentionally invalid; never reseal. Session09 genuine ASan default-worker stack overflow257 and session10 allocator-control/leaked-instance assertions remain retained. Session11 nested257 fixtures use explicit8MiB SDK worker stacks and legal joins. Condition-only sanitizer fixture omits allocator qualification; separate Release allocation/mutex interposer proof retained. No claim of no sanitizer failure ever, arbitrary stack-depth or general lifecycle. New success logs are checked for observed assertions/ASan/UBSan/TSan diagnostics.

Session12: Start complete AUPropertyRegistry.hpp gate/capture/admission/exit/conditional-notify/WaitDemand/remove/reclaim and generated native_au_adapter.py routing plus ownership trace. Reproduce and freeze ONE remaining actual loaded-AU wait/order or callback ownership defect BEFORE ONE coherent bounded repair. Include every add/remove demand publisher and withdrawn/new-matching/second-remover entry owner. Bothselectors, active/withdrawn frames, nested/overlap, firstthread, ContextName wrapper-this and legal teardown required. Capture/admission/exit waits, matching broadcasts, host-sized dispatch/remove/reclaim and audio-reentered allocating writers remain. Do not merely repeat audits or remove locks without complete ownership replacement. Continue APVTS regular/final listener ownership, separate VST3 cache and immutable prepared resources/lifecycle review after chosen registry boundary. No cap, listener loss, gesture coalescing, TLS/table/queue or retained host payload workaround.

Tools remain CMake/CTest4.4.4 at recorded uv path, format18.1.8 sibling, AppleClang21/macOS26.6.2/25G83arm64; JUCE8.0.12/doctest2.4.12 exact pins/cache, probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54. Temporary native trees /private/tmp/sys01-s09-repair,asan,tsan finalOFF; core/private/tmp/sys01-s09-session02-core. Reconstruct missingtemps from pins/cache/source export. Absolute DYLD_LIBRARY_PATH may resolve frozen rpaths; never modify frozen bytes. Only installedAbletonLive12Suite permitted, REAPERskipped/pluginvalabsent/temporaryAU auvalrequiresforbiddeninstall. ActualDAW-origin/externalOS/architecture/signing/rights/accessibility/humans/budgets/soak/MB-04 remainopen.

Preserve all identity/IDs/keys/programs/schema40/empty-history recall/smoothing256batch rejection/eightrestores/singleaudioowner/stereo/generator/disabledinput/editor relay/removal/gesture contracts in inherited detail. Use NEW evidence/sprint-10-session-12 and unique command IDs, actual argv/cwd/UTC/exits/source/tool/dependency/artifact hashes/failures/invalidation. Complete appropriate regressions, separate sanitizers, generated wrapper/first-thread/lifecycle checks, final defaultsOFF absence, source/evidence inventories, rollingstate, new template handoff and COMPLETE session13 prompt. Remain Sprint10 until full exit, no waiver.

Session11 exact source export SHA2563bd1ef595e97439a86ab3396c828776bfdc1477efb78f4d8473b05a6a28cd354; resume-inputs SHA2561238550c47109fc0e6e025fd69060bb635aaf32fe6f79c542afa41cc984899bb. Final diagnosticsOFF Release AU fingerprint7a88104cbb80efd214d3f92a49d0c2e5e9b46e45d692d90d89a8c0763b9cb7bd, binary3740106d611150e8506b2eb902d124e7555cc5677064df39694e27bd47054c8c; VST3 fingerprintb74647a2fdb6272dcf9a3381793670a863c0fd0814ec910bc973aea73d924c85, binary4cdf0a26240bc0a75f0c8ccd773f08f5f2e6d9c3f5a630218da1daa59830aae9. All jobs complete,none to resume.

The following SESSION10-to-11 prompt is retained as COMPLETE inherited ownership and verification detail. Its session numbers, current inventories, next action and artifact identities are historical; current SESSION11 authority above takes precedence. Read all named historical contracts; do not rerun or overwrite historical evidence.

Work in /Users/lhommeduchili/algorhythmia/sys-01.

Continue SYS 01 v1.0.0 Sprint 10, session 11: remaining AU registry admission/exit wait/order and acknowledged callback ownership. This is an implementation session. Reproduce and freeze one concrete remaining plugin-controlled loaded-AU wait/order or callback ownership defect, implement one coherent bounded repair, and verify it. Remain Sprint10; no Sprint11 advancement or gate waiver.

Read docs/release-plan-1.0.0/rolling-state.md and handoffs/sprint-10-session-10.md FIRST, then au-property-notification-demand-session-10.md, au-property-exit-order-session-09.md, au-property-removal-context-session-08.md, au-property-callback-ownership-session-07.md, au-updater-ownership-session-06.md, au-property-registry-session-05.md, handoffs/sprint-10-session-08.md, au-host-notifications-session-04.md (SDK guard claim superseded), native-vst3-suppression-session-03.md, native-suppression-session-02.md, native-notifications-session-01.md, au-ingress-ownership-session-03.md, docs/release-1.0.0/native-ownership.md, and Sprint10/acceptance/evidence-invalidation/handoff sections of audit-production-plan-2026-10-18.md. Read session10 checkpoint.json, final-identities.json, ownership-trace.json, ownership-trace-provenance.json, initial-state.json, negative-identities.json, negative-freeze.json, fixture-identities.json, failures.json, invalidation.md, finalization.json and this COMPLETE prompt, plus session09 checkpoint/failures/finalization for the retained stack-overflow boundary. Consult exact logs for counts/limits.

Check Git status/applicable AGENTS and compare LIVE with session10 resume-inputs.json and resume-reference-inputs.json BEFORE edits. Session10 began with all151source+19reference inputs matching session09. Session10 final source inventory contains152 inputs and final reference inventory contains21; consult checkpoint for exact hashes. Gitroot /Users/lhommeduchili/algorhythmia, HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5 unchanged. No applicable AGENTS found. Preserve dirty/untracked SYS01 and unrelated monorepo work, all frozen evidence. No staging/reset/stash/discard/overwrite/commit/tag/publication/subagents/contact/install/personal plugin directory changes. No dependent background job remains to resume; consult finalization authority.

Sprint09/S09-OWN-01 complete at named local boundary. S10-NOTIFY-01/AU-SUPPRESS/VST3-SUPPRESS/AU-UPDATER-01 remain locally verified at bounded contracts. AU-NOTIFY only caller-serialized off-audio restore identity/per-call stack events. S10-AU-REGISTRY-02 releases the gate around host callbacks; REGISTRY-03 rejects callback-origin required other-thread matching-frame wait before any allocation/withdrawal/publication. Session09 S10-AU-REGISTRY-04 locally verifies constant-work intrusive frame unlink and acknowledged exit order. Session10 S10-AU-REGISTRY-05 locally verifies idle exit notification demand while retaining acknowledged waiting removers. Registry gate remains BLOCKING, Sprint10 IN PROGRESS, release NO-GO/development-unqualified. Whole G01/actualDAW-origin/externalAU/MB-04 remain open.


Session10 actual loaded negative uses unchanged session09 ON Release AU fingerprint c74530ef1f986775c3dbdb73300cad9af1ca7357f3116c58426c65325aa7f3b7, binary64f2fd1e6843e00cee785e445e6986d2c7ee4457fe3423de711c55315a565c59. Actual native bypass SetParameter consumed in real Render dispatches1/8/64/257 registered duplicates on fresh workers. With no remover pending, each frame exit calls pthread_cond_broadcast:1/8/64/257 instead0. External condition interposer counts original calls and captures actual stack condition_variable::notify_all -> AUPropertyRegistry::changed -> bypass -> Render. No product observer/admission decision added for negative, no synthetic PropertyChanged. Existing diagnostics delimit full registry. This is actual unnecessary plugin-triggered OS notification work, not reproduced deadlock/race/timing miss. Host empty callbacks/barriers and probe stack unwinding are separately owned. Real condition control observes1; allocation/mutex control1/1/1/1 retained. Frozen complete source/header/fixture source+executable/interposer source+library/generatedAU+SDK+VST3/generator/cache/compile/bundles/pinned29fileSDK/report copied before repair. Sandbox first factory registration failure and formatting violations are retained separately. Authorized in-process testing installs nothing.

Session10 ONLY product change is AUPropertyRegistry.hpp. A gate-owned size_t waitingRemovals tracks stack WaitDemand scopes after allocations/withdrawal/publication/reclaim, before acknowledged.wait can atomically release gate. RAII decrements with gate owned, including exceptional wait exit; demand spans spurious wakeups. Frame exit still gate-held constant-work unlink, then notify_all only when demand nonzero. If exit wins gate first, later remover predicate sees no frame; if remover wins first, exit sees demand and broadcasts. All simultaneous removers retain demand until their own acknowledgement; no required wakeup is lost. Same-thread/no-match immediate scopes cannot expose demand to other gate-held exits. Callback-origin rejection remains before any mutation/allocation/demand. Both selectors/second removers/withdrawn frames retain full acknowledgement. Idle broadcasts0 at1/8/64/257; pending broadcasts1/8/64/257, both removers wait then ack2. No cap/TLS/table/reader allocation/deletion/notification loss/gesture coalescing. Capture/admission/exit mutex remain, complete registry0/0/3/0; pending broadcasts and host-sized dispatch/remove/reclaim/audio-reentered allocating writers remain prohibited/open. This is NOT nonwaiting exit or whole G01 qualification.

Final session10 exact formatted-source Release and separate ASan-UBSan/TSan ON/OFF native/editor matrices, condition controls, first-thread and lifecycle tests, strictAU1274, inherited registry/ownership/suppression/VST3/seals/adapter/recall3of3, updater128fresh/4x256/64teardown, editornested/direct7/64detach/Release3072four scales/sanitizercontrols/gestures/undo/recall/close, core16of16, format and3generator drift rejections are recorded in checkpoint/logs. Consult actual counts and diagnostics; do not infer more than successful final logs. Final rebuilt defaultsOFF hooks/probe absent, generated/fixture separate sanitizer flags verified. Session09 original real ASan default-stack overflow remains frozen; all nested257 fixtures retain explicit8MiB worker stack and join. All jobs completed at finalization; no job to resume. Eleven historical inventories291/505/357/364/409/366/397/422/492/470/546 remain unchanged. Final session10 inventory is authority; never reseal historical bytes. Source/binary integrity is not release acceptance.

Session09 actual diagnostic Release negative AU fingerprint92abd2f1546ae62063516334d406da0d1d8f47e9f5bf0c1aed7eb038c97dba37, binaryf3b183212166fd41a0ca731bd940f01f55a22fb2b8845ac2ed51cfb739ad07ec. Native bypass SetParameter consumed by real Render admits older A; newer real SDK NickName callback nests1/8/64/257 frames and holds. A returns before those frames. Old RAII unlink visits1/8/64/257 predecessors under gate, expected0. This is observed host-sized exit work, NOT reproduced deadlock/race/timing miss. DefaultOFF observer counts only actual predecessor traversals; added to old algorithm for negative, does not change admission/order, no synthetic PropertyChanged. Historical shipping/default baseline separately retained. Negative complete source export/fixture source+executable/generatedAU+SDK+VST3/generator/cache/compile/bundle/pinned29fileSDK copy and report frozen BEFORE repair. Never alter those bytes. Baseline sandbox register failure separately retained; authorized in-process execution installs nothing. Unique command ID collision retained as tooling failure before configure.

Session09 repair only AUPropertyRegistry.hpp and AURegistryTests.cpp implementation. Admission under retained gate sets stack frame incoming-link pointer to registry head, rewires previous head incoming link to new frame.next, publishes head. Exit under same gate writes *previous=next, then if next exists next->previous=previous; notifies waiters. Predecessor exiting first transfers its successor's incoming link before its own stack disappears; successor exiting first updates live predecessor.next. Head/middle/tail/concurrent/nested frames use same invariant. Inactive unlinked frame does nothing. No list search/TLS/table/cap/reader allocation or reclamation. Observer reports0 visits; source proves one mandatory pointer update and at most one successor backlink update. Gate acquire and notify still prohibited; this is NOT nonwaiting exit.

Remove still scans ALL live frames under gate including withdrawn frames/second removers. ANY same-instance caller-thread callback frame plus matching OTHER-thread frame returns CannotDoInCurrentContext(-10863) before allocation/withdrawal/publication. Error authorizes NO context destruction; host retains/retries outside callbacks after owners joined, excluding new matching registration. Same-thread active/ancestor withdrawal remains legal but contexts live until frames return. Off-callback remove withdraws then CV-waits with gate released for all matching other-thread frames. Active check/admission linearize with withdrawal under gate. Ordered immutable snapshots/duplicates/no cap/outer-vs-nested visibility/writer-only retired use_count1 reclaim/normal reader never last-owner deletes remain. DO NOT remove registry gate until EVERY writer, reader and context has replacement publication and acknowledged reclamation.

All SDK virtual Add/Remove/PropertyChanged callers still route: state/RestoreState/layout/channel/stream/input connection/render callback/MaximumFramesPerSlice/LastRenderError/name/preset and conditionalMIDI/hotmap. SDK vector empty, const accessor no pinned caller. MIDI runtime unselected/source-only. ContextName constructor callback owns wrapper-this through legal shutdown; nested64 and surviving registration verified. Native bypass and fixed message/timer updater use same registry. Legacy no-context public function unavailable arm64; real loaded SDK Lookup selector tested. Generator/SDK/pins/shared processor/DSP/editor/VST3 implementation unchanged.

Legal wrapper teardown excludes and joins ALL complete wrapper/processor entries, fixed message owner, never disposal inside callback. Updater stop/clear before registry shutdown while wrapper-this lives. Processor removeListener alone acknowledges neither complete updateHostDisplay nor final forwarding. Optional AUEntryGuard locks only nonnull GetMutex; SYS01 null. Restore lanes assume caller-serialized same-role off-audio; concurrent same-role/recursive stateMutex restore, owner migration and arbitrary concurrent lifecycle unqualified. Updater60Hz construction timer/off-message atomicflags+immutable pthread comparison/message synchronous nesting/existing flags coalescing retained; stopped loop delays, closed editor polls.

Start with complete registry probe0/0/3/0 and gate/capture/admission/exit/conditional-notify/WaitDemand/remove/reclaim in AUPropertyRegistry.hpp, native_au_adapter.py generated overrides and ownership trace. Capture/admission/exit still block; dispatch/remove/reclaim host-sized, reentered audio writers allocate/free. Cross-instance cycles, callback waiting for its own off-callback remover, arbitrary parameter reentry/lock orders remain unqualified. Choose an actual remaining defect before repair; no timing acceptance from finite warm zeros. Distinguish plugin scaffolding from host fixture barriers/OS bodies. Include first-thread entry, both removals, active/withdrawn/nested/overlapping frames, ContextName wrapper-this and legal teardown. No listener cap, loss or gesture coalescing.

Then APVTS ParameterAdapter/LockedListeners, regular/final parameter and processor listeners, CachedParamValues and immutable resources/lifecycle. Pinned source confirms virtual authority -> atomic raw mirror -> APVTS listener call under recursive CriticalSection -> listenersNeedCalling=false/needsUpdate=true; add/remove same lock. Flag stores actually follow listener call; raw mirror precedes it. Parameter reverse regular+final dispatch retains parameter listenerLock and stable editor synchronized removal. Processor updateHostDisplay reads size outside lock, obtains each pointer under lock then invokes unlocked; final ParameterChangeForwarder list traversal has no processor listenerLock. Every writer/reader/lifetime must be reviewed before replacement. VST3 CachedParamValues uses SEPARATE atomic float values and grouped atomic change flags, immutable IDs after shared construction; component/controller shared refs retain storage. Off-message paramChanged writes cache, process drains changed flags and calls host output queues. No packed float/flag claim. Cache storage setup/destruction/resource acquisition remain legally excluded, not arbitrary-lifecycle safe. Prepared maps/scratch/DSP/callback/FL locks open.

Session09 initial ASan registry and observer tests genuinely overflowed default worker stack at257-level real SDK recursion;1/8/64 passed. Original failing logs/executables retained. Fixture now uses pthread_create explicit8MiB SDK worker stack in ALL trees, joins before destruction; same loaded AU bytes pass full257. No product stack-depth/lifecycle waiver. Release/separateASan-UBSan/TSan ON/OFF strictAU1274/callbacks/registry/inherited ownership/suppression/VST3/seals/adapter/recall3of3/editor checks pass at exact checkpoint boundaries. New older-exit1/8/64/257 tests both selectors wait then acknowledge2; observer0. Inherited crossadd/order257/secondremover/overlap2/2/nested64/active+withdrawn rejection/destruction128 pass. Updater128fresh/4x256/64legalteardown. Editor nested/direct7/64detach, Release3072renders/four scales, separate sanitizercontrols/gestures/undo/recall/close. Core16of16/format/3generator drift rejections. Final successful logs have no observed assertion/ASan/UBSan/TSan diagnostic; initial actual ASan stack-overflow is retained, never claim no sanitizer failure ever occurred. GeneratedAU/SDK/VST3 and fixture sanitizer flags verified; hooks/probe absent finalOFF defaults. Consult final identities for exact hashes and logs for whole default lock counts; G01 stays open.

Preserve org.algorhythmia.sys01/Algr/Sys1, seven keys/nativeIDs, four programs/order, schema1exactly40, parameter-only deterministic empty-history recall,10ms smoothing, shared256whole-batch rejection/no replay, eight restores, single audio owner, stereo effect/generator/disabledinput and formats. Stable editor off-message atomicdirty only/no payload/post/UIpointer, seven authoritative packed FLOAT/GENERATION words (distinct from VST3 cache), synchronized removal/message gestures/undo/recall/visibility remain.

Tools CMake/CTest4.4.4 /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin; format18.1.8 siblingclang_format/data/bin; AppleClang21/macOS26.6.2/25G83arm64. JUCE8.0.12 revision29396c22c93392d6738e021b83196283d6e4d850 at /private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850, doctest2.4.12 revision1da23a3e8119ec5cce4f9388e91b065e20bf06f5 build/_deps/doctest-src; pins cmake/dependencies.lock.json verified4377/270. Native/private/tmp/sys01-s09-repair,sys01-s09-asan,sys01-s09-tsan finalOFF; core/private/tmp/sys01-s09-session02-core. Reconstruct missing temps from pins/cache/source export. Frozen rpaths may require absoluteDYLD_LIBRARY_PATH; NEVER modify frozen bytes. Probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54. Eleven previous evidence inventories unchanged; Sprint08 live-source intentionally invalid,never reseal.

Only installed AbletonLive12Suite permitted; REAPER skipped/pluginval absent/temporaryAU auval bind requires forbidden install. ActualDAW-origin/externalOS/architecture/signing/rights/accessibility/humans/budgets/soak/MB-04 remain unqualified. Sprint10 exit needs immutable prepared resources,nonwaiting audio,off-audio configuration waits,acknowledged EVERY-writer reclamation,safe invalid concurrent lifecycle silence/error,complete zero-prohibited callbacks and shippingformat concurrency/lifecycle checks. No waiver or advancement.

Use new evidence/sprint-10-session-11 and unique command IDs, actual argv/cwd/UTC/exits/tool/dependency/source/artifact hashes/failures/invalidation. Freeze actual loaded negative before repair. Complete appropriate native/editor/core regressions,separate sanitizers generated-wrapper/first-thread/lifecycle checks and final diagnosticsOFF defaults with hook/probe absence. Finish compact rolling state,new handoff from handoff-template.md,exact checkpoint/remaining gates/next action and COMPLETE ready-to-paste following-session prompt. Continue Sprint10 if incomplete.

Session10 retained sanitizer tooling failure: the initial ASan condition fixture
used the Release malloc interposer control. ASan allocator interceptors superseded
those bindings (alloc/free0/0,mutex1,unfair3), so the control failed before focused
condition checks; early std::exit left loaded instances alive and emitted actual
JUCE leak assertions. Original executable/library/log/report remain retained. The
condition-only separate sanitized fixture retains real broadcast control1, full
loaded registry/nesting/both selectors/idle counts/legal teardown on SAME AU bytes;
Release separately retains allocation/free/mutex/unfair controls and0/0/3/0. Do not
claim sanitizer allocation interposition or no assertion failure ever occurred.

Exact session10 checkpoint source export SHA256 122a55584e3b7f6a0f0bdfaf650c54dd6c2c08694d991d5fbe992251b6d49b4f; resume-inputs SHA256 9213e7850a25b757a618b1d71f3c7705248b6f5b953ee338b3b445e1ebb75f85. Final diagnosticsOFF Release AU fingerprint b176242561f746c3551ab3f6b24d8a3064b0ef8187611fe81d192bd4f61056e0, binary 5cb34fe3b1d15d14b6c968455cf4dd42fabd81789a17b6d29f0d235440ec654e; VST3 fingerprint 41719ca2b332a4a1fe13a54a533f3b5b8af954cbbc5cfd88ca54132f291cc0c1, binary 27a564bd180a3bd18b38f98f443dfbafac0db6387b2999c28c6d651e2ca692a8. Complete registry single-listener alloc/free/mutex/unfair0/0/3/0, count8/64/257 mutex17/129/515. Whole default freshAU1-frame mutex9/hostile603,VST3fresh1024-frame mutex9,alloc/free0/0. Final condition command IDs s10s10-condition-repair, s10s10-condition-asan-only, s10s10-condition-tsan-only. Original s10s10-condition-asan FAIL is retained and is not a passing command. Final matrices completed via s10s10-resume-authorized after s10s10-complete-authorized stopped at that fixture control failure. All complete,none to resume; exact final inventory in finalization/evidence-inventory.

```
