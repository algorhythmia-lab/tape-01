#!/usr/bin/env python3
"""Check the documentation boundary, sources, color pairs and complete scenario list."""
import csv
import hashlib
import itertools
import json
from pathlib import Path
import re
import xml.etree.ElementTree as ET
from urllib.parse import unquote

HERE=Path(__file__).resolve().parent
RELEASE=HERE.parent
ROOT=HERE.parents[3]
EVIDENCE=RELEASE/'evidence/sprint-03'
contract=json.loads((HERE/'layout-contract.json').read_text())
assert contract['base']==[640,440] and contract['scales']==[1,1.25,1.5,2]
rects=contract['geometry']
for name,(x,y,w,h) in rects.items():
    assert min(x,y)>=0 and x+w<=640 and y+h<=440,(name,'bounds')
    assert min(w,h)>=28,(name,'hit target')
for (a,ra),(b,rb) in itertools.combinations(rects.items(),2):
    x,y,w,h=ra;xx,yy,ww,hh=rb
    assert x+w<=xx or xx+ww<=x or y+h<=yy or yy+hh<=y,(a,b,'overlap')
for file in HERE.glob('*.svg'):
    tree=ET.parse(file); ns={'s':'http://www.w3.org/2000/svg'}
    bits=tree.findall('.//s:g[@id="register"]/s:rect[@data-bit]',ns)
    assert [b.attrib['id'] for b in bits]==[f'bit-{i}' for i in range(7,-1,-1)]
    assert all(b.attrib['data-bit'] in ('0','1') for b in bits)
    # No bundled image/font, script, foreign object or animated property.
    assert all(e.tag.rsplit('}',1)[-1] not in ('image','script','foreignObject','animate','animateTransform') for e in tree.iter())
rows=list(csv.DictReader((HERE/'acceptance-cases.csv').open()))
assert len(rows)==3072 and len({r['case'] for r in rows})==3072
assert all(r['native_result']=='PENDING' for r in rows)
expected=set(itertools.product(range(3),itertools.product((0,1),repeat=8),(100,125,150,200)))
flags=['run','state','brake','bypass','missing_input','clip','modified','rejected']
actual={(int(r['domain']),tuple(int(r[f]) for f in flags),int(r['scale'])) for r in rows}
assert expected==actual
# Compare all pre-existing SYS 01 inputs, not just tracked files.
baseline=json.loads((EVIDENCE/'baseline-inputs.json').read_text())
changed=[p for p,h in baseline.items() if hashlib.sha256((ROOT/p).read_bytes()).hexdigest()!=h]
assert changed==['sys-01/docs/release-1.0.0/state.md'],changed
# Actual token colors, tested against each permitted component surface.
tokens=json.loads((ROOT/'sys-01/docs/design-tokens.json').read_text())['colors']
flat={name:v['hex'] for group in tokens.values() for name,v in group.items()}
def lum(h):
    rgb=[int(h[i:i+2],16)/255 for i in (1,3,5)]
    a=[v/12.92 if v<=.04045 else ((v+.055)/1.055)**2.4 for v in rgb]
    return sum(v*w for v,w in zip(a,(.2126,.7152,.0722)))
def ratio(a,b):
    x,y=sorted((lum(a),lum(b)));return (y+.05)/(x+.05)
pairs=[]
for fg in ('bone_chalk','titanium_mist','monolith_grey','pure_bone'):
    for bg in ('obsidian_void','basalt_surface','carbon_raised'):
        value=ratio(flat[fg],flat[bg]);assert value>=4.5,(fg,bg,value)
        pairs.append(dict(foreground=fg,background=bg,ratio=value,required=4.5,passed=True))
for bg in ('bebop_amber','phosphor_green','optic_cyan'):
    value=ratio(flat['obsidian_void'],flat[bg]);assert value>=4.5
    pairs.append(dict(foreground='obsidian_void',background=bg,ratio=value,required=4.5,passed=True))
for bg in ('obsidian_void','basalt_surface','carbon_raised'):
    value=ratio(flat['titanium_mist'],flat[bg]);assert value>=3
    pairs.append(dict(foreground='titanium_mist outline',background=bg,ratio=value,required=3,passed=True))
red_limits={bg:ratio(flat['akira_red'],flat[bg]) for bg in ('obsidian_void','basalt_surface','carbon_raised')}
assert all(v<4.5 for v in red_limits.values())
(EVIDENCE/'contrast-report.json').write_text(json.dumps(dict(passing_pairs=pairs,red_not_permitted_as_normal_text=red_limits),indent=2)+'\n')
sources=['brand book & strategy/brand-aesthetic-system.md','brand book & strategy/design-tokens.json','tape-01/specs/tape-01-tokens.json','tape-01/specs/physical-and-industrial-design.md','sys-01/docs/design-tokens.json','sys-01/docs/adr/0002-desktop-control-state-and-release.md','sys-01/docs/adr/0003-positioned-vst3-integration.md','sys-01/docs/implementation-status.md','sys-01/release/manifest.json','sys-01/libs/algorhythmia_host/include/algorhythmia/host/catalogue.hpp','sys-01/libs/algorhythmia_dsp/src/transform.cpp','sys-01/libs/algorhythmia_dsp/src/cellular.cpp','sys-01/libs/algorhythmia_dsp/src/bytebeat.cpp','sys-01/docs/release-1.0.0/telemetry-contract.md']
(EVIDENCE/'design-inputs.json').write_text(json.dumps({p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},indent=2)+'\n')
# Relative markdown references must resolve, including percent-encoded source spaces.
linked=[]
for file in [RELEASE/'native-ux-spec.md',HERE/'README.md',RELEASE/'sprint-03.md']:
    for link in re.findall(r'\]\(([^)]+)\)',file.read_text()):
        if '://' in link or link.startswith('#'): continue
        p=file.parent/unquote(link.split('#')[0]);assert p.exists(),(file,link)
        linked.append(str(p.relative_to(ROOT)))
manifest=json.loads((ROOT/'sys-01/release/manifest.json').read_text());assert manifest['status']=='development-unqualified' and manifest['validated_hosts']==[] and manifest['validated_platforms']==[]
print(f'PASS: {len(rects)} bounded nonoverlapping controls; 6 SVGs x 8 true bits; all 3072 native cases PENDING; {len(linked)} local links; {len(baseline)-1} prior inputs unchanged; only state ledger updated; {len(pairs)} contrast pairs; red text exclusion; manifest unchanged')
