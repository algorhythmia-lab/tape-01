#!/usr/bin/env python3
"""Check or apply clang-format to first-party C/C++ sources only."""

import argparse
import os
import pathlib
import shutil
import subprocess


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--check", action="store_true", help="Check without modifying files (default)")
    mode.add_argument("--fix", action="store_true", help="Apply formatting")
    parser.add_argument("--clang-format", default="clang-format-18")
    args = parser.parse_args()
    formatter = shutil.which(args.clang_format)
    if formatter is None:
        parser.error(f"Cannot find {args.clang_format}; install clang-format 18 or pass --clang-format")
    root = pathlib.Path(__file__).resolve().parent.parent
    sources = []
    for tree in (root / "libs", root / "apps"):
        for current, directories, files in os.walk(tree):
            directories[:] = [
                name for name in directories
                if not name.startswith((".", "build")) and name not in ("_deps", "vendor", "third_party")
            ]
            sources.extend(
                str(pathlib.Path(current) / name)
                for name in files
                if pathlib.Path(name).suffix in (".c", ".cpp", ".h", ".hpp", ".mm")
            )
    if not sources:
        parser.error("No project C/C++ sources found")
    options = ["-i"] if args.fix else ["--dry-run", "--Werror"]
    subprocess.run([formatter, *options, *sorted(sources)], check=True)


if __name__ == "__main__":
    main()
