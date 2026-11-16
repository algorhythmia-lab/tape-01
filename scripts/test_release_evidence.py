#!/usr/bin/env python3
"""Negative controls for stale reports, log substitution, path escape and mutation."""
import json
import pathlib
import subprocess
import sys
import tempfile
from release_evidence import fingerprint, validate_report, within

ROOT = pathlib.Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory(prefix='sys01-evidence-test-') as temp:
    root = pathlib.Path(temp)
    artifact = root / 'test.vst3'
    artifact.mkdir()
    executable = artifact / 'binary'
    executable.write_bytes(b'candidate one')
    report_path = root / 'report.json'
    command = [sys.executable, str(ROOT / 'scripts/validate_artifact.py'), '--artifact', str(artifact),
               '--output', str(report_path), '--kind', 'negative-control-fixture', '--success-pattern', 'VALIDATED',
               '--', sys.executable, '-c', 'import sys; print("VALIDATED", sys.argv[1])', '{artifact}']
    subprocess.run(command, check=True)
    report = json.loads(report_path.read_text())
    validate_report(report, artifact, root, require_pass=True)
    def rejected(action):
        try:
            action()
        except (ValueError, FileNotFoundError):
            return
        raise AssertionError('Invalid evidence accepted')
    executable.write_bytes(b'candidate two')
    rejected(lambda: validate_report(report, artifact, root))
    executable.write_bytes(b'candidate one')
    log = root / report['log']
    original = log.read_bytes()
    log.write_bytes(b'substituted PASS')
    rejected(lambda: validate_report(report, artifact, root))
    log.write_bytes(original)
    rejected(lambda: within(root, '../escape'))
    (artifact / 'link').symlink_to(executable)
    rejected(lambda: fingerprint(artifact))
    (artifact / 'link').unlink()
    mutation = command.copy()
    mutation[mutation.index(str(report_path))] = str(root / 'mutation.json')
    mutation[mutation.index('import sys; print("VALIDATED", sys.argv[1])')] = 'import pathlib,sys; (pathlib.Path(sys.argv[1])/"binary").write_bytes(b"changed")'
    assert subprocess.run(mutation).returncode != 0
    assert not (root / 'mutation.json').exists()
    executable.write_bytes(b'candidate one')
    failure = command.copy()
    failure[failure.index(str(report_path))] = str(root / 'failure.json')
    failure[failure.index('import sys; print("VALIDATED", sys.argv[1])')] = 'import sys; print("no success marker"); sys.exit(0)'
    assert subprocess.run(failure).returncode != 0
    failed_report = json.loads((root / 'failure.json').read_text())
    rejected(lambda: validate_report(failed_report, artifact, root, require_pass=True))
print('Evidence negative controls PASS')
