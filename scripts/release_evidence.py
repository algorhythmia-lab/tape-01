#!/usr/bin/env python3
"""Shared exact-artifact evidence primitives; no network or credentials."""
import hashlib
import json
import pathlib
import re


def sha256(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def canonical(value):
    return (json.dumps(value, sort_keys=True, indent=2) + '\n').encode()


def inventory(root):
    root = pathlib.Path(root)
    paths = [root] if root.is_file() else sorted(root.rglob('*'))
    result = {}
    for path in paths:
        if path.is_symlink():
            raise ValueError(f'Symlinks are not accepted in candidate artifacts: {path}')
        if path.is_file():
            name = path.name if root.is_file() else path.relative_to(root).as_posix()
            result[name] = {'sha256': sha256(path), 'size': path.stat().st_size}
    if not result:
        raise ValueError(f'Empty artifact: {root}')
    return result


def fingerprint(root):
    return hashlib.sha256(canonical(inventory(root))).hexdigest()


def within(root, name):
    relative = pathlib.PurePosixPath(name)
    if relative.is_absolute() or '..' in relative.parts or not relative.parts or '\\' in name or ':' in name:
        raise ValueError(f'Unsafe relative path: {name}')
    path = root / relative
    if not path.resolve().is_relative_to(root.resolve()):
        raise ValueError(f'Path escapes root: {name}')
    return path


def validate_report(report, artifact, directory, require_pass=False):
    if report['schema'] != 1 or report['artifact_sha256'] != fingerprint(artifact):
        raise ValueError('Report does not match artifact bytes')
    if inventory(artifact) != report['artifact_files']:
        raise ValueError('Report file inventory mismatch')
    if sha256(within(directory, report['log'])) != report['log_sha256']:
        raise ValueError('Report log hash mismatch')
    if report['status'] not in ('PASS', 'FAIL'):
        raise ValueError('Unknown report status')
    pattern = report.get('success_pattern')
    output = within(directory, report['log']).read_text(errors='replace')
    expected_pass = report['exit_code'] == 0 and (not pattern or re.search(pattern, output) is not None)
    if (report['status'] == 'PASS') != expected_pass:
        raise ValueError('Report status disagrees with command/log success criterion')
    if report['status'] == 'PASS' and report['exit_code'] != 0:
        raise ValueError('Passing report has failed command')
    if require_pass and report['status'] != 'PASS':
        raise ValueError('Report failed')
