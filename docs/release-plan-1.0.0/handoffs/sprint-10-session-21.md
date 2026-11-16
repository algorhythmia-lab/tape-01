# SYS 01 Sprint10 session21 handoff

## Checkpoint

Date/time/timezone: 2026-10-22T15:37:54.715894+00:00 / America/Santiago.
Sprint/session: 10/21. Objective: freeze one actual loaded-AU defect, make one bounded
repair, and verify publication and callback acknowledgement.

S10-AU-REGISTRY-16 LOCALLY VERIFIED at retained matching-entry wait predicate /
ordered publication / acknowledged callback ownership. Sprint10 IN PROGRESS;
release NO-GO/development-unqualified. Registry gate BLOCKING. G01, actual DAW/external
AU, MB-04, immutable prepared resources and safe invalid concurrent lifecycle remain
open. No advancement or gate waiver. Next: **Session22**.

Git root `/Users/lhommeduchili/algorhythmia`; SYS 01 root
`/Users/lhommeduchili/algorhythmia/sys-01`. HEAD
`5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` unchanged; no applicable AGENTS.
All 162 source + 41 reference inputs matched Session20 BEFORE edits. Final 163 + 43.
All 22 historical evidence inventories and ALL dirty/untracked SYS01/unrelated
monorepo work preserved. No subagents/install/staging/reset/stash/discard/commit/tag/
contact/publication/personal plugin-directory changes. All jobs complete; none to resume.

Evidence: [session21](../evidence/sprint-10-session-21).
Source export SHA256 `f35a001a50f63a7d92e5e03bdf5ad012fd311df9f81d85da3452c533cb5b1a30`;
resume inputs SHA256 `67cd97ae5ab410a3f93f5d13312a00e803a31277d7ac36752096b872b01cd534`.
Exact bundles/binaries in final-identities.json; complete command/report counts and
inventory in finalization.json/evidence-inventory.json. Integrity is not acceptance;
Sprint08 live comparison remains intentionally invalid, never reseal.

## Changes and decisions

Only product [AUPropertyRegistry.hpp](../../../apps/desktop/AUPropertyRegistry.hpp)
changes. Successful waiting removers own no caller-thread callback frame: the initial
COMPLETE ALL-frame scan rejects callback-origin required other-thread waits BEFORE any
allocation/mutation. Collection retains every matching live Entry including withdrawn
entries; CollectionMarks clears ALL temporary marks under gate before publication/demand.
EVERY later matching add reserves EACH independent demand before ANY attachment/count/
publication. The same gate owns ALL admission/exit/liveFrames updates. Therefore the
waiting predicate can check retained Entries' liveFrames != 0 instead of walking
unrelated or repeated live frames. See [ownership review](../au-property-retained-predicate-session-21.md).

Actual negative: real native bypass SetParameter consumed in Render holds its registered
host callback; separate real SDK NickName publication holds unrelated nested frames.
Both real WithUserData and SDK Lookup selectors at depths1/8/64/257 visit2/9/65/258
before waiting and1/8/64/257 after target return, expected1/1. BOTH independent removers
already acknowledge2 while unrelated frames remain held; complete Render/property/remover
joins precede withdrawal/context destruction. Negative AU fingerprint
`28390c8fa86d0c0fb45169300d25046783899e7caaffde7d5da5de590ddd95d6`.
Complete negative source/header/export/fixture/executable/generated AU-SDK-VST3/cache/
compile/pinned SDK/bundles/report frozen BEFORE repair; never edit frozen bytes.
Avoidable OFF-CALLBACK gate-held traversal, not reproduced deadlock/race/audio allocation/
deadline/budget evidence. Final ON visits1/1 at ALL depths/BOTH selectors; default-OFF
statuses/delivery/ownership PASS. No new allocation/lock or reader demand scan.

Preserve initial ALL-frame rejection/no-ack/absent/withdrawn-only paths, independent
WaitDemand predicates/spurious wakes/gate-held RAII exception cleanup; geometric EACH
matching demand reserve with ALL throwing work before attachment/count/publication;
capacity-only failure/no cap/loss; ordered duplicates/survivors/outer-nested visibility;
reader non-reclaim/writer retired use_count1 reclamation; constant unlink/backlinks/
final Entry notification/local decision/unlock-before-broadcast/COMPLETE owner-CV lifetime.
Retained matching Entries remain host-sized, including completed or unadmitted new Entries.
No constant total remove/add or realtime qualification. EVERY writer/capture/admission/
exit still holds BLOCKING gate. No gate removal without replacement publication and
acknowledged EVERY-writer-reader-context reclamation. No TLS/table/queue/payload/gesture
coalescing. Same-thread ancestors retain contexts; errors authorize no context release.
Host excludes new registration and withdraws later matching publication before destruction.
Legal teardown joins COMPLETE wrapper/processor/message-owner entries including unlocked
tails. Listener removal alone acknowledges neither whole changed nor updateHostDisplay/
final forwarding. Updater shutdown precedes registry while ContextName wrapper-this lives.

ALL generated SDK Add/Remove/PropertyChanged routing/empty SDK vectors/uncalled accessor/
conditional MIDI source-only/GetMutex null retained. APVTS authority->raw atomic mirror->
recursive LockedListeners callback->flags; regular/final parameter lock and synchronized
editor removal; processor size outside/pointer under/invoke unlocked/final forwarding
unlocked; VST3 separate atomic floats/grouped flags/immutable IDs/shared component-controller
storage retained. Prepared maps/buffers/DSP scratch/history/callback/framework locks and
caller-serialized offaudio restore retain legal lifecycle exclusion. Arbitrary concurrent/
recursive restore, owner migration/host cycles/lifecycle remain unqualified. Preserve
identity/7keys-nativeIDs/4programs-order/schema40/empty-history recall/10ms smoothing/
shared256 whole-batch rejection-no replay/8restores/singleaudioowner/formats/editor dirty+
7 packed FLOAT-GENERATION words/gestures-undo-recall-visibility.

## Verification

| Check | Command registry reference | Result | Boundary |
|---|---|---|---|
| Initial integrity | initial-state / historical-integrity-initial | 162+41 match;22 inventories unchanged | BEFORE edits |
| Actual loaded negative | s10s21-negative-new-bundle-authorized | expected exit1;visits2/9/65/258 and1/8/64/257 | BOTH real selectors;ack2 PASS |
| Release | s10s21-complete-repair-authorized | ON/OFF PASS | exact final source/bundles |
| ASan-UBSan | s10s21-complete-asan-authorized | ON/OFF PASS | separate product/generated/fixtures |
| TSan | s10s21-complete-tsan-authorized | ON/OFF PASS;no retry | separate product/generated/fixtures |
| New predicate regression | *-final-*-predicate | all6 ON/OFF PASS;ON visits1/1 | unrelated1/8/64/257;ack2 while held |
| Inherited faults/protocols | *collection/storage/add/demand-growth/publication/boundary/absent/withdrawn* |24/120/34/84cases/tree;ownership PASS | cleanup/retry/newmatching/secondowners/pausedtail/overlap |
| Native/editor | native/editor/notifications/updater reports/logs | PASS | firstthread128/ContextName64/order257/strictAU1274/VST3201894/adapter-recall/editor |
| Core/pins/generation/probes | s10s21-remaining-authorized |16/16;4377+270pins;3 drift rejections;probes PASS | exact source and bundles |
| Format/checkpoint/final records | s10s21-format-pinned / checkpoint-v2 / final-records | PASS | defaultsOFF/hookabsence/generated/inventories |

All three complete drivers passed without retry on SAME immutable driver SHA256
`dc0148098ac3531132112586dccf91bba8a58d9a76ae289fa94e9dc56e9eb73f`. All successful final reports have no observed assertion/ASan/UBSan/TSan
diagnostic. Explicit8MiB SDK worker/complete joins are fixture conditions, not arbitrary
host-stack/lifecycle acceptance. Only two EXTERNAL inherited mutex bootstrap shims exclude
instrumentation; product/generated/callback/direct fixtures separately instrumented.
Editor offscreen unpaced CPU is not DAW budget. All3 native trees finalOFF; fixture hooks/
probe dependency absent; generated wrappers unchanged Session20.

Session21 failures retained: sandbox factory registration; initial elevated frozen Session20
bundle factory registration failure before cases; expected loaded negative on freshly rebuilt
same-source bundle; formatter discovery-path error corrected using pinned formatter; checkpoint
KeyError inherited driver-input key corrected in evidence script only. Root cause of frozen
bundle factory registration not qualified; frozen bytes untouched. Read-only ps sandbox denial
retained; filtered elevated check changed no process state. TSan completed normally, no timeout
or retry. Historical real stack overflow/allocator leaks/bootstrap-rpath/invalid fixture
leaks-race-SEGV-KeyError/session19core+TSanCTesttimeouts+counterrace+stopped drivers remain
immutable; never failure-free history.

Exact final Release OFF AU `092282ca3d257845f4b31cfbe9652f2f8b5178abcd852dd5beffe73cb388da2a`,
binary `31b3ca82bc294435dffebdb8fc412dc8d74655377877f31841cd7b741bd09884`;
VST3 `11f3dac8465ab886c71a7a8a639a050419d586afde2696f10fd2d9c9fc4adb3c`,
binary `cd659d2238ab035db14be93fee4037aca9bac7a8cec6e823a1c135eba1176dcb`.
Registry alloc/free/mutex/unfair0/0/3/0; whole alloc/free0/0 but AUfresh1mutex9/hostile603,
VST3fresh1024mutex9 remain. Larger idle counts not remeasured. No wholeG01 qualification.

CMake/CTest4.4.4;clang-format18.1.8;AppleClang21/macOS26.6.2-25G83arm64.
JUCE8.0.12 revision29396c22c93392d6738e021b83196283d6e4d850;doctest2.4.12
revision1da23a3e8119ec5cce4f9388e91b065e20bf06f5. Tool/dependency/source/generated/
fixture/artifact hashes and actual argv/cwd/UTC/exits recorded. Original probe
59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54.
Affected callback/publication/ownership/performance/soak/signing evidence renewed ONLY
at named local boundaries. Actual DAW/externalAU/platforms/OS-architecture/signing-rights/
accessibility/humans/budgets-soak/MB04 unqualified. Only installed Live12Suite permitted;
REAPER skipped/pluginval absent/temporaryauval requires forbidden install.

## Resume

Session22: review COMPLETE registry gate/capture/admission/exit/unlocked-tail/ownerCV/
ALL-frame rejection/no-ack/CollectionMarks/unique retention/geometric EACH-add demand
throwing-before-ALLattachment-publication/retained Entry predicate/liveFrames/newmatching/
spuriouswake/RAIIcleanup/reclaim/generatedrouting. Choose ONE remaining concrete actual
loaded-AU wait/order or callback ownership defect; reproduce and freeze COMPLETEsource/
fixture/generatedSDK/cache/bundle/report BEFORE ONE bounded repair. Review ALL writer/
reader/context paths before ownership replacement or gate removal. Then APVTS/regular-final
parameter+processor listeners/VST3 cache/prepared resources/lifecycle. Do not merely repeat
audits or generalize this predicate repair to constant total removal or nonwaiting audio.

All jobs complete; none to resume. Native `/private/tmp/sys01-s09-repair,asan,tsan` OFF;
core `/private/tmp/sys01-s09-session02-core`. Reconstruct missingtemps exactly from pins/
cache/sourceexport;DYLD_LIBRARY_PATH may resolve frozen rpaths, never edit frozen bytes.
No new installation/contact/publication/personal plugin authorization.

Remaining Sprint10 exit: immutable prepared resources/nonwaiting audio/offaudio configuration
waits/EVERY-writer acknowledged reclamation/safe invalid concurrent lifecycle silence-error/
complete zero-prohibited shipping callbacks/concurrency-lifecycle verification. Capture/
admission/exit waits/OS broadcasts/host-sized dispatch-initialscan-predicate-copy-demand-
reclaim/allocating audio-reentered writers/APVTS-parameter-processor listener ownership/
G01/actualDAW/externalAU/MB04 remain open. No advancement or waiver.

## Complete next session prompt

[Complete Session22 prompt](../evidence/sprint-10-session-21/next-session-prompt.txt)
contains Session21 authority plus the complete immutable Session20 inherited prompt.
