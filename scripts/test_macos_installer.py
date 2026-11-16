#!/usr/bin/env python3
"""Exercise real release bundles only inside disposable user-prefix fixtures."""
import argparse
import hashlib
import os
import pathlib
import plistlib
import subprocess
import tempfile


def files(root):
    return {p.relative_to(root).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
            for p in root.rglob('*') if p.is_file() and not p.is_symlink()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('release', type=pathlib.Path)
    args = parser.parse_args()
    release = args.release.resolve()
    installer = release / 'install-macos.sh'
    archive = next(release.glob('*-macOS-universal.tar.gz'))
    subprocess.run(['/bin/bash', '-n', str(installer)], check=True)
    with tempfile.TemporaryDirectory(prefix='sys01-installer-test-') as directory:
        root = pathlib.Path(directory)
        prefix = root / 'tester home'
        base = ['/bin/bash', str(installer), '--archive', str(archive), '--prefix', str(prefix)]

        def run(arguments=(), expected=0, environment=None):
            result = subprocess.run(base + list(arguments), env=environment, text=True,
                                    stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            if (result.returncode == 0) != (expected == 0):
                raise AssertionError(result.stdout)
            return result.stdout

        run(['--help'])
        run(['--dry-run'])
        assert not prefix.exists(), 'dry run created installation directories'
        damaged = root / 'damaged.tar.gz'
        damaged.write_bytes(archive.read_bytes() + b'changed')
        assert 'SHA-256 mismatch' in run(['--archive', str(damaged)], expected=1)
        assert not prefix.exists(), 'corrupt archive modified the installation prefix'
        run()
        targets = [prefix / name for name in ('Applications/SYS 01.app',
                   'Library/Audio/Plug-Ins/VST3/SYS 01.vst3',
                   'Library/Audio/Plug-Ins/Components/SYS 01.component')]
        assert all(p.is_dir() for p in targets)
        for target in targets:
            subprocess.run(['codesign', '--verify', '--deep', '--strict', str(target)], check=True,
                           stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
            quarantine = subprocess.run(['xattr', '-p', 'com.apple.quarantine', str(target)],
                                        stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            assert quarantine.returncode != 0, 'unexpected quarantine attribute'
        first = files(prefix)
        run()
        assert files(prefix) == first, 'repeat installation changed bundle contents'

        # Distinguish the old version so rollback must restore its actual bytes.
        marker = targets[0] / 'Contents/Resources/old-version-fixture.txt'
        marker.write_text('previous version, retained by rollback\n')
        subprocess.run(['codesign', '--force', '--sign', '-', str(targets[0])], check=True,
                       stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
        previous = files(prefix)
        shim = root / 'shim'
        shim.mkdir()
        move = shim / 'mv'
        move.write_text('''#!/bin/bash
case "$1" in
    */.sys01-stage.*/new)
        case "$2" in *'SYS 01.vst3') echo 'injected second-bundle failure' >&2; exit 1 ;; esac ;;
esac
exec /bin/mv "$@"
''')
        move.chmod(0o755)
        environment = dict(os.environ, PATH=str(shim) + ':' + os.environ.get('PATH', ''))
        assert 'injected second-bundle failure' in run(expected=1, environment=environment)
        assert files(prefix) == previous, 'partial update did not restore the previous bundles'
        assert not list(prefix.rglob('.sys01-stage.*')), 'rollback left staging directories'
        run()
        assert files(prefix) == first, 'successful upgrade did not replace the old version'

        # A same-name bundle from another vendor and a symlink must both survive.
        foreign = root / 'foreign home'
        other = foreign / 'Applications/SYS 01.app/Contents'
        other.mkdir(parents=True)
        (other / 'Info.plist').write_bytes(plistlib.dumps({'CFBundleIdentifier': 'example.other'}))
        original = files(foreign)
        run(['--prefix', str(foreign)], expected=1)
        assert files(foreign) == original
        linked = root / 'linked home'
        (linked / 'Applications').mkdir(parents=True)
        (linked / 'Applications/SYS 01.app').symlink_to(targets[0], target_is_directory=True)
        run(['--prefix', str(linked)], expected=1)
        assert (linked / 'Applications/SYS 01.app').is_symlink()
        assert files(prefix) == first
    print('Installer PASS: dry run, corrupt download rejection, install, repeat install, '
          'rollback after a partial update, upgrade, foreign-bundle and symlink rejection; '
          'all inside disposable prefixes with spaces in their names')


if __name__ == '__main__':
    main()
