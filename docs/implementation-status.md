# SYS 01 implementation delivery // 2026-10-10

The audit's baseline repairs, host-neutral control/state layer, first native
VST3 slice and DSP-bound editor are implemented. This is a development candidate;
the complete 1.0.0 release plan has not passed its qualification gates. The owner
accepted SYS 01, macOS/Windows VST3 plus macOS AU, the stereo effect/generator
shell and parameter-only deterministic recall. Rights/licensing remains a
release gate at the owner's explicit direction.

| Plan phase | Delivered | Outstanding exit evidence |
|---|---|---|
| 0 / product contract | Scope and feature disposition in ADR 0002; stable IDs and canonical catalogue | Named minimum machine, instance budget and supported host matrix |
| 1 / source baseline | Max selectors/defaults; nested ignores; central version; presets; pinned dependencies; deterministic source export; reviewed Git baseline | Distribution rights approval |
| 2 / first host slice | Stereo routing/generation/input; 256 positioned events; gain/bypass; coherent reset/restore; telemetry; real VST3 loaded-binary conformance fixture | Real DAW session, offline bounce and routing qualification |
| 3 / instrument UI | Vector spool controls/register; numeric entry; direct domain/STATE/brake; presets/reset/undo; focus and native accessible controls; 100/125/150/200% scaling | Native assistive technology, DAW scaling and musician first-minute tasks |
| 4 / qualification | Six rates; lifecycle/restore fuzz; native concurrent restore; core race/allocation checks; validator and platform CI definitions | Windows CI execution, AU native timing, full-format realtime audit, callback deadline profiling, sound review, musician beta and realtime soak |
| 5 / delivery | Reproducible source export; local development bundles/reports/hashes; install/limitations/diagnostic instructions | Tagged approved release, exact platform/DAW matrix, rights, signing/notarization, clean-machine install and publication |

Local verification uses macOS arm64, Apple Clang 21, CMake 4.4.4, JUCE 8.0.12
and pinned doctest. Evidence belongs to this build/platform, not every supported
configuration. The development package retains the exact VST3 executable hash,
test reports and four interface snapshots in its `reports` directory.

| Check | Local result |
|---|---|
| Release CTest including native adapter and loaded VST3 | 14/14 passed |
| Debug AddressSanitizer/UndefinedBehaviorSanitizer CTest | 12/12 passed |
| ThreadSanitizer core and native adapter concurrency | Core test and native concurrent restore/lifecycle fixture passed; does not cover every framework path |
| VST3 positioned automation | Every point/offset, tie order, full 256-event capacity, cross-queue overflow rejection without following-block replay, nonfinite points and final targets passed |
| Stock wrapper negative control | Same binary fixture failed with the positioned integration disabled; failure retained |
| pluginval | Level 5 and level 10 passed on VST3; final level 10 includes editor tests and exact candidate executable |
| Clang static analyzer | Ten first-party translation units passed; dependency headers excluded from promoted diagnostics |
| clang-format 18.1.8 | First-party C/C++ passed |
| Workflow syntax | actionlint passed; CI jobs have not run remotely |
| Source export | Byte reproducibility, manifest verification and clean extracted core Release build (11 tests) passed |
| Native UI | Four scales rendered and visually inspected; no screen-reader or musician qualification claimed |

The supplied JUCE VST3 source is verified by SHA-256 and adapted only in a
generated build translation unit. The upstream checkout remains unchanged.
[ADR 0003](adr/0003-positioned-vst3-integration.md) defines this boundary and its
dependency upgrade requirements. AU uses its upstream wrapper and currently
receives ordinary automation at block boundaries; it is not timing-qualified.

The portable callback and shared scheduler have bounded storage/events and
allocation/symbol tests. **The complete native callback is not certified free
of locks/allocation:** upstream lifecycle synchronization, parameter listeners
and MIDI handling remain open audit items. Passing pluginval does not close
that gate. Explicit resets can produce discontinuities; listening tests must
qualify these before release.

Max remains a repaired integration patch, not a finished `.amxd`. Standalone is
a development harness. Host sync, captured-audio recall, MIDI learn, CLAP/AAX,
and hardware shipping are explicitly deferred. No website download or public
artifact is published by this implementation. Ad-hoc local bundle signatures
are not Developer ID signing or notarization.

Use [the desktop guide](desktop-guide.md) for build and routing instructions.
`release/manifest.json` remains the machine-readable qualification authority;
empty validated platform/host arrays are deliberate. A release tag must only
be created after the outstanding gates have evidence and owner approval.


## Sprint 01 extension / 2026-10-14

The October 10 table above remains historical evidence for its named binaries.
Sprint 01 adds checksum-pinned AU MIDI bounds, fixed VST3 CC ingress and total
queue-point bounds, merged-batch rejection recovery, immutable block telemetry
with actual left/right peaks, loaded AU/VST3 concurrency/lifecycle/editor fixtures,
and a test-only macOS allocation/lock probe. Both wrappers retain upstream
lifecycle/listener locks. ASan/UBSan and TSan now instrument generated format glue
as well as the shared processor; see the exact runs and limits in
[the Sprint 01 ledger](release-1.0.0/sprint-01.md).

Sprint 01 is **PARTIAL**, whole-format realtime remains **BLOCKED**: wrapper locks
are measured, a local editor-open callback exceeded its deadline, and agreed
minimum fixtures/DAW workloads are unavailable. AU timing parity, host matrix,
validators on other formats/platforms, realtime soak and downstream scope remain
unqualified. The immutable telemetry contract handed to Sprint 04 is documented
in `docs/release-1.0.0/telemetry-contract.md`; no new UI features are claimed.


## Sprint 02 extension / 2026-10-14

Sprint 02 is **PARTIAL**. Final macOS arm64 and x86_64/Rosetta suites pass 15/15,
including an AU characterization test that explicitly does not establish timing
parity. VST3 adds exhaustive catalogue/offset/rate coverage, literal stable IDs,
quantization thresholds and rejection/replay checks, plus a freshly retained
stock-wrapper negative bundle. CMake now seals both final macOS development
bundles after metadata generation, repairing the actual Live scan rejection;
all four strict integrity checks pass. Official Steinberg validator, strict
Apple auval and pinned pluginval evidence binds exact final hashes in
[Sprint 02](release-1.0.0/sprint-02.md). AU ordinary/scheduled offsets fail the
accepted contract; [the proposed scope decision](release-1.0.0/au-timing-scope-decision.md)
is not an accepted exception.

Actual Live 12.4.6 AU/VST3 scan/load, disabled-hardware-input generation, asymmetric
transform, bypass, offline bounce and reopen were exercised on recorded hashes.
VST3 recovered/reopened PCM matches exactly; AU host reports bind earlier named
binaries, not the final thin AU. Static MIDI CC sends did not qualify timed CC
delivery; ordinary DAW offsets and recorded MIDI remain open. REAPER tests were
skipped per the owner's latest instruction and its temporary app was removed;
install no other apps for testing. Native Intel/minimum macOS and Windows require
external fixtures. Stable IDs/schema/free distribution, planned formats and
deferred scope remain unchanged. No release tag/publication.


## Sprint 04 extension / 2026-10-14

Sprint 04 is **PARTIAL**. The native editor now uses vector spool controls, eight
true register elements, a uniformly scaled three-band face, shared mapped entry,
independent status/recovery and a modal inspector with real sampled stereo peaks.
Existing APVTS/native roles and all audio/state boundaries remain intact. Release
regressions pass 15/15 plus the separate 3072-state native editor fixture. Named
native harness actions/captures are retained; the actual 200% window capture is
partly offscreen and Live floating UI access remains unqualified. Earlier 51/75 ms
snapshot maxima are retained despite a lower final run. VoiceOver/Windows, pointer
tasks, DAW gestures and declared-load paint/response qualification remain open.
See [Sprint 04](release-1.0.0/sprint-04.md) for exact hashes, commands and limits.
No release gate, supported host array or manifest scope is silently promoted.


## Sprint 05 extension / 2026-10-15

Sprint 05 is **PARTIAL**. A repeatable 44-case musical set at six rates measures
raw stereo peak/RMS/DC/deltas/rails before attenuation; 48 kHz artifacts repeat
exactly. Native fixtures exercise 72 saved-state reopens, rate reset, malformed/
future state preserving running history, input-disabled generation and parameter
undo after destructive reset/preset. Four existing studies/names/order remain
literal compatibility fixtures. No production DSP, mapping, state or preset value
was changed. The [musical guide](musical-guide.md) explains routing and recall.

Finite/bounded samples are not sound acceptance. An independent strict PCM oracle
**fails** the DC investigation threshold for the 88.2 kHz high-trim transform case;
recall deltas and intentional high-gain clipping also await review. Eleven fixed
−24 dB audition files and [review notes](release-1.0.0/listening-notes-05.md) are
prepared, with **no listening or musician participation claimed**. New Live project
cycles remain unperformed after the UI tool timed out. See [Sprint 05](release-1.0.0/sprint-05.md)
for exact commands, failures, hashes and resume actions. G05/release remain open;
no format/host qualification, publication or release tag is added.


## Sprint 06 extension / 2026-10-15

Sprint 06 is **PARTIAL**. Archive/full-tree dependency pins, standalone source
export fixes, separate native sanitizer coverage and exact-artifact report/source/
toolchain manifests are implemented. The isolated source export builds without
brand siblings/shared dependency overrides and passes 21/21 native tests. A local
macOS arm64 packet includes source/notices, hashes, provenance and eight bound
reports; Steinberg/pluginval pass while the strict AU offset report remains FAIL.
Owner-selected AGPL-3.0-only text and selected verified dependency notices are
retained; rights/corresponding-source review is unfinished. Developer ID identities,
Windows signing credentials and clean supported-system fixtures are missing;
signing/notarization and install/scan/upgrade/uninstall/rollback remain unperformed.
See [Sprint 06](release-1.0.0/sprint-06.md) for commands/hashes and resume actions.
No supported-host/platform qualification, public release or tag is added.

Sprint 06 owner follow-up selected Windows ZIP/VST3/notices/PowerShell install and
rollback, with Authenticode when credentials exist. Fixture-only transactional
scripts and synthetic Windows CI checks are prepared; PowerShell/Windows execution
is unavailable locally. The route choice is resolved, while signing and actual
clean-system/host qualification remain blocked. No public release or tag.
