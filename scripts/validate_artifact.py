#!/usr/bin/env python3
"""Run a validator on immutable artifact bytes; retain command, tool and log hashes."""
import argparse
import datetime
import json
import pathlib
import re
import shutil
import subprocess
import sys
from release_evidence import canonical, fingerprint, inventory, sha256


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--artifact', type=pathlib.Path, required=True)
    parser.add_argument('--output', type=pathlib.Path, required=True)
    parser.add_argument('--kind', required=True)
    parser.add_argument('--success-pattern', help='Additional required validator success expression')
    parser.add_argument('command', nargs=argparse.REMAINDER)
    args = parser.parse_args()
    command = args.command[1:] if args.command[:1] == ['--'] else args.command
    if not command or not any('{artifact}' in arg for arg in command):
        parser.error('Command must explicitly name {artifact}')
    if args.output.exists() or args.output.with_suffix('.log').exists():
        parser.error('Refusing to overwrite report/log')
    args.artifact = args.artifact.resolve()
    before = fingerprint(args.artifact)
    command = [arg.replace('{artifact}', str(args.artifact)) for arg in command]
    tool = pathlib.Path(shutil.which(command[0]) or command[0]).resolve(strict=True)
    command[0] = str(tool)
    tool_hash = sha256(tool)
    start = datetime.datetime.now(datetime.timezone.utc).isoformat()
    result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if fingerprint(args.artifact) != before:
        raise SystemExit('Validator modified artifact; no valid report emitted')
    if sha256(tool) != tool_hash:
        raise SystemExit('Validator tool changed during execution')
    passed = result.returncode == 0 and (not args.success_pattern or re.search(args.success_pattern, result.stdout.decode(errors='replace')) is not None)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    log = args.output.with_suffix('.log')
    with log.open('xb') as output:
        output.write(result.stdout)
    report = dict(schema=1, kind=args.kind, status='PASS' if passed else 'FAIL',
                  artifact_name=args.artifact.name, artifact_sha256=before,
                  artifact_files=inventory(args.artifact), command=command,
                  tool_sha256=tool_hash, exit_code=result.returncode,
                  started_utc=start, ended_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),
                  success_pattern=args.success_pattern, log=log.name, log_sha256=sha256(log))
    with args.output.open('xb') as output:
        output.write(canonical(report))
    print(f'{args.kind}: {report["status"]}; artifact {before}')
    return 0 if passed else 1


if __name__ == '__main__':
    sys.exit(main())
