# SYS 01 Sprint 10 session 08 handoff

## Checkpoint

2026-10-20 12:13 America/Santiago (15:13 UTC). Sprint/session10/08.
Objective S10-AU-REGISTRY-03: reject callback-origin wait for another thread's
matching callback frame without false acknowledgement or registry withdrawal.
LOCALLY VERIFIED at per-instance removal error/acknowledgement boundary.
Sprint10 IN PROGRESS; release NO-GO/development-unqualified. Registry gate
BLOCKING retained; G01 whole native realtime,actualDAW-origin/external AU and
MB-04 open. No Sprint11 advancement or waiver.

Gitroot/HEAD /Users/lhommeduchili/algorhythmia /
5c45cf6bed431317a1b412b1fc12ee62b66cfdd5; SYS01root
/Users/lhommeduchili/algorhythmia/sys-01. No applicable AGENTS. All149source+
15reference inputs matched session07 BEFORE edits. Dirty/untracked SYS01 and
unrelated work preserved; no staging/reset/stash/discard/commit/tag/publication/
subagents/contact/install/personal plugin edits.
150-input source export SHA256 c8a7749ee127ae754826c158214c30dff85def04f5aed400e95006e1abbea76c;
resume-inputs SHA256 71953fdc0abe6bccc8812c47aec83b5781f5081482128053ab53967709302526.
Exact source/artifact/command inventories and fingerprints in session08
checkpoint.json/final-identities.json/negative-identities.json/commands.json.

## Changes and decisions

Only apps/desktop/AUPropertyRegistry.hpp and AURegistryTests.cpp implementation
changed. New ownership record ../au-property-removal-context-session-08.md.
No generator/SDK/pin/shared processor/DSP/editor/VST3 generator change.

Actual old DEFAULT AU fingerprint
e37e3b2e5e3b85ee0c33a9bd03efb5771323c6ba9f35d7d7238c44aff25c3f55,
binary18f43aad137a2d75a49fe17e2cd643eb46834300c5e6157df14200b63bbc8196.
Real native bypass Render callback removes overlapping real SDK NickName callback
which waits for remover callback's return. Both selectors require2second watchdog
release: return-before-release0/status0/calls1 vs1/-10863/2. Actual negative report/
bundle/fixture source+executable/pre-repair complete source/generated AU+SDK/cache/
compile/generator/pinned29file SDK frozen BEFORE registry repair. Separate sandbox
factory registration failure retained; approved temporary in-process run installs
nothing. No invented sanitizer failure or historical edit.

Remove scans live frames under existing gate BEFORE allocations/mutation. ANY
same-instance caller-thread callback frame plus matching OTHER-thread frame means
CannotDoInCurrentContext(-10863). Includes already-withdrawn live frames. Error
preserves registrations/publication and acknowledges NO context destruction; host
retains/retries outside callbacks after joining owners/excluding new matching add.
Conservative error even if host believes other callback independent. Gate makes
check/withdrawal atomic with admission. Existing off-callback successful removal
still withdraws and waits with gate released for matching other-thread frames,
including second removers. Same-thread active/ancestor contexts survive until return.

Gate/current/retired/active/Frame protocol unchanged. Ordered immutable snapshots,
duplicates/no cap,stack Frames,callback unlocked,RAII unlink/notify,writer-only
retired use_count1 reclaim,normal reader never last-owner deletes. Every SDK virtual
caller still routes; vector empty/constaccessor unused,conditionalMIDI source-only.
ContextName wrapper-this and both selectors,nesting/overlap/first-thread covered.
Legal teardown excludes/joins ALL complete wrapper/processor dispatches on fixed
message owner outside callbacks; updater stop/clear precedes registry shutdown.
Processor removeListener does not acknowledge complete change/final dispatch.
No pending product decision needed for this bounded error path.

Preserve org.algorhythmia.sys01/Algr/Sys1,sevenkeys/nativeIDs,fourprograms/order,
schema1exactly40,parameter-only deterministic empty-history recall,10ms smoothing,
shared256whole-batch rejection/no replay,eightrestores,singleaudioowner,VST3/stereo/
generator/disabledinput scope. Stable editor off-message atomicdirty/seven packed
float-generation words/synchronized listener removal/messagegestures/undo/recall/
visibility retained. VST3 cache has separate atomic float values/grouped flag words;
this is NOT editor authority packing. APVTS/regular/final listeners/resources open.

## Verification

| Check | Command registry reference | Exit/result | Boundary |
|---|---|---|---|
| Inputs/tools/pins |initial-state.json,s10s08-*-version/platform/compiler/*-pin|149+15match,pins4377/270|Before edit/pinned sources|
| Actual negative |s10s08-negative-authorized|exit1 both2second waits|Frozen loaded DEFAULT AU|
| Focused repair |s10s08-focused|exit0 both errors/no withdrawal|Loaded Release AU|
| Expanded cases |s10s08-{repair,asan,tsan}-{on,default}-native|allPASS four cases + inherited fixtures|Exact loaded bundles|
| Complete registry probe |s10s08-registry-probe|0/0/3/0,control1/1/1/1|Locks remain G01 blocker|
| Native regressions |same native IDs|strict1274/callback/ownership/suppression/VST3/seals/3of3 PASS|ON/OFF exact bytes|
| Editor/notifications |s10s08-{repair,asan,tsan}-{on,default}-{editor,notifications}|nested7/direct/64detach,controls,Release3072/four scales PASS|Separate trees|
| Updater |s10s08-{repair,asan,tsan}-on-updater|128fresh/4x256/64teardown PASS|Inherited legal lifecycle|
| Whole default probes |s10s08-default-au-probe/default-vst3-probe|alloc/free0/0,mutex9/fresh and AUhostile603|Whole G01 still open|
| Core/format/generation |s10s08-core/format-final/generation|16of16,formatPASS,3driftrejections|Shared source unchanged|
| Final checkpoint |s10s08-checkpoint|PASS|OFFflags/hook-probeabsence/sanitizerflags/exactidentities/frozeninventories|

No observed final assertion/ASan/UBSan/TSan diagnostic in finite runs. Generated
AU/SDK/VST3 and fixtures sanitizer flags verified ON/OFF; no hook/probe finalOFF.
Release editor3072renders/four scales/gestures/undo/recall/close pass. Separate
sanitizer controls-only intentionally omits full image/paint budget claim. Nine
historical inventories [291, 505, 357, 364, 409, 366, 397, 422, 492] unchanged;
Sprint08 live-source intentionally invalid,never reseal. Source delta/export,
commands with actual argv/cwd/UTC/exits/tool/log hashes,failures/invalidation and
numbered ownership map retained. Optional sandbox ps inventory denied; existing
job sessions/logs establish completion, no required work blocked.

Tools CMake/CTest4.4.4 at
/Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin;
format18.1.8 siblingclang_format/data/bin,AppleClang21/macOS26.6.2/25G83arm64.
JUCE8.0.12 pinned29396c22c93392d6738e021b83196283d6e4d850,
doctest2.4.12 pinned1da23a3e8119ec5cce4f9388e91b065e20bf06f5;
exact trees cmake/dependencies.lock.json. Probe
59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.

Affected AU callback/concurrency/host/validator/lifecycle/performance/soak/signing
proof invalidated,renewed at local named boundary only. ActualDAW-origin/external
OS/architecture,signing/rights/accessibility/humans/budgets/soak/MB-04 unqualified.
Only installed AbletonLive12Suite allowed;REAPER skipped/pluginval absent/temporary
auval bind needs forbidden install. No external acceptance inferred.

Final default fingerprints (binary SHA256 in final-identities.json):

| Tree | AU | VST3 |
|---|---|---|
|repair|dac47d28aade5669b8e4d45f6e2b56beb246caae279569a8b15937369845644a|86d67343c7cb2920f293de3ba2b8b460f177b2d66a93acbf1801eb4d96bf7e41|
|asan|4224f2d9b6f12fd31a5a3d1d7f48f18b9fa965e098cee1a261a3550821be918b|63053efd5dfcdddfddc5d366808a6e9e55a0f9eae3c48af261cbfdc82b6942a5|
|tsan|978fe7a584ddc639f6fb9b82c233bf00c9bf2efefd55a2de2d2286de4e1c04c4|d3f030fa9d70f2e67afec3887275edd679e63dad768d7c2f8b810171dae8ecdb|

## Resume

All jobs complete;none to resume. Native/private/tmp/sys01-s09-repair,
sys01-s09-asan,sys01-s09-tsan finalOFF;core/private/tmp/sys01-s09-session02-core.
Reconstruct missing temp trees from cache/pins/source export. Frozen rpaths may
need absoluteDYLD_LIBRARY_PATH;never modify frozen bytes.

Session09 immediate action: AUPropertyRegistry.hpp gate/capture/Frame admission/
exit/unlink/notify/remove/reclaim,ownership-trace and complete0/0/3/0 probe. Freeze
one actual remaining admission/exit wait/order or callback ownership defect before
repair. Access gate stays until every writer/reader/context replacement publication
and acknowledged reclamation. Cross-instance cycles,callback waiting for own
OFF-callback remover,parameter lock orders/arbitrary reentry remain open. Then
APVTS/regular/final listeners,CachedParamValues,immutable prepared resources.
Sprint10 requires nonwaiting audio/off-audio waits/every-writer acknowledged
reclamation/safe invalid-lifecycle silence-error/zero-prohibited complete callbacks/
shippingformat concurrency-lifecycle. No Sprint11 or waiver.

## Complete next session prompt

Also retained verbatim ../evidence/sprint-10-session-08/next-session-prompt.txt.

```text
Work in /Users/lhommeduchili/algorhythmia/sys-01.

Continue SYS 01 v1.0.0 Sprint 10, session 09: nonwaiting AU registry admission/exit and acknowledged callback ownership. This is an implementation session. Reproduce and freeze one concrete remaining plugin-controlled registry admission/exit wait/order or callback ownership defect on an actual loaded bundle, implement one coherent bounded repair, and verify it. Remain Sprint10; do not advance Sprint11 or waive gates.

Read docs/release-plan-1.0.0/rolling-state.md and handoffs/sprint-10-session-08.md first. Then read au-property-removal-context-session-08.md, au-property-callback-ownership-session-07.md, au-updater-ownership-session-06.md, au-property-registry-session-05.md, handoffs/sprint-10-session-07.md, au-host-notifications-session-04.md (SDK guard claim superseded), native-vst3-suppression-session-03.md, native-suppression-session-02.md, native-notifications-session-01.md, au-ingress-ownership-session-03.md, docs/release-1.0.0/native-ownership.md, and Sprint10/acceptance/evidence-invalidation/handoff sections of audit-production-plan-2026-10-18.md. Read session08 checkpoint.json, final-identities.json, ownership-trace.json, initial-state.json, negative-identities.json and this COMPLETE prompt.

Check Git status and applicable AGENTS.md. Compare live inputs against session08 resume-inputs.json and resume-reference-inputs.json BEFORE editing; resolve intervening changes. Session08 matched all149source+15reference inputs at start. Gitroot /Users/lhommeduchili/algorhythmia, HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5 unchanged. Preserve dirty/untracked SYS01 and unrelated monorepo work. No bulk staging/reset/discard/stash/overwrite/commit/tag/publication. No subagents, installation, personal plugin directory changes or contact with others.

Sprint09/S09-OWN-01 complete at named local boundary. S10-NOTIFY-01/AU-SUPPRESS/VST3-SUPPRESS/AU-UPDATER-01 locally verified at bounded contracts. AU-NOTIFY only caller-serialized off-audio restore identity/per-call stack events. S10-AU-REGISTRY-02 gate-released host callbacks with per-frame removal acknowledgement. S10-AU-REGISTRY-03 callback-origin removal rejects an otherwise required other-thread matching-frame wait BEFORE any allocation/withdrawal/publication. Registry access gate remains BLOCKING; Sprint10 IN PROGRESS; release NO-GO/development-unqualified. G01 whole native realtime, actual DAW-origin/external AU and MB-04 DC open.

Session08 actual negative uses session07 DEFAULT Release AU fingerprint e37e3b2e5e3b85ee0c33a9bd03efb5771323c6ba9f35d7d7238c44aff25c3f55, binary18f43aad137a2d75a49fe17e2cd643eb46834300c5e6157df14200b63bbc8196. Native BYPASS consumed by real Render enters callback A, independent real SDK NickName SetProperty enters B. A removes B; B waits for A to return. Plugin removal waits for B frame; watchdog release2seconds required. Both actual WithUserData and loaded SDK legacy Lookup selectors observe returned-before-release0/status0/target-calls1 instead1/-10863/2. Old inherited tests pass. Frozen failure/report/bundle/source/fixture executable/source/generated AU/SDK/cache/compile commands and pinned29file SDK copy retained BEFORE registry repair. Initial sandbox registration failure separately retained, approved in-process run installs nothing. Do not invent a race/sanitizer failure or change frozen bytes.

Current remove scans all live Frames under gate: ANY same-instance caller-thread frame plus matching OTHER-thread frame means kAudioUnitErr_CannotDoInCurrentContext(-10863), with NO mutation/withdrawal/publication. Includes already-withdrawn frames. Host must retain context after failure and retry outside callbacks after owners joined, excluding new matching registrations. Conservatively rejects even when host knows other callback independent. If no other-thread matching frame exists, same-thread active/ancestor withdrawal remains successful but contexts live until those frames return. Off-callback removal retains withdrawal and released-gate CV wait for ALL matching frames including second removers; successful return acknowledges context return. Gate makes precheck/withdrawal atomic with admission; no false acknowledgement. Cross-instance cycles and a callback waiting for its own OFF-callback remover still unqualified.

All registry writers/readers still use one mutex for current/retired/closed/Entry.active/Frames. Dispatch captures immutable ordered snapshot; active check+stack Frame admission under gate; callbacks run unlocked. RAII exit unlinks/notify under gate. Normal readers never last-owner delete: writers retain old snapshots before publication, only writers reclaim retired use_count1. No TLS/table/cap/retained host payload/queue/gesture coalescing/ingress extension. Capture/admission/exit lock, frame unlink/notify and host-sized traversal remain prohibited/unbounded. Audio-reentered writers allocate/free. DO NOT remove registry gate until every writer, reader and context has replacement publication and acknowledged reclamation.

All virtual SDK AddPropertyListener/RemovePropertyListener/PropertyChanged still route to registry: state/RestoreState/layout/channel/stream/input connection/render callback/MaximumFramesPerSlice/LastRenderError/name/preset and conditional MIDI mapping/hot map. SDK vector empty; const GetPropertyListeners no pinned caller. Constructor ContextName callback owns wrapper this and survives host listener removal; nested64 tested. Independent NickName/native bypass overlap2/2, order257 duplicates, cross-add, second remover, active/withdrawn callback-origin tests and128 fresh-thread context destructions verified. Conditional MIDI mapping has source coverage only. Legacy public no-context function unavailable arm64; loaded SDK Lookup selector tested. Generator/SDK/pins unchanged; implementation changes only AUPropertyRegistry.hpp and AURegistryTests.cpp.

Legal wrapper teardown excludes/joins ALL complete wrapper/processor dispatches, fixed message owner and never disposal inside callbacks. Updater stops/clears before registry shutdown while wrapper this lives. Processor removeListener alone does not acknowledge complete updateHostDisplay or final dispatch. AUEntryGuard locks only nonnull GetMutex; SYS01 leaves nullptr. Restore identity lanes assume serialized same-role off-audio publisher; concurrent same-role/recursive stateMutex restore unqualified. Updater60Hz Timer registered during fixed message-owner construction; off-message only flags/lock-free fetch_or/immutable pthread comparison. Message delivery synchronous/nested, existing flags coalescing retained; stopped loop delays, closed editor still polls. Owner migration/concurrent lifecycle unqualified.

Start complete registry probe0/0/3/0 and AUPropertyRegistry.hpp gate/capture/Frame admission/exit/unlink/notify/remove/reclaim, native_au_adapter.py/generated overrides and ownership-trace.json. Freeze next actual failure and exact source/fixture/generated/SDK/cache/artifact bytes BEFORE repair. Include first-thread entry, both removals, active/withdrawn frames, nested/overlapping dispatches, ContextName wrapper-this and legal teardown. Distinguish host fixture waits and OS/host bodies from plugin scaffolding. No listener cap/notification loss/gesture coalescing. Whole G01 not proven by finite warm zeros/sanitizers.

Then APVTS ParameterAdapter/LockedListeners, regular/final parameter and processor listeners, CachedParamValues and immutable resources/lifecycle. Session08 trace confirms ParameterAdapter reads virtual authority, publishes raw mirror and flags, calls APVTS listeners under recursive CriticalSection; add/remove same lock. Parameter listenerLock surrounds reverse regular+final listeners and synchronized stable editor relay removal. AudioProcessor final ParameterChangeForwarder traverses processor listeners without listenerLock; updateHostDisplay indexes then acquires per-pointer lock but releases before callback. removeListener alone acknowledges neither COMPLETE path. CachedParamValues has separate atomic float values and grouped atomic flag words (NOT one packed float/flag word), immutable ID vector after shared-processor construction; component/controller references keep storage, setup/destruction require legal exclusion. Full every-writer/readout/reclamation needs review. Prepared maps/scratch/DSP/callback/FL locks remain open.

Session08 Release and separate ASan/UBSan and TSan diagnosticON/defaultOFF matrices retain strictAU1274/callbacks/registry/inherited ownership/suppression/VST3/seals/adapter/recall3/3/editor. Four new active/withdrawn/both-selector removal cases pass; inherited crossadd/order257/secondremove/overlap2/2/nesting64/destruction128 pass. Editor nested/direct7/64detach pass; Release3072renders/four scales and sanitizercontrols/undo/recall/close. Core16/16,format and3generator drift rejections. Exact facts/identities/final flags/hook absence/probe numbers are checkpoint/handoff authority; consult actual logs. Whole native default probes retain locks and G01 blocker; no actual DAW/budget/soak proof. All final jobs complete,none to resume.

Preserve org.algorhythmia.sys01/Algr/Sys1,seven keys/nativeIDs,fourprograms/order,schema1exactly40,parameter-only deterministic empty-history recall,10ms smoothing,shared256whole-batch rejection/no replay,eight restores,singleaudioowner,VST3/stereo effect/generator/disabledinput/release scope. Stable editor off-message atomicdirty only,no payload/post/UI pointer; seven authoritative packed FLOAT/GENERATION words (distinct from VST3 cache), synchronized listener removal,message gestures/undo/recall/visibility retained. Shared processor/DSP/editor/VST3 generator source unchanged.

Tools: CMake/CTest4.4.4 /Users/lhommeduchili/.cache/uv/archive-v0/wM3XA43IaEp9J_1H/lib/python3.14/site-packages/cmake/data/bin,format18.1.8 siblingclang_format/data/bin,AppleClang21/macOS26.6.2/25G83arm64. JUCE8.0.12 revision29396c22c93392d6738e021b83196283d6e4d850 at /private/tmp/sys01-s09-deps/JUCE-29396c22c93392d6738e021b83196283d6e4d850; doctest2.4.12 revision1da23a3e8119ec5cce4f9388e91b065e20bf06f5 at build/_deps/doctest-src; pins cmake/dependencies.lock.json. Native trees /private/tmp/sys01-s09-repair,sys01-s09-asan,sys01-s09-tsan finalOFF,core/private/tmp/sys01-s09-session02-core. Reconstruct missing temps from pins/cache/source export. Frozen rpath absent: absolute DYLD_LIBRARY_PATH; NEVER modify frozen bytes. Probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54. Exact exports/resume/artifacts in checkpoint.json/final-identities.json; nine historical inventories unchanged. Sprint08 live-source intentionally invalid,never reseal.

Only installed AbletonLive12Suite allowed; REAPER skipped,pluginval absent,temporary AU auval bind requires forbidden install. ActualDAW-origin/externalOS/architecture,signing/rights/accessibility/humans/performance/soak/MB-04 unqualified. Sprint10 exit requires immutable prepared resources,nonwaiting audio acquisition,off-audio configuration waits,acknowledged reclamation EVERY writer,safe silence/error invalid concurrent lifecycle,complete zero-prohibited plugin callbacks and shipping-format concurrency/lifecycle checks. No waivers.

Use new evidence/sprint-10-session-09 and unique command IDs; retain actual argv/cwd/UTC/results/tool/dependency/source/artifact hashes,failures,invalidation. Renew affected native/editor/core and separate sanitizer checks including generated wrappers/first-thread/lifecycle. Diagnostics defaultOFF; verify hooks/probe absent final rebuilt defaults. Finish compact rolling state,new handoff from handoff-template.md,exact tested checkpoint/remaining gates/next action and COMPLETE ready-to-paste following-session prompt. Continue Sprint10 if incomplete.
```
