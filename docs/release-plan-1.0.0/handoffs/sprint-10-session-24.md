# SYS 01 Sprint10 Session24 handoff

## Checkpoint

UTC 2026-10-22T21:40:03.322926+00:00; America/Santiago. Sprint/session10/24.
S10-AU-MODE-01 LOCALLY VERIFIED at serialized native offline-mode continuity.
Sprint10 IN PROGRESS; release NO-GO/development-unqualified. Module registry gate and
framework callback locks BLOCKING. Prepared resources, nonwaiting acquisition,
EVERY-writer acknowledged reclamation, safe invalid concurrent lifecycle, G01,
actual DAW/external AU and MB-04 remain open. No advancement or waiver. Next: Session25.
All jobs complete; none to resume.

Git root /Users/lhommeduchili/algorhythmia; SYS01 root
/Users/lhommeduchili/algorhythmia/sys-01. HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5
unchanged; no applicable AGENTS. Initial165source+47reference inputs match Session23;
all25 historical inventories unchanged. Final166source+49reference. Dirty/untracked
SYS01 and unrelated monorepo work preserved. No subagents/install/staging/stash/reset/
discard/commit/tag/contact/publication/personal plugin changes. Only installedLive12Suite
permitted when relevant; missing external machines/signers/participants do not block local work.

Evidence [Session24](../evidence/sprint-10-session-24); exact commands, UTC, argv/cwd,
exits, tool/input/log hashes, sanitizer flags and bundles in checkpoint/commands/final-identities.
Source export SHA256 `00b210865dee8d9a9bdb30b4527cc1a1f94b21b89d35901adead0e70d9ec5494`;
resume inputs SHA256 `6068d65e11e2f317949e512c9e8f0e0db8735fdffed560348fcb918f890d9f81`.
Integrity is not acceptance; never reseal Sprint08's intentionally invalid live comparison.

## Changes and decisions

Only product scripts/native_au_adapter.py changes. The pinned generated AU OfflineRender
branch retains its existing callback lock/setNonRealtime/readback, but no longer calls
processor prepareToPlay. SYS01 uses identical DSP in both modes; unnecessary preparation
reset its ongoing history. Actual lifecycle preparation still resets resources/history.
No registry, parameter, buffer map, DSP source, listener, epoch or queue changes.
[Review](../au-mode-continuity-session-24.md).

Negative: unchanged Session23 shipping OFF AU
0453be945d32fe083298a4afb4e0ee6ccb5cc39987bf80eabb4f673749e30eb6;
binary0c05b7900202a8ffda8c541484deef76ba74680111fd7ed505e2c5c3d16a7126.
Two instances per study match32blocks. Toggling OfflineRender before64furtherblocks
causes64/64differences in bytebeat and cellularFM; silent-input live-transform control0.
Actual native calls on fresh workers COMPLETEjoin before serialized mode/lifecycle calls.
Complete negative sourceexport/generator/product/header/fixture+executable/generatedAU-
SDK-dispatch-VST3/cache/compile/pinnedSDK/bothbundles/reports frozen BEFORErepair.
No reproduced race/deadline/DAW claim. SDK CanSetMaxFrames rejects while initialized:
the initially investigated frame-limit mismatch was not established as a defect.

Module-wide registry source unchanged: ALL-frame callerorigin recognition/EVERYinstance;
matches and collectionowner==this; requiredotherthread callback-origin wait rejects-10863
BEFOREallocation/mutation; nullcallbackadd-50beforegate. EACHmatchingadd reservesEVERYdemand
beforeANYattachment/count/publication; CollectionMarks append-beforemark/ALLgateheldcleanup;
retainedEntryliveFrames/withdrawn-newmatching/independentremovers/spuriouswake/RAIIcleanup;
orderedduplicates/survivors/outer-nestedvisibility/reader nonreclaim/writeruse_count1;
constant crossinstanceincomingunlink/localfinalEntry notify/unlock-beforebroadcast remain.
Same-threadancestors retaincontext untilreturn; errorNOrelease. COMPLETEownerCV/wrapper/
processor/message/updater joins INCLUDINGunlockedtails before shutdown; foreigninstance may
remainlive; moduleunload joinsALLparticipants. Updaterstop-clear before registryshutdown
whileContextNamewrapperthis lives. Gate never spans hostcallback but remainsBLOCKING/host-sized.
No gateremoval without replacementpublication+acknowledged EVERYwriter-reader-contextreclaim.

PinnedSDK ALLroutes/emptyvectors/accessoruncalled/MIDIsourceonly/GetMutexnull remain.
APVTSauthority->rawatomicmirror->recursiveLockedListenerscallback->flags; regular-final
parameterLock/editor synchronizedremoval; processorsizeoutside/pointerunder/invokeunlocked/
finalforwardingunlocked remain. VST3 separateatomicfloats/groupedflags/immutableIDs/shared
storage unchanged. Preparedbuffer maps/scratch/MIDI/DSP/history retain legal lifecycle
exclusion; callbacklock does not provide immutablepublication/reclamation. Caller-serialized
sameroleoffaudio restore only; recursive/concurrentrestore/arbitraryhostcycles/ownermigration/
concurrentdestruction remainunqualified. Allidentity/7keys-nativeIDs/4programs-order/schema40/
emptyhistoryrecall/10ms/shared256wholebatchrejectnoreplay/8restores/singleaudioowner/formats/
editor7packedFLOAT-GENERATION/dirty/gestures-undo-recall-visibility preserved. No cap/loss/
TLS/table/queue/payload/gesturecoalescing. No pending new decision prevents authorized work.

## Verification

| Check | Exact command registry reference | Result | Boundary |
|---|---|---|---|
| Initial integrity |initial-state/historical-integrity-initial|165+47match;25inventories unchanged|BEFOREproductedits|
| Actual negative |s10s24-negative-authorized|expectedexit1;differences64/64/0|serializednative mode calls|
| Release ON/OFF |s10s24-complete-repair-authorized|exit0;144.98s|native/editor/continuity/ownership|
| ASan-UBSan ON/OFF |s10s24-complete-asan-authorized|exit0;187.58s|separateinstrumentedproduct/generated/fixtures|
| TSan ON/OFF |s10s24-complete-tsan-authorized|exit0;442.13s;no retry|separateinstrumentedproduct/generated/fixtures|
| Mode continuity |*-offline|ALL6x3studies0differences/64toggles|32baseline/COMPLETEfreshworkerjoins|
| Queued ingress continuity |s10s24-supplemental-*-authorized;*-mode-ingress|ALL6x3studies0differences|positionedFLUX/OUTPUT/MIDIBIAS beforetoggle|
| Native ownership |*-registry/cross-instance/cross-audio/ownership/ownership-v3|PASS|BOTHselectors;1/8/64/257;ContextName/firstworker/legaljoins|
| Native/editor |native/notifications/updater/editor+regressions|strictAU1274/VST3201894;adapter/recall/editorPASS|existingnativefixturesrebuilt|
| Core/pins/format/generation |s10s24-core/juce-pin/doctest-pin/format/generation|16/16;4377+270;3driftrejectionsPASS|exactpinnedsource|
| Final probes/checkpoint |default-au/vst3-probe;s10s24-checkpoint|PASS;OFFhook/probeabsence|wholeallocfree0/0;locksremain|

Same immutable matrix driver `903c27e83ddf528cb808adfed5351f9d11934d9947d00ced10d77fcba5251484` for all3trees; no retry.
Successful finalreports noobservedassertion/ASan/UBSan/TSandiagnostic. Only2inheritedEXTERNAL
bootstrapmutexshims excludeinstrumentation in reusedcrossaudiofixtures; product/generated/
newcontinuityfixtures instrumented. Cross-selector existing exactexecutables copied/retained;
newmodefixtures separatelycompiled withsanitizerflags. Older direct24/120/34/84/53/11fault-
protocols inheritedUNCHANGED, NOT rerun thissession (registryunchanged). Explicit8MiBSDKstacks
in inheritednestedtests/freshworkerjoins/unpacededitor are finiteconditions, not arbitrary
hoststack-lifecycle or DAWbudgetproof. Actualsource/generateddeltas verified; SDK/VST3 unchanged.

Failures retained: sandboxfactoryregistration beforecases;expectednegativeexit1; enum-bitwise
compilerwarning innewfixtures. ALLSession23andolder failures remain immutable. No failure-free
history. ToolsCMake-CTest4.4.4/format18.1.8/AppleClang21/macOS26.6.2-25G83arm64/exactJUCE8.0.12
revision29396c22c93392d6738e021b83196283d6e4d850/doctest2.4.12revision1da23a3e8119ec5cce4f9388e91b065e20bf06f5.

FinalReleaseOFF AU `61c3f1af60ccffcb022b3ad326463e30e1e74dac2add439c81e932a62c8ee163`,
binary `5193c4f21b68bf7f2cfdc93f44f9709f11ede6d2c570f0ad0db48c8b6f412a30`;
VST3 `11f3dac8465ab886c71a7a8a639a050419d586afde2696f10fd2d9c9fc4adb3c`,
binary `cd659d2238ab035db14be93fee4037aca9bac7a8cec6e823a1c135eba1176dcb`.
Wholeallocfree0/0 but AUfresh1mutex9/hostile603,VST3fresh1024mutex9. RegistrygateBLOCKING;
registry0/0/3/0 inherited notremeasured. No G01/DAW/MB04 advancement.

## Resume

Session25:progressarchitecturalSprint10exit. Start scripts/native_au_adapter.py generated
prepareToPlay/Cleanup/Reset/processBlock and PluginProcessor prepare/process resources,
then EVERY lifecycle writer/reader/context path, APVTSregular-finalparameter/processor
listeners and VST3cache/resources. ChooseONEactualremainingloadedwait/order/callbackownership
orpreparedresource defect. Reproduce/freezeCOMPLETEnegative BEFOREonecoherentboundedrepair.
Do not removecallback/registrylocks without immutablepublication/nonwaitingacquisition+
EVERYwriteracknowledgedreclamation and safeinvalidlifecycle path. No manufacturedfailure or
historicalaudit/packaging-onlysession. Preservemodehistory andpositionedingresscontinuity.

Alljobscomplete;none toresume. Native/private/tmp/sys01-s09-repair,asan,tsan finalOFF;
core/private/tmp/sys01-s09-session02-core. Reconstructmissingtemps frompins/cache/sourceexports;
DYLD_LIBRARY_PATHmayresolvefrozenrpaths,nevereditfrozenbytes. OnlyinstalledLive12Suite;
REAPERskipped/pluginvalabsent/temporaryauvalinstallforbidden. ExternalOS-architecture/signing/
rights/accessibility/humans/budgets-soak/actualDAW/externalAU/MB04remainopen. Sprint10exit
immutablepreparedresources/nonwaitingaudio/offaudioconfigurationwaits/EVERYwriterreclaim/
safeinvalidconcurrentlifecycle silence-error/completezeroprohibitedshippingcallbacks+
concurrency-lifecycletests. No advancement/waiver.

## Complete next session prompt

[COMPLETE ready-to-paste Session25 prompt](../evidence/sprint-10-session-24/next-session-prompt.txt)
contains Session24 authority plus ENTIRE immutable Session23prompt. Session24 prevails over
all inherited historical jobs/statuses/identities/instructions.
