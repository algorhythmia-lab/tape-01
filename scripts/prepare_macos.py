#!/usr/bin/env python3
"""Prepare a local Developer ID PKG using existing Keychain credentials only."""
import argparse
import json
import pathlib
import shutil
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--vst3', type=pathlib.Path, required=True)
    parser.add_argument('--au', type=pathlib.Path, required=True)
    parser.add_argument('--source', type=pathlib.Path, required=True)
    parser.add_argument('--output', type=pathlib.Path, required=True)
    parser.add_argument('--application-identity', required=True)
    parser.add_argument('--installer-identity', required=True)
    parser.add_argument('--notary-profile', required=True, help='Existing notarytool Keychain profile; never a password')
    parser.add_argument('--plan', action='store_true', help='Print reviewable commands; no writes or signing')
    args = parser.parse_args()
    if not args.application_identity.startswith('Developer ID Application:') or not args.installer_identity.startswith('Developer ID Installer:'):
        parser.error('Explicit Developer ID Application and Installer identities required')
    if args.output.exists():
        parser.error('Output must be a new staging directory')
    root = pathlib.Path(__file__).resolve().parents[1]
    if args.plan:
        version = (root / 'VERSION').read_text().strip()
    else:
        from package_candidate import source_contents
        contents = source_contents(args.source)
        version = contents['VERSION'].decode().strip()
    payload = args.output / 'payload'
    vst = payload / 'Library/Audio/Plug-Ins/VST3/SYS 01.vst3'
    au = payload / 'Library/Audio/Plug-Ins/Components/SYS 01.component'
    pkg = args.output / 'SYS-01.pkg'
    commands = []
    for bundle in (vst, au):
        commands += [['/usr/bin/codesign', '--force', '--options', 'runtime', '--timestamp', '--sign', args.application_identity, str(bundle)],
                     ['/usr/bin/codesign', '--verify', '--deep', '--strict', '--verbose=2', str(bundle)]]
    commands += [['/usr/bin/pkgbuild', '--root', str(payload), '--identifier', 'org.algorhythmia.sys01.pkg', '--version', version, '--install-location', '/', '--sign', args.installer_identity, str(pkg)],
                 ['/usr/bin/pkgutil', '--check-signature', str(pkg)],
                 ['/usr/bin/xcrun', 'notarytool', 'submit', str(pkg), '--keychain-profile', args.notary_profile, '--wait', '--output-format', 'json'],
                 ['/usr/bin/xcrun', 'stapler', 'staple', str(pkg)],
                 ['/usr/bin/xcrun', 'stapler', 'validate', str(pkg)],
                 ['/usr/sbin/spctl', '--assess', '--type', 'install', '--verbose=2', str(pkg)]]
    if args.plan:
        print(json.dumps({'commands': commands, 'postcondition': 'Rerun validators on signed bundles; capture PKG hashes after stapling. Clean install evidence remains required.'}, indent=2))
        return
    if sys.platform != 'darwin':
        parser.error('Execution requires macOS')
    args.output.mkdir(parents=True)
    for source, target in ((args.vst3, vst), (args.au, au)):
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copytree(source, target, symlinks=True)
    notices = payload / 'Library/Application Support/algorhythmia/SYS 01' / version
    notices.mkdir(parents=True)
    for name, data in contents.items():
        if name in ('LICENSE', 'NOTICE') or name.startswith('licenses/'):
            path = notices / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(data)
    shutil.copy2(args.source, notices / 'source.tar.gz')
    for index, command in enumerate(commands):
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        (args.output / f'step-{index:02}.log').write_bytes(result.stdout)
        if result.returncode:
            raise SystemExit(f'Signing/notarization step {index} failed; retained local log')
        if 'notarytool' in command:
            response = json.loads(result.stdout)
            if response.get('status') != 'Accepted':
                raise SystemExit('Notarization not Accepted; no stapling attempted')
    print('Local signed/notarized PKG prepared; rerun exact-byte validators and clean-machine tests')


if __name__ == '__main__':
    main()
