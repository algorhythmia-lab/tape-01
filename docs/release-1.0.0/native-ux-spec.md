# SYS 01 // NATIVE DESIGN AND INTERACTION CONTRACT

revision 1 // 2026-10-14 // sprint 03 // design contract COMPLETE; native implementation pending sprint 04.

this contract defines the next native editor against the existing engine. the
[annotated layouts](native-ux-layouts/README.md) are design fixtures, not plugin
screenshots or evidence of host/accessibility qualification. no processing,
parameter, preset, schema, timing, format or release qualification changes here.

## 01 // AUTHORITY AND PREREQUISITES

precedence: [ADR 0002](../adr/0002-desktop-control-state-and-release.md) for desktop
behavior and release scope; [ADR 0003](../adr/0003-positioned-vst3-integration.md)
for native event delivery; the existing catalogue and DSP helpers for exact
values; the source-exportable [native tokens](../design-tokens.json) for colors;
the [brand aesthetic](../../../brand%20book%20&%20strategy/brand-aesthetic-system.md)
and [brand tokens](../../../brand%20book%20&%20strategy/design-tokens.json) for
visual language; [TAPE tokens](../../../tape-01/specs/tape-01-tokens.json) and
[physical specification](../../../tape-01/specs/physical-and-industrial-design.md)
for familial geometry. those four repository-root references are relative to
this document's release directory via three parent levels. historical
[vst reference](../vst-reference-ui-spec.md) promises yield to ADR 0002.

retain twin equal spools, a central eight-element register, outward lower
DRIVE/STATE positions, planar dark surfaces, crisp seams and the text wordmark
`algorhythmia // SYS 01`. translate physical tactility into vector geometry and
direct controls. physical dimensions (100.4 × 63.8 mm, 21.4 mm hubs, 42.5 mm
spacing) are inspiration, not a desktop pixel scale or new minimum. omit fake
screws, glass, gold circuitry, fake ports, shadows, proprietary graphic logos,
anime marks, batteries, rotating playheads and clock-lock/sync lamps. no decorative
motion. rings indicate parameter targets only. register changes require DSP data.

ADR 0002 supersedes hardware cellular 20–360 BPM, transform 1–2000 ms, per-domain
STATE memory, hardware chords, desktop hold-save and sync subdivisions. retain
cellular 30–300 BPM, transform -2..+2x, shared binary STATE and free clock.

prerequisite evidence was hash-checked before editing: sprint 00 35/36 and
sprint 01 99/100 match; each sole mismatch is the intentionally superseded live
`state.md`. sprint 02 187/187 match, including its current state hash. retained
baseline copies were separately verified against all three historical state hashes. see
[verification](evidence/sprint-03/prerequisite-verification.json). this verifies
retention, not fresh native execution. sprint 01/02 remain PARTIAL; AU offsets,
whole-wrapper realtime, native minimum machines and complete DAW matrix remain
open. none prevents specifying truthful display/input behavior. the manifest
remains `development-unqualified` with empty validated arrays.

## 02 // OWNERSHIP AND PRESERVED SURFACE

seven saved host targets retain exact catalogue order, keys and defaults:

| host key | default | surface / meaning |
|---|---:|---|
| `param_flux` | .5 | left spool; domain-dependent rate/speed |
| `param_bias` | .5 | right spool; continuous domain macro |
| `transport_drive` | 0 | DRIVE PAUSED / DRIVE RUN |
| `subregister_state` | 0 | STATE 0 / STATE 1; one shared latch |
| `engine_state` | 0 | 00 BYTEBEAT / 01 CELLULAR / 02 TRANSFORM at 0/.5/1 |
| `output_trim` | .75 | OUTPUT -60..+20 dB; default 0 dB |
| `bypass` | 0 | wet / dry bypass; engine continues |

retain `org.algorhythmia.sys01`, `Algr`, `Sys1`, schema 1's exact 40-byte codec,
four factory studies in their current order, parameter-only deterministic restart
with empty tape/voices/phases, stereo effect/generator shell and free full
release. planned formats remain macOS VST3/AU and Windows VST3. standalone remains
a development harness. no new host parameter for brake, inspector, meters,
scale, clip holds, counters or modified status. no MIDI learn, host sync, mix,
captured-audio persistence, separate instrument ID, CLAP/AAX, public standalone,
hardware shipping or Max for Live implementation.

primary controls and value fields always show **saved host targets**. the register
and actual RUN/BRAKE telemetry show **DSP results**. CC20 FLUX, CC21 BIAS, CC22 run,
CC23 domain are fixed transient performance inputs; they never move the saved
knob, dirty a preset, write automation, or enter session state. an inspector
explains audible divergence. do not invent an actual FLUX/BIAS or MIDI activity
lamp from telemetry that exposes neither. host-target/DSP-domain and run
comparisons are available; continuous effective values are not in the current
snapshot. a rejected event can leave a visible host target unapplied to sound.

## 03 // COORDINATE AND SCALE CONTRACT

base canvas **640 × 440 logical units**, origin top left. minimum remains 640 ×
440; no evidence justifies shrinking it. use a single uniform transform for all
paint, child bounds, fonts, strokes, hit tests, popup anchors and focus rings.
OS backing scale is separate; do not multiply it twice. host-requested arbitrary
sizes use `s = min(width/640, height/440)` within 1..2, centered letterboxing in
obsidian; never stretch x/y independently. negotiate supported preset sizes;
when the host refuses, keep all controls reachable at the largest uniform fit.
smaller-than-minimum host embedding is an explicit qualification failure, not
permission to crop essentials. no scroll required on the performance face.

| selector | canvas in logical units | 1-unit stroke | 14-unit label | minimum 28-unit target |
|---|---|---:|---:|---:|
| 100% | 640 × 440 | 1 | 14 | 28 |
| 125% | 800 × 550 | 1.25 | 17.5 | 35 |
| 150% | 960 × 660 | 1.5 | 21 | 42 |
| 200% | 1280 × 880 | 2 | 28 | 56 |

map boundaries through the same transform; round device pixel edges consistently,
never round each sibling's scale independently. a 2-unit focus ring retains its
clearance. scale is editor presentation, never automation/state-schema content.
restore the last local editor preference if available; otherwise 100%. scale
changes preserve focus and in-progress edit text; commit/cancel behavior is
unchanged. no smaller alternative until real text extents, native focus and
first-minute task evidence exists across the required platforms.

| area / component | x,y,w,h at 100% | contract |
|---|---|---|
| identity / free clock / DETAILS | 24,16,392,24 / 424,16,104,24 / 540,16,76,28 | text identity; free clock is factual, no clock-lock icon |
| session preset | 24,56,240,28 | full factory name + reserved modified marker; accessible full text |
| UNDO / REDO / scale | 276,56,60,28 / 344,56,60,28 / 520,56,96,28 | native buttons / menu |
| domain group | 144,100,352,28 | three equal segments, exact parent boundary; direct radio choice |
| performance region | 24,140,592,208 | no panels over the spools/register |
| FLUX / BIAS spools | 84,158,128,128 / 428,158,128,128 | centers 148,222 and 492,222; same diameter |
| spool labels | 84,140,128,18 / 428,140,128,18 | persistent FLUX / BIAS |
| direct value fields | 84,288,128,28 / 428,288,128,28 | target number with unit; not hidden until hover |
| register / caption | 258,202,124,40 / 238,176,164,20 | exactly eight bits, no ninth status element |
| context | 238,250,164,52 | two short lines: domain macro / actual DSP run or brake |
| DRIVE / BRAKE / STATE | 48,320,128,28 / 256,320,128,28 / 464,320,128,28 | outward DRIVE/STATE; brake always directly reachable |
| signal divider | 24,356,592,1 | planar separation |
| OUTPUT label / slider | 24,364,52,28 / 80,364,212,28 | continuous wet trim |
| output entry / BYPASS / RESET | 300,364,80,28 / 392,364,104,28 / 512,364,104,28 | RESET never unlabeled icon |
| input / limit / rejection | 24,400,188,18 / 224,400,104,18 / 344,400,272,18 | independent, reserved status slots |
| recall hint / telemetry health | 24,420,304,18 / 344,420,272,18 | short persistent copy; no scrolling ticker |

hit targets never overlap; labels belong to their controls. the numeric entry is
a separate accessible edit child of the same slider. avoid duplicate slider
values in tab order. long preset text uses measured elision before the reserved
`*` marker, with full name on focus and in accessibility. all essential statuses
above fit without elision, including uint32 rejection count 4294967295. locale
1.0 uses the fixed English copy below; future localization requires remeasurement.

## 04 // SEMANTIC COLORS AND COMPONENT STATES

all hex values originate in the native token snapshot. use neutral outlines and
inset contrast for structure; colored area remains <=5% of the face, including
simultaneous faults. an active domain is underlined and named, never filled with
a large accent panel. disabled labels remain readable rather than faded opacity.

| semantic role | foreground / fill | background / boundary | redundant cue |
|---|---|---|---|
| canvas / recess / raised control | #08090A / #121417 / #1A1D20 | own planar surface | crisp geometry |
| primary label / entry / icon | #E2E4DE | all three dark surfaces | text |
| secondary label / unit / help / disabled label | #8E96A0 | all three dark surfaces | words; no opacity reduction |
| selected neutral symbol / hover text | #B8BDC3 | #1A1D20 | underline or inset marker |
| title / focus ring / error entry text | #F5F7F2 | dark surface | focus ring 2 units + 2-unit gap; error message |
| structural divider | #2A2F35 | dark surface | decorative only; not sole control boundary |
| idle functional boundary / inactive bit | #8E96A0 outline / #121417 fill | dark surface | stable outline; bit text 0 |
| pressed / editing control | #E2E4DE outline / #1A1D20 fill | dark surface | inset mark, caret / pressed role |
| bytebeat active bit / domain marker | #FFB000 | #121417 | bit 1 / domain 00 label |
| cellular active bit / domain marker | #00FF66 | #121417 | bit 1 / domain 01 label |
| transform active bit / domain marker | #54E5FF | #121417 | bit 1 / domain 02 label |
| active bit digit | #08090A | domain accent fill | 1; inactive digit #8E96A0 on #121417 |
| run indication | #00FF66 small pip | dark surface; label #E2E4DE | RUN / PAUSED text; paused hollow pip |
| brake / input quiet / dry bypass | #FFB000 small mark | dark surface; label #E2E4DE | BRAKE ON / INPUT QUIET / BYPASS DRY |
| modified / rejected / unknown telemetry | #FF5500 small mark | dark surface; label #E2E4DE | `*` + accessible modified / REJECTED count / TELEMETRY UNKNOWN |
| clipping / limiting / invalid entry boundary | #D80027 small pip or border | #F5F7F2 adjacent outline if needed | LIMITING / INVALID VALUE, in #E2E4DE |
| unavailable decorative detail | #4B535D | dark surface | never essential text, state or sole boundary |

ordinary labels require >=4.5:1; large text >=3:1; essential non-text outlines
require >=3:1. #D80027 cannot serve as ordinary red text on these dark surfaces;
use the bone warning label and a bone/pure-bone outline for an essential error
boundary. #2A2F35 and #4B535D are not accessible control outlines. hover is raised
carbon, pressed uses an inset line, selected uses a persistent check/underline,
disabled uses a dashed/slashed marker plus disabled role and readable text,
focus uses pure bone independent of domain/fault. value error keeps caret,
original text, INVALID VALUE message and focus; never replace with zero.

no blinking alerts, glowing blur, rainbow rings or spinning knobs. palette
distribution is a spatial budget, not permission to reduce legibility.

## 05 // TYPE AND FONT RIGHTS INVENTORY

ADR 0002's **system fonts** remain selected. no font binary is added or downloaded
in this sprint. use the JUCE platform default sans family for labels/body; use
its platform default monospaced family for numbers only when resolved and
measured. otherwise use sans with fixed-width value boxes. never synthesize
condensation or rely on a locally installed paid face. exact resolved family,
version and glyph fallback must be recorded on each native QA machine.

| role | base size / weight / leading | copy |
|---|---|---|
| identity | 20 / regular / 24 | lowercase algorhythmia; SYS 01 uppercase |
| section / control / domain | 14 / medium / 18 | uppercase, tracking .04em maximum within measured box |
| target value | 16 / regular tabular if available / 20 | digits, sign and explicit unit |
| status / compact help | 12 / regular / 18 | uppercase statuses, lowercase explanations |
| inspector prose | 14 / regular / 22 | lowercase; real technical identifiers preserved |

font sizes scale uniformly. no essential 11-unit microcopy. ASCII alternatives
are required for unsupported minus/multiplication glyphs (`-`, `x`); use `DB`,
`BPM`, `TICKS/S`, `%`, `X` consistently in UI. unit case does not change the
physical quantity or host parameter name. preserve API identifiers and legal
names verbatim in technical records; the binary case law governs authored UI
copy and prose, not rewriting source keys, paths or third-party notices.

| reference family inventory | selection / license and distribution disposition |
|---|---|
| platform default sans / mono (actual family unresolved) | selected via OS/framework; no font files bundled; record resolved identity and platform availability during native QA; no redistribution grant inferred |
| DIN PRO CONDENSED, DIN 1451 MITTELSCHRIFT, DIN CONDENSED | brand references only; exact vendor/package and license not supplied or approved; no embedding |
| univers ultra-condensed, helvetica compressed, arial narrow | references only; exact license/embedding grant unverified; no embedding |
| neue haas grotesk text, helvetica now | references only; exact license/embedding grant unverified; no embedding |
| inter, barlow condensed | possible future candidates; no selected version, license file or hash; review exact upstream package before selection |
| jetbrains mono, IBM plex mono, OCR-B | references/candidates only; no selected package/version/license or embedding approval |
| system fallback names in tokens (SF mono, menlo, consolas, segoe UI, roboto) | fallback references, not bundled assets; use only through available system resolution; do not copy files from the OS |

rights inventory for sprint 03 assets: original SVG vectors and text, no external
icons, fonts, logos or raster images; first-party provenance remains subject to
G08 and the recorded AGPL-3.0-only implementation in sprint 06. adopting a bundled
condensed face later requires upstream URL/revision, actual font hash, license
text, copyright/reserved names and embedding/modification/redistribution review,
notice inventory and all-scale text remeasurement. no type choice closes G08.

## 06 // SHARED MAPPED VALUES AND ENTRY

normalize only at the existing control boundary. sprint 04 must consolidate
formatting, parsing, tooltips, accessibility and inspector text in a shared
host/editor value helper, not repeated paint equations. it delegates to existing
`Transform::speedForFlux`, `Cellular::ruleForBias`, catalogue defaults/validity
and `host::trimDb`. bytebeat/cellular linear rate conversion and inverse parsing
can be added to that helper once; do not duplicate DSP algorithms. no helper
implementation or DSP refactor is required in this design sprint.

| domain | FLUX target format / inverse | BIAS target format / explanation | STATE 0 / 1 |
|---|---|---|---|
| 00 BYTEBEAT | `4250 TICKS/S` at .5; 500+7500f; inverse (v-500)/7500 | `50.0% MACRO`; b×100; continuous equation morph + stereo azimuth + drive; not a single integer bank switch | base / alternate equation bank, ramped blend |
| 01 CELLULAR | `165.0 BPM` at .5; 30+270f; inverse (v-30)/270; four generations/beat | `50.0% MACRO`; helper rule 30/90/110/150 on [0,.25), [.25,.5), [.5,.75), [.75,1]; macro also alters FM coupling | base / alternate pitch behavior; shared binary latch |
| 02 TRANSFORM | `0.00 X` at .5; helper -2..+2 with abs(f-.5)<=.005 detent; inverse (v+2)/4 for nonzero; zero canonical .5 | `50.0% MACRO`; wear + drive + recursion; inspector may show target recursion 1.2b (60% at .5), explicitly coupled | forward-relative / reversed-relative sign; effective target speed = base speed × (1-2s) |
| all | OUTPUT `0.0 DB` at .75; -60+80o; inverse (v+60)/80 | no mix control | host state remains normalized |

STATE 1 can make negative base transform FLUX play forward; display base saved
speed in the knob and a clearly named effective **target** direction in the
inspector. never claim measured instantaneous speed during smoothing. transform
zero-speed read head is distinct from BRAKE: zero stops writing; BRAKE freezes
writing while the read position may continue. no loop-length-in-ms label.
bytebeat paused stops tick advancement; cellular paused stops evolution but
existing voices can decay; transform paused stops write/read advancement but
history/filter output can remain. do not equate PAUSED with mute or zero meters.
bytebeat brake approaches a very slow tick, not an instantaneous stop; cellular
brake stops evolution while voices decay. all claims follow current DSP.

readout formatting is rounded presentation, not target quantization: retain
full normalized value until an edit is committed. show normalized target to six
decimals in inspector when exact reconstruction is needed. shared helper tests
must cover endpoints, .25/.5/.75 boundaries, .495/.505 detent and neighbors using
actual float comparisons, negative speed + STATE, roundtrip tolerance and all
catalogue defaults. a 0.496 target renders zero without silently changing to .5.

entry activates on single click in field or Enter when its slider is focused;
double-click on dial body resets to catalogue default (not entry). accept ASCII
signed decimal, optional surrounding whitespace and the field's optional unit,
case-insensitively; FLUX suffix is TICKS/S, BPM or X for its current domain,
BIAS suffix is % or % MACRO, OUTPUT suffix is DB; no grouping, comma decimal, expressions, exponent, NaN,
infinity, unknown unit or partial numeric prefix. percent accepts 0..100;
FLUX accepts its domain range; OUTPUT -60..20. reject out-of-range rather than
silently clamp. blank/invalid input has no parameter write or host gesture.
a nonzero transform entry whose inverse falls inside the helper's zero detent
is rejected with `inside zero detent; enter 0 or a larger magnitude`, rather
than silently becoming zero; the existing float helper owns the boundary.
Enter or valid focus-loss commits once; Escape restores pre-edit text/value.
invalid focus-loss cancels back to current target, with one accessible validation
message; no focus trap. if host automation updates during typing, preserve the
edit buffer and mark `TARGET CHANGED` in field help; Escape accepts newest host
target; Enter explicitly writes the user's valid value. if domain changes while
FLUX is being edited, cancel that unit-specific edit and announce `DOMAIN CHANGED`;
never reinterpret the old unit in a new domain. BIAS percent/OUTPUT entries survive.

## 07 // KEYBOARD, FOCUS, HOST GESTURES

native components expose names, roles, values, limits, selected/disabled/focused
states and actions. sliders expose mapped value plus unit and saved-target
meaning; domain is a three-item radio group, STATE/run/bypass/brake are toggles.
register is one read-only accessible group with eight bit children or a full
binary/hex value description; bits do not create eight keyboard stops. no focus
on decorative labels. tooltips also appear on keyboard focus and never contain
the only instruction. normal navigation never changes a target accidentally.

Tab order: preset → UNDO → REDO → scale → DETAILS → domain group → FLUX slider →
FLUX entry → BIAS slider → BIAS entry → DRIVE → BRAKE → STATE → OUTPUT slider →
OUTPUT entry → BYPASS → RESET. Shift+Tab reverses; disabled UNDO/REDO are skipped.
radio arrows move/select within domain, Home/End first/last; Tab exits group.
popups follow native arrows/typeahead/Enter/Escape. no focus wrapping outside
an open modal; host receives boundary traversal according to its embedding.

| focused target | keys / pointer behavior |
|---|---|
| continuous slider | Up/Right +.01 normalized; Down/Left -.01; Shift fine .001; PageUp/Down .1; Home/End 0/1; clamp at range; each completed key press is one undo/host gesture; held repeat coalesces until key-up |
| dial pointer | relative vertical drag, 180 logical units full range; Shift 10× finer; target marker follows saved value; no absolute jump on mouse-down; wheel only when focused, .01 per detent, Shift .001 |
| value field | Enter edit/commit, Escape cancel; native selection/copy/paste; no performance shortcut interception |
| toggles | Space or Enter changes once; autorepeat ignored; checked/unchecked announced |
| RESET / preset | explicit action resets parameters and audio history; persistent hint `parameter recall; reset clears audio`; focus help says undo cannot restore audio; no delayed hold-save |
| UNDO / REDO | buttons always available when history permits; Cmd+Z / Cmd+Shift+Z on macOS, Ctrl+Z / Ctrl+Y on Windows only while editor owns focus and no text edit; text edit retains native text undo |
| DETAILS | Enter/Space opens modal inspector; Escape/CLOSE returns focus here |

no global Space transport, no unmodified letter shortcuts, no interception of
DAW transport/record shortcuts outside focused controls. lost pointer capture,
window close or editor destruction ends an active host gesture exactly once.
scale and domain changes cannot leave a dangling gesture. don't warp cursor or
hide focus. visible focus stays around the individual component, not the whole
editor. return focus to the initiating control after menus/dialogs. native
screen-reader actions must use the same command path as pointer/keyboard.

for saved targets: begin host change gesture at drag start / first held key;
write `setValueNotifyingHost` through existing attachments; end at release /
key-up. numeric commit, toggle, default reset or direct domain selection is one
balanced begin/write/end. avoid double gestures from both attachment and custom
handler. incoming host automation, timer reads and MIDI never send UI gestures.
parameter undo groups one drag/key-hold/commit. preset/reset use the existing
coherent restore/program transaction and one parameter-only undo transaction;
notify host for changed parameters through that path once, not by seven unrelated
UI writes. no promise of DAW-wide atomic automation across seven parameters.

BRAKE is an ephemeral direct queued edge, not a host parameter or saved latch.
Space/click toggles ON/OFF for accessible sustained use; label says BRAKE ON/OFF,
not HOLD. enqueue failure restores its previous requested state and exposes
rejection; successful enqueue shows pending until telemetry confirms. release
on editor close using existing processor path; on reopen derive actual brake
from telemetry, not a stale widget check. no hardware tap/hold/chord recognizer
is added. run/STATE remain separate controls with no hidden long-press behavior.

## 08 // TRUE TELEMETRY, RECOVERY AND SIMULTANEOUS STATES

consume the existing immutable snapshot at most once per 30 Hz message-thread
tick and distribute it to all views. no second inspector consumer. initial
sequence zero renders `TELEMETRY UNKNOWN`, bits as unavailable outlines with
`-` digits, meters `--`, and actual DSP status unknown. host controls still work.
no new sequence for 250 ms of message-thread monotonic time marks `TELEMETRY STALE`;
freeze last valid bits, gray neutral outline, `stale` accessible suffix, meter
`--`. suspended/idle hosts may legitimately stop callbacks; never call that a
DSP failure. after open drain, require a later advancing sequence before treating
backlog as fresh. sequence advance is progress, not measured wall-clock sample
age. drops identify display loss, never audio failure; do not interpolate fake
register frames. compare uint32 counters modulo wrap; first observation is a
baseline, not billions of new failures. counter reset/reprepare rebaselines.

exactly eight separate register elements, left-to-right b7..b0. each element
has a persistent outline and literal 0/1 (or unavailable `-`), with `word &
(1 << (7-i))` as its only bit source. bytebeat is calculation word; cellular is
the existing rule byte; transform is the existing bilateral 4+4 peak-flux word,
**not** a rule and **not** calibrated DBFS output metering. label changes to
`WORD`, `RULE` or `TAPE FLUX L/R` in its fixed caption slot. preview overrides
bits only when actual `preview` is true, uses supplied 0xC0/0x18/0x03 word and
`DOMAIN PREVIEW`; no desktop animation invents a preview. bits use actual DSP
domain color even when saved domain differs. one accessible description updates
on focus/request; no 30 Hz live-announcement stream.

optional **real per-channel meters** belong inside DETAILS, not the spool rings.
if implemented, four lanes IN L/R and OUT L/R use the separately measured arrays
from the same snapshot. input is sanitized external input; output is rendered
post trim/bypass/limiter. never duplicate an aggregate into two channels or label
peaks RMS/VU. scale -60..0 DBFS, `20 log10(max(p,1e-3))`, silence `-INF`, numeric
headroom above 0 shown for hostile input without extending the lane. rise to
sampled block peak immediately, fall 20 dB/s based on UI elapsed time capped at
250 ms per tick; no audio callback work. optional 1 s UI peak hold is labeled
`HOLD`, lost/stale intervals show `--`. sampled maxima can miss intervening
blocks; no intersample/true-peak or loudness claim. no meter equals clip flag.

| condition | primary evidence and fixed visible treatment | recovery / simultaneous rule |
|---|---|---|
| paused / run | saved DRIVE PAUSED/RUN; context `DSP PAUSED` / `DSP RUN` from snapshot | do not infer silence; disagreement says `DSP RUN / TARGET PAUSED` or reverse in inspector |
| binary STATE | STATE 0/1 checked marker; saved latch independent of domain | changing domain preserves it; actual effective state unavailable in current telemetry |
| brake | BRAKE ON/OFF/pending; context `DSP BRAKE` when confirmed | takes context priority over DSP RUN text but DRIVE stays visible; release explicitly |
| bypass | BYPASS DRY checked; wet trim remains editable and saved | no dimming of engine register; engine continues; dry is untrimmed, no mix claim |
| missing input | `NO INPUT` only if native bus-disabled state is known | transform help `route stereo input`; no auto-run/reset/domain switch |
| silent connected input | `INPUT QUIET` after 500 ms observed fresh zero peaks | cannot distinguish muted source, cable, or silence; captured transform may continue; generators are normal with no input |
| unknown input connectivity | `INPUT UNKNOWN` if no trustworthy bus info; peaks may still say quiet | current snapshot has peaks, no connectivity flag; obtain non-audio host bus configuration safely in sprint 04 or retain unknown, never infer disconnect |
| clipping | independent `LIMITING` slot; existing per-block clipped flag held 1 s UI time | lower wet OUTPUT; if BYPASS DRY, explain dry is untrimmed and lower upstream; do not assume held clip is current wet signal |
| modified preset | reserved `*` in preset; accessible `modified host targets` | compare all seven host values to selected catalogue preset exactly; MIDI/brake/display state never dirties; no disk-unsaved claim |
| rejected event/state | independent `REJECTED n` for cumulative count; new increment marks unresolved | valid edit/reset/preset is explicit recovery; final targets may differ from sound; no retry/replay or automatic reset |
| display drop / stale | health slot `DISPLAY DROPS n`, or UNKNOWN/STALE first | detailed counters remain in inspector; never overwrite rejection/limiting/input |

rejections persist as history across a valid recovery. inspector ACKNOWLEDGE
clears only the UI unresolved marker/announcement, not processor count, host
values or sound; primary still shows the cumulative count. without an accepted
applied-target acknowledgement, do not claim that another edit cleared every
mismatch. when initial count is nonzero, present history as such rather than a
new failure announcement. invalid local numeric input is a field validation
error, not an invented audio rejection increment.

simultaneous priority is **within a slot only**: health UNKNOWN > STALE > display
drops > TELEMETRY LIVE; context DSP BRAKE > actual RUN/PAUSED; input known disabled
> unknown > quiet > INPUT ACTIVE. no cross-slot override: BYPASS DRY + BRAKE ON +
NO INPUT + LIMITING + REJECTED n + modified `*` can all remain visible with STATE
and domain. no automatic disabling of performance controls for these conditions.
if a validation editor is open, its error remains local; status facts persist.
accessible polite announcements coalesce changed fault facts at most once/second,
only while open, never every telemetry frame; focus can query all facts. no
modal error for an audio rejection, clip, missing input or display drop.

## 09 // SECONDARY INSPECTOR

DETAILS opens a modal sheet at 40,104,560,300 within the same scaled canvas;
performance is unobscured when closed. trap focus only inside the open modal,
CLOSE always visible at top right, Escape closes, focus returns to DETAILS.
use a keyboard-scrollable body with named sections; no resizable extra window
or forced larger minimum. core sound actions stay on the face. audio continues;
opening/closing does not reset, send gestures or alter saved parameters.

show only useful mapping/recovery content: current saved domain/FLUX/BIAS/STATE/
run/trim/bypass; exact normalized host targets; helper-derived units and coupled
macro explanation; DSP domain/run/brake and target disagreement if known; fixed
CC20–23 table with `performance only; not saved`; rejection count/unresolved
history and `make a valid edit or reset; reset clears audio`; display drop/stale
meaning; parameter-only recall and undo limitation. show no unavailable effective
continuous value as fact. host automation is step-at-point plus 10 ms smoothing;
AU ordinary/scheduled offsets remain unqualified/block-boundary. avoid a sync
button or unlocked badge that implies sync exists. optional meters use section
08 and can be omitted with no loss of primary recovery information.

## 10 // ACCEPTANCE MATRIX AND HANDOFF

D = design acceptance in this sprint. N = native implementation/qualification in
sprint 04/07. D passing never changes manifest accessibility or host support.

| id | procedure / exact expected result | D evidence / N exit |
|---|---|---|
| UX-01 authority | compare all keys/defaults/preset order/ranges to catalogue/ADR; no added schema or deferred feature | D source audit + unchanged-input check; N existing contract tests remain passing |
| UX-02 layout | three annotated fixtures at 640×440; measure every control/status rectangle, bit count and separation | D generated vector geometry + reviewed raster previews; N native screenshots and font extents |
| UX-03 scales | 100/125/150/200%; uniform transform, focus and popup hit tests at each; arbitrary 1000×700 letterbox | D 12 full-size fixture renders and scale math; N actual macOS/Windows/DAW checks |
| UX-04 state product | all 3 domains × run 2 × STATE 2 × brake 2 × bypass 2 × input missing 2 × clip 2 × modified 2 × rejected 2 × scales 4 = **3072 cases** | D enumerated contract scenarios; N render all cases, zero clipped/hidden essential labels or unreachable actions |
| UX-05 coexistence | all eight status dimensions active, then release each independently; count 4294967295; negative transform speed with STATE 1 | D signal fixture + rules; N statuses never mask one another; target/DSP divergence retained |
| UX-06 register | word 00/FF/80/01/18 and all 256 words; preview C0/18/03; initial/stale | D exactly 8 vector elements per fixture; N bit order/helpers, no decorative frames; accessible full value |
| UX-07 mapping | FLUX/BIAS endpoints, cellular quarter boundaries, float detent neighbors, STATE sign, trim defaults; parse valid/invalid/overflow paste | D equations/helper binding reviewed; N shared-helper tests, no duplicate UI equations or silent clamps |
| UX-08 gesture | drag, fine drag, key-repeat, reset, valid/cancel/invalid entry, focus loss, automation during edit, domain during edit, close mid-drag | D complete behavior contract; N host fixture counts balanced begin/end and no notifications for cancel/timer/MIDI |
| UX-09 focus | Tab/Shift+Tab full order, radio arrows, slider keys, edit, dialogs, inspector, disabled undo, DAW key coexistence | D specified; N keyboard-only tasks at four scales, visible local focus and no unintended parameter edit |
| UX-10 accessibility | native names/roles/value/unit/actions, dynamic STATE/run, fault descriptions, inspector close/return, no telemetry speech flood | D semantics specified; N VoiceOver and Windows screen-reader evidence; each platform recorded |
| UX-11 palette/type | every text/background pair >=4.5, controls >=3; red only redundant; all font packages accounted; accent <=5% | D contrast report + source inventory + fixture pixel budget; N actual resolved fonts/scale/focus contrast and fallback glyph checks |
| UX-12 truth/recovery | no input vs quiet, running paused residual sound, bypass dry clipping, MIDI vs host, rejected unapplied targets, recall clears history | D fixture copy + ownership; N actual DAW audio/automation and reset/undo, no false success message |
| UX-13 telemetry | unknown/stale/reopen backlog, drops, counter wrap, independent L/R and optional meter hold | D snapshot contract preserved; N deterministic UI fixtures; one consumer/30 Hz, zero closed-editor timer work |
| UX-14 response/budget | feedback <=33.33 ms; timer+paint max <16.67 ms under declared load, measure separately from audio | D inherited budget; N measurements, no screenshots-as-timing-proof |
| UX-15 musician | >=4/5 first-time users generate, edit FLUX/BIAS, select domain, recover in 60 s; separate transform task; all understand recall after guidance | D uncluttered face/task contract; N real participants in sprint 07, no claimed beta here |

additional native scenarios: known connected silent input, connectivity unknown,
preview, UI validation error, pending/rejected brake edge, telemetry initial/stale/
drop/wrap, selected preset truncation, modal inspector keyboard scrolling, host
resize refusal and held historical clip with bypass. these are in addition to,
not encoded as passes by, the 3072-case Cartesian checklist. live-test coverage
must bind exact binary, host, OS/CPU, scale/font and observed result.

sprint 04 implements presentation/helpers/fixtures using existing attachments,
telemetry and restore paths; it must not repair AU/realtime by silently changing
scope. sprint 06 owns font/license packaging if a new face is selected. sprint
07 owns native assistive technology, host scaling, sound/first-minute and timing
qualification. **manual checks unperformed in sprint 03:** native editor changes,
DAW scaling/automation gesture recording, screen readers, musician tasks,
listening, UI timing and optional meters. keep these gates open. no release tag,
publication or external app installation is authorized by this design contract.
