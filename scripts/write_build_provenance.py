#!/usr/bin/env python3
"""Capture the built artifact/source/toolchain boundary after CMake dependencies finish."""
import argparse
import json
import pathlib
import platform
import re
import subprocess
import sys
from package_source import ROOT, source_files
from release_evidence import canonical, fingerprint, inventory, sha256


def is_build_input(name):
    parts = pathlib.PurePosixPath(name).parts
    return name not in ('SOURCE_MANIFEST.sha256', 'SOURCE_PROVENANCE.json') and (
        parts[0] not in ('docs', 'release', 'licenses') or name == 'docs/design-tokens.json')


def build_inputs():
    return {p.relative_to(ROOT).as_posix(): sha256(p) for p in source_files()
            if is_build_input(p.relative_to(ROOT).as_posix())}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', type=pathlib.Path, required=True)
    parser.add_argument('--artifact', action='append', type=pathlib.Path, required=True)
    args = parser.parse_args()
    args.artifact = [p.resolve() for p in args.artifact]
    build = args.build.resolve()
    cache = (build / 'CMakeCache.txt').read_text()
    settings = {}
    all_settings = {}
    for line in cache.splitlines():
        if line and not line.startswith(('#', '//')) and '=' in line and ':' in line.split('=', 1)[0]:
            key, value = line.split('=', 1)
            key = key.split(':')[0]
            all_settings[key] = value
            if key.startswith(('CMAKE_', 'ALGORHYTHMIA_', 'BUILD_TESTING')):
                settings[key] = value
    compiler = settings.get('CMAKE_CXX_COMPILER')
    if not compiler:
        # Visual Studio generators can keep the selected compiler outside the cache.
        for compiler_file in sorted((build / 'CMakeFiles').glob('*/CMakeCXXCompiler.cmake')):
            match = re.search(r'set\(CMAKE_CXX_COMPILER "([^"]+)"\)', compiler_file.read_text())
            if match:
                compiler = match.group(1)
                break
        if not compiler:
            raise SystemExit('Selected C++ compiler not found in cache or generated compiler metadata')
        settings['CMAKE_CXX_COMPILER'] = compiler
    toolchain_result = subprocess.run([compiler, '--version'] if not platform.system() == 'Windows' else [compiler], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    toolchain = toolchain_result.stdout
    try:
        revision = subprocess.check_output(['git', '-C', str(ROOT), 'rev-parse', 'HEAD'], stderr=subprocess.DEVNULL, text=True).strip()
        status = subprocess.check_output(['git', '-C', str(ROOT), 'status', '--porcelain', '--', '.'], text=True)
        dirty = bool(status)
    except subprocess.CalledProcessError:
        revision, status, dirty = None, 'export without Git; see SOURCE_PROVENANCE.json', None
        source = ROOT / 'SOURCE_PROVENANCE.json'
        if source.exists():
            origin = json.loads(source.read_text())
            revision, dirty = origin['revision'], origin['source_dirty']
    sdk = subprocess.check_output(['xcrun', '--show-sdk-version'], text=True).strip() if platform.system() == 'Darwin' else settings.get('CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION', 'see compiler settings')
    dependencies = [json.loads(p.read_text()) for p in sorted((build / 'dependencies').glob('*.json'))]
    for dependency in dependencies:
        source_key = {'juce': 'JUCE_SOURCE_DIR', 'doctest': 'doctest_SOURCE_DIR'}[dependency['name']]
        subprocess.run([sys.executable, str(ROOT / 'scripts/verify_dependency.py'), dependency['name'], all_settings[source_key]], check=True)
    commands = build / 'compile_commands.json'
    report = dict(schema=1, sdk_version=sdk, compile_commands=json.loads(commands.read_text()) if commands.exists() else None, revision=revision, source_dirty=dirty, git_status=status,
                  build_inputs=build_inputs(), platform=platform.platform(), architecture=platform.machine(),
                  toolchain=toolchain, compiler_sha256=sha256(pathlib.Path(compiler).resolve()), settings=settings,
                  cmake_cache_sha256=sha256(build / 'CMakeCache.txt'),
                  dependencies=dependencies,
                  artifacts=[dict(name=p.name, sha256=fingerprint(p), files=inventory(p)) for p in args.artifact])
    (build / 'build-provenance.json').write_bytes(canonical(report))
    print('Captured build-provenance.json for', len(args.artifact), 'artifacts')


if __name__ == '__main__':
    main()
