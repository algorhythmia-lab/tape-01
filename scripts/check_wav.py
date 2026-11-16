#!/usr/bin/env python3
"""Exercise the actual renderer and validate its portable PCM WAV contract."""

import pathlib
import struct
import subprocess
import sys
import tempfile
import wave


def main() -> None:
    if len(sys.argv) != 2:
        raise SystemExit("Usage: check_wav.py /path/to/algorhythmia_render")
    executable = pathlib.Path(sys.argv[1]).resolve(strict=True)
    with tempfile.TemporaryDirectory(prefix="algorhythmia-wav-") as directory:
        output = pathlib.Path(directory) / "smoke.wav"
        subprocess.run([str(executable), str(output)], check=True, timeout=20)
        data = output.read_bytes()
        expected_frames = 48_000 * 5
        expected_bytes = expected_frames * 2 * 2
        assert len(data) == 44 + expected_bytes, "Unexpected file length"
        assert data[:4] == b"RIFF" and data[8:12] == b"WAVE", "Invalid RIFF/WAVE signature"
        assert struct.unpack_from("<I", data, 4)[0] == len(data) - 8, "Incorrect RIFF size"
        assert data[12:16] == b"fmt " and data[36:40] == b"data", "Incorrect chunk layout"
        assert struct.unpack_from("<I", data, 16)[0] == 16, "Invalid PCM format chunk length"
        assert struct.unpack_from("<HHIIHH", data, 20) == (1, 2, 48_000, 192_000, 4, 16)
        assert struct.unpack_from("<I", data, 40)[0] == expected_bytes, "Incorrect data size"
        with wave.open(str(output), "rb") as wav:
            assert wav.getnchannels() == 2
            assert wav.getframerate() == 48_000
            assert wav.getsampwidth() == 2
            assert wav.getnframes() == expected_frames
            pcm = wav.readframes(expected_frames)
            assert any(pcm), "Renderer emitted only silence"
        invalid = subprocess.run(
            [str(executable), str(pathlib.Path(directory) / "missing" / "output.wav")],
            capture_output=True,
            text=True,
            timeout=5,
            check=False,
        )
        assert invalid.returncode != 0, "An output I/O failure must return a failure status"
        assert "Render failed:" in invalid.stderr
    print("WAV contract and output I/O error handling passed")


if __name__ == "__main__":
    main()
