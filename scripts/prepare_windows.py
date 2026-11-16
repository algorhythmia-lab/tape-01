#!/usr/bin/env python3
"""Prepare staged Authenticode signing using an explicit existing Windows certificate."""
import argparse
import json
import pathlib
import re
import shutil
import subprocess
import sys

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--vst3', type=pathlib.Path, required=True)
parser.add_argument('--output', type=pathlib.Path, required=True)
parser.add_argument('--thumbprint', required=True, help='Public SHA-1 selector for the authorized certificate store identity')
parser.add_argument('--timestamp-url', required=True, help='Owner-approved RFC3161 HTTPS endpoint')
parser.add_argument('--signtool', default='signtool.exe')
parser.add_argument('--plan', action='store_true')
args = parser.parse_args()
if not re.fullmatch('[A-Fa-f0-9]{40}', args.thumbprint) or not args.timestamp_url.startswith('https://'):
    parser.error('Explicit certificate thumbprint and HTTPS timestamp endpoint required')
if args.output.exists():
    parser.error('Refusing existing output')
module = args.output / 'SYS 01.vst3/Contents/x86_64-win/SYS 01.vst3'
signature_check = """param([string]$Binary, [string]$Thumbprint)
$ErrorActionPreference = 'Stop'
$s = Get-AuthenticodeSignature -LiteralPath $Binary
if ($s.Status -ne 'Valid' -or $null -eq $s.TimeStamperCertificate -or $s.SignerCertificate.Thumbprint -ne $Thumbprint) { exit 1 }
$s | Format-List Status,SignerCertificate,TimeStamperCertificate
"""
check_path = args.output / 'check-signature.ps1'
commands = [[args.signtool, 'sign', '/sha1', args.thumbprint, '/fd', 'SHA256', '/tr', args.timestamp_url, '/td', 'SHA256', str(module)],
            [args.signtool, 'verify', '/pa', '/all', '/v', str(module)],
            ['powershell.exe', '-NoProfile', '-NonInteractive', '-File', str(check_path), '-Binary', str(module), '-Thumbprint', args.thumbprint]]
if args.plan:
    print(json.dumps({'commands': commands, 'signature_check_script': signature_check, 'postcondition': 'Recreate build provenance and validators for signed bytes, package_candidate.py then archive_candidate.py. Owner-selected Windows ZIP/VST3/notices/PowerShell install and rollback route; install scripts require the explicit disposable fixture.'}, indent=2))
    sys.exit(0)
if sys.platform != 'win32':
    parser.error('Execution requires Windows and an authorized certificate store/provider')
if not (args.vst3 / 'Contents/x86_64-win/SYS 01.vst3').is_file():
    parser.error('Expected a built x64 VST3 bundle')
args.output.mkdir(parents=True)
check_path.write_text(signature_check, encoding='utf-8')
shutil.copytree(args.vst3, args.output / 'SYS 01.vst3')
for index, command in enumerate(commands):
    result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    (args.output / f'step-{index}.log').write_bytes(result.stdout)
    if result.returncode:
        raise SystemExit(f'Signing step {index} failed; inspect retained local log')
print('Staged signature and RFC3161 timestamp verified; rerun final-byte validators')
