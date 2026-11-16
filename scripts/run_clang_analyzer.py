#!/usr/bin/env python3
"""Analyze first-party library and adapter translation units from compile_commands."""
import argparse
import json
import pathlib
import shlex
import subprocess

ROOT = pathlib.Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("database", type=pathlib.Path)
parser.add_argument("--clang", default="clang++")
args = parser.parse_args()
count = 0
for entry in json.loads(args.database.read_text()):
    source = pathlib.Path(entry["file"]).resolve()
    if not source.is_relative_to(ROOT) or source.suffix not in (".cpp", ".mm") or "tests" in source.parts:
        continue
    if source.name.endswith("Tests.cpp") or source.name == "RealtimeProbe.cpp":
        continue
    command = entry.get("arguments") or shlex.split(entry["command"])
    cleaned = [args.clang, "--analyze", "-Xanalyzer", "-analyzer-output=text", "-Xanalyzer", "-analyzer-werror"]
    i = 1
    while i < len(command):
        if command[i] in ("-o", "-MF", "-MT", "-MQ"):
            i += 2
        elif command[i] in ("-c", "-MD", "-MMD") or command[i].startswith("-fsanitize="):
            i += 1
        elif command[i].startswith("-I") and len(command[i]) > 2 and not pathlib.Path(command[i][2:]).resolve().is_relative_to(ROOT):
            # Dependency headers retain their compiler settings, but first-party
            # analysis must not promote upstream header diagnostics to errors.
            cleaned.extend(("-isystem", command[i][2:]))
            i += 1
        else:
            cleaned.append(command[i])
            i += 1
    subprocess.run(cleaned, cwd=entry["directory"], check=True, timeout=120)
    count += 1
if count == 0:
    raise SystemExit("No first-party translation units analyzed")
print(f"Clang analyzer passed for {count} first-party translation units")
