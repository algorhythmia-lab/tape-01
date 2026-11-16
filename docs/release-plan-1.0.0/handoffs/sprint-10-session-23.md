# SYS 01 Sprint10 Session23 handoff

## Checkpoint

Date/time/timezone:2026-10-22T19:06:03.540129+00:00 / America/Santiago. Sprint/session:10/23.
Objective: reproduce/freeze one actual loaded AU wait/ownership defect, implement one
bounded repair and verify. **S10-AU-REGISTRY-18 LOCALLY VERIFIED** at module-wide SYS01
AU callback-origin wait rejection / instance-specific acknowledged ownership boundary.
Sprint10 IN PROGRESS; release NO-GO/development-unqualified. Registry gate BLOCKING.
G01/prepared resources/nonwaiting audio/EVERY-writer reclamation/safe invalid lifecycle/
actualDAW/externalAU/MB-04 remain open. No advancement or waiver. Next:**Session24**.
All jobs completed; none to resume.

Git root:/Users/lhommeduchili/algorhythmia; SYS01 root:/Users/lhommeduchili/algorhythmia/sys-01.
HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5 unchanged; no applicable AGENTS.
BEFORE edits all164source+45reference Session22 inputs match; all24 historical evidence
inventories verified unchanged. Final165source+47reference. ALL dirty/untracked SYS01 and
unrelated monorepo work preserved. No subagents/install/staging/stash/reset/discard/commit/
tag/contact/publication/personal plugin-directory changes. Only installedLive12Suite permitted.

Evidence:[Session23](../evidence/sprint-10-session-23). Source export SHA256
`0daefd9a6684a7c27652b273167f118cac0da47274662dc09f937e0c728cce25`; resume inputs SHA256`9e93142eb1f4c3496ea7507d036368b90198231d2381a0451e3f7a3a66ccdb95`.
Actual argv/cwd/UTC/exits/tool/input/log hashes in commands.json; exact bundle/binary hashes
in final-identities.json; flags/counts/failures/inventory in checkpoint/finalization.
Integrity is not acceptance; Sprint08 live comparison intentionally invalid, never reseal.

## Changes and decisions

Only product [AUPropertyRegistry.hpp](../../../apps/desktop/AUPropertyRegistry.hpp) changes.
Existing gate and intrusive stack Frame head now share one ownership domain for all SYS01
AU instances in this loaded module. Initial ALL-frame scan recognizes caller-thread
callbacks on EVERY instance. Matching other-thread demand and collection require
frame.owner==this. Cross-instance callback-origin required wait rejects -10863 BEFORE
allocation/marks/withdrawal/publication/demand/reclaim. Null callback add still rejects -50
before gate. Contexts remain valid; errors authorize NO release. Retry offcallback after
excluding new registrations/joining owners. Same-thread ancestors remain legal until return.

Unchanged Session22 OFF negative AU fingerprint
fa8a8f66af73066c1f65e217da54fff6ad47165930a896458f666ef39b6579af,
binary09897985d283163c5be768efef4ab4acec5b437e8e925b33a7dcbd48afd2bab0.
Actual SDK NickName publishers on two instances, BOTHremoval selectors/BOTHdirections:
all4 removal calls return only after1second HOST watchdog releases target, status0 and
registration withdrawn. This establishes forbidden waiting, not an unrecoverable deadlock.
Host hold released and COMPLETE calls joined; contexts never prematurely destroyed.
COMPLETEnegative source/export/header/fixture+executable/generatedAU-SDK-dispatch-VST3/
cache/compile/pinned29fileSDK/bundles/reports frozen BEFORErepair. Never edit frozen bytes.

EVERY writer/capture/admission/exit uses module gate. Entry/publication/retired/WaitDemand/CV
stay instance-owned. EACH matching add reserves EVERY demand BEFOREANYattachment/count/
publication; CollectionMarks append-before-mark/ALLgate-heldcleanup; retainedEntryliveFrames
predicate, independent removers/withdrawn/newmatching/spuriouswake retained. Constant unlink
transfers successor incoming pointer before predecessor stack disappears across instances.
FinalEntry/localnotify/unlock-before-broadcast/COMPLETEownerCV-context-wrapper-message-updater
lifetimes retained. Shutdown joins COMPLETE own entries/tails; foreign instance may staylive.
Module unload excludes/joins ALL participating entries. Updaterstop-clear before ownshutdown
whileContextNamewrapperthis lives. Orderedduplicates/survivors/outer-nestedvisibility/reader
nonreclamation/writeruse_count1 retained. No TLS/table/cap/loss/queue/payload/gesturecoalescing.

Tradeoff: module gate contention and initial scan now span all live SYS01 AU instanceframes.
This remains BLOCKING/host-sized, not realtime qualification. Recognition excludes unrelated
plugins/VST3/separate module images/arbitraryhost dependency chains. Do not remove gate without
replacementpublication+acknowledged EVERY-writer-reader-context reclamation. [Review](../au-property-cross-instance-session-23.md).

ALL SDK virtual routes/emptyvectors/uncalledaccessor/MIDIsourceonly/GetMutexnull unchanged.
APVTSauthority->rawatomicmirror->recursiveLockedListenerscallback->flags; regular-final
parameterlistenerLock/editor synchronizedremoval; processorsizeoutside/pointerunder/invoke
unlocked/finalforwardingunlocked; VST3separateatomicfloats/groupedflags/immutableIDs/shared
storage unchanged. Preparedmaps/buffers/DSPscratch/history/callbacklocks and caller-serialized
same-role offaudio restore retainlegal lifecycleexclusion. Arbitrary lifecycle/recursive-
concurrentrestore/ownermigration/otherpluginhostcycles remainunqualified. AllIDs/7keys-nativeIDs/
4programs-order/schema1exact40/emptyhistoryrecall/10mssmoothing/shared256rejectnoreplay/
8restores/singleaudioowner/formats/editor dirty+7packedFLOAT-GENERATION/gestures-undo-visibility preserved.

## Verification

| Check | Command reference | Result | Boundary |
|---|---|---|---|
| Initial integrity |s10s23-initial-integrity|164+45match;24inventories unchanged|BEFOREedits|
| Actual negative |s10s23-negative-authorized|expectedexit1;all4wait/status0/withdraw|SDKpublication/BOTHselectors/directions|
| Release ON/OFF |s10s23-complete-repair-authorized|exit0;187.22s|native/editor/ownership/inheritedfixtures|
| ASan-UBSan ON/OFF |s10s23-complete-asan-authorized|exit0;225.73s|separateproduct/generated/fixtures|
| TSan ON/OFF |s10s23-complete-tsan-authorized|exit0;516.01s|normalcompletion/no retry|
| Cross-instance native |*-cross-instance reports|all6x4casesPASS|before-release/-10863/preserved|
| Render-origin removal |extra-repair-asan/extra-tsan-authorized;*-cross-audio|all6x8casesPASS;Releasealloc/free0/0|SetParameter->Render->hostcallback->foreignremove1/8/64/257|
| Direct ownership |cross-protocol;asan/tsan-cross-protocol|11checks/treePASS|zeroallocbudget/reject/withdrawn/BOTHacks/foreigntuple/lifetime/ancestor/exception|
| Inherited protocols |collection/storage/add/demand-growth/null +boundary/publication/absent/withdrawn|24/120/34/84/53checks per treePASS|faults/reserves/marks/spuriouswake/newmatching/tails|
| Native/editor/core |native/notifications/updater/editor+remaining-authorized|strictAU1274/VST3201894;adapter/recall/editor;core16/16PASS|firstthread128/ContextName64/order257/legaljoins|
| Pins/format/generators/probes |juce-pin/doctest-pin/format-pinned/generation/defaultprobes|4377+270pins;3driftrejectionsPASS|defaultOFFhook/probeabsence|
| Final checkpoint |s10s23-checkpoint/final-records;finalize.py|PASS|allidentities/flags/history/frozennegative|

Release initial driver SHA25625afe902556ef79a3f9b0a89d6661b21a2008e34750302f8b9df198126287896.
New directcrossfixture added separately while Release driver alreadyrunning; separate runPASS.
ASan/TSan same retained sanitizerdriverSHA256aabf3f9b293eb9e4711145bd2e90ad49d008a5f82053401a212a4390454f0dfa
includes directcrossfixture. No retry or product changes between matrices.
Allsuccessfulfinalreports noobservedassertion/ASan/UBSan/TSandiagnostic. Only2external
bootstrapmutexshims excludeinstrumentation; product/generated/callback/directfixtures separately
instrumented. Explicit8MiBSDKstacks/completejoins/unpacededitor not arbitraryhoststack-lifecycle/DAWbudgets.

Failuresretained:sandboxfactoryregistration beforecases;expectedactualnegativeexit1.
Unregistered accidentalnonexistentcommand exit127 recorded, no mutation;correctedcontinuation.
AllSession22nulladmissionFAIL/nullSIGSEGV/duplicatepinID andALLhistorical failures preserved.
FinalOFFAU`0453be945d32fe083298a4afb4e0ee6ccb5cc39987bf80eabb4f673749e30eb6`,binary`0c05b7900202a8ffda8c541484deef76ba74680111fd7ed505e2c5c3d16a7126`;
VST3`11f3dac8465ab886c71a7a8a639a050419d586afde2696f10fd2d9c9fc4adb3c`,binary`cd659d2238ab035db14be93fee4037aca9bac7a8cec6e823a1c135eba1176dcb`.
Registryalloc/free/mutex/unfair0/0/3/0;wholeallocfree0/0 butAUfresh1mutex9/hostile603,VST3fresh1024mutex9
remain. Module gate/scans BLOCKING. No G01/DAW/MB04 advancement.

## Resume

Session24:progress actual Sprint10architecturalexit. Review module-wide registry/EVERYwriter
reader/context and remaining APVTS/regular-finalparameter/processor listener/preparedresource
lifetimes. Establish ONEactualremainingloadedwait/order/callbackownership or preparedresource
defect; freeze COMPLETEnegative BEFORE coherentboundedrepair. No inventedfailure or repeated
historicalaudit-packaging sessions. Cross-instance wait repair only recognizes participating
SYS01AUmoduleframes; callbackorigin outside this domain remains unqualified. Preserve all
matching/rejection/publication/acknowledgement/constantunlink/CV/modulelifetime invariants.

Alljobscomplete;none toresume. Native/private/tmp/sys01-s09-repair,asan,tsan finalOFF;core
/private/tmp/sys01-s09-session02-core. Reconstructmissingtemps exactlyfrompins/sourceexports/
cache;absoluteDYLD_LIBRARY_PATH mayresolvefrozenrpaths,NEVEReditfrozenbytes.
ToolsCMake-CTest4.4.4/format18.1.8/AppleClang21/macOS26.6.2-25G83arm64/JUCE8.0.12revision29396c22c93392d6738e021b83196283d6e4d850/
doctest2.4.12revision1da23a3e8119ec5cce4f9388e91b065e20bf06f5. Probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.
OnlyinstalledLive12Suite;REAPERskipped/pluginvalabsent/temporaryAUauvalinstallforbidden.
Missingexternalmachines/signers/participants do notpreventindependentlocalwork.
Sprint10exit:immutablepreparedresources/nonwaitingaudio/offaudioconfigurationwaits/
EVERY-writeracknowledgedreclamation/safeinvalidconcurrentlifecycle silence-error/complete
zero-prohibitedshippingcallbacks+concurrency-lifecycletests. No advancement/waiver.

## Complete next session prompt

[COMPLETE ready-to-paste Session24 prompt](../evidence/sprint-10-session-23/next-session-prompt.txt)
contains Session23authority/handoff/review plus ENTIRE immutableSession22prompt.
Session23 overrides superseded inheritedstatuses/jobs/identities/instructions.
