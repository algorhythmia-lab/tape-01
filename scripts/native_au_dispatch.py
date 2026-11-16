#!/usr/bin/env python3
"""Generate pinned AU SDK dispatch; validation belongs to bounded AU ingress.

SDK prevalidation otherwise traverses unbounded scheduled arrays and rejects
nonfinite setters before the wrapper can reject their complete musical epoch.
"""
import hashlib
import pathlib
import sys

source, output = map(pathlib.Path, sys.argv[1:])
dispatch = source.parent / 'AU/AudioUnitSDK/AUPlugInDispatch.cpp'
pins = {
    source: 'b1e308ccbaf7e29f017245a3499f213c319675233081c6397d3a5fe15fc2047d',
    dispatch: '618e7a7202ce44359c0b00e46ae86e8382b7c94809fd8e15021f74724eda4a03',
}
for path, digest in pins.items():
    if hashlib.sha256(path.read_bytes()).hexdigest() != digest:
        raise SystemExit('JUCE AU dispatch differs from reviewed 8.0.12: '+str(path))
text = dispatch.read_text()
begin = text.index('static bool IsValidParameterValue(')
end = text.index('\n}', text.index('static bool AreValidParameterEvents(')) + 2
text = text[:begin] + '// SYS 01 validates inside its bounded SetParameter/ScheduleParameter overrides.\n' + text[end:]
for old in (
    '\tif (!IsValidParameterValue(value)) {\n\t\treturn kAudioUnitErr_InvalidParameterValue;\n\t}\n',
    '\tif (!AreValidParameterEvents(events, numEvents)) {\n\t\treturn kAudioUnitErr_InvalidParameterValue;\n\t}\n',
):
    if text.count(old) != 1:
        raise SystemExit('AU dispatch anchor not unique')
    text = text.replace(old, '')
output.parent.mkdir(parents=True, exist_ok=True)
generated = output.parent / 'Sys01AUDispatch.cpp'
generated.write_text(text)
unit = source.read_text()
anchor = '#include <juce_audio_plugin_client/AU/AudioUnitSDK/AUPlugInDispatch.cpp>'
if unit.count(anchor) != 1:
    raise SystemExit('AU SDK include anchor not unique')
output.write_text(unit.replace(anchor, '#include "Sys01AUDispatch.cpp"'))
