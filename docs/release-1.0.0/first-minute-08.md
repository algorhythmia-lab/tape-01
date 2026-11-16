# SYS 01 / first minute

This local 1.0.0 candidate is unqualified. No host/platform support is declared.
For an already staged test instance on an active stereo audio track:

1. Lower the monitoring level. Select **01 / magnetic pulse**, leave OUTPUT at
   0 dB, disable BYPASS and enable RUN. Bytebeat generates without external input;
   allow the host to process the track even when its input is silent.
2. Move FLUX for clock rate and BIAS for its coupled sound macro. Try binary
   STATE, then **02 / cellular walls**. DOMAIN and STATE are separate controls;
   STATE is shared across domains. The clock runs freely, without host sync.
3. For external sound, route a stereo source into this effect and select
   **03 / forward tape**. Enable RUN, release BRAKE, put FLUX at .75 (+1x),
   and let the empty tape fill. Transform is wet only; silence after reset is
   expected until input arrives. Center FLUX is stopped; its left side reverses.
4. BYPASS fades to untrimmed dry input while the engine advances. For generators
   with silent input, bypass is silent. Lower OUTPUT if LIMIT appears; studies
   and dry bypass can differ in level.
5. Save the host project for **parameters only**. Reopen, RESET, preset recall
   and sample-rate preparation erase tape/voices/phase. UNDO restores parameter
   edits, never erased audio. Render/record your performance to preserve it.

The dials show saved host targets; MIDI CC20/21/22/23 is transient performance
input and is not saved as those targets. Direct numeric entry uses mapped units.
BRAKE is a performance control, not a saved parameter. See
TROUBLESHOOTING.md for silence, rejection and scan recovery.
