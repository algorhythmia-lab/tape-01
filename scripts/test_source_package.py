#!/usr/bin/env python3
"""Check reproducible source export and tooling ownership without network."""
import hashlib
import pathlib
import subprocess
import sys
import tarfile
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[1]
for config in (".clang-format", ".clang-tidy", ".gitignore"):
    workspace = ROOT.parent / config
    if workspace.exists():
        assert workspace.read_bytes() == (ROOT / config).read_bytes(), f"source-export config drift: {config}"

with tempfile.TemporaryDirectory(prefix="sys01-export-test-") as directory:
    outputs = [pathlib.Path(directory) / name for name in ("a.tar.gz", "b.tar.gz")]
    for output in outputs:
        subprocess.run([sys.executable, str(ROOT / "scripts/package_source.py"), "--output", str(output)], check=True)
    assert outputs[0].read_bytes() == outputs[1].read_bytes(), "non-reproducible source archive"
    with tarfile.open(outputs[0]) as archive:
        prefix = f"algorhythmia-{(ROOT / 'VERSION').read_text().strip()}/"
        names = {member.name.removeprefix(prefix) for member in archive.getmembers()}
        for required in (".clang-format", ".clang-tidy", ".gitignore", "VERSION", "CMakePresets.json", "apps/desktop/CMakeLists.txt", "apps/desktop/NativeEditorHost.mm", "scripts/windows/Install-Sys01.ps1", "scripts/windows/Rollback-Sys01.ps1", "scripts/windows/Uninstall-Sys01.ps1", "scripts/windows/New-Sys01TestFixture.ps1", "scripts/windows/Sys01.Install.psm1", "scripts/windows/Test-Sys01Install.ps1", "LICENSE", "licenses/inventory.json", "licenses/JUCE/modules/juce_graphics/fonts/harfbuzz/COPYING", "libs/algorhythmia_host/CMakeLists.txt", "release/manifest.json", "SOURCE_MANIFEST.sha256"):
            assert required in names, required
        assert not any(part.startswith("build") or part == "_deps" for name in names for part in pathlib.PurePosixPath(name).parts[:-1])
        manifest = archive.extractfile(prefix + "SOURCE_MANIFEST.sha256").read().decode()
        for line in manifest.splitlines():
            digest, name = line.split("  ", 1)
            assert hashlib.sha256(archive.extractfile(prefix + name).read()).hexdigest() == digest, name
print("source export is reproducible; tooling and hashes verified")
