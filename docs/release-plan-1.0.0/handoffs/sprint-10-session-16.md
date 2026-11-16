# SYS 01 Sprint10 session16 handoff

## Checkpoint

2026-10-21T13:23:46.370622-03:00. Sprint/session10/16. S10-AU-REGISTRY-11 LOCALLY VERIFIED at add
snapshot preparation / ordered publication and independent demand ownership boundary.
Sprint10 IN PROGRESS; release NO-GO/development-unqualified. Registry gate BLOCKING;
wholeG01, actualDAW/externalAU and MB-04 remain open. No advancement or waiver.

Gitroot /Users/lhommeduchili/algorhythmia; SYS01root /Users/lhommeduchili/algorhythmia/
sys-01. HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5 unchanged; no applicable AGENTS.
Initial157source+31reference ALLmatched session15 before product editing; final158+33.
Dirty/untracked/unrelated work preserved; no subagents/install/staging/reset/stash/
discard/commit/tag/contact/publication/personalpluginchanges. All jobs complete;
none to resume. Exact identities/commands/flags/failures in [evidence](../evidence/sprint-10-session-16).
Sourceexport852339cb95d768c19fb2215494f0326a908d7c561b576b7b53cf95d44ff089a4; resumeinputs70254f66586600b619298169a1ee4ad43e2bd7ee3ccefe6c2fa683042b99e262.
Integrity is not acceptance. All17 historical inventories unchanged; Sprint08 live
comparison intentionally invalid, never reseal.

## Changes and decisions

Only product implementation AUPropertyRegistry.hpp add changed. New
[ownership record](../au-property-add-publication-session-16.md), session16 fixtures,
frozen negative, source/evidence inventories and continuation prompt added.
Unchanged session15 ON Release AU8ef64259b675bd072947ddc646a289e60d48ec3f87a9bd691b7300ba7bce9732,
binary3dd60af6a8a8b9b5909f94d3b309b9fd5c0be044da5cf2c109499e90157bd4c9.
Real bypass SetParameter consumed in Render freshworker invokes host callback
which calls actualSDK add. ExistingNickName duplicates1/8/64/257, appendone:
allocfree4/1, expected3/0. Both real WithUserData and loadedSDK Lookup legacy
removals preserve callbacks2/9/65/258 and preventreplay. Negative printed overall
publication FAIL includes allocation mismatch; delivery counts themselves correct.
Original-call control1/1/1/1; captured registryadd->SDKadd->hostcallback->registrychanged
->bypasslistener->publishNativeParameter->Render->SDK->AudioUnitRender. Complete
source/fixtures/executable/probe/generatedAU-SDK-VST3/cache-compile/29fileSDK/bundles/
reports frozen BEFORE repair. Hostsetup/barriers/unwinding separate; no synthetic
PropertyChanged/productnegativeobserver. Prohibited audio allocation/free, not
reproduceddeadlock/race/deadlinemiss/timingbudget claim.

Under SAMEgate construct empty unpublished snapshot, reserve(current size+1),
ordered insert, construct newEntry and append withoutgrowth. ALLthrowing snapshot/
storage/copy/Entry/append/EACHmatchingdemandreserve/retired.push_back remain BEFORE
ANYdemand attachment/count/publication. Reservefailurecapacityonly; no logical
publication/count/withdrawal. EVERYpendingmatchingindependentdemand retains/counts
newEntry before current publication; newframe extends BOTHremovers. Same current/
retired/active/capture/admission/withdrawal/ALLframe callbackoriginrejection/
publishedMatch/absentreturn/withdrawnonlywait/WaitDemand/RAIIgateheldcleanup/reclaim.
Orderedduplicates/nocap/outer-nestedvisibility/writeruse_count1retiredreclaim/normal
readerneverlastdelete retained. Snapshot+storage+Entry allocations,host-sizedcopy,
demandcapacity/reclaim and gate remain prohibited. No reader allocation/reclaim,
TLS/table/queue/listenerloss/payload workaround/gesturecoalescing or gate removal.

Constantunlink/backlink/liveFrames/localfinalnotify/unlockBEFOREbroadcast/ownerCV
completechangedlifetime unchanged. COMPLETEwrapper/processor entries+fixedmessage
owner exclusion/joins BEFOREshutdown/CVdestruction, including unlockednotifytail.
Listenerremove alone acknowledges neitherwholechanged nor updateHostDisplay/final
forwarding. Same-thread active/ancestor contexts surviveuntilreturn/errorsNOrelease;
context destruction excludes newmatchingadd and withdraws laternewregistration.
Never callback disposal. Updaterstopclearbeforeregistryshutdown whileContextName
wrapperthislives/GetMutexnull. ALLSDKvirtualroutehere/SDKvectorempty/accessoruncalled/
conditionalMIDIsourceonly. Caller-serializedsameroleoffaudiorestore; recursive/
concurrentrestore,ownermigration,arbitrarylifecycle/hostdependencycycles unqualified.

Continued pinned review:APVTSauthority->rawatomicmirror->LockedListenerscallback->
flags;regular-finalparameter listenerLock+stableeditor synchronizedremoval;
processor sizeoutsidelock/pointerunderlock/invokeunlocked/finalforwardingunlocked.
VST3 separateatomicfloats/groupedflags/immutableIDs/sharedstorage,notpackedwords/
eventhistory. Preparedmaps/scratch/DSPprepare-release/callback-FLlocks/resources/
invalidconcurrentlifecycle remain. Neverreplacewithout EVERYwriter-reader-lifetime
publication/reclamation. Preserveidentity/7keys-nativeIDs/4programs/schema40/
parameter-onlyemptyhistoryrecall/10ms/shared256rejectnoreplay/8restores/singleaudio
owner/formats/editoratomicdirty+7packedfloatgeneration/removal/gesturesundo.
No pending decision stops independent authorized work; broader gates remain open.

## Verification

| Check | Command registry reference | Result | Boundary |
|---|---|---|---|
| Initial inputs/integrity |initial-state.json/historical-integrity-initial.json|157+31match/all17inventoriesunchanged|BEFORE product edit|
| Loaded negative |s10s16-negative-authorized|expected exit1;allocfree4/1;deliverycount+1|unchanged session15 ON Release AU|
| Loaded add ON/default |s10s16-*-final-*-add|PASSbothselectors1/8/64/257;Release3/0|sanitizersstatus-delivery-lifetime,notmallocproof|
| DIRECT addfault/newmatch |s10s16-*-add-protocol|PASS34cases/tree;budget0..2fail/3success;pending0..5fail/6success|BOTHdemandowners ack2/calls2/unrelated1|
| Release/ASan-UBSan/TSan ON/OFFnative-editor |s10s16-complete-*-authorized and nested records|PASSall3trees|generatedAU/SDK/VST3instrumentation|
| Withdrawn/absent/order |s10s16-*-final-*-withdrawn/absent/condition|PASSbothselectors;withdrawnRelease1/1;absent0/0;broadcastgateheld0|legal complete joins|
| Pausedtail/publication/protocol |s10s16-*-boundary/publication/absent-protocol/withdrawn-protocol|PASSbothselectors/newmatch/secondwait/overlapbothorders/exception|same independentack/lifetime|
| Core/pins/format/generation |s10s16-core/juce-pin/doctest-pin/format-final/generation|16of16/4377+270/format/3driftrejectionsPASS|exactpins/source|
| Releasecallbackprobes |s10s16-release-registry-probe/default-au-probe/default-vst3-probe|registry0/0/3/0;whole0allocfree,locksremain|finite exactbundles|
| Checkpoint/finalization |s10s16-checkpoint/final-records/finalization.json|PASS|finalOFFabsence/flags/source-export/evidenceintegrity|

Inherited active/withdrawnrejection/secondremovers/overlapbothorders/order257/
ContextName64/firstthreadcontextdestruction128/updater128fresh+4x256+64teardown/
editornested+direct7+64detach/Release3072fourscales/sanitizercontrols-gestures-undo-
recall-close/strictAU1274/VST3201894points/callbacks-suppression-seals/adapter-recall
3of3 PASS. Loadedorderingbothselectors1/8/64/257 gateheld0,broadcast1,ack2/control2/1.
DIRECTpausednotifywriter+secondbeforebroadcast/newmatchingwait/broadcast1/1 PASS.
Registry0/0/3/0;wholedefaultallocfree0/0 but AUfresh1mutex9/hostile603/VST3fresh1024
mutex9 remain. Largeridlecountsnotremeasured. Final defaultsOFF symbols/strings/
probeabsent, generatedwrappersidenticalsession15. RELEASEVST3bundlesidentical;
sanitizer rebuild identities are separately recorded, source unchanged.

Actualsession16failuresretained:sandboxnegativefactoryregistration beforetests;
authorizednegativeexpectedFAILwithlegaljoins. InitialrunningReleasedriver didnot
include newAddProtocolTests yet; exactinitial-complete_matrix.py preserved matching
start hashd77b96f7e8300877dcdbb3f08d210e1bf0355108ed9e39a8e96927f1eb61d899.
Releaseaddprotocolseparatelycompiled/runPASS;ASan/TSan include normally. Existing
SYS01_DEFAULT_ORDERING correctlyusedall3trees. No otherfailedsession16commands.
Retain historicalsession09ASandefaultstackoverflow257/session10allocator-leaks/
session14externalTSanbootstrap+missingrpath/session15invalidfixtureleaks+TSanrace-
SEGV/checkpointKeyError. Successfulfinalreportsnoobservedassertion/ASan/UBSan/TSan;
notfailurefreehistory. Only inherited2externalmutexbootstrapshim functions exclude
instrumentation; product/generatedAU-SDK-VST3/callback-protocolfixtures instrumented.
Explicit8MiBSDKstack/legaljoins,notarbitraryhoststack-lifecycle. Editorunpaced
offscreenCPU,notDAW/UIbudget. ExternalAU/actualDAW/OS/architecture/signing/rights/
accessibility/humans/budgets/soak/MB04unperformed/unqualified. OnlyinstalledLive12
Suiteallowed;REAPERskipped/pluginvalabsent/temporaryauvalrequiresforbiddeninstall.

ToolsunchangedCMake-CTest4.4.4/format18.1.8/AppleClang21/macOS26.6.2-25G83arm64/
JUCE8.0.12revision29396c22c93392d6738e021b83196283d6e4d850/doctest2.4.12revision
1da23a3e8119ec5cce4f9388e91b065e20bf06f5/probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.
Tool/source/fixture/generated/cache/dependency/artifact/log identities recorded.

FinalReleaseOFFAU64cf0e356ebc3990132d521f8c9cf463ac57e2c9c987f96e52337c26d52a51c1,
binaryba8462f59019606b68873cbb91490e8dedc09360292d201d84f6f1987753c29f;
VST35292ca55840ba915439f0b142da96246ef5669d659972a3fce03ad74658a498e,
binary41dc05f829fad1af84b95d9ba244f4211240fec5601ed6421729c8ce7ed83ce5.
Otherexactidentities/counts in checkpoint/final-identities/finalization/evidenceinventory.

## Resume

Alljobscomplete;none toresume. Native/private/tmp/sys01-s09-repair,asan,tsanfinalOFF;
core/private/tmp/sys01-s09-session02-core. Reconstructmissingtemps exactpins/cache/
sourceexport;absoluteDYLD_LIBRARY_PATHmayresolvefrozenrpaths,nevereditbytes.
Session17reviewcomplete AUPropertyRegistry.hpp gate/capture/admission/exit/unlocked
notifytail/ownerCV/absent/withdrawn-only/publishedMatch/WaitDemand/remove/reclaim/
EVERYadd fullsnapshotreserve/orderedcopy/demandreservation-retiredthrowingboundary,
native_au_adapter.py/routingtrace. ReproducefreezeONEremainingactual loadedAU wait/
order/callbackownership defect BEFOREONEboundedrepair. VerifyEVERYwriter-reader
publication/reclamation/bothselectors-active-withdrawn-newmatching-secondremovers-
nestedoverlapbothorders-firstthread-ContextName-completelegaljoins-contextdestruction-
orderduplicates-visibility-readernonreclaim. ThenAPVTS/regular-finalparameter+
processorlisteners/VST3cache/immutablepreparedresources/lifecycle. Capture/admission/
exitwaits/finalOSbroadcasts/host-sizeddispatch-remove-reclaim-add-demand/allocating
mutatingaudio-reenteredwriters/host-crossinstancecycles/parameterlockorders remain.
Sprint10exitrequiresimmutablepreparedresources/nonwaitingaudio/offaudiowaits/EVERY
writeracknowledgedreclamation/safeinvalidlifecycle silence-error/completezero-
prohibitedshippingcallbacks-concurrency-lifecycle. No advancement/waiver/install/
contact/publication.

## Complete next session prompt

[COMPLETE ready-to-paste session17 prompt](../evidence/sprint-10-session-16/next-session-prompt.txt)
contains session16 authority plus all inherited detailed requirements.
