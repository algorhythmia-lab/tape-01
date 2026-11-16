# ADR 0001 — One C++ engine, native Max SDK adapter

Date: 2026-09-30. Status: accepted for the bootstrap.

The project requires musical algorithms to be authored once and built for both desktop validation and RP2350. The source of truth is the portable C++20 library. The Max SDK external `algorhythmia~` wraps it, and an editable Max patch routes Live audio and control messages. This follows the SDK's native external and DSP callback model. [Max SDK](https://github.com/Cycling74/max-sdk), [MSP object lifecycle](https://sdk.cdn.cycling74.com/max-sdk-7.1.0/chapter_msp_anatomy.html).

A separate gen~ implementation would introduce a second musical algorithm source. Gen export is therefore outside this bootstrap. A future AU/VST adapter can link the same core after its framework and distribution requirements are chosen.

Max and Pico SDKs stay optional, supplied dependencies. The native core and CLI need neither SDK; unit tests fetch only pinned doctest sources. Host-specific linking, sample conversion, lifecycle hooks, and packaging remain in `apps/`.

The initial `.maxpat` is an integration aid. Producing a distributable `.amxd` still requires building the external for the intended host architecture, editing/saving an Audio Effect in Live, exposing automatable parameters, implementing state recall, and testing the packaged device.
