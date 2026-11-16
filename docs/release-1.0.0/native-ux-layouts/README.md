# SYS 01 // ANNOTATED NATIVE LAYOUTS

sprint 03 design fixtures, authored 2026-10-14. these are original vector layouts
with offline raster previews; they are not captures of an implemented editor.
behavior and acceptance authority: [native UX contract](../native-ux-spec.md).

| fixture | annotated review board | editable vector | condition |
|---|---|---|---|
| session strip | [PNG](01-session-annotated.png) | [SVG](01-session-annotated.svg) | bytebeat / paused / STATE 0 / known disabled input / preset focus |
| twin-spool performance face | [PNG](02-performance-annotated.png) | [SVG](02-performance-annotated.svg) | cellular / run / STATE 1 / brake / modified / rejected / display drops / FLUX focus |
| signal strip | [PNG](03-signal-annotated.png) | [SVG](03-signal-annotated.svg) | transform / run / STATE 1 / brake / bypass / no input / held limiting / modified / rejected / bypass focus |

![session strip](01-session-annotated.png)

![twin-spool performance face](02-performance-annotated.png)

![signal strip](03-signal-annotated.png)

unannotated `01-session.svg`, `02-performance.svg`, `03-signal.svg` are exactly
640 × 440 logical units. each has `-100.png`, `-125.png`, `-150.png`, `-200.png`
renders at 640×440, 800×550, 960×660 and 1280×880. annotations sit outside the
product canvas. the preview renderer resolves its own generic sans font; native
system-font rendering and keyboard/assistive behavior remain unqualified.

[layout-contract.json](layout-contract.json) gives every control rectangle and
fixture state. [acceptance-cases.csv](acceptance-cases.csv) enumerates all 3072
combinations from UX-04, explicitly **PENDING** native rendering/interaction.
three visual examples do not represent full Cartesian screenshot coverage.
known-disabled input and a held historical clip are intentional authored facts
in the signal fixture; they cannot be inferred from zero peaks alone.

reproduce without network or app installation:

```sh
python3 sys-01/docs/release-1.0.0/native-ux-layouts/build_layouts.py
# provide the existing tooling runtime's sharp package via NODE_PATH.
node sys-01/docs/release-1.0.0/native-ux-layouts/render_layouts.cjs
node sys-01/docs/release-1.0.0/native-ux-layouts/check_layouts.cjs
python3 sys-01/docs/release-1.0.0/native-ux-layouts/check_contract.py
```

exact local runtime paths, argv, results and hashes appear in
[commands.json](../evidence/sprint-03/commands.json). `check_layouts.cjs` measures
actual offline glyph bounds within authored boxes and accent pixel area; it
cannot establish JUCE font, native accessibility or DAW scaling. `check_contract.py`
checks geometry, bit order, scenario completeness, source preservation, links
and contrast. no shipping code or font binary is added. the native implementation
must consume existing snapshots and attachment paths, not import these SVGs as
fake live controls.
