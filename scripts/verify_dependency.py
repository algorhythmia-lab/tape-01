#!/usr/bin/env python3
"""Verify even local dependency overrides against the reviewed archive contents."""
import argparse
import hashlib
import json
import pathlib

ROOT = pathlib.Path(__file__).resolve().parents[1]


def inventory(root):
    files = {}
    for path in sorted(root.rglob('*')):
        relative = path.relative_to(root)
        if '.git' in relative.parts:
            continue
        if path.is_symlink():
            raise ValueError(f'Dependency symlink rejected: {relative}')
        if path.is_file():
            files[relative.as_posix()] = hashlib.sha256(path.read_bytes()).hexdigest()
    content = ''.join(f'{digest}  {name}\n' for name, digest in sorted(files.items()))
    return hashlib.sha256(content.encode()).hexdigest(), len(files)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('name', choices=['juce', 'doctest'])
    parser.add_argument('source', type=pathlib.Path)
    parser.add_argument('--output', type=pathlib.Path)
    args = parser.parse_args()
    expected = json.loads((ROOT / 'cmake/dependencies.lock.json').read_text())['dependencies'][args.name]
    digest, count = inventory(args.source)
    if (digest, count) != (expected['tree_sha256'], expected['file_count']):
        raise SystemExit(f'{args.name}: pinned tree mismatch ({digest}, {count} files)')
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps({'name': args.name, **expected, 'verified_tree_sha256': digest}, indent=2) + '\n')
    print(f'{args.name}: verified {count} files at {expected["revision"]}')


if __name__ == '__main__':
    main()
