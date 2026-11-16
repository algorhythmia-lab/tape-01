#!/usr/bin/env python3
"""Prevent parameter documentation from drifting from the compiled catalogue."""
import pathlib
import subprocess
import sys

actual = subprocess.check_output([sys.argv[1]], text=True)
expected = pathlib.Path(sys.argv[2]).read_text()
if actual != expected:
    raise SystemExit("Parameter documentation is stale: run host_catalogue > docs/parameters.md")
print("parameter documentation matches the executable catalogue")
