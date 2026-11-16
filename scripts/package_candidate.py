#!/usr/bin/env python3
"""Assemble an unqualified local packet, enforcing source/build/report correspondence."""
import argparse
import hashlib
import json
import pathlib
import shutil
import tarfile
import tempfile
from release_evidence import canonical, fingerprint, inventory, sha256, validate_report

ROOT = pathlib.Path(__file__).resolve().parents[1]


def source_contents(archive_path):
    with tarfile.open(archive_path) as archive:
        files = {}
        prefixes = set()
        for member in archive.getmembers():
            if not member.isfile() or '/' not in member.name or '..' in pathlib.PurePosixPath(member.name).parts or member.name.startswith('/') or '\\' in member.name or ':' in member.name:
                raise ValueError('Source archive contains unsafe entry')
            prefix, name = member.name.split('/', 1)
            prefixes.add(prefix)
            if len(prefixes) != 1:
                raise ValueError('Multiple source archive roots')
            if name in files:
                raise ValueError('Duplicate source entry')
            files[name] = archive.extractfile(member).read()
        manifest = files['SOURCE_MANIFEST.sha256'].decode()
        expected = {}
        for line in manifest.splitlines():
            digest, name = line.split('  ', 1)
            expected[name] = digest
        actual = {name: hashlib.sha256(data).hexdigest() for name, data in files.items() if name != 'SOURCE_MANIFEST.sha256'}
        if expected != actual:
            raise ValueError('Source archive manifest mismatch')
        return files


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--artifact', action='append', type=pathlib.Path, required=True, help='Exact VST3/AU bundle or executable (repeatable)')
    parser.add_argument('--reports', type=pathlib.Path, required=True)
    parser.add_argument('--provenance', type=pathlib.Path, required=True)
    parser.add_argument('--source', type=pathlib.Path, required=True)
    parser.add_argument('--output', type=pathlib.Path, required=True)
    args = parser.parse_args()
    if args.output.exists():
        parser.error('Refusing to overwrite existing packet')
    provenance = json.loads(args.provenance.read_text())
    contents = source_contents(args.source)
    from write_build_provenance import is_build_input
    expected_inputs = {name: hashlib.sha256(data).hexdigest() for name, data in contents.items() if is_build_input(name)}
    if expected_inputs != provenance['build_inputs']:
        parser.error('Incomplete or stale build input inventory')
    for name, digest in provenance['build_inputs'].items():
        if name not in contents or hashlib.sha256(contents[name]).hexdigest() != digest:
            parser.error(f'Source export does not match build input: {name}')
    origin = json.loads(contents['SOURCE_PROVENANCE.json'])
    if (origin['revision'], origin['source_dirty']) != (provenance['revision'], provenance['source_dirty']):
        parser.error('Source/build revision or dirty-status mismatch')
    if not provenance['dependencies'] or not provenance['toolchain']:
        parser.error('Missing verified dependency/toolchain inventory')
    lock = json.loads(contents['cmake/dependencies.lock.json'])['dependencies']
    required = {'juce'} | ({'doctest'} if provenance['settings'].get('BUILD_TESTING') == 'ON' else set())
    if {dep['name'] for dep in provenance['dependencies']} != required:
        parser.error('Missing or unexpected dependency inventory')
    for dep in provenance['dependencies']:
        if dep['verified_tree_sha256'] != lock[dep['name']]['tree_sha256']:
            parser.error('Dependency tree mismatch')
    artifacts = {}
    for artifact in args.artifact:
        if artifact.name in [p.name for p in artifacts.values()]:
            parser.error('Duplicate artifact name')
        digest = fingerprint(artifact)
        if not any(item['sha256'] == digest and item['files'] == inventory(artifact) for item in provenance['artifacts']):
            parser.error(f'Artifact changed since build provenance: {artifact}')
        artifacts[digest] = artifact
    if len(artifacts) != len(args.artifact):
        parser.error('Duplicate artifact fingerprint')
    reports = {}
    used = set()
    covered = set()
    for path in sorted(args.reports.glob('*.json')):
        report = json.loads(path.read_text())
        artifact = artifacts.get(report['artifact_sha256'])
        if artifact is None:
            parser.error(f'Report names another artifact: {path.name}')
        validate_report(report, artifact, args.reports)
        reports[path.name] = report
        used.update((path.name, report['log']))
        covered.add(report['artifact_sha256'])
    if covered != set(artifacts):
        parser.error('Every artifact requires at least one bound report')
    if used != {p.relative_to(args.reports).as_posix() for p in args.reports.rglob('*') if p.is_file()}:
        parser.error('Unbound report files are not accepted')
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='sys01-candidate-', dir=args.output.parent) as staging:
        destination = pathlib.Path(staging) / 'packet'
        destination.mkdir()
        binary_paths = {}
        for digest, artifact in artifacts.items():
            target = destination / 'binaries' / artifact.name
            target.parent.mkdir(exist_ok=True)
            if artifact.is_dir():
                shutil.copytree(artifact, target)
            else:
                shutil.copy2(artifact, target)
            binary_paths[digest] = target.relative_to(destination).as_posix()
            if fingerprint(target) != digest:
                parser.error('Artifact changed during copy')
        shutil.copytree(args.reports, destination / 'reports')
        shutil.copy2(args.source, destination / 'source.tar.gz')
        shutil.copy2(args.provenance, destination / 'build-provenance.json')
        # Always use notices from the source actually built, not the current checkout.
        for name, data in contents.items():
            if name in ('NOTICE', 'LICENSE', 'VERSION', 'CHANGELOG.md') or name.startswith('licenses/'):
                path = destination / name
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(data)
        windows_tools = destination / 'windows'
        windows_tools.mkdir()
        for name, data in contents.items():
            if name.startswith('scripts/windows/') and not name.endswith('/Test-Sys01Install.ps1'):
                (windows_tools / pathlib.PurePosixPath(name).name).write_bytes(data)
        (destination / 'INSTALL.md').write_bytes(contents['docs/release-1.0.0/packaging-06.md'])
        manifest = json.loads(contents['release/manifest.json'])
        manifest.update(schema=2, qualification='development only; not approved for distribution',
                        revision=provenance['revision'], source_dirty=provenance['source_dirty'],
                        version=contents['VERSION'].decode().strip(),
                        toolchain=provenance['toolchain'], dependencies=provenance['dependencies'],
                        artifacts=binary_paths,
                        reports={name: {'artifact_sha256': r['artifact_sha256'], 'status': r['status']} for name, r in reports.items()},
                        reproducibility='Source tar metadata normalized; signed bundle bit identity unproved',
                        prerequisites=json.loads(contents['release/prerequisites.json']),
                        windows_packaging=json.loads(contents['release/windows-route.json']))
        manifest['hashes'] = inventory(destination)
        (destination / 'candidate.json').write_bytes(canonical(manifest))
        from verify_candidate import verify
        verify(destination)
        if args.output.exists():
            parser.error('Destination appeared during assembly')
        destination.rename(args.output)
    print(f'Created local development packet {args.output}')


if __name__ == '__main__':
    main()
