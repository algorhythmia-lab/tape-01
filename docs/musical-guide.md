# SYS 01 / playing and recalling studies

SYS 01 is a free desktop stereo effect/generator in development. Use an active
stereo audio track: some hosts suspend effects when a track is silent. Select a
factory study, start with OUTPUT at 0 dB and a low monitoring level, then RUN.
The four existing program names, order and values are retained:

| Program | Useful starting point |
|---|---|
| initial / paused | Silent recovery/default controls; RUN starts bytebeat. |
| 01 / magnetic pulse | Generates without audio input. FLUX sets 500–8000 ticks/s; BIAS blends equations, saturation/filtering and stereo azimuth. STATE selects the alternate bank. |
| 02 / cellular walls | Generates without audio input. FLUX sets 30–300 BPM, four generations/beat. BIAS selects rules 30/90/110/150 and changes FM character; STATE shifts voice pitch. |
| 03 / forward tape | Route external stereo audio **into** this effect. FLUX .75 is +1x; BIAS .2 gives restrained feedback. Empty tape initially produces silence. |

FLUX and BIAS are coupled musical controls, not conventional independent synth
parameters. DOMAIN changes the process; binary STATE is shared across domains.
Dial edits, run, output and bypass use 10 ms ramps; domain switches crossfade.
Bytebeat edges and cellular attacks remain part of the sound. These ramps do not
guarantee that every change is inaudible.

Transform is wet only. Route a stereo source to its track/bus, enable RUN, choose
forward tape and allow the tape to fill. With FLUX outside the center detent,
speed is `4 * FLUX - 2`: left of center is reverse, right is forward, endpoints
are -2x/+2x. Approximately .495–.505 is a true zero-speed detent. STATE reverses
the direction. A stationary head stops writing and its DC-filtered output can
decay to silence. Move FLUX to .75 and release BRAKE to recover. No external input
means transform has nothing to record after reset; generators need no input.

BRAKE slows bytebeat with inertia, stops new cellular generations while voices
decay, and freezes transform **writing** while the playback head keeps moving.
It is not saved as a host parameter. RUN/PAUSE fades sound and stops computation
after the ramp. BYPASS fades to the untrimmed stereo dry input while the engine
keeps advancing; it does not erase tape. There is no wet/dry mix parameter.

OUTPUT spans -60..+20 dB on the wet signal. +20 dB can drive the hard output limit
and alter the sound substantially. Lower OUTPUT when the LIMIT indication appears.
The factory studies have different levels; none is loudness normalized. Bypass
can also change level when dry and generated/wet signals differ. Transform has
32768 frames of storage; its +1x creative delay is 2048 frames (about 46.4 ms at
44.1 kHz, 42.7 ms at 48 kHz, 10.7 ms at 192 kHz). It reports no compensating latency.

Save the DAW project to save the seven declared host parameters. Preset recall,
RESET, project reopen and prepare/sample-rate changes clear tape, voices and
phase; they start the selected settings deterministically with empty tape.
They do not resume a captured performance, and hard recall can make an audible
jump. Fade the track before destructive recall when a seamless transition matters.
Render/record a performance in the DAW if you need to preserve its actual sound.

UNDO/REDO restores parameter edits, including parameters changed by RESET or a
study. It cannot recover erased tape, voice envelopes or phase. Undo is a smoothed
parameter edit: a short departing-domain sound can occur during its crossfade.
Schema 1 is exactly 40 bytes; malformed/future states are rejected rather than
partly restored. Performance MIDI CC20/21/22/23 changes FLUX/BIAS/run/domain in
sound; it does not change the declared parameters saved by the project.

Use the numeric fields for mapped units; FLUX meaning follows DOMAIN. Keep track
of host targets versus DSP/performance telemetry. A rejected batch can leave
visible targets different from sound; a valid subsequent edit or coherent recall
recovers. Host clock sync, captured-audio recall and MIDI learn remain deferred.
See [the development host guide](desktop-guide.md) for build/installation limits,
and [the Sprint 05 listening packet](release-1.0.0/listening-notes-05.md) for measured
conditions and outstanding sound/DAW checks. No host support is inferred from a render.
