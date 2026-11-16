#!/usr/bin/env python3
"""Run auval only in an explicitly disposable GitHub-hosted macOS test account."""
import os
import pathlib
import shutil
import subprocess
import sys
from release_evidence import fingerprint

if os.environ.get('RUNNER_ENVIRONMENT') != 'github-hosted' or os.environ.get('RUNNER_OS') != 'macOS':
    raise SystemExit('Requires a disposable GitHub-hosted macOS runner; no personal plugin directories touched')
source = pathlib.Path(sys.argv[1]).resolve(strict=True)
if source.suffix != '.component':
    raise SystemExit('Expected the exact AU bundle')
# GitHub-hosted runner HOME is disposable. This path is never used locally.
target = pathlib.Path.home() / 'Library/Audio/Plug-Ins/Components/SYS01-CI.component'
if target.exists():
    raise SystemExit('Refusing to replace existing test plugin')
before = fingerprint(source)
target.parent.mkdir(parents=True, exist_ok=True)
shutil.copytree(source, target)
try:
    if fingerprint(target) != before:
        raise SystemExit('Installed test copy mismatch')
    result = subprocess.run(['/usr/bin/auval', '-v', 'aumf', 'Sys1', 'Algr', '-strict'])
    if fingerprint(target) != before:
        raise SystemExit('Installed test copy changed')
    sys.exit(result.returncode)
finally:
    # Do not delete an unexpected/tampered tree.
    if fingerprint(target) == before:
        shutil.rmtree(target)
