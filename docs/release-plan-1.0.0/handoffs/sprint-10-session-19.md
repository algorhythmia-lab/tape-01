# SYS 01 Sprint10 session19 handoff

## Checkpoint

Date/time/timezone: 2026-10-21T23:29:16.907946+00:00 / America/Santiago.
Sprint/session: 10/19. Objective: freeze one actual loaded-AU defect, make one bounded
repair, and verify its ownership and publication boundary.

S10-AU-REGISTRY-14 is LOCALLY VERIFIED at linear unique removal retention / ordered
publication and acknowledged callback ownership. Sprint10 IN PROGRESS; release
NO-GO/development-unqualified. Registry gate BLOCKING; G01, actual DAW/external AU,
and MB-04 remain open. No advancement or waiver.

Git root `/Users/lhommeduchili/algorhythmia`; SYS 01 root
`/Users/lhommeduchili/algorhythmia/sys-01`. HEAD
`5c45cf6bed431317a1b412b1fc12ee62b66cfdd5` unchanged; no applicable AGENTS.
All 160 source + 37 reference inputs matched session18 BEFORE edits; final 161 + 39.
All 20 historical inventories, dirty/untracked SYS 01 inputs, and unrelated monorepo
work preserved. No subagents, installation, staging, reset, stash, discard, commits,
tags, contact, publication, or personal plugin-directory changes. All jobs complete;
none to resume. Integrity is not acceptance; Sprint08 live comparison remains
intentionally invalid and must never be resealed.

Evidence: [session19](../evidence/sprint-10-session-19).
Source export SHA256 `ce220288f27a36c43d2be96d0859da0407da1a11906649f02d9e0351e0d1e7ee`;
resume inputs SHA256 `3f2f02575fbd7466ba8836ac81441e5b137a5080ecd110adadd7fa845479ea7f`.
Exact bundle/binary identities are in `final-identities.json`; command/report counts
and the complete final inventory are in `finalization.json` / `evidence-inventory.json`.

## Changes and decisions

Only product implementation [AUPropertyRegistry.hpp](../../../apps/desktop/AUPropertyRegistry.hpp)
changed. A temporary, gate-owned `Entry::retainedForRemoval` flag replaces repeated
linear search of the awaited vector. `CollectionMarks` encloses ONLY collection.
Append the shared_ptr before marking; clear every retained mark on success or
exception, under the same gate, BEFORE publication, WaitDemand attachment, or wait.
Every collector uses that gate; readers never access marks. Frame walk plus unique
cleanup is linear; vector growth still allocates. See
[ownership review](../au-property-unique-retention-session-19.md).

Frozen negative: session18 algorithm with equivalent comparison-only `find_if`
diagnostic, real bypass SetParameter consumed in Render → host callback → nested SDK
NickName callbacks. Each ancestor withdraws itself and adds a distinct matching
registration. Two off-callback removers use BOTH real selectors. Depths 1/8/64/257:
collection visits 2/9/65/258; dedup comparisons 0/28/2016/32896, expected zero.
Waited=1, acknowledged=2, calls=depth, no replay already correct. This proves
avoidable quadratic gate-held OFF-CALLBACK retention work; it is not an audio
allocation, deadlock, race, deadline, or budget claim. Negative AU
`1f391a3df6e7a6d153afaba6f0fd3fe4be974fc76ba9c74f79a3dc9b8b1b517b`, binary
`1496fabb4a0135fe6f28ac5eaafea173bfbd3d3642bb08014240c586fac91e4d`.
Complete source/header/export/fixture/executable/generated AU-SDK-VST3/cache/compile/
pinned SDK/bundle/reports were frozen BEFORE repair. Never edit frozen bytes.

Initial ALL-frame rejection before allocation/mutation; no-ack return; absent and
withdrawn-only paths; independent demands; new-add reserve-before-attachment;
ALL-matching predicates and spurious wakes; publication, notification, reclamation,
and complete wrapper/context lifetime protocols remain unchanged. Every writer,
capture, admission, and exit still shares the blocking gate. Same-thread ancestor
contexts live until return; errors authorize no release. Context destruction excludes
new registration and withdraws later publication. Legal teardown joins COMPLETE
wrapper/processor/message-owner entries, including unlocked notification tails,
before registry/CV destruction. Updater shutdown precedes registry shutdown while
ContextName's wrapper context lives. Listener removal alone does not acknowledge
whole changed/updateHostDisplay/final forwarding. No callback disposal or gate removal.

Preserve ALL SDK routing / empty SDK vectors / uncalled accessor / MIDI source-only,
APVTS authority→raw mirror→LockedListeners→flags, regular/final parameter listeners
and synchronized editor removal, processor unlocked invocation/final forwarding,
VST3 separate atomic floats/grouped flags/immutable IDs/shared storage, and immutable
prepared-resource/lifecycle requirements. Preserve identity, seven keys/native IDs,
four programs, schema40, empty-history recall, 10ms smoothing, shared256 whole-batch
rejection/no replay, eight restores, single audio owner, formats, editor dirty flag
and seven packed FLOAT-GENERATION words, gestures/undo/recall/visibility. No cap, loss,
TLS, table, queue, retained payload, or gesture coalescing. No pending decision stops
independent authorized work.

## Verification

| Check | Command registry reference | Result | Boundary |
|---|---|---|---|
| Initial integrity | initial-state / historical-integrity-initial | 160+37 matched; 20 inventories unchanged | BEFORE edits |
| Loaded negative | s10s19-negative-authorized | expected exit1; comparisons 0/28/2016/32896 | real Render, both selectors |
| Corrected loaded repair | s10s19-*-final-*-unique-corrected | all six PASS; ON comparisons0, visits2/9/65/258; waited1/ack2 | same AU bytes; shipping OFF lifetime/status |
| Release / ASan-UBSan | s10s19-complete-repair-authorized / complete-asan-authorized | ON/OFF PASS | separate instrumentation |
| TSan completion | native + regressions-retry + resume-tsan-remaining-authorized + corrected-fixture-authorized | final ON/OFF checks PASS | failed original drivers retained |
| Direct collection faults | s10s19-*-collection-protocol | 24 cases/tree PASS | both selectors, distinct entries1/8/64, every vector-growth failure budget |
| Inherited direct faults/protocols | s10s19-*-storage-protocol / add-protocol / publication / boundary / absent-protocol / withdrawn-protocol | 120/34 cases and ownership checks PASS | mark cleanup, retry acknowledgement, independent demands, new matching entries, exceptions |
| Native/editor regressions | reports / native command logs | PASS | registry/updater/order257/ContextName64/first-thread128/strictAU1274/VST3201894/adapter-recall/editor |
| Core/pins/format/generation | s10s19-core-retry-authorized / juce-pin / doctest-pin / format-final / generation | 16/16; pins4377+270; format and three drift rejections PASS | exact source and pinned dependencies |
| Callback probes | release-registry-probe / default-au-probe / default-vst3-probe | registry0/0/3/0; whole alloc/free0/0; locks remain | finite exact bundles |
| Checkpoint/final records | s10s19-checkpoint / final-records; finalize.py | PASS | source/export/flags/default-OFF absence/frozen evidence/inventories |

All final successful reports have no observed assertion/ASan/UBSan/TSan diagnostic.
Generated wrappers match session18; final three native trees are OFF, fixture hooks
and probe dependency absent. Rebuilt identities are recorded anew. Final Release OFF
AU `9f089d9b490ec99b2788cfa54df6d2f534e60064881c0b91b76d73e3598765f8`, binary
`629d1fb79bb2e5e21109175ff62da8b6c6fbb7d4558789557fedcc0b0fbd7ffa`;
VST3 `3e11dabea5e74776a331d1ceae2d112701800e170a2407de55193ad33d7079a3`, binary
`f05c268e5f7a72c170c1263c55187617999c974e3b8ce53b999c265c605fc7c3`.

Six nonzero registered commands are retained in `failures.json`: expected loaded
negative; core musical CTest timeout493.69s after functional PASS (sandbox ps denied);
TSan VST3 CTest timeout3983.08s after 201894-point PASS; its stopped complete driver;
two TSan races in the fixture's plain diagnostic visits counter; its stopped resume
driver. Core retry16/16 PASS6.14s; TSan regressions retry3/3 PASS82.14s on same bytes.
Race source/executable/report are frozen in `race-*` / `fixture-race-freeze.json`.
Only that fixture counter changed to atomic; all six focused Release/ASan/TSan ON/OFF
tests reran on unchanged AU bytes. Neither failed original TSan driver is a passing
matrix. Final remaining driver passed all omitted checks.

Historical stack overflow, allocator leaks, external TSan bootstrap/rpath failures,
fixture leaks/races/SEGV/KeyError, and ps denials remain frozen. Never claim failure-free
history. Only two EXTERNAL bootstrap shims exclude instrumentation; product/generated/
callback/direct fixtures are instrumented. Explicit8MiB SDK worker stacks and complete
joins are not arbitrary host-stack/lifecycle proof. Editor unpaced offscreen CPU is
not DAW/UI budget evidence. Registry mutex3 and whole AU fresh1 mutex9/hostile603,
VST3 fresh1024 mutex9 remain; larger idle counts were not remeasured.

Tools: CMake/CTest4.4.4, clang-format18.1.8, AppleClang21, macOS26.6.2-25G83 arm64,
exact JUCE8.0.12/doctest2.4.12 pins; tool/dependency hashes in command/provenance records.
Original probe SHA256 `59af83e5386e152daed50a215e31a38423ca1dfab08e6c0d001952e32238ba54`.
Actual DAW/external AU/OS-architecture/signing/rights/accessibility/human/budget/soak/
MB-04 qualification remains unperformed. Only installed Live12 Suite; REAPER skipped,
pluginval absent, temporary auval requires forbidden installation. Affected AU
callback/publication/ownership/performance/soak/signing evidence is invalidated and
renewed only at the exact local boundaries; historical bytes remain untouched.

## Resume

Session20: review COMPLETE registry gate/capture/admission/exit/unlocked tail/owner CV,
ALL-frame rejection/no-ack decision/CollectionMarks cleanup/retention/publication/
WaitDemand/predicates/reclaim/EVERY add's demand and throwing boundary/generated
routing. Choose ONE remaining concrete actual loaded-AU wait/order/callback ownership
defect, reproduce and freeze the complete source/fixture/generated SDK/cache/bundle/
reports BEFORE one bounded repair. Then APVTS/regular-final listeners/VST3 cache/
prepared resources/lifecycle. Do not remove the gate without replacement publication
and acknowledged reclamation for EVERY writer-reader-context path.

All jobs complete; none to resume. Native trees `/private/tmp/sys01-s09-{repair,asan,tsan}`
final OFF; core `/private/tmp/sys01-s09-session02-core`. Reconstruct missing temporary
trees from exact pins/cache/export. Absolute DYLD_LIBRARY_PATH can resolve frozen
rpaths; NEVER edit frozen bytes. No new installation/contact/publication authorization.

Remaining Sprint10 exit: immutable prepared resources; nonwaiting audio; off-audio
configuration waits; complete acknowledged reclamation; safe invalid/concurrent
lifecycle silence/error; zero prohibited shipping callbacks and concurrency/lifecycle
verification. Capture/admission/exit waits, final condition broadcasts, host-sized
dispatch/remove/reclaim/add-demand, allocating audio-reentered writers, APVTS/parameter/
processor listener ownership, G01/actual DAW/external AU/MB-04 remain open.

## Complete next session prompt

[Complete session20 prompt](../evidence/sprint-10-session-19/next-session-prompt.txt)
contains session19 authority and the entire immutable session18 prompt for inherited
ownership, verification, and authorization detail.
