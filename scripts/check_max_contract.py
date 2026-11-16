#!/usr/bin/env python3
"""Verify every visible control reaches a registered external selector."""
import json
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parents[1]


def check(patch, external):
    boxes = {item["box"]["id"]: item["box"] for item in patch["patcher"]["boxes"]}
    edges = {(tuple(item["patchline"]["source"]), tuple(item["patchline"]["destination"]))
             for item in patch["patcher"]["lines"]}
    registered = set(re.findall(r'class_addmethod\([^;]+?"([a-z0-9]+)"', external))
    expected = {"time": "flux", "form": "bias", "play": "drive", "mode": "state"}
    errors = []
    for control, selector in expected.items():
        message = f"{control}-message"
        if boxes[message].get("text") != f"prepend {selector}" or selector not in registered:
            errors.append(f"{control}: expected registered selector {selector}")
        if ((control, 0), (message, 0)) not in edges or ((message, 0), ("engine", 0)) not in edges:
            errors.append(f"{control}: broken control-to-engine wiring")
    for channel in range(2):
        if (("input", channel), ("engine", channel)) not in edges or \
                (("engine", channel), ("output", channel)) not in edges:
            errors.append(f"broken stereo routing: channel {channel}")
    if errors:
        raise ValueError("\n".join(errors))


if __name__ == "__main__":
    check(json.loads((ROOT / "apps/max_for_live/patchers/study01.maxpat").read_text()),
          (ROOT / "apps/max_for_live/external/algorhythmia_tilde.cpp").read_text())
    print("Max controls and stereo routing match the external")
