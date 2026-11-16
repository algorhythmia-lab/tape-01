#!/usr/bin/env python3
"""Create exact-bundle native reports in CI or a local build; never install plugins."""
import argparse
import pathlib
import platform
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--build', type=pathlib.Path, required=True)
parser.add_argument('--reports', type=pathlib.Path, required=True)
parser.add_argument('--pluginval', type=pathlib.Path, help='Optional hash-verified external validator executable')
args = parser.parse_args()
build = args.build.resolve()
base = build / 'apps/desktop'
artifacts = base / 'sys01_artefacts/Release'
windows = platform.system() == 'Windows'
executable = lambda name: str(base / 'Release' / (name + '.exe')) if windows else str(base / name)


def report(name, artifact, command, pattern=None):
    argv = [sys.executable, str(ROOT / 'scripts/validate_artifact.py'), '--artifact', str(artifact),
            '--output', str(args.reports / (name + '.json')), '--kind', name]
    if pattern:
        argv.extend(['--success-pattern', pattern])
    subprocess.run([*argv, '--', *command], check=True)


vst = artifacts / 'VST3/SYS 01.vst3'
module = '{artifact}/Contents/x86_64-win/SYS 01.vst3' if windows else '{artifact}'
report('vst3-positioned-conformance', vst, [executable('sys01_vst3_contract'), module])
if platform.system() == 'Darwin':
    au = artifacts / 'AU/SYS 01.component'
    report('au-native-callback', au, [executable('sys01_au_contract'), '{artifact}'])
    report('au-offset-characterization', au, [executable('sys01_au_offsets'), '{artifact}'])
    report('au-positioned-conformance', au, [executable('sys01_au_offsets'), '{artifact}', '--require-positioned'])
    for name, artifact in [('vst3-seal', vst), ('au-seal', au)]:
        report(name, artifact, ['/usr/bin/codesign', '--verify', '--deep', '--strict', '--verbose=2', '{artifact}'])
if args.pluginval:
    # GUI tests are deliberately omitted in headless CI; not DAW qualification.
    report('pluginval-level10-headless', vst, [str(args.pluginval.resolve()), '--strictness-level', '10',
           '--skip-gui-tests', '--random-seed', '1012', '--validate', '{artifact}'], r'SUCCESS')
print('Native reports complete; strict AU/VST3 parity is local binary evidence, not DAW qualification')
