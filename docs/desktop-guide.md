# SYS 01 desktop development build

configure from `sys-01` with `cmake --preset desktop`, then `cmake --build --preset desktop` and `ctest --preset desktop`. first configuration fetches pinned JUCE and doctest; offline builds may supply `ALGORHYTHMIA_JUCE_SOURCE` and `FETCHCONTENT_SOURCE_DIR_DOCTEST`. macOS outputs AU, VST3 and a development app under `build/desktop/apps/desktop/sys01_artefacts/Release`; Windows outputs VST3 and standalone using the selected configuration. dependencies do not enter the portable DSP library.

these binaries are unqualified development artifacts. install a copy in your DAW's local test plugin location and rescan. bytebeat/cellular use a stereo audio effect slot and generate when its input is silent. some DAWs suspend effects on silent tracks; use a routed/active audio track and qualify that host before relying on it. transform needs incoming stereo audio. no host compatibility matrix is yet approved.

select `01 / magnetic pulse` to hear the first study. FLUX changes rate; BIAS changes equation/rule/character. DOMAIN selects the process; STATE changes its binary alternate register; DRIVE pauses or resumes. click the register window to toggle brake; click again to release. brake freezes writing in transform or stops evolution in cellular. choose `03 / forward tape` with external audio to hear live displacement. BYPASS smoothly returns the dry input.

use vertical dial drag, Shift for fine adjustment, double-click to reset, or edit the numerical field. Tab stays focus navigation; the DAW spacebar is not intercepted. choose 100/125/150/200% interface scale or resize the window. native JUCE controls provide names, values and accessible roles; screen-reader behavior still requires platform qualification. the eight-element register and meters come from DSP snapshots at 30 Hz, only while the editor exists. no animation represents an unavailable host clock.

the editor uses the tape 01 cassette silhouette, recessed six-notch spools, round
DRIVE/STATE controls and eight square register lights from the webpage's visual
language. colors use the shared brand tokens; embedded Barlow Condensed, JetBrains
Mono and Inter match the webpage. the identity, study selector, undo/redo and
settings share one row inside the cassette. the preset spans the CELLULAR and
TRANSFORM columns; BYPASS shares TRANSFORM's right edge. the unlabelled register
window is also the brake toggle, with keyboard and accessible actions. spool
focus follows the circular rim. settings contains interface scale, RESET,
metering, mappings and diagnostics; Escape closes it. routine diagnostic prose
stays off the performance face. limiting, unresolved rejection and quiet transform
input appear only when applicable. NO INPUT sits above OUTPUT in the lower bay
and reports observed silence, not a proven disconnected bus. the webpage itself
is unchanged.

the standalone app has a transparent, frameless cassette window. the top-left
screw minimizes and the top-right screw closes and saves the app; hover or
keyboard focus reveals their symbols. drag empty casing to move the window and
drag its edge to resize, or use the existing scale selector. on macOS, Cmd+M
minimizes, Cmd+W/Cmd+Q closes and Cmd+, opens settings. minimizing keeps audio
running. settings also contains Audio / MIDI, Save State, Load State and Reset
App (with confirmation). the existing parameter RESET remains unchanged. audio
device selection, MIDI inputs and input muting use the existing standalone
settings; INPUT MUTED appears inside settings when applicable. the DAW plugin
editor retains its existing background, controls and settings layout.

`sys01_standalone_tests <output-folder>` verifies cassette transparency and
unchanged face pixels at all four scales, accessible screws, state restoration,
app reset, and native minimize/restore with audio still attached. run it from an
interactive desktop session with access to the OS window services; it uses an
in-memory settings store. opt into running it through CTest with
`-DALGORHYTHMIA_INTERACTIVE_WINDOW_TESTS=ON`.

OUTPUT spans -60..+20 dB. high trim can hit the output limiter; lower it to preserve headroom. transform is wet-only, starts with empty tape, and zero speed can decay to silence. its 32768-frame history and 2048-frame +1x delay change duration with sample rate. creative delay is not reported processing latency.

factory presets, RESET and session reopen restore controls and restart deterministic sound state with empty tape. captured audio, phases and voice envelopes are not saved. RESET clears captured audio immediately; parameter UNDO cannot recover it. save your DAW project to store the declared host parameters. malformed/future states are rejected with a visible rejection count.

MIDI CC20/21 control FLUX/BIAS, CC22 switches DRIVE at 64, CC23 selects domains in thirds. these are transient sample-positioned performance events; they do not rewrite host automation lanes or stored parameter targets. free clock only; host sync and MIDI learn are deferred. VST3 automation points preserve sample offsets with step targets plus DSP smoothing; AU remains block-boundary pending native qualification. upstream whole-format synchronization/allocation paths remain outside the core realtime proof.

report issues with plugin version, OS/architecture, DAW/version/format, sample rate, block size, routing and reproduction steps. do not include project audio or private session content. retain plugin validator reports and checksums with candidate artifacts; see `release/manifest.json` and `docs/adr/0002-desktop-control-state-and-release.md` for uncompleted release gates.

For playing, routing, factory studies, destructive recall and undo, see the
[musical user guide](musical-guide.md). Measured sound fixtures and pending human
review are indexed in [Sprint 05](release-1.0.0/sprint-05.md).
