#!/usr/bin/env python3
"""Write local environment diagnostics without project/audio contents."""
import argparse
import hashlib
import json
import pathlib
import platform

ROOT = pathlib.Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--output", type=pathlib.Path, required=True)
parser.add_argument("--binary", type=pathlib.Path, help="Optional plugin executable to hash")
parser.add_argument("--host", default="unspecified", help="DAW and version, supplied by user")
args = parser.parse_args()
report = {"schema": 1, "product": "SYS 01", "version": (ROOT / "VERSION").read_text().strip(),
          "os": platform.system(), "os_release": platform.release(), "architecture": platform.machine(),
          "host": args.host, "recall": "parameters only", "qualification": "development-unqualified"}
if args.binary:
    report["binary_sha256"] = hashlib.sha256(args.binary.read_bytes()).hexdigest()
# Exclusive creation: never overwrite user reports. No home path, username,
# project, audio or machine identifier is collected.
with args.output.open("x") as output:
    json.dump(report, output, indent=2)
    output.write("\n")
print(f"Created {args.output}")
