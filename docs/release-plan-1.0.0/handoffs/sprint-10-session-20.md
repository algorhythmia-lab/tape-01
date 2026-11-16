# SYS 01 Sprint10 session20 handoff

## Checkpoint

Date/time/timezone: 2026-10-22T03:10:44.492952+00:00 / America/Santiago.
Sprint/session: 10/20. Objective: freeze one actual loaded-AU defect, make one bounded
repair, and verify publication and callback acknowledgement.

S10-AU-REGISTRY-15 LOCALLY VERIFIED at amortized linear matching demand retention
growth / ordered publication / acknowledged callback ownership. Sprint10 IN PROGRESS;
release NO-GO/development-unqualified. Registry gate BLOCKING; G01, actual DAW/external
AU, MB-04, immutable prepared resources and safe invalid lifecycle remain open.
No advancement or gate waiver. Next: **Session21**.

Git root `/Users/lhommeduchili/algorhythmia`; SYS 01 root
`/Users/lhommeduchili/algorhythmia/sys-01`. HEAD
`5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` unchanged; no applicable AGENTS.
Initial161 source+39 reference all matched session19 BEFORE edits. Final162+41.
All21 historical inventories, dirty/untracked SYS01 and unrelated monorepo work
preserved. No subagents/install/staging/reset/stash/discard/commit/tag/contact/
publication/personal plugin-directory changes. All jobs complete; none to resume.

Evidence: [session20](../evidence/sprint-10-session-20).
Source export SHA256 `85b1c4ac389be0be51b26694464b0c475033b53ef9eba927b08e021503ccdf93`;
resume inputs SHA256 `8b120d9b689e25f31f1b8d201bb3d79706077b5b236057940b4fdaf7f026a9fd`.
Exact bundles/binaries in final-identities.json; final command/report counts and
complete inventory in finalization.json/evidence-inventory.json. Integrity is not
acceptance; Sprint08 live comparison intentionally invalid, never reseal.

## Changes and decisions

Only product [AUPropertyRegistry.hpp](../../../apps/desktop/AUPropertyRegistry.hpp)
changes. EACH matching waiting remover's vector grows only when full, geometrically,
with overflow-safe saturation at max_size. At vector limit, size+1 still throws.
ALL throwing preparation (snapshot/storage/Entry/EACH demand reserve/retired retention)
precedes ANY attachment/count/publication. Earlier reserve failure may change capacity
only, never ownership or publication. ALL matching independent owners receive new
entries/counts before publication under SAME gate. No registration cap or loss.
Quiescent default-OFF diagnostic counts actual retention growth only. See
[ownership review](../au-property-demand-growth-session-20.md).

Actual negative uses session19 algorithm with count-only observer. Real bypass
SetParameter -> Render -> host callback -> actual SDK NickName callback -> native add
while two independent removers wait. BOTH real selectors. Additions1/8/64/257:
reallocations2/16/128/514, shared_ptr copies2/72/4160/66306. Wait/ack2/calls additions+1/
no replay already correct. Frozen AU `7559f5cf72e545ef425778e05df05c7a61e5d667aaefa499bfb445f06b4f8d4e`.
Complete negative source/header/export/fixture/executable/generated AU-SDK-VST3/cache/
compile/pinned SDK/bundles/reports frozen BEFORE repair; never edit frozen bytes.
Avoidable quadratic AUDIO-reentered gate-held retention work, not total-add linearity,
allocation-free audio/deadline/deadlock/race/budget proof.

Final ON growths2/8/14/18 and copies2/30/254/1022; OFF statuses/delivery/lifetime pass.
New matching publication holds a distinct callback after original Render return:
BOTH removers remain waiting, recheck, then acknowledge after new owner returns.
Complete Render/property/remover joins precede later matching withdrawal/context
destruction. Explicit8MiB SDK worker stack remains a fixture condition only.

Preserve ALL-frame rejection BEFORE allocation/mutation/errorNOrelease; no-ack,
absent/withdrawn-only; CollectionMarks append-before-mark and gate-held ALL cleanup
success/exception BEFORE publication/demand/wait; independent WaitDemand predicates/
spurious wake/RAII exception cleanup; ordered duplicates/outer-nested visibility;
writeruse_count1 reclamation and reader non-reclaim; constant unlink/liveFrames/local
notify/unlock-before-broadcast/complete ownerCV lifetime. EVERY writer/capture/admission/
exit still holds gate. Same-thread ancestors retain context; context destruction excludes
new registration and withdraws later publication. Complete wrapper/processor/message
owner joins include unlocked tails; listener removal alone does not acknowledge whole
changed/updateHostDisplay/final forwarding. Updater shutdown precedes registry while
ContextName wrapper-this lives. No callback disposal/gate removal/TLS/table/queue/
payload/cap/loss/gesture coalescing.

ALL SDK virtual Add/Remove/PropertyChanged routing/empty SDK vector/uncalled accessor/
MIDI source-only/GetMutex null retained. APVTS authority->rawatomicmirror->LockedListeners
callback->flags; regular/final parameter lock/editor synchronized removal; processor
sizeoutside/pointerunder/invokeunlocked/finalforwardingunlocked; VST3 separate atomic
floats/groupedflags/immutable IDs/shared component-controller storage retained.
Prepared maps/buffers/DSPscratch/history/callback/framework locks and caller-serialized
offaudio restore retain legal lifecycle exclusion. Immutable resources/nonwaiting
audio/safe invalid lifecycle remain required. Preserve identity/7keys-nativeIDs/
4programsorder/schema40/emptyhistoryrecall/10ms/shared256rejectnoreplay/8restores/
singleaudioowner/formats/editor dirty+7packed FLOAT-GENERATION/gesture-undo-recall-visibility.

## Verification

| Check | Command registry reference | Result | Boundary |
|---|---|---|---|
| Initial integrity | initial-state / historical-integrity-initial | 161+39 match;21 inventories unchanged | BEFORE edits |
| Actual loaded negative | s10s20-negative-authorized | expected exit1;copies2/72/4160/66306 | real Render + BOTH selectors |
| Release | s10s20-complete-repair-authorized | ON/OFF PASS | exact final source/bundles |
| ASan-UBSan | s10s20-complete-asan-authorized | ON/OFF PASS | separate product/generated/fixtures |
| TSan | s10s20-complete-tsan-authorized | ON/OFF PASS | separate product/generated/fixtures |
| New demand-growth faults | s10s20-supplemental-authorized |84cases/tree PASS;252 total | full/spare capacities/EACH reserve fault/retry/ack2 |
| Inherited faults/protocols | *collection/storage/add/publication/boundary/absent/withdrawn* |24/120/34cases/tree and ownership PASS | cleanup/newmatching/secondremovers/pausedtail/overlap |
| Native/editor | native/editor/notifications/updater logs/reports | PASS | firstthread128/ContextName64/order257/strictAU1274/VST3201894/adapter-recall/editor |
| Core/pins/generation/probes | s10s20-remaining-authorized |16/16;4377+270pins;3driftrejections;probes PASS | exact source and bundles |
| Format/checkpoint/final records | s10s20-format-final / checkpoint / final-records | PASS | defaultsOFF/hookabsence/generated/inventories |

All six loaded demand-growth ON/OFF checks PASS; all final successful reports contain
no observed assertion/ASan/UBSan/TSan diagnostic. All three complete drivers passed
without retry on same immutable driver SHA256
`76b7e3aafb69a12d0ff16c08c96d170bf682d3f106776d3e648625514eafd47d`.
Only nonzero registered command is expected loaded negative. Historical genuine
stack overflow/allocator leaks/bootstrap-rpath/invalid fixture leaks-race-SEGV-KeyError/
ps denial/session19core+TSanCTesttimeouts+fixturecounterrace+failed drivers remain frozen.
Never failure-free history or arbitrary host-stack/lifecycle/realtime qualification.
Only two EXTERNAL bootstrap shims exclude instrumentation; product/generated/callback/
direct fixtures remain separately instrumented. Editor offscreen unpaced CPU not DAW budget.

Final all three native trees OFF; fixture hooks including registryDemandProbe and probe
dependency absent. Generated wrappers equal session19. Exact final Release OFF AU
`c8d84fcd6d23b7fb98a7ecf28c99971da333751d27f738c6f246061ff6d3bcc0`,
binary `ae4dd72575a3a307b8b30d79a9e4a8524041aafd1236309ddbb6fae93fa3456c`;
VST3 `0114a62dfcf3aa911405cfaea465fd7efbe64a2aa03eba197304d2c811049ea8`,
binary `70e603f49254e7a8df6065e31882354c28011f6da42312fddc5a34ad78cdfcfa`.
Registry alloc/free/mutex/unfair0/0/3/0; whole alloc/free0/0 but AUfresh1mutex9/hostile603,
VST3fresh1024mutex9 remain. Larger idle counts not remeasured. No wholeG01 qualification.

CMake/CTest4.4.4, clang-format18.1.8, AppleClang21/macOS26.6.2-25G83 arm64.
JUCE8.0.12 revision29396c22c93392d6738e021b83196283d6e4d850; doctest2.4.12
revision1da23a3e8119ec5cce4f9388e91b065e20bf06f5. Tool/dependency/source/generated/
fixture/artifact hashes and actual argv/cwd/UTC/exits in records. Original probe
59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.
Affected AUcallback/publication/ownership/performance/soak/signing renewed ONLY at named
local boundaries; historical evidence immutable. Actual DAW/externalAU/platforms/
OS-architecture/signing-rights/accessibility/humans/budgets-soak/MB04 unqualified.
Only installed Live12Suite;REAPERskipped/pluginvalabsent/temporaryauval requires forbidden install.

## Resume

Session21: review COMPLETE registry gate/capture/admission/exit/unlocked-tail/ownerCV,
ALL-frame rejection/no-ack, CollectionMarks cleanup, EACH add's geometric reserve
and ALL throwing-before-attachment/publication boundary, WaitDemand predicates/
reclamation and generated routing. Freeze ONE remaining concrete actual loaded-AU
wait/order/callback ownership defect BEFORE ONE coherent bounded repair. Review ALL
writer/reader/context paths before replacing ownership or removing gate. Then APVTS/
regular-final parameter+processor listeners/VST3 cache/prepared resources/lifecycle.
Do not repeat only audits or claim geometric demand growth makes all add work linear.

All jobs complete; none to resume. Native `/private/tmp/sys01-s09-{repair,asan,tsan}`
final OFF; core `/private/tmp/sys01-s09-session02-core`. Reconstruct missing temps
exactly from pins/cache/export; DYLD_LIBRARY_PATH may resolve frozen rpaths, never edit bytes.
No new installation/contact/publication/personal plugin authorization.

Remaining Sprint10 exit: immutable prepared resources; nonwaiting audio; offaudio
configuration waits; EVERY-writer acknowledged reclamation; safe invalid/concurrent
lifecycle silence/error; complete zero-prohibited shipping callbacks and concurrency/
lifecycle verification. Capture/admission/exit waits, OSbroadcasts, host-sized dispatch/
remove/reclaim/add snapshot and demand scans, allocating audio-reentered writers,
listener ownership/G01/actualDAW/externalAU/MB04 remain open. No advancement or waiver.

## Complete next session prompt

[Complete Session21 prompt](../evidence/sprint-10-session-20/next-session-prompt.txt)
contains session20 authority plus complete immutable session19 inherited detail.
