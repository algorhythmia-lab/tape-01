#!/usr/bin/env python3
"""Summarize measured investigation triggers; write fixed -24 dB audition copies.

No threshold here establishes perceptual sound quality or a musician acceptance.
"""
import argparse
import array
import csv
import hashlib
import json
import math
from pathlib import Path
import struct
import sys
import wave


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('renders', type=Path)
    parser.add_argument('evidence', type=Path)
    parser.add_argument('--require-settled-dc', action='store_true',
                        help='Fail on abs settled 10-second mean > .01 (investigation gate).')
    args = parser.parse_args()
    args.evidence.mkdir(parents=True, exist_ok=True)
    rows = list(csv.DictReader((args.renders / 'metrics.csv').open()))
    assert rows and all(int(r['nonfinite']) == 0 for r in rows)
    assert all(float(r['peak']) <= 1 for r in rows)
    summary = []
    indexed = {(r['case'], r['channel'], r['window']): r for r in rows}
    for r in rows:
        flags = []
        if abs(float(r['dc'])) > .01:
            flags.append('abs_mean_dc_gt_0.01')
        if float(r['max_delta']) > .1:
            flags.append('sample_delta_gt_0.1')
        if int(r['rail_samples']):
            flags.append('output_at_hard_limit')
        jump = None
        if r['window'].startswith('second-'):
            second = int(r['window'].split('-')[1])
            prior = indexed.get((r['case'], r['channel'], f'second-{second-1}'))
            if prior and min(float(r['rms']), float(prior['rms'])) > 1e-8:
                jump = 20 * math.log10(float(r['rms']) / float(prior['rms']))
                if abs(jump) > 3:
                    flags.append('adjacent_1s_rms_change_gt_3dB')
        if flags:
            summary.append(dict(case=r['case'], channel=int(r['channel']), window=r['window'],
                                flags=flags, rms_jump_db=jump,
                                measurements={k:float(r[k]) for k in ('peak','rms','dc','max_delta','rail_samples')}))
    files = {}
    for path in sorted(args.renders.iterdir()):
        if path.is_file():
            files[path.name] = dict(bytes=path.stat().st_size, sha256=digest(path))
    (args.evidence / 'render-hashes.json').write_text(json.dumps(files, indent=2)+'\n')
    for name in ('metrics.csv', 'timeline.csv'):
        (args.evidence / name).write_bytes((args.renders / name).read_bytes())
    (args.evidence / 'review-flags.json').write_text(json.dumps(dict(
        status='MEASUREMENT ONLY; human sound acceptance PENDING',
        rate=int(rows[0]['rate']), flags=summary,
        max_settled_abs_dc=max(abs(float(r['dc'])) for r in rows if r['window']=='settled-10s'),
        nonfinite=0, bounded=True), indent=2)+'\n')
    # Fixed attenuation, no loudness/peak normalization and no fades that hide transitions.
    # Only PCM16 copies are audition material; raw float WAVs remain the measurement source.
    names = ['factory-1','factory-2','factory-3','performance-d0','performance-d1',
             'performance-d2','recall-1','recall-3','speed-1','speed-3','speed-5']
    if int(rows[0]["rate"]) != 48000:
        names = []
    auditions = {}
    for name in names:
        source = args.renders / (name+'.wav')
        if not source.is_file():
            continue
        raw = source.read_bytes()
        assert raw[:4] == b'RIFF' and raw[8:16] == b'WAVEfmt '
        fmt, channels, rate, _, _, bits = struct.unpack_from('<HHIIHH', raw, 20)
        assert (fmt, channels, bits) == (3, 2, 32) and raw[36:40] == b'data'
        floats = array.array('f'); floats.frombytes(raw[44:])
        if sys.byteorder != 'little':
            floats.byteswap()
        assert all(math.isfinite(x) and abs(x) <= 1 for x in floats)
        pcm = array.array('h', (round(x * 10**(-24/20) * 32767) for x in floats))
        if sys.byteorder != 'little':
            pcm.byteswap()
        path = args.evidence / (name+'-minus24dB.wav')
        with wave.open(str(path), 'wb') as out:
            out.setparams((2, 2, rate, 0, 'NONE', 'not compressed'))
            out.writeframes(pcm.tobytes())
        auditions[path.name] = dict(sha256=digest(path), source_sha256=files[source.name]['sha256'],
                                   attenuation_db=-24, rate=rate,
                                   peak_dbfs=20*math.log10(max(abs(x) for x in floats)*10**(-24/20))
                                   if any(floats) else None,
                                   listening='UNPERFORMED')
    (args.evidence / 'auditions.json').write_text(json.dumps(auditions, indent=2)+'\n')
    print(f'PASS: {len(files)} raw artifacts hashed; {len(rows)} measurement windows; '
          f'{len(summary)} review-flag rows; {len(auditions)} fixed -24 dB audition copies. '
          'No listening or perceptual pass inferred.')
    if args.require_settled_dc:
        failures = [r for r in rows if r['window'] == 'settled-10s' and abs(float(r['dc'])) > .01]
        if failures:
            print('FAIL: settled DC investigation threshold exceeded:',
                  [(r['case'], r['channel'], r['dc']) for r in failures])
            raise SystemExit(1)


if __name__ == '__main__':
    main()
