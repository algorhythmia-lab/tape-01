# Sprint 05 / listening packet and review notes

2026-10-15, America/Santiago. **PARTIAL / listening UNPERFORMED.** No human listener
or musician participated in this run; no audio monitoring or subjective sound
approval is claimed. The notes below describe measured conditions and questions
for a listener, not an account of hearing these files. G05 stays open.

The automated source is the shared host processor, macOS 26.6.2 arm64 / Apple
Clang 21, Release, strict floating-point flags. It is not a DAW bounce. Each steady
fixture lasts 11 seconds; the main measurement is seconds 1–11, with additional
one-second windows. Performance timelines last 17 seconds. Each transition has
a 20 ms post-event delta window that includes the boundary sample. Raw float WAVs
are measured **before** attenuation; a whole second's RMS change is only a review
trigger, not perceptual loudness or an isolated cause measurement.

Input when enabled: stereo 220 Hz left / 331 Hz right sinusoids, each -18 dBFS
**peak** (-21.01 dBFS RMS), deterministic phase, no normalization. Generator
extremes/factories receive null/silent input. Transform receives that stereo input,
except `transform-silent`; `transform-hostile` injects NaN/+infinity every 17 samples.
Speed fixtures prefill at +1x for one second before changing FLUX. The raw recipe,
initial `.sys1` bytes, rate and frame boundaries are reproducible from the fixture.

Eleven **48 kHz PCM16** audition files are retained in
[evidence/sprint-05/musical-48000/](evidence/sprint-05/musical-48000/). All have a fixed
**-24 dB** digital attenuation, no normalization and no fades. Their peaks are at
most -24 dBFS; this is not a calibrated acoustic monitoring level. Do not enable
player loudness normalization, EQ or gain compensation. Use the same low hardware
monitor setting for every file and record device, speaker/headphone model, volume,
room and optional calibrated SPL before making comparisons. Actual monitor device,
hardware level, SPL, listener, date and listening duration: **UNPERFORMED**.

| 48 kHz source, left channel | Peak | RMS | 10 s mean DC | Max sample delta | Review focus |
|---|---:|---:|---:|---:|---|
| factory-1 / magnetic pulse | .714069 | .241999 | -.00000276 | .429814 | Equation edges and stereo motion; do not classify deliberate bytebeat steps as defects solely by delta. |
| factory-2 / cellular walls | .593388 | .120491 | .00003050 | .249171 | Voice attacks and gaps; compare level to bytebeat at unchanged OUTPUT. |
| factory-3 / forward tape | .124044 | .087936 | approximately 0 | .003609 | Distinct stereo input retained, delayed wet output. |
| recall-1 | .711255 | .229505 | .00001361 | .536378 | Same-study recall at 5 s, RESET at 7 s, study return at 8 s; destructive restart can click. |
| recall-3 | .157689 | .083166 | .00001392 | .078776 | Empty tape restart and delayed refill; right delta reaches .138429. |

Factory bytebeat/cellular left RMS differs by about **6.06 dB** on this input/run;
that is measured energy, not an equal-loudness judgment. All four program values
and order remain unchanged; no loudness normalization or migration is introduced.
The default nontransition studies have no samples on the output rails. At 48 kHz,
all settled 10-second windows have abs mean DC <= .003973. Across six rates, consult
`review-summary.json` for maxima; shorter windows can trigger DC investigation even
when the ten-second mean is smaller.

Performance files use the same named schedule in every starting domain:
1 s FLUX=0, 2 s FLUX=1, 3 s BIAS=0, 4 s BIAS=1, 5 s STATE=1,
6/7 s BRAKE on/off, 8/9 s pause/run, 10/11 s OUTPUT +20/0 dB,
12/13 s bypass on/off, 14/15/16 s next/other/original DOMAIN.
At +20 dB the left rail counts in the first settled ten seconds are 42,998
(bytebeat start), 30,050 (cellular start), 37,489 (transform start). The largest
stereo sample delta reaches 2.0. The gain range and hard limiter are accepted
behavior; the resulting sound still needs listening acceptance. No limiter,
FLUX/BIAS map or preset gain was silently changed to hide these flags.

`speed-1/3/5` audition near reverse / stationary / near forward after prefill.
Flux targets for `speed-0..6` are .25/.494/.495/.5/.505/.506/.75.
At .495/.5/.505 the head settles to zero and output decays to exact silence in
the final second. Review onset/stop/reversal, sustained feedback at BIAS extremes,
brake release, bypass level changes and reset/preset boundaries in both channels.
The sparse sine source does not qualify transients, dense music or every tape seam;
repeat with a transient-rich stereo musical source before G05 closes.

Native UI RESET → UNDO was exercised after recording tape. Parameters returned
exactly, and tape stayed empty after the existing 10 ms crossfade. During that
crossfade a departing bytebeat peak of **.0283604395** was measured. The first test
incorrectly required immediate silence and failed; the repaired oracle allows the
accepted ramp, checks later silence, and preserves the failed log. This is not
proof of recovered audio or a new DSP defect. Listen to this transition in the DAW.

Open review items (none is silently waived or declared musician-approved):

- **MB-01:** hard recall/reset boundary deltas above .1; classify audible clicks,
  obtain product/musician acceptance of the intentional destructive boundary, or
  design a reviewed transition that preserves deterministic/empty-tape semantics.
- **MB-02:** +20 dB saturation, bypass comparisons and >3 dB adjacent-window energy
  changes. Distinguish requested gain/bytebeat edges from unintended level surprises.
- **MB-03:** transform reversal/detent/feedback and brake transitions using actual
  stereo music, all six rates and repeated domain switches. No universal click
  detector or musical-quality pass is claimed by the metrics.
- **MB-04:** at 88.2 kHz, `performance-d2` right settled DC is
  **−.0115717612862**, above the .01 investigation trigger. During seconds 10–11
  (+20 dB hard limiting), its mean is **−.116651873266**. An independent Python
  PCM oracle reproduces this and exits **1**; the one-case recipe, source hash
  and compressed one-second raw float diagnostic excerpt are retained in
  `evidence/sprint-05/MB-04-recipe.json`. This is an unresolved numeric DC finding,
  not a passed sound gate. No speculative output-filter/gain change is introduced.
- **HOST-05:** new actual Live saved projects, three reopen cycles per study and
  sample-rate changes, with exact candidate/hash, routing and PCM retained. The
  Ableton UI selection attempt timed out (1028.1775 s); no new host action succeeded.

The review sheet for each item must record listener, file SHA-256, source and input,
rate/block/host build, monitoring level/device, exact time/channel, what was heard,
severity, intended/unintended classification, action and retest. No new unintended
DSP defect has been confirmed by a listener; flagged cases remain unqualified.
The automated fixtures guard measured boundaries, canonical factory states,
partition determinism, invalid-state rejection and destructive undo. There is no
production DSP change or fabricated defect fix.
