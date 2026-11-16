# SYS 01 Sprint10 session13 handoff

## Checkpoint

2026-10-21 00:11 America/Santiago. Sprint/session10/13. S10-AU-REGISTRY-08
LOCALLY VERIFIED at absent removal publication/reclamation / retained acknowledged
waiting removers boundary. Sprint10 IN PROGRESS; release NO-GO/development-unqualified.
Registry gate BLOCKING; wholeG01,actualDAW/externalAU,MB-04 open. No advancement/waiver.

Git root /Users/lhommeduchili/algorhythmia; SYS01 root
/Users/lhommeduchili/algorhythmia/sys-01. HEAD5c45cf6bed431317a1b412b1fc12ee62b66cfdd5
unchanged; no applicable AGENTS. Initial154source+25reference inputs ALL matched
session12. Dirty/untracked/unrelated work preserved; no subagents,installation,
staging,reset,stash,discard,commit,tag,contact,publication,personal plugin changes.
Final155source+27reference inputs. Source export
03c3328459d79c3559f83cb1b7714a7dc32a4db9cf5abc734f3426788b39b6b3;
resume-inputs bd4c46779f282c5f01284808b10d34d6232558ddb6a49ac1b79f0857a16c7c57.
All exact identities/flags/commands/failures/checkpoint/finalization in
[evidence](../evidence/sprint-10-session-13). Integrity is not acceptance.

## Changes and decisions

Only existing product implementation changed: AUPropertyRegistry.hpp. New
[ownership record](../au-property-absent-removal-session-13.md) and session-owned
loaded/direct fixtures. Unchanged session12 final ON Release AU fingerprint
63bfef40f73334c80da13d508a88328e0a479f16c4562934c99b3cb0dc23a8f1,
binary28b7b5854bd6b15713560a1dbca0cfe8792590cc9a3f6ef7ccc090c2155c9b21.
Actual native bypass Render callbacks remove an absent NickName registration.
Both real WithUserData and loaded SDK Lookup legacy selectors allocate/free in
registry remove. Counts1/8/64/257:legacy allocation2/16/128/514,free0/14/126/512;
WithUserData allocation3/16/128/514,free3/16/128/514. Probe control1/1/1/1; actual
allocation stack registry -> SDK removal -> host callback -> registry -> bypass ->
Render. No synthetic PropertyChanged/product observer. Actual prohibited audio
allocation/free,not reproduced deadlock/race/deadline miss. Host/unwinding separate.
Complete source/fixture/executable/probe/generated/cache/compile/29-fileSDK/bundles/
reports frozen BEFORE repair; never edit. Sandbox registration failure and SDK
unity lookup StopIteration tooling error retained; authorized loading installs nothing.

After ALL-frame scan and callback-origin required wait rejection, under gate return
noErr only when no matching other-thread frame AND no matching published Entry.
No allocation/retirement/publication/reclaim/demand there. Gate linearizes with EVERY
add/admission/withdrawal/exit. Published matches still mutate. Withdrawn other-thread
matches still wait; independent second removers retain acknowledgement. Same-thread
withdrawn contexts live until return. Later add is later publication; host destruction
must exclude new matching add. Errors authorize no release. Current/retired ordered
snapshots/duplicates/no cap/visibility/reader non-reclamation,constant unlink,
liveFrames/final Entry notification,WaitDemand retention/counts/exception cleanup/
EVERY-add reserve-before-publication remain. Gate retained.

All SDK routing/vector-empty/accessor-uncalled/source-only MIDI,ContextName wrapper
lifetime,complete joined wrapper/processor teardown,fixed message owner,updater
stop/clear before shutdown,GetMutexnull and serialized same-role offaudio restore
remain. Processor removeListener alone does not acknowledge full callbacks. APVTS
virtual authority -> raw mirror -> LockedListeners -> flags; regular/final listener
lock/editor removal; processor unlocked callback lifetime; VST3 separate atomic
floats/groupedflags/immutableIDs/sharedstorage;preparedmaps/scratch/DSP/callback/FL
locks remain open. Compatibility identity/7keys+IDs/4programs/schema1exactly40/
parameter-only empty-history recall/10ms smoothing/shared256wholebatch rejection/
noreplay/8restores/singleaudioowner/stereo generator disabledinput/formats/editor
atomicdirty+7packedfloat-generationwords/synchronizedremoval/gestures/undo preserved.

## Verification

| Check | Command registry reference | Result | Boundary |
|---|---|---|---|
| Initial comparison |initial-state.json|154+25 match|BEFORE edits|
| Loaded negative |s10s13-negative-authorized|exit1,actual allocations/frees|Frozen session12 ON AU|
| Loaded absent removal |s10s13-absent-repair/asan/tsan|PASS bothselectors1/8/64/257|Release0/0,separate sanitizer status/order|
| Shipping defaults |s10s13-absent-default-repair/asan/tsan|PASS|Exact OFF bundles,Release0/0|
| Direct allocation fault |s10s13-absent-protocol-repair/asan/tsan|PASS0alloc/free,preserved1/1,same-threadwithdrawn1|Supplemental direct protocol|
| Condition/publication |s10s13-condition-*/publication-*|intermediate0/ack0,final1/ack2,bothorders,newmatch PASS|Loaded condition + direct protocol|
| ON/OFF native/editor/updater |s10s13-*-final-*-native/editor/notifications/updater|PASS|Release,separateASan-UBSan/TSan|
| Core/pins/format/generators |s10s13-core/juce-pin/doctest-pin/format-final/generation|16of16,4377/270,format,3drift rejections PASS|Exact pins/source|
| Registry/whole probes |s10s13-release-registry-probe/default-au-probe/default-vst3-probe|registry0/0/3/0;whole0/0,locksremain|Finite Release scopes|
| Integrity |s10s13-checkpoint/finalization.json|PASS|OFFabsence,separateflags,14historicalinventories|

Inherited active/withdrawn callback-origin errors,bothselectors/secondremovers,
ordered257duplicates,overlap2/2,nestedContextName64,first-thread/contextdestruction128,
updater128fresh/4x256/64teardown,editor nested/direct7/64detach/Release3072four scales,
sanitizercontrols/gestures/undo/recall/close,strictAU1274,VST3 201894points,callback/
ownership/suppression/seals,adapter/recall3of3 PASS. Conditioncontrol1,idle/unrelated0,
olderunlink0 and new matching publication checks PASS. Absent removal retains1mutex
per call. Registry0/0/3/0. Whole defaultAUfresh1mutex9/hostile603,VST3fresh1024mutex9,
alloc/free0/0;G01 BLOCKING. Larger idle counts not remeasured in session13.

Successful final logs contain no observed assertion/ASan/UBSan/TSan diagnostic.
Historical session09 genuine default-stack overflow257 and session10 allocator/
leak assertions stay frozen. All nested257 fixtures use explicit8MiB SDK stacks/
legaljoins;sanitizer fixtures do not prove malloc interposition. No arbitrary stack,
lifecycle or whole realtime claim. Release editor is unpaced offscreen CPU,not DAW/
UIbudget qualification. All14 historical inventories291/505/357/364/409/366/397/422/
492/470/546/561/521/527 unchanged;Sprint08live comparison intentionally invalid.
Tools CMake/CTest4.4.4,format18.1.8,AppleClang21,macOS26.6.2/25G83arm64;exact
JUCE8.0.12/doctest2.4.12pins unchanged;probe59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.
Affected AUcallback/concurrency/lifecycle/host/validator/performance/soak/signing
proof invalidated. ActualDAW/externalplatform/signing/rights/accessibility/humans/
budgets/soak/MB-04 unperformed. No waiver.

Final Release OFF AU38d456f76206661922f8a1824e2f1a720602a115b4fa0165f358d811b11a1605,
binary1332af27bad4196e23b291d2c12e6b493243df4a4e95ccd376ef89187c78caef;
VST3ba83114ccd9a5ced94b7dda594ad8ae3b4fe9b9a34eaf63ba32f1c55335bf79f,
binaryb7bd4e8955c2799762954224f8076b63d409d9f241c75fe8f4466906fc58d081.
All ON/OFF/sanitizer identities in final-identities.json.

## Resume

All jobs completed;none to resume. Native/private/tmp/sys01-s09-repair,asan,tsan
finalOFF;core/private/tmp/sys01-s09-session02-core. Reconstruct missing trees from
exact pins/cache/export. Absolute DYLD_LIBRARY_PATH may resolve frozen rpaths;
never change frozen bytes. No installation/contact/publication authorized.

Session14 review complete registry gate/capture/admission/exit/finalnotify/absent
return/WaitDemand/remove/reclaim/EVERYadd/generatedrouting/trace. Reproduce/freeze ONE
remaining actual loaded-AU wait/order/callbackownership defect BEFORE ONE bounded
repair. Capture/admission/exit waits,finalbroadcasts,host-sized work,mutatingaudio
writersallocate remain. Then APVTS/listeners/VST3cache/immutableresources/lifecycle.
EVERYwriter/reader/context needs replacement publication+acknowledgedreclamation
before gate removal. Verify both selectors,withdrawn/newmatching/secondremovers,
nested/overlapexitorders,first-thread,ContextName,legalteardown. Sprint10 exit still
requires immutable preparedresources,nonwaiting audio,offaudio waits,EVERYwriter
reclamation,safeinvalidlifecycle silence/error,completezero-prohibitedshipping
callbacks/concurrency/lifecycle. Only installed AbletonLive12Suite allowed;REAPER
skipped/pluginval absent/temporaryauval requires forbidden install. No advancement.

## Complete next session prompt

[Complete ready-to-paste session14 prompt](../evidence/sprint-10-session-13/next-session-prompt.txt)
contains current session13 authority and ALL inherited detailed requirements.
