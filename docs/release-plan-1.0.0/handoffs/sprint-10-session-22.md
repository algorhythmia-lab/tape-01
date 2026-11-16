# SYS 01 Sprint10 session22 handoff

## Checkpoint

Date/time/timezone: 2026-10-22T16:53:45.765760+00:00 / America/Santiago.
Sprint/session:10/22. Objective: reproduce/freeze one actual loaded-AU callback ownership
defect, implement one bounded repair and verify it. **S10-AU-REGISTRY-17 LOCALLY VERIFIED**
at null callback admission rejection / ordered publication / acknowledged ownership.
Sprint10 IN PROGRESS; release NO-GO/development-unqualified. Registry gate BLOCKING.
G01, actual DAW/externalAU, MB-04, immutable prepared resources, nonwaiting acquisition,
EVERY-writer acknowledged reclamation and safe invalid concurrent lifecycle remain open.
No waiver or advancement. Next: **Session23**. All jobs completed; none to resume.

Git root /Users/lhommeduchili/algorhythmia; SYS01 root /Users/lhommeduchili/algorhythmia/sys-01.
HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5 unchanged; no applicable AGENTS.
All163source+43reference Session21 inputs matched BEFORE edits; all23 historical evidence
inventories unchanged. Final164source+45reference. ALL dirty/untracked SYS01 and unrelated
monorepo work preserved; git status unchanged. No subagents/install/staging/stash/reset/
discard/commit/tag/contact/publication/personal plugin-directory changes.

Evidence: [session22](../evidence/sprint-10-session-22).
Source export SHA256 `823ab877af25add556aeb924ca4596b8e68b915885b6df9493844a907d2bb21f`;
resume inputs SHA256 `5d1e5636adac7fb62adb712b171e80e48464ba06edc2cc21605393ae818340e2`.
Exact bundles/binaries in final-identities.json; actual argv/cwd/UTC/exits/tools/input/log
hashes in commands.json. Final inventory/counts in finalization/evidence-inventory.
Integrity is not acceptance; Sprint08 live comparison intentionally invalid, never reseal.

## Changes and decisions

Only product [AUPropertyRegistry.hpp](../../../apps/desktop/AUPropertyRegistry.hpp) changes.
Before registry gate acquisition, allocation, demand attachment or publication, reject a
null callback with kAudio_ParamError(-50). Null contexts remain valid. This prevents an
invalid function pointer from acquiring Entry ownership and later being invoked.
Valid admission/publication/removal paths unchanged; no diagnostic hook/dependency added.
After shutdown null admission also rejects without reading current/closed; arbitrary
concurrent destruction and invalid nonnull function pointers remain unqualified.

Negative uses UNCHANGED Session21 shipping OFF AU fingerprint
092282ca3d257845f4b31cfbe9652f2f8b5178abcd852dd5beffe73cb388da2a,
binary31b3ca82bc294435dffebdb8fc412dc8d74655377877f31841cd7b741bd09884.
Both public add and actual loaded SDK Lookup add accept null callback status0. BOTH real
WithUserData/SDK Lookup removals clean invalid registrations; valid null contexts/duplicates
already pass. Separate actual SDK NickName publication calls accepted null and process
exits SIGSEGV(-11). COMPLETE negative source/header/export/fixture+executable/generated
AU-SDK-VST3/cache/compile/pinned29fileSDK/bundles/reports frozen BEFORE repair, never edit.
This is intentional invalid callback admission, not arbitrary pointer/lifecycle/DAW/realtime
acceptance. Neutral ABI fixture types avoid caller nonnull UBSan checks preempting product
behavior; original compiler warnings are retained with exact negative source/executable.

Preserve COMPLETE initial ALL-frame callback-origin other-thread removal rejection BEFORE
allocation/mutation/errorNOrelease; independent simultaneous demands/withdrawn/newmatching/
spuriouswake; CollectionMarks append-before-mark/ALL cleanup under gate before publication/
WaitDemand, including exceptions; EVERY valid matching add reserves EACH demand BEFORE ANY
attachment/count/publication; capacity-only failed reserve. Retained Entry liveFrames
predicates unchanged. Constant unlink/backlinks/finalEntry/localnotify/unlock-before-broadcast/
complete ownerCV/context/wrapper/message/updater lifetime; reader non-reclamation/writer
retired use_count1; orderedduplicates/survivors/outer-nested visibility unchanged. Errors
never authorize context destruction; legal teardown joins COMPLETE wrapper/processor/message
entries including unlocked tails; exclude new registrations and withdraw later matches.
Updater stop/clear precedes registry shutdown while ContextName wrapper-this lives.
No gate removal without replacement publication+acknowledged EVERYwriter-reader-context
reclamation. No cap/loss/TLS/table/queue/retainedpayload/gesturecoalescing.

ALL generated SDK routes/empty SDKvectors/uncalled accessor/MIDI source-only/GetMutexnull
remain. APVTSauthority->rawatomicmirror->recursiveLockedListeners callback->flags;
regular/final parameter listenerLock/editor synchronized removal; processor sizeoutside/
pointerunder/invokeunlocked/finalforwardingunlocked; VST3 SEPARATE atomicfloats/groupedflags/
immutableIDs/sharedstorage remain. Prepared maps/buffers/DSPscratch/history/callback locks/
serialized offaudio restore retain legal lifecycle exclusion. All identity/7keys-nativeIDs/
4programs-order/schema1exactly40/emptyhistoryrecall/10ms/shared256wholebatchreject-noreplay/
8restores/singleaudioowner/formats/editor dirty+7packed FLOAT-GENERATION/gestures-undo-
recall-visibility preserved. [Review](../au-property-invalid-admission-session-22.md).

## Verification

| Check | Command registry reference | Result | Boundary |
|---|---|---|---|
| Initial integrity | initial-state/historical-integrity-initial |163+43 match;23 inventories unchanged|BEFORE edits|
| Loaded negative admission |s10s22-negative-authorized|expected fixture exit1;status0 fourcases|public+SDK add/BOTH removals|
| Loaded negative invocation |s10s22-negative-publication-authorized|expected fixture -11|actual SDK NickName dispatch|
| Release |s10s22-complete-repair-authorized|ON/OFF PASS exit0;183.21s|product/generated/native/editor|
| ASan-UBSan |s10s22-complete-asan-authorized|ON/OFF PASS exit0;226.25s|separate instrumented tree|
| TSan |s10s22-complete-tsan-authorized|ON/OFF PASS exit0;519.63s|separate instrumented tree;no retry|
| Null admission |*-null-admission reports|all6PASS;public+SDK status-50|Releasealloc/free0/0;validnullcontext+orderedduplicates|
| Publication after rejection |*-rejected-publication reports|all6PASS|exact former fault sequence|
| Native audio reentry |s10s22-audio-admission-authorized|all6PASS;BOTHremovals/existing1/8/64/257|realSetParameter->Render->hostcallback->nulladd;Release0/0|
| Null fault/pending demands |s10s22-null-supplemental-authorized + tsan-default-rejected-publication-authorized|53checks/treePASS|zeroallocbudget;closed;both independent pendingdemands/newmatching|
| Inherited protocols |collection/storage/add/demand-growth/publication/boundary/absent/withdrawn|24/120/34/84faultcases/tree+ownershipPASS|marks/reserves/retry/spuriouswake/pausedtail/newmatching/overlap|
| Native/editor/core |native/editor/notifications/updater + remaining-v2-authorized|strictAU1274/VST3201894;adapter-recall/editor;core16/16PASS|firstthread128/ContextName64/order257;finite fixtures|
| Pins/format/generation/probes |juce-pin/doctest-pin/format-pinned/generation/defaultprobes|4377+270pins;3driftrejections;PASS|exacttools/source/bundles|
| Final checkpoint |s10s22-checkpoint/final-records;finalize.py|PASS|defaultsOFF/hookabsence/generated unchanged/identities/history|

All3 complete drivers pass no retry on SAME immutable initial-complete_matrix.py SHA256
14b815d08cadd0a0e4d5380148bdbab43ae6d8b2afa2fe0a2301e5bd6080d100.
All successful final reports have no observed assertion/ASan/UBSan/TSan diagnostic.
Only two inherited EXTERNAL mutex bootstrap shims exclude instrumentation; product,
generated AU/SDK/VST3 and direct/loaded fixtures separately instrumented. Explicit8MiB
SDK stacks/legal joins are fixture conditions, not arbitrary hoststack-lifecycle proof.
Unpaced offscreen editor CPU is not DAW budget. Three native trees finalOFF; hooks and
probe dependency absent; generated wrappers unchanged Session21.

Failures retained: sandboxfactoryregistration before cases; expected nulladmissionFAIL;
expected real nullcallSIGSEGV; remaining driver reused already-completed pin command ID
and stopped before remaining checks. Separate remaining.py omits ONLY two alreadypassed
pin checks, completes remainingchecks exit0. Interactive ps sandboxdenial then registered
read-only elevated inspection succeeds, no process mutation. All historical stackoverflow/
allocatorleaks/bootstrap-rpath/invalidfixture-leaks-race-SEGV-KeyError/session19timeouts-
counterrace-stoppeddrivers/session21factory-format-key failures preserved; never failure-free
history. TSan completed normally, no timeout/retry.

FinalReleaseOFF AU `fa8a8f66af73066c1f65e217da54fff6ad47165930a896458f666ef39b6579af`,
binary `09897985d283163c5be768efef4ab4acec5b437e8e925b33a7dcbd48afd2bab0`;
VST3 `11f3dac8465ab886c71a7a8a639a050419d586afde2696f10fd2d9c9fc4adb3c`,
binary `cd659d2238ab035db14be93fee4037aca9bac7a8cec6e823a1c135eba1176dcb`.
Registry alloc/free/mutex/unfair0/0/3/0; wholeallocfree0/0 but AUfresh1mutex9/hostile603,
VST3fresh1024mutex9 remain. No wholeG01 qualification.
CMake-CTest4.4.4/format18.1.8/AppleClang21/macOS26.6.2-25G83arm64;
JUCE8.0.12revision29396c22c93392d6738e021b83196283d6e4d850;
doctest2.4.12revision1da23a3e8119ec5cce4f9388e91b065e20bf06f5;
originalprobe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.

## Resume

Session23: progress toward architectural Sprint10exit. Review COMPLETE registry and
EVERY writer/reader/context path; select ONE actual loaded remaining wait/order/callback
ownership defect, freeze COMPLETEnegative before one boundedrepair. Null rejection closes
only invalid null admission, not arbitrary invalid pointers/lifecycle. Then APVTSregular-
finalparameter+processor listener ownership/VST3cache/preparedresources/lifecycle. Do not
merely repeat old audits/package evidence or generalize finite null tests to realtime.

All jobs completed; none to resume. Native /private/tmp/sys01-s09-repair,asan,tsan OFF;
core/private/tmp/sys01-s09-session02-core. Reconstruct missingtemps exactly from sourceexport/
cache/pins; absolute DYLD_LIBRARY_PATH may resolve frozenrpaths, NEVEReditfrozenbytes.
Only installedLive12Suite permitted;REAPERskipped/pluginvalabsent/temporaryauvalinstall
forbidden. Missing external machines/signers/participants do not prevent independent work.
ActualDAW/externalOS-architecture/signing-rights/accessibility/humans/budgets-soak/MB04 open.

Actual Sprint10exit (audit-production-plan-2026-10-18.md):immutablepreparedresources/
nonwaitingaudio/offaudioconfigurationwaits/EVERYwriteracknowledgedreclamation/safeinvalid-
concurrentlifecycle silence-error/completezero-prohibitedshippingcallbacks+concurrency-
lifecycle tests. Capture/admission/exit waits/OSbroadcast/host-sizeddispatch-scan-copy-
demand-retention-reclaim/allocatingvalidwriters/listenerownership/G01 remain blockers.
No advancement or waiver.

## Complete next session prompt

[Complete ready-to-paste Session23 prompt](../evidence/sprint-10-session-22/next-session-prompt.txt)
contains Session22 authority and the ENTIRE immutable Session21 prompt. Session22 overrides
superseded inherited statuses/jobs/identities/instructions.
