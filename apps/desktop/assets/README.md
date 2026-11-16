# SYS 01 // APP ICON

the icon reduces the tape face to its horizontal shell, paired reels, central register and lower trapezoid. no lettering, screws, control labels, telemetry colors, gradients or shadows. obsidian, basalt and bone match the software palette.

- `sys01-icon.svg`: 1024 px vector master for dock, finder and high-density displays.
- `sys01-icon-small.svg`: 32 px optical variant, with stronger strokes, larger occupied area and a single register cell for small system surfaces.

the transparent canvas rectangles are intentional: JUCE measures drawable bounds when selecting the native icon resolution. keep them, the explicit view boxes and the square canvas dimensions.

`juce_add_plugin` consumes both masters and generates the macOS `.icns` or windows `.ico` during configuration. no image tooling is required to build from a source archive. artwork changes trigger configuration, relinking and the existing local ad-hoc macOS seal. the native build remains responsible for release signing.

verify locally with `cmake --build build/desktop --target sys01_Standalone`. the macOS resource is `build/desktop/apps/desktop/sys01_artefacts/Release/Standalone/SYS 01.app/Contents/Resources/Icon.icns`.
