#!/usr/bin/env python3
"""Verify retained notice bytes and the explicit bundled-font inventory."""
import hashlib
import json
import pathlib

ROOT = pathlib.Path(__file__).resolve().parents[1]
manifest = json.loads((ROOT / 'licenses/inventory.json').read_text())
lock = json.loads((ROOT / 'cmake/dependencies.lock.json').read_text())['dependencies']
for item in manifest['notices']:
    path = ROOT / item['notice']
    assert path.resolve().is_relative_to((ROOT / 'licenses').resolve())
    assert hashlib.sha256(path.read_bytes()).hexdigest() == item['sha256'], item['notice']
    assert item['revision'] == lock[item['dependency']]['revision']
assert hashlib.sha256((ROOT / 'LICENSE').read_bytes()).hexdigest() == '0d96a4ff68ad6d4b6f1f30f713b18d5184912ba8dd389f86aa7710db079abcb0'
fonts = json.loads((ROOT / 'licenses/fonts/inventory.json').read_text())
allowed = set()
for font in fonts['fonts']:
    asset, notice = ROOT / font['asset'], ROOT / font['notice']
    for path in (asset, notice):
        assert path.resolve().is_relative_to((ROOT / 'licenses/fonts').resolve())
    assert hashlib.sha256(asset.read_bytes()).hexdigest() == font['sha256'], font['asset']
    assert hashlib.sha256(notice.read_bytes()).hexdigest() == font['notice_sha256'], font['notice']
    assert 'SIL OPEN FONT LICENSE Version 1.1' in notice.read_text()
    allowed.add(asset)
actual = {p for tree in ('apps', 'libs', 'licenses') for p in (ROOT / tree).rglob('*')
          if p.suffix.lower() in ('.ttf', '.otf', '.woff', '.woff2')}
assert actual == allowed, 'Font asset missing from the verified inventory'
print('Selected dependency notices and three OFL font hashes verified; wider rights audit remains pending')
