#!/usr/bin/env python3
"""Verify a packet without the original checkout, caches or private credentials."""
import argparse
import json
from release_evidence import inventory, within, validate_report


def verify(root):
    manifest = json.loads((root / 'candidate.json').read_text())
    actual = inventory(root)
    del actual['candidate.json']
    if actual != manifest['hashes']:
        raise ValueError('Candidate file inventory/hash mismatch')
    if manifest['status'] != 'development-unqualified' or manifest['validated_platforms'] or manifest['validated_hosts']:
        raise ValueError('This sprint cannot qualify release platforms or hosts')
    covered = set()
    for name, binding in manifest['reports'].items():
        report = json.loads(within(root / 'reports', name).read_text())
        artifact = within(root, manifest['artifacts'][binding['artifact_sha256']])
        validate_report(report, artifact, root / 'reports')
        if report['status'] != binding['status'] or report['artifact_sha256'] != binding['artifact_sha256']:
            raise ValueError('Report binding mismatch')
        covered.add(binding['artifact_sha256'])
    if covered != set(manifest['artifacts']):
        raise ValueError('Unreported artifact')
    from package_candidate import source_contents
    source_contents(root / 'source.tar.gz')
    print('Candidate hashes and every report/artifact binding verified; qualification unchanged')


if __name__ == '__main__':
    import pathlib
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('packet', type=pathlib.Path)
    verify(parser.parse_args().packet)
