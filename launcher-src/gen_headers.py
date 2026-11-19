import json, re
exec(open('amoled.py').read().split('# ---- table ----')[0])   # amo()
# ---- palette header (dark + amoled tables) ----
D=json.load(open('mumu.json')); A=json.load(open('amoled.json'))
def u_argb(c): return (c[0]<<24)|(c[1]<<16)|(c[2]<<8)|c[3]
def u_rgba(c): return (c[3]<<24)|(c[0]<<16)|(c[1]<<8)|c[2]
with open('palette.h','w') as f:
    f.write('#define TOKEN_COUNT %d\n'%len(D))
    f.write('static const unsigned int kLight[TOKEN_COUNT]={'+','.join('0x%08Xu'%u_argb(o) for n,o,c in D)+'};\n')
    f.write('static const unsigned int kDark[TOKEN_COUNT]={'+','.join('0x%08Xu'%u_rgba(c) for n,o,c in D)+'};\n')
    f.write('static const unsigned int kAmoled[TOKEN_COUNT]={'+','.join('0x%08Xu'%u_rgba(c) for n,o,c in A)+'};\n')
# ---- pattern header (dark + amoled replacements) ----
def blacken(b):
    def rep(m):
        h=m.group(1).decode(); 
        if len(h)==3: r,g,bb=(int(x,16)*17 for x in h); nr=amo(r,g,bb); return ('#'+''.join('%X'%round(v/17) for v in nr)).encode()
        a=h[:2] if len(h)==8 else ''; rgb=h[-6:]
        r,g,bb=int(rgb[0:2],16),int(rgb[2:4],16),int(rgb[4:6],16)
        nr=amo(r,g,bb); return ('#'+a+'%02X%02X%02X'%nr).encode()
    return re.sub(rb'#([0-9A-Fa-f]{8}|[0-9A-Fa-f]{6}|[0-9A-Fa-f]{3})(?![0-9A-Fa-f])',rep,b)
SD=json.load(open('strpatch_dark.json')); MU=json.load(open('strpatch_mumu.json'))
S=[(o.encode(),n.encode()) for o,n in SD]
MUN={o.encode():n.encode('latin1') for o,n,_ in MU['base']}
I=[(bytes.fromhex(o),bytes.fromhex(n)) for o,n in json.load(open('iconpatch.json'))]
import os
P=[(o,MUN[o],blacken(n)) for o,n in S]+[(o,n,n) for o,n in I]
P+=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(o)) for o,n in MU['extra']]
if os.environ.get('WITH_Z') and os.path.exists('zpatch.json'):
    P+=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in json.load(open('zpatch.json'))]
    P+=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in json.load(open('splashpatch.json'))]
    P+=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in json.load(open('boxpatch.json'))]
    P+=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in json.load(open('logopatch.json'))]
    P+=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in json.load(open('icopatch.json'))]
    P+=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in json.load(open('modalpatch.json'))]
    P+=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in json.load(open('qrlogopatch.json'))]
    P+=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in json.load(open('bgpatch.json'))]
    P+=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in json.load(open('namelogopatch.json'))]
    P+=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in json.load(open('starfix.json'))]
    # full audit: icons the generic SVG rules got wrong (keepfix.py) and light discs/panels (discpatch.py)
    P+=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in json.load(open('keepfix.json'))]
    P+=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in json.load(open('discpatch.json'))]
    # Delete Account agreement text (an HTML page shown in a now-dark text box): light text
    _h=b'color: #0c0c0c; margin: 0;'; P.append((_h,b'color: #E6E6E6; margin: 0;',b'color: #E6E6E6; margin: 0;'))
    # UTF-16 stylesheets (Get Started / sign-in, Delete Account, icon+text buttons) - see wstrpatch.py
    P+=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in json.load(open('wstrpatch.json'))]
# TEXT_TOKEN_SWAPS: some stylesheets colour text with a *fill* token (black overlay). In dark mode those
# stay dark, so the text vanishes (e.g. the verification code). Point them at the matching text token.
for sp in ('color: ','color:'):
    for a in ('230','255','102'):
        o=(sp+'@ThemeColor(kFillBlackAlpha'+a+')').encode(); n=(sp+'@ThemeColor(kTextBlackAlpha'+a+')').encode()
        P.append((o,n,n))
# SCROLLBAR_HANDLE: the thin scrollbars used kFillBlackAlpha64, which is now see-through (remote-session
# title bar); point them at kFillBlackAlpha56 (white 22%) so the handle still shows.
_o=b'background:@ThemeColor(kFillBlackAlpha64)'; _n=b'background:@ThemeColor(kFillBlackAlpha56)'
P.append((_o,_n,_n))
# TEXT_TIERS: hard-coded stylesheet text colours get the same accessible levels as the palette
import re as _re
_TEXT_ALPHAS={'89':113,'153':197,'156':197,'230':255,'0.6':197,'0.9':255}
def _tier(o,n):
    m=_re.fullmatch(rb'rgba\(0, ?0, ?0, ?([0-9.]+)\)',o)
    if not m or m.group(1).decode() not in _TEXT_ALPHAS: return n
    want=_TEXT_ALPHAS[m.group(1).decode()]
    h=_re.match(rb'#([0-9A-F]{2})FFFFFF',n)
    if not h or int(h.group(1),16)>=want: return n
    return n.replace(h.group(0),('#%02XFFFFFF'%want).encode(),1)
P=[(o,_tier(o,n),_tier(o,a)) for o,n,a in P]
# ICON_GLYPH_FIX: pale glyphs drawn ON the cyan icons (the star in the Favorite Device bookmark, a line
# in another icon) must stay light - they aren't backgrounds.
_GLYPH={b'fill="#DEF0FF"':b'fill="#FFFFFF"',b'stroke="#DEF0FF"':b'stroke="#FFFFFF"'}
P=[(o,_GLYPH.get(o,n),_GLYPH.get(o,a)) for o,n,a in P]
P.sort(key=lambda t:(t[0][:2],-len(t[0]),t[0]))   # same first bytes: longest pattern first (a whole-icon fix beats a shorter one)
for o,n,a in P: assert len(o)==len(n)==len(a)
def carr(name,b): return 'static const unsigned char %s[%d]={%s};\n'%(name,len(b),','.join(str(c) for c in b))
with open('strpatch.h','w') as f:
    f.write('#define NPAT %d\n#define MAXPAT %d\n'%(len(P),max(len(o) for o,n,a in P)))
    for i,(o,n,a) in enumerate(P):
        f.write(carr('po%d'%i,o)); f.write(carr('pn%d'%i,n))
        if a!=n: f.write(carr('pa%d'%i,a))
    f.write('static const unsigned int kPatLen[NPAT]={'+','.join(str(len(o)) for o,n,a in P)+'};\n')
    f.write('static const unsigned char *const kPatOld[NPAT]={'+','.join('po%d'%i for i in range(len(P)))+'};\n')
    f.write('static const unsigned char *const kPatNew[NPAT]={'+','.join('pn%d'%i for i in range(len(P)))+'};\n')
    f.write('static const unsigned char *const kPatNewA[NPAT]={'+','.join(('pa%d' if a!=n else 'pn%d')%i for i,(o,n,a) in enumerate(P))+'};\n')
json.dump([[o.hex(),n.hex(),a.hex()] for o,n,a in P],open('allpatch.json','w'))
ch=[(o,n,a) for o,n,a in P if a!=n]
print('patterns',len(P),'with different AMOLED version:',len(ch))
for o,n,a in ch[:40]:
    if len(o)<60: print(' ',n.decode(),'->',a.decode())

# ACCENT_FIXUPS: where the accent colour appears, and which Windows accent shade replaces it
# shade: 0 Light3, 1 Light2, 2 Light1, 3 Base, 7 = dark accent tint (backgrounds)
import colorsys as _cs, re as _re2
def _shade(r,g,b,theme):
    h,l,s=_cs.rgb_to_hls(r/255,g/255,b/255)
    if theme==0:   # Dark: MuMu cyan family
        if not (0.52<=h<=0.56 and s>=0.5): return -1
        cuts=(0.62,0.5,0.4,0.3)
    else:          # AMOLED: original StarDesk blue family
        if not (0.55<=h<=0.68 and (max(r,g,b)-min(r,g,b))>=45): return -1
        cuts=(0.66,0.55,0.42,0.3)
    if l>=cuts[0]: return 0
    if l>=cuts[1]: return 1
    if l>=cuts[2]: return 2
    if l>=cuts[3]: return 3
    return 7
def _table_shades(rows,theme):
    out=[]
    for n,o,c in rows:
        sh=_shade(c[0],c[1],c[2],theme)
        if sh>=0 and n.startswith('kText'): sh=0          # accent text always uses the lightest, most readable shade
        # BUTTON_CODES: 10/11/12 = button rest/hover/pressed: the launcher picks the accent shade white text reads on
        from contrast import BUTTON_REST,BUTTON_HOVER,BUTTON_PRESSED
        # Windows dark theme: accent button = Light2, hover = Light1, pressed = Base (AccentFillColorDefault/Secondary/Tertiary)
        if n in BUTTON_REST: sh=1
        elif n in BUTTON_HOVER: sh=2
        elif n in BUTTON_PRESSED: sh=3
        out.append(sh)
    return out
fix=[]
for i,(o,n,a) in enumerate(P):
    for theme,b in ((0,n),(1,a)):
        if not all(32<=c<127 or c in (9,10,13) for c in b): continue
        for m in _re2.finditer(rb'#([0-9A-Fa-f]{8}|[0-9A-Fa-f]{6})(?![0-9A-Fa-f])',b):
            hx=m.group(1); off=m.start(1)+(2 if len(hx)==8 else 0); rgb=hx[-6:]
            sh=_shade(int(rgb[0:2],16),int(rgb[2:4],16),int(rgb[4:6],16),theme)
            if sh>=0: fix.append((i,off,sh,theme))
with open('accent.h','w') as f:
    f.write('static const signed char kAccentShadeDark[TOKEN_COUNT]={%s};\n'%','.join(str(x) for x in _table_shades(D,0)))
    f.write('static const signed char kAccentShadeAmoled[TOKEN_COUNT]={%s};\n'%','.join(str(x) for x in _table_shades(A,1)))
    lin=lambda c:(c/255/12.92 if c/255<=0.03928 else ((c/255+0.055)/1.055)**2.4)
    f.write('static const unsigned int kLin[256]={%s};\n'%','.join(str(round(lin(c)*1000000)) for c in range(256)))
    f.write('#define NACCFIX %d\n'%len(fix))
    f.write('typedef struct { unsigned short pat, off; unsigned char shade, theme; } AccentFix;\n')
    f.write('static const AccentFix kAccentFix[NACCFIX>0?NACCFIX:1]={%s};\n'%','.join('{%d,%d,%d,%d}'%x for x in fix))
print('accent locations: table dark',sum(1 for x in _table_shades(D,0) if x>=0),'amoled',sum(1 for x in _table_shades(A,1) if x>=0),'| in styles/artwork',len(fix))
