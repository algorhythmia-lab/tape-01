#!/usr/bin/env python3
"""Prove local dependency override drift fails closed, including extra files."""
import pathlib
import shutil
import subprocess
import sys
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[1]
source = pathlib.Path(sys.argv[1])
with tempfile.TemporaryDirectory(prefix='sys01-pin-test-') as temp:
    target = pathlib.Path(temp) / 'doctest'
    shutil.copytree(source, target, ignore=shutil.ignore_patterns('.git'))
    command = [sys.executable, str(ROOT / 'scripts/verify_dependency.py'), 'doctest', str(target)]
    subprocess.run(command, check=True)
    header = target / 'doctest/doctest.h'
    original = header.read_bytes()
    header.write_bytes(original + b'\n// drift\n')
    assert subprocess.run(command).returncode != 0
    header.write_bytes(original)
    (target / 'extra.cmake').write_text('unexpected dependency input\n')
    assert subprocess.run(command).returncode != 0
print('Pinned dependency negative controls PASS')
