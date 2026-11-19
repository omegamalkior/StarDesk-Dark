# UTF-16 stylesheets. The newer screens (Get Started / sign-in, Delete Account, the icon+text buttons)
# build their stylesheets from QStringLiterals: UTF-16 text in .data, which the UTF-8 rules in strpatch.py
# never see. Same idea here: black text -> white text at readable levels, white boxes -> dark boxes,
# StarDesk blue -> the theme's accent colours. Replacements keep the exact length (padded with spaces).
import re, json
d = open('stardesk/bin/StarDesk.exe', 'rb').read()
DATA = d[0x6a6d000:0x6a6d000 + 0x30f600]          # .data (raw)
M = {n: c for n, o, c in json.load(open('mumu.json'))}
A = {n: c for n, o, c in json.load(open('amoled.json'))}

def hx(c, a=255):
    return '#%02X%02X%02X' % tuple(c[:3]) if a == 255 else '#%02X%02X%02X%02X' % (a, c[0], c[1], c[2])

# surfaces (Dark, AMOLED). The dialogs these sit in are kFillSolidWhite (70 / 12).
INPUT = ((54, 54, 54), (5, 5, 5))          # text boxes: a step darker than the dialog (like MuMu)
BTN = ((81, 81, 81), (30, 30, 30))         # Windows dark ControlFillColorDefault over the dialog
BTN_HOVER = ((86, 86, 86), (38, 38, 38))   # ControlFillColorSecondary
BTN_DOWN = ((76, 76, 76), (22, 22, 22))    # ControlFillColorTertiary / disabled
TEXT2 = ((186, 188, 192), (186, 188, 192))  # secondary grey text (#606266 on light)
TEXT_ALPHA = {'0.9': 255, '0.6': 197, '153': 197, '0.45': 150, '0.35': 112}   # same tiers as the palette

def brand(name):
    return (M[name][:3], A[name][:3])

rules = []   # (regex on the decoded string, function(match) -> (dark, amoled) replacement texts)

def rgba_black(m):
    prop, val = m.group(1), m.group(2)
    a = val.split(',')[-1].strip().rstrip(')').strip()
    alpha = round(float(a) * 255) if '.' in a else int(a)
    if prop.lower().startswith('color'):
        alpha = TEXT_ALPHA.get(a, alpha)
    w = hx((255, 255, 255), alpha) if alpha else 'transparent'
    return w, w

def white_bg(m):
    prop, val = m.group(1), m.group(2)
    return hx(INPUT[0]), hx(INPUT[1])

SURF = {  # exact light values -> (dark, amoled)
    'rgba(255,255,255,255)': (hx(BTN[0]), hx(BTN[1])),
    'rgba(238,238,238,255)': (hx(BTN_DOWN[0]), hx(BTN_DOWN[1])),
    'rgba(234,234,234,204)': (hx(BTN_DOWN[0], 204), hx(BTN_DOWN[1], 204)),
    'rgba(249, 249, 249, 0.6)': (hx(INPUT[0], 153), hx(INPUT[1], 153)),
    'rgba(117,117,115,115)': (hx((255, 255, 255), 40), hx((255, 255, 255), 40)),
    'rgba(11,102,221,77)': (hx(M['kTextBrand'], 128), hx(A['kTextBrand'], 128)),
    'rgba(0,122,255,255)': (hx(brand('kFillBrand')[0]), hx(brand('kFillBrand')[1])),
    'rgba(0,112,244,255)': (hx(brand('kFillBrandHover')[0]), hx(brand('kFillBrandHover')[1])),
    'rgba(0,112,244,204)': (hx(brand('kFillBrandHover')[0], 204), hx(brand('kFillBrandHover')[1], 204)),
    '#0A80FF': (hx(brand('kFillBrand')[0]), hx(brand('kFillBrand')[1])),
}
TEXT = {  # text colours (after "color:")
    '#3A7BFC': brand('kTextBrand'), '#0A80FF': brand('kTextBrand'),
    '#275BD6': brand('kTextBrandHover'), '#1740B0': brand('kTextBrandPressed'),
    '#606266': TEXT2, '#C94032': ((250, 173, 165), (245, 105, 90)),
}

out = {}
def add(old, dark, amo):
    assert len(dark) <= len(old) and len(amo) <= len(old), (old, dark, amo)
    dark, amo = dark.ljust(len(old)), amo.ljust(len(old))
    o = old.encode('utf-16-le')
    if o in DATA and dark != old:
        out[o] = (dark.encode('utf-16-le'), amo.encode('utf-16-le'))

strs = set(m.group().decode('utf-16-le') for m in re.finditer(rb'(?:[\x09\x0a\x0d\x20-\x7e]\x00){6,}', DATA))
for s in strs:
    # black (any alpha) -> white; text gets the readable tiers
    for m in re.finditer(r'(color|background(?:-color)?|border(?:-[a-z]+)?)(\s*:\s*(?:\d+px\s+solid\s+)?rgba\(\s*0\s*,\s*0\s*,\s*0\s*,\s*[0-9.]+\s*\))', s, re.I):
        whole = m.group(0); val = re.search(r'rgba\([^)]*\)', whole).group(0)
        a = val[5:-1].split(',')[-1].strip()
        alpha = round(float(a) * 255) if '.' in a else int(a)
        if m.group(1).lower() == 'color':
            alpha = TEXT_ALPHA.get(a, alpha)
        new = hx((255, 255, 255), alpha) if alpha else 'transparent'
        add(whole, whole.replace(val, new), whole.replace(val, new))
    # white text boxes
    for m in re.finditer(r'background(?:-color)?\s*:\s*(#FFFFFF|rgb\(255, 255, 255\))', s, re.I):
        whole = m.group(0); val = m.group(1)
        add(whole, whole.replace(val, hx(INPUT[0])), whole.replace(val, hx(INPUT[1])))
    for val, (dk, am) in SURF.items():
        for m in re.finditer(r'(?:background(?:-color)?\s*:\s*|solid\s+)' + re.escape(val), s):
            whole = m.group(0); add(whole, whole.replace(val, dk), whole.replace(val, am))
    for val, (dk, am) in TEXT.items():
        for m in re.finditer(r'color\s*:\s*' + re.escape(val), s):
            whole = m.group(0)
            if whole.lower().startswith('background'): continue
            add(whole, whole.replace(val, hx(dk)), whole.replace(val, hx(am)))
        for m in re.finditer(r'border(?:-[a-z]+)?\s*:\s*1px solid ' + re.escape(val), s):
            whole = m.group(0); add(whole, whole.replace(val, hx(dk)), whole.replace(val, hx(am)))

# icon+text buttons: hover a step lighter than rest (both are plain white in the light theme)
add('hover"]{background-color:rgba(255,255,255,255)', 'hover"]{background-color:'+hx(BTN_HOVER[0]), 'hover"]{background-color:'+hx(BTN_HOVER[1]))

# Contact us icons: for StarDesk's own image CDN the client asks the server for a resized copy
# ("?fop=imageView/..."). Make that a URL fragment ("#fop=...") so the original image is downloaded
# (Qt scales it smoothly itself). Not a colour change: applies to both dark themes.
_fop='?fop=imageView/1/w/%1/h/%2/q/95'
add(_fop, '#'+_fop[1:], '#'+_fop[1:])

# the "color:" rule also matched inside "background-color:" - drop those (handled by SURF / black rules)
res = []
for o, (n, a) in sorted(out.items()):
    t = o.decode('utf-16-le')
    print(DATA.count(o), repr(t), '->', repr(n.decode('utf-16-le')), '|', repr(a.decode('utf-16-le')))
    res.append([o.hex(), n.hex(), a.hex()])
json.dump(res, open('wstrpatch.json', 'w'))
print('wide patterns', len(res))
