> shared-engine implementation revision (2026-10-10): SYS 01 follows [ADR 0002](adr/0002-desktop-control-state-and-release.md). domain and binary STATE are separate; STATE is shared across domains; cellular is 30–300 BPM; transform FLUX is -2..+2x with wet-only 32768-frame history. host sync, captured-audio recall, exact hardware rendering and unfinished chords are deferred. earlier conflicting promises below are historical design intent, not shipped behavior.

# ALGORHYTHMIA // SYS 01 // VST & SOFTWARE REFERENCE UI SPECIFICATION

**DOCUMENT ID:** ALGO-S01-SPEC-UI-01\
**PRODUCT:** SYS 01 (DESKTOP SOFTWARE INSTRUMENT & VST PLUGIN)\
**HARDWARE TWIN:** TAPE 01 REV B (COMPACT CASSETTE PLATFORM)\
**CLASSIFICATION:** SOFTWARE UI SPECIFICATION // VST3, AU, CLAP, MAX FOR LIVE\
**VERSION:** 1.0.0\
**EFFECTIVE DATE:** OCTOBER 2026\
**PARENT SPECIFICATIONS:** [`brand-aesthetic-system.md`](../../brand%20book%20&%20strategy/brand-aesthetic-system.md), [`physical-and-industrial-design.md`](../../tape-01/specs/physical-and-industrial-design.md)\
**TARGET AUDIENCE:** JUCE developers, Max for Live patchers, frontend web engineers, and autonomous AI agents.

---

## 1. EXECUTIVE SUMMARY & SOFTWARE INTERFACE THESIS

sys 01 is the native desktop software twin of tape 01. it executes the identical deterministic C++20 DSP engine (`libs/algorhythmia_dsp`) inside host DAWs (ableton live, bitwig studio, logic pro, reaper).

the user interface of sys 01 rejects generic skeuomorphic wooden cheeks or glossy faux-3D plastic. instead, it presents an **exact 1:1 digital representation of the tape 01 option 1 hardware**:
- **SMOKED OBSIDIAN GLASS SURFACE:** semi-translucent dark substrate revealing vector ENIG gold circuit traces beneath the controls.
- **TWIN 21.4 MM DIGITAL SPOOLS WITH 16-LED HALO RINGS:** continuous circular drag manipulation of `FLUX` and `BIAS` with dynamic playhead and parameter telemetry.
- **GOLDEN RECTANGLE DISPLAY WINDOW ($20.0 \times 12.36\text{ MM}$):** frosted telemetry screen displaying real-time 8-micro-LED Turing register activity.
- **ERGONOMIC DIAGONAL BUTTONS:** `DRIVE` / transport clutch trigger at lower-left; `STATE` Turing register / domain manifold selector at lower-right.
- **DAW TELEMETRY RACK HEADER:** sample-accurate host sync, tempo lock, buffer metrics, and MIDI CC mapping readouts.

```
+---------------------------------------------------------------------------------------------------------+
| [•] ALGORHYTHMIA // SYS 01 VST3        [BPM: 120.0]  [SYNC: HOST]  [BUFFER: 64]  [PRESET: B000_INIT]    |
+---------------------------------------------------------------------------------------------------------+
|  TOP PORTS: [IN: STEREO]  [SYNC IN: HOST CLOCK]  [USB: MIDI CH 1]  [SYNC OUT: ACTIVE]  [OUT: STEREO 0DB]|
|                                                                                                         |
|       ( SPOOL A )                         [ 8-BIT TAPE DISPLAY ]                    ( SPOOL B )         |
|     16-LED HALO RING                         +-------------+                      16-LED HALO RING      |
|    /----------------\                        | [•• •••• ••]|                     /----------------\     |
|   |   FLUX: 42.8%    |                       +-------------+                    |   BIAS: 81.2%    |    |
|   |  (ROTARY DRAG)   |                      AMBER / GREEN / CYAN                |  (ROTARY DRAG)   |    |
|    \----------------/                        GOLDEN RECTANGLE                    \----------------/     |
|                                                                                                         |
|   (O) DRIVE [SPACEBAR]                                                              (O) STATE [TAB]     |
|                                                                                                         |
+---------------------------------------------------------------------------------------------------------+
| MIDI MAPPING: FLUX -> CC #20  |  BIAS -> CC #21  |  AUTOMATION: SAMPLE ACCURATE  |  DSP: ZERO ALLOC     |
+---------------------------------------------------------------------------------------------------------+
```

---

## 2. VST PLUGIN WINDOW DIMENSIONS & RATIOS

- **DEFAULT WINDOW SIZE:** **640 px width $\times$ 440 px height** (resizable vector GUI up to 200% for 4K displays).
- **CASSETTE CORE ASPECT RATIO:** **100.4 / 63.8** ($\approx 1.5737$, golden ratio approximation matching physical hardware).
- **COLOR SYSTEM:**
  - substrate canvas: `color-obsidian-void` (`#08090A`).
  - virtual cassette body: `color-basalt-surface` (`#121417` at 85% opacity over gold circuit vectors).
  - circuit traces: ENIG gold (`#B89230` at 30% to 50% opacity).
  - typography & labels: bone chalk (`#E2E4DE`) and titanium mist (`#8E96A0`).
  - telemetry accents: phosphor green (`#00FF66`), optic cyan (`#54E5FF`), bebop amber (`#FFB000`), akira red (`#D80027`).

---

## 3. PARAMETER CONTROL SPECIFICATION & MIDI MAPPINGS

| GUI CONTROL | PARAMETER ID | DATA RANGE | DEFAULT | MIDI CC | HOST AUTOMATION |
|---|---|---|---|---|---|
| **LEFT SPOOL WHEEL** | `param_flux` | `0.000` to `1.000` | `0.500` | CC #20 | sample-accurate continuous |
| **RIGHT SPOOL WHEEL** | `param_bias` | `0.000` to `1.000` | `0.500` | CC #21 | sample-accurate continuous |
| **LEFT DIAGONAL BUTTON** | `transport_drive` | `0` (stop) / `1` (play) | `0` | CC #22 / MMC | instantaneous clutch / tap-latch |
| **RIGHT DIAGONAL BUTTON**| `engine_state` | `0` (byte), `1` (cell), `2` (live) | `0` | CC #23 | discrete 3-state domain cycle |
| **LATERAL POWER SLIDER** | `engine_bypass` | `0` (bypassed) / `1` (active) | `1` | CC #24 | hard true-bypass crossfade |

---

## 4. DUAL 16-LED HALO RING VISUALIZATION BEHAVIORS

- **FLUX SPOOL RING:**
  - 16 circular dots rendered around the virtual dial perimeter.
  - a single bright phosphor green (`#00FF66`) dot revolves clockwise around the ring, synchronized to host quarter-note tempo or bytebeat calculation clock ticks.
- **BIAS SPOOL RING:**
  - 16 circular dots rendered around the virtual dial perimeter.
  - illuminates as a progressive optic cyan (`#54E5FF`) arc starting from 6 o'clock or 12 o'clock, reflecting the exact floating-point parameter position (`0.0` to `1.0`).

---

## 5. CENTRAL 8-MICRO-LED TURING TELEMETRY BEHAVIORS

- **DOMAIN SELECTION PREVIEW (HOLD `STATE`):**
  - `DOMAIN 00` (bytebeat): `[ • • · · · · · · ]` — amber (`#FFB000`), positions 1 & 2 active.
  - `DOMAIN 01` (cellular fm): `[ · · · • • · · · ]` — green (`#00FF66`), positions 4 & 5 active (bilateral 3-2-3 symmetry).
  - `DOMAIN 02` (live transform): `[ · · · · · · • • ]` — cyan (`#54E5FF`), positions 7 & 8 active.
- **COMPUTATIONAL RUNNING TELEMETRY:**
  - bytebeat: direct bitwise rendering of lowest 8 bits ($b_7 \dots b_0$).
  - cellular: 8-bit Wolfram rule code.
  - live transform: bilateral 4-bit + 4-bit stereo peak saturation VU meter.
- **BILATERAL MASTER GAIN LADDER:**
  - expands symmetrically from center pair outward towards the perimeters upon `HOLD DRIVE + HOLD STATE + TURN FLUX`.

---

## 6. REPOSITORY CODE & TEST BENCH MAPPING

- **IN-BROWSER VST BENCH:** implemented in [`algorhythmia.com/src/pages/Sys01Page.tsx`](../../algorhythmia.com/src/pages/Sys01Page.tsx).
- **VIRTUAL CASSETTE ENGINE:** implemented in [`algorhythmia.com/src/components/CassetteViewer.tsx`](../../algorhythmia.com/src/components/CassetteViewer.tsx).
- **C++20 CORE ENGINE:** implemented in `sys-01/libs/algorhythmia_dsp/`.

---

*ALGORHYTHMIA // SYS 01 VST REFERENCE UI SPECIFICATION // ALGO-S01-SPEC-UI-01 // EOF*
