#!/usr/bin/env python3
"""Create a reproducible source-only archive without requiring a Git checkout."""

import argparse
import gzip
import hashlib
import io
import os
import pathlib
import re
import tarfile
import tempfile
import subprocess
import json


ROOT = pathlib.Path(__file__).resolve().parent.parent
ROOT_FILES = (
    "CMakeLists.txt", "CMakePresets.json", "VERSION", "README.md", "CHANGELOG.md", ".clang-format", ".clang-tidy", ".gitignore",
    "LICENSE", "LICENSE.md", "LICENSE.txt", "COPYING", "NOTICE",
)
SOURCE_TREES = (
    ".github/workflows", ".github/ISSUE_TEMPLATE", "licenses", "cmake", "libs/algorhythmia_dsp", "libs/algorhythmia_host", "apps/desktop", "release", "apps/cli_harness",
    "apps/max_for_live", "apps/firmware_stub", "docs", "scripts",
)
EXCLUDED_DIRECTORIES = {
    "_deps", "vendor", "third_party", "sdk", "sdks", "max-sdk", "max-sdk-base",
    "evidence", "pico-sdk", "node_modules", "__pycache__", "dist", "coverage", "strategy",
}
SOURCE_SUFFIXES = {
    ".c", ".cc", ".cpp", ".cxx", ".m", ".mm", ".h", ".hpp", ".cmake", ".md", ".txt",
    ".py", ".sh", ".ps1", ".psm1", ".yml", ".yaml", ".json", ".maxpat", ".maxhelp", ".svg", ".in",
}


def source_files() -> list[pathlib.Path]:
    files = [ROOT / name for name in ROOT_FILES if (ROOT / name).exists()]
    for relative in SOURCE_TREES:
        tree = ROOT / relative
        if tree.is_symlink():
            raise ValueError(f"Source tree must not be a symlink: {relative}")
        for current, directories, names in os.walk(tree, followlinks=False):
            directories[:] = [
                name for name in directories
                if not name.startswith((".", "build", "cmake-build"))
                and name.lower() not in EXCLUDED_DIRECTORIES
            ]
            for directory in directories:
                if (pathlib.Path(current) / directory).is_symlink():
                    raise ValueError(f"Source directory must not be a symlink: {directory}")
            for name in names:
                path = pathlib.Path(current) / name
                if relative == "licenses" or name in ("CMakeLists.txt", ".gitkeep") or (
                    not name.startswith(".") and path.suffix.lower() in SOURCE_SUFFIXES
                ):
                    files.append(path)
    for path in files:
        if path.is_symlink() or not path.is_file():
            raise ValueError(f"Source entry must be a regular file: {path.relative_to(ROOT)}")
    return sorted(set(files), key=lambda path: path.relative_to(ROOT).as_posix())


def add_bytes(archive: tarfile.TarFile, name: str, content: bytes, mode: int = 0o644) -> None:
    info = tarfile.TarInfo(name)
    info.size = len(content)
    info.mode = mode
    info.mtime = 0
    archive.addfile(info, io.BytesIO(content))


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--version", default=(ROOT / "VERSION").read_text().strip(), help="Archive label (default: VERSION)")
    parser.add_argument("--output", type=pathlib.Path, help="New .tar.gz archive path")
    parser.add_argument("--list", action="store_true", help="List included source files without writing")
    args = parser.parse_args()
    if not re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9._-]{0,63}", args.version):
        parser.error("Version must be 1–64 letters, digits, dots, hyphens, or underscores")
    try:
        files = source_files()
    except ValueError as error:
        parser.error(str(error))
    if args.list:
        for path in files:
            print(path.relative_to(ROOT).as_posix())
        return
    output = (args.output or ROOT / "dist" / f"algorhythmia-source-{args.version}.tar.gz").absolute()
    if not output.name.endswith(".tar.gz"):
        parser.error("Output must have the .tar.gz suffix")
    if output.exists() or output.is_symlink():
        parser.error(f"Refusing to overwrite existing output: {output}")
    output.parent.mkdir(parents=True, exist_ok=True)
    prefix = f"algorhythmia-{args.version}"
    # Stage on the output filesystem; hard-link publication cannot overwrite a
    # destination created concurrently, unlike rename/replace on POSIX.
    with tempfile.TemporaryDirectory(prefix=".algorhythmia-package-", dir=output.parent) as staging:
        temporary = pathlib.Path(staging) / "source.tar.gz"
        with temporary.open("xb") as raw:
            with gzip.GzipFile(filename="", mode="wb", fileobj=raw, mtime=0) as compressed:
                with tarfile.open(fileobj=compressed, mode="w", format=tarfile.PAX_FORMAT) as archive:
                    manifest = []
                    provenance = ROOT / "SOURCE_PROVENANCE.json"
                    if provenance.exists():
                        origin = provenance.read_bytes()
                    else:
                        try:
                            revision = subprocess.check_output(["git", "-C", str(ROOT), "rev-parse", "HEAD"], stderr=subprocess.DEVNULL, text=True).strip()
                            dirty = bool(subprocess.check_output(["git", "-C", str(ROOT), "status", "--porcelain", "--", "."], text=True))
                        except subprocess.CalledProcessError:
                            revision, dirty = None, None
                        origin = (json.dumps({"revision": revision, "source_dirty": dirty}, sort_keys=True, indent=2) + "\n").encode()
                    add_bytes(archive, f"{prefix}/SOURCE_PROVENANCE.json", origin)
                    manifest.append(f"{hashlib.sha256(origin).hexdigest()}  SOURCE_PROVENANCE.json\n")
                    for path in files:
                        relative = path.relative_to(ROOT).as_posix()
                        content = path.read_bytes()
                        mode = 0o755 if path.suffix in (".sh", ".py") else 0o644
                        add_bytes(archive, f"{prefix}/{relative}", content, mode)
                        manifest.append(f"{hashlib.sha256(content).hexdigest()}  {relative}\n")
                    add_bytes(archive, f"{prefix}/SOURCE_MANIFEST.sha256", "".join(manifest).encode())
        try:
            os.link(temporary, output)
        except FileExistsError:
            parser.error(f"Refusing to overwrite existing output: {output}")
    print(f"Created {output} ({len(files)} source files)")


if __name__ == "__main__":
    main()
