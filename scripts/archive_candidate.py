#!/usr/bin/env python3
"""Create a deterministic ZIP of a verified local candidate (including signed bytes)."""
import argparse
import pathlib
import zipfile
from verify_candidate import verify

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('packet', type=pathlib.Path)
parser.add_argument('--output', type=pathlib.Path, required=True)
args = parser.parse_args()
verify(args.packet)
if args.output.resolve().is_relative_to(args.packet.resolve()):
    parser.error('Output cannot be inside packet')
with zipfile.ZipFile(args.output, 'x', compression=zipfile.ZIP_DEFLATED, compresslevel=9) as archive:
    for path in sorted(args.packet.rglob('*')):
        if path.is_file():
            entry = zipfile.ZipInfo(path.relative_to(args.packet).as_posix(), date_time=(1980, 1, 1, 0, 0, 0))
            entry.create_system = 3
            entry.external_attr = (0o100755 if path.stat().st_mode & 0o111 else 0o100644) << 16
            entry.compress_type = zipfile.ZIP_DEFLATED
            archive.writestr(entry, path.read_bytes(), compresslevel=9)
print('Created deterministic packet ZIP; embedded signing timestamps retained verbatim')
