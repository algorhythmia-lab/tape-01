#!/usr/bin/env python3
"""Build static, original vector design fixtures; never loads the plugin."""
import csv
import html
import itertools
import json
import math
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
TOKENS = json.loads((ROOT / 'sys-01/docs/design-tokens.json').read_text())['colors']
def token(group, key):
    return TOKENS[group][key]['hex']
C = dict(void=token('substrate', 'obsidian_void'), basalt=token('substrate', 'basalt_surface'),
         carbon=token('substrate', 'carbon_raised'), graphite=token('substrate', 'graphite_border'),
         muted=token('neutrals', 'titanium_mist'), bone=token('neutrals', 'bone_chalk'),
         focus=token('neutrals', 'pure_bone'), amber=token('telemetry', 'bebop_amber'),
         green=token('telemetry', 'phosphor_green'), cyan=token('telemetry', 'optic_cyan'),
         orange=token('telemetry', 'nerv_orange'), red=token('telemetry', 'akira_red'))
SCENES = [
 dict(slug='01-session', domain=0, run=False, state=False, brake=False, bypass=False,
      missing=True, clip=False, modified=False, rejected=False, word=0x00,
      preset='initial / paused', flux='4250 TICKS/S', bias='50.0% MACRO', f=.5, b=.5,
      context='morph / azimuth / drive', health='TELEMETRY LIVE', focus='preset',
      notes=[('SESSION STRIP', 'saved factory name; * reserved for modified targets',
              'UNDO / REDO are parameter-only; scale is presentation'),
             ('TWIN-SPOOL FACE', 'equal FLUX / BIAS controls; no rotating playhead',
              'domain selection is separate from shared binary STATE'),
             ('SIGNAL STRIP', 'known disabled bus: NO INPUT is normal in generation',
              'PAUSED does not promise silence; no missing-input modal'),
             ('DESIGN FIXTURE', '640 x 440 logical units at 100%; no font files bundled',
              'focus shown on preset; values are authored scenario data')]),
 dict(slug='02-performance', domain=1, run=True, state=True, brake=True, bypass=False,
      missing=False, clip=False, modified=True, rejected=True, word=0x6e,
      preset='02 / cellular walls', flux='165.0 BPM', bias='62.5% MACRO', f=.5, b=.625,
      context='rule 110 / coupled FM', health='DISPLAY DROPS 12', focus='flux',
      notes=[('SAVED TARGETS', 'domain 01, DRIVE RUN, STATE 1, BIAS macro 62.5%',
              'brake is transient; it does not enter the saved schema'),
             ('EIGHT TRUE ELEMENTS', '01101110 is the rule byte; left-to-right b7 to b0',
              'register uses actual DSP domain; preview is explicit only'),
             ('BRAKE + REJECTION', 'DSP BRAKE does not erase RUN or REJECTED count',
              'valid edit or reset recovers; no retry of rejected targets'),
             ('MODIFIED + DISPLAY LOSS', '* means changed host targets, not unsaved tape',
              'display drops are independent of rejected audio events')]),
 dict(slug='03-signal', domain=2, run=True, state=True, brake=True, bypass=True,
      missing=True, clip=True, modified=True, rejected=True, word=0x3c,
      preset='03 / forward tape', flux='1.00 X', bias='20.0% MACRO', f=.75, b=.2,
      context='wear / recursion macro', health='TELEMETRY LIVE', focus='bypass',
      notes=[('SIMULTANEOUS CONDITIONS', 'RUN + STATE 1 + BRAKE + BYPASS + NO INPUT',
              'LIMITING + REJECTED + modified * remain independent'),
             ('TRANSFORM TRUTH', '1.00 X is saved base speed; STATE 1 reverses target',
              'BRAKE freezes writing; head can keep reading captured tape'),
             ('SIGNAL AND RECOVERY', 'BYPASS DRY is untrimmed; lower upstream for dry clips',
              'LIMITING may be a 1 s held flag; no automatic reset'),
             ('OPTIONAL METERS', 'DETAILS may show real separate IN/OUT L/R peaks',
              'central 4+4 tape-flux word is not calibrated output DBFS')])
]
GEOMETRY = {
 'details':[540,16,76,28], 'preset':[24,56,240,28], 'undo':[276,56,60,28],
 'redo':[344,56,60,28], 'scale':[520,56,96,28], 'domains':[144,100,352,28],
 'flux':[84,158,128,128], 'bias':[428,158,128,128],
 'flux-entry':[84,288,128,28], 'bias-entry':[428,288,128,28],
 'drive':[48,320,128,28], 'brake':[256,320,128,28], 'state':[464,320,128,28],
 'output-slider':[80,364,212,28], 'output-entry':[300,364,80,28],
 'bypass':[392,364,104,28], 'reset':[512,364,104,28]}

def rect(x,y,w,h,fill,stroke=None,extra=''):
    return f'<rect x="{x}" y="{y}" width="{w}" height="{h}" fill="{fill}"' + (f' stroke="{stroke}"' if stroke else '') + f' {extra}/>'

def text(label,x,y,w,h,size=14,color=None,anchor='middle',ident=None):
    # Store the intended text box for actual glyph-bound verification after rendering.
    xx = x+w/2 if anchor=='middle' else x
    attrs = f' data-box="{x},{y},{w},{h}"' + (f' id="{ident}"' if ident else '')
    return f'<text{attrs} x="{xx}" y="{y+h/2}" font-size="{size}" fill="{color or C["bone"]}" text-anchor="{anchor}" dominant-baseline="central">{html.escape(label)}</text>'

def button(ident,label,selected=False,focus=False,disabled=False):
    x,y,w,h=GEOMETRY[ident]
    s=rect(x,y,w,h,C['carbon'] if selected else C['basalt'],C['muted'])
    if selected:
        s+=rect(x+8,y+h-4,w-16,1,C['bone'])
    if disabled:
        s+=f'<path d="M {x+4} {y+h-4} l 5 -5" stroke="{C["muted"]}"/>'
    s+=text(label,x+5,y,w-10,h,14,C['muted'] if disabled else C['bone'],ident=ident+'-label')
    if focus:
        s+=rect(x-4,y-4,w+8,h+8,'none',C['focus'],'stroke-width="2"')
    return s

def face(scene):
    accent=[C['amber'],C['green'],C['cyan']][scene['domain']]
    s=rect(0,0,640,440,C['void'])
    s+=text('algorhythmia // SYS 01',24,16,392,24,20,anchor='start',ident='identity')
    s+=text('FREE CLOCK',424,16,104,24,12,C['muted'],ident='clock')
    s+=button('details','DETAILS')
    s+=button('preset',scene['preset']+(' *' if scene['modified'] else ''),focus=scene['focus']=='preset')
    s+=button('undo','UNDO',disabled=not scene['modified'])+button('redo','REDO',disabled=True)+button('scale','100%')
    for i,label in enumerate(('00 BYTEBEAT','01 CELLULAR','02 TRANSFORM')):
        x=144+i*352/3; w=352/3
        s+=rect(x,100,w,28,C['carbon'] if i==scene['domain'] else C['basalt'],C['muted'])
        s+=text(label,x+3,100,w-6,28,14,ident='domain-'+str(i))
        if i==scene['domain']: s+=rect(x+8,124,w-16,2,accent)
    for ident,label,val,v in [('flux','FLUX',scene['flux'],scene['f']),('bias','BIAS',scene['bias'],scene['b'])]:
        x,y,w,h=GEOMETRY[ident]; cx=x+w/2; cy=y+h/2
        s+=text(label,x,140,w,18,14,ident=ident+'-title')
        s+=f'<circle cx="{cx}" cy="{cy}" r="56" fill="{C["carbon"]}" stroke="{C["muted"]}"/>'
        s+=f'<circle cx="{cx}" cy="{cy}" r="42" fill="{C["basalt"]}" stroke="{C["graphite"]}"/>'
        for n in range(16):
            a=math.radians(-225+n*18); px=cx+62*math.cos(a); py=cy+62*math.sin(a)
            s+=f'<circle cx="{px:.3f}" cy="{py:.3f}" r="1.6" fill="{C["bone"] if n<=round(v*15) else C["graphite"]}"/>'
        a=math.radians(-225+270*v)
        s+=f'<path d="M {cx+33*math.cos(a):.3f} {cy+33*math.sin(a):.3f} L {cx+51*math.cos(a):.3f} {cy+51*math.sin(a):.3f}" stroke="{C["bone"]}" stroke-width="3"/>'
        s+=f'<circle cx="{cx}" cy="{cy}" r="5" fill="none" stroke="{C["muted"]}"/>'
        if scene['focus']==ident: s+=rect(x-4,y-4,w+8,h+8,'none',C['focus'],'stroke-width="2"')
        ex,ey,ew,eh=GEOMETRY[ident+'-entry']; s+=rect(ex,ey,ew,eh,C['basalt'],C['muted'])
        s+=text(val,ex+4,ey,ew-8,eh,16,ident=ident+'-value')
    caption=['WORD','RULE','TAPE FLUX L/R'][scene['domain']]
    s+=text(caption,238,176,164,20,12,C['muted'],ident='register-caption')
    s+='<g id="register">'+rect(258,202,124,40,C['basalt'],C['graphite'])
    for i in range(8):
        bit=(scene['word']>>(7-i))&1; x=266+i*14
        s+=rect(x,210,10,24,accent if bit else C['basalt'],accent if bit else C['muted'],f'id="bit-{7-i}" data-bit="{bit}"')
        s+=text(str(bit),x,210,10,24,12,C['void'] if bit else C['muted'],ident=f'bit-text-{7-i}')
    s+='</g>'
    # Context is secondary and has fixed short copy; detailed mapping remains in DETAILS.
    ctx=['morph / azimuth','rule 110 / FM','wear / recursion'][scene['domain']]
    s+=text(ctx,238,250,164,22,12,C['muted'],ident='macro-context')
    s+=text('DSP BRAKE' if scene['brake'] else ('DSP RUN' if scene['run'] else 'DSP PAUSED'),238,276,164,22,12,C['bone'],ident='dsp-context')
    s+=button('drive','DRIVE RUN' if scene['run'] else 'DRIVE PAUSED',scene['run'])
    s+=button('brake','BRAKE ON' if scene['brake'] else 'BRAKE OFF',scene['brake'])
    s+=button('state','STATE 1' if scene['state'] else 'STATE 0',scene['state'])
    s+=rect(24,356,592,1,C['graphite'])+text('OUTPUT',24,364,52,28,12,ident='output-title')
    s+=rect(80,377,212,2,C['muted'])+rect(80+212*.75-3,368,6,20,C['bone'])
    s+=rect(300,364,80,28,C['basalt'],C['muted'])+text('0.0 DB',304,364,72,28,16,ident='output-value')
    s+=button('bypass','BYPASS DRY' if scene['bypass'] else 'BYPASS OFF',scene['bypass'],scene['focus']=='bypass')
    s+=button('reset','RESET')
    s+=text('NO INPUT' if scene['missing'] else 'INPUT ACTIVE',24,400,188,18,12,anchor='start',ident='input-status')
    s+=text('LIMITING' if scene['clip'] else 'LIMIT --',224,400,104,18,12,anchor='start',ident='limit-status')
    s+=text('REJECTED 4294967295' if scene['rejected'] else 'REJECTED 0',344,400,272,18,12,anchor='start',ident='rejection-status')
    if scene['clip']: s+=rect(324,405,4,6,C['red'])
    if scene['rejected']: s+=rect(612,405,4,6,C['orange'])
    s+=text('parameter recall; reset clears audio',24,420,304,18,12,C['muted'],anchor='start',ident='recall-hint')
    s+=text(scene['health'],344,420,272,18,12,C['muted'],anchor='start',ident='health-status')
    return s

def document(body,w,h,title):
    return f'<svg xmlns="http://www.w3.org/2000/svg" width="{w}" height="{h}" viewBox="0 0 {w} {h}" font-family="sans-serif"><title>{html.escape(title)}</title><desc>static authored design fixture; not a native plugin capture. no embedded fonts.</desc>{body}</svg>\n'

for scene in SCENES:
    body=face(scene)
    (HERE/(scene['slug']+'.svg')).write_text(document(body,640,440,scene['slug']+' // SYS 01 design fixture'))
    board=rect(0,0,1180,590,C['void'])
    board+=text('SYS 01 // '+scene['slug'].upper()+' // ANNOTATED CONTRACT',24,16,1132,30,20,anchor='start')
    board+=text('design fixture / not a native capture / original vectors / system font resolution',24,50,1132,24,14,C['muted'],anchor='start')
    board+='<g transform="translate(24 96)">'+body+'</g>'
    board+=rect(24,96,640,440,'none',C['muted'])
    board+=text('100% / 640 x 440 / uniform 125%, 150%, 200% / min unchanged',24,548,640,24,14,C['muted'],anchor='start')
    for i,(heading,line1,line2) in enumerate(scene['notes']):
        y=102+i*110
        board+=text(f'{i+1:02d} // {heading}',700,y,450,24,16,anchor='start')
        board+=rect(700,y+29,450,1,C['graphite'])
        board+=text(line1,700,y+38,450,22,14,C['muted'],anchor='start')
        board+=text(line2,700,y+62,450,22,14,C['muted'],anchor='start')
    (HERE/(scene['slug']+'-annotated.svg')).write_text(document(board,1180,590,scene['slug']+' annotated contract'))

(HERE/'layout-contract.json').write_text(json.dumps(dict(base=[640,440],scales=[1,1.25,1.5,2],geometry=GEOMETRY,scenes=SCENES),indent=2)+'\n')
flags=['run','state','brake','bypass','missing_input','clip','modified','rejected']
with (HERE/'acceptance-cases.csv').open('w') as f:
    writer=csv.writer(f);writer.writerow(['case','domain',*flags,'scale','native_result'])
    for n,(domain,values,scale) in enumerate(itertools.product(range(3),itertools.product((0,1),repeat=8),(100,125,150,200)),1):
        writer.writerow([f'UX-04-{n:04d}',domain,*values,scale,'PENDING'])
print('generated 3 faces + 3 annotated vector boards, geometry/scene contract, 3072 native PENDING scenarios')
