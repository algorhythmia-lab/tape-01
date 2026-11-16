#!/usr/bin/env python3
"""Package a clean tagged-source macOS public beta and complete dependency source."""
import argparse
import gzip
import hashlib
import io
import json
import pathlib
import re
import subprocess
import tarfile

from package_source import ROOT, source_files
from verify_dependency import inventory


def run(*command):
    return subprocess.check_output(command, text=True).strip()


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def archive(output, entries):
    """Do not include host xattrs, resource forks, user IDs or timestamps."""
    with output.open('xb') as raw:
        with gzip.GzipFile(filename='', mode='wb', fileobj=raw, mtime=0) as zipped:
            with tarfile.open(fileobj=zipped, mode='w', format=tarfile.PAX_FORMAT) as tar:
                for name, content, mode in sorted(entries):
                    entry = tarfile.TarInfo(name)
                    entry.size, entry.mode, entry.mtime = len(content), mode, 0
                    tar.addfile(entry, io.BytesIO(content))


def tree_entries(source, prefix):
    for path in sorted(source.rglob('*')):
        if '.git' in path.relative_to(source).parts:
            continue
        if path.is_symlink():
            raise ValueError(f'Symlink in release input: {path}')
        if path.is_file():
            yield f'{prefix}/{path.relative_to(source).as_posix()}', path.read_bytes(), path.stat().st_mode & 0o777


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', type=pathlib.Path, required=True)
    parser.add_argument('--output', type=pathlib.Path, required=True)
    parser.add_argument('--juce-source', type=pathlib.Path, required=True)
    parser.add_argument('--doctest-source', type=pathlib.Path, required=True)
    args = parser.parse_args()
    release = json.loads((ROOT / 'release/public-beta.json').read_text())
    tag = release['tag']
    if not re.fullmatch(r'v[0-9]+\.[0-9]+\.[0-9]+-beta\.[0-9]+', tag):
        parser.error('Expected a versioned public beta tag')
    revision = run('git', '-C', str(ROOT), 'rev-parse', 'HEAD')
    if run('git', '-C', str(ROOT), 'status', '--porcelain'):
        parser.error('Commit the exact release source before packaging')
    if args.output.exists():
        parser.error('Refusing to replace an existing release directory')
    lock = json.loads((ROOT / 'cmake/dependencies.lock.json').read_text())['dependencies']
    for name, source in (('juce', args.juce_source), ('doctest', args.doctest_source)):
        measured, count = inventory(source)
        if (measured, count) != (lock[name]['tree_sha256'], lock[name]['file_count']):
            parser.error(f'{name}: pinned source does not match')
    binary_root = args.build / 'apps/desktop/sys01_artefacts/Release'
    bundles = [binary_root / folder / name for folder, name in
               (('Standalone', 'SYS 01.app'), ('VST3', 'SYS 01.vst3'), ('AU', 'SYS 01.component'))]
    binary_manifest = {}
    for bundle in bundles:
        subprocess.run(['codesign', '--verify', '--deep', '--strict', str(bundle)], check=True)
        executable = bundle / 'Contents/MacOS/SYS 01'
        architectures = run('lipo', '-archs', str(executable)).split()
        if set(architectures) != set(release['architectures']):
            parser.error(f'Unexpected architectures: {bundle}')
        binary_manifest[bundle.name] = {
            'architectures': architectures,
            'files': {p.relative_to(bundle).as_posix(): digest(p)
                      for p in sorted(bundle.rglob('*')) if p.is_file()},
        }
    release.update(source_revision=revision, source_dirty=False, dependencies=lock, bundles=binary_manifest)
    manifest = (json.dumps(release, indent=2) + '\n').encode()
    payload = f'SYS-01-{tag[1:]}-macOS-universal'
    args.output.mkdir(parents=True)
    binary_archive = args.output / f'{payload}.tar.gz'
    entries = []
    for bundle in bundles:
        entries.extend(tree_entries(bundle, f'{payload}/binaries/{bundle.name}'))
    entries.extend((f'{payload}/{name}', (ROOT / name).read_bytes(), 0o644)
                   for name in ('LICENSE', 'NOTICE', 'docs/install.md', 'docs/musical-guide.md', 'README.md'))
    entries.extend(tree_entries(ROOT / 'licenses', f'{payload}/licenses'))
    entries.append((f'{payload}/manifest.json', manifest, 0o644))
    archive(binary_archive, entries)

    # Include full pinned framework/test sources, including bundled third-party notices.
    source_prefix = f'SYS-01-{tag[1:]}-source'
    source_entries = [(f'{source_prefix}/{p.relative_to(ROOT).as_posix()}', p.read_bytes(),
                       0o755 if p.suffix in ('.sh', '.py') else 0o644) for p in source_files()]
    origin = (json.dumps({'revision': revision, 'source_dirty': False}, indent=2) + '\n').encode()
    source_entries.append((f'{source_prefix}/SOURCE_PROVENANCE.json', origin, 0o644))
    for name, source in (('JUCE', args.juce_source), ('doctest', args.doctest_source)):
        source_entries.extend(tree_entries(source, f'{source_prefix}/dependencies/{name}'))
    sums = ''.join(f'{hashlib.sha256(data).hexdigest()}  {name.removeprefix(source_prefix + "/")}\n'
                   for name, data, _ in sorted(source_entries)).encode()
    source_entries.append((f'{source_prefix}/SOURCE_MANIFEST.sha256', sums, 0o644))
    archive(args.output / f'SYS-01-{tag[1:]}-source.tar.gz', source_entries)
    installer = (ROOT / 'scripts/install-macos.sh.in').read_text()
    for key, value in {'RELEASE_TAG': tag, 'ARCHIVE_NAME': binary_archive.name,
                       'ARCHIVE_SHA256': digest(binary_archive), 'PAYLOAD': payload}.items():
        installer = installer.replace(f'@{key}@', value)
    (args.output / 'install-macos.sh').write_text(installer)
    (args.output / 'install-macos.sh').chmod(0o755)
    (args.output / 'manifest.json').write_bytes(manifest)
    sums = ''.join(f'{digest(path)}  {path.name}\n' for path in sorted(args.output.iterdir()) if path.is_file())
    (args.output / 'SHA256SUMS.txt').write_text(sums)
    print(f'Packaged {tag} from clean revision {revision}: {args.output}')


if __name__ == '__main__':
    main()
