import json, re, struct
def lum(r,g,b): return r*0.299+g*0.587+b*0.114
def amo(r,g,b,factor=0.55):
    """dark surface -> near-black; neutral if it was only slightly tinted, keep hue if clearly coloured"""
    L=lum(r,g,b)
    sat=max(r,g,b)-min(r,g,b)
    if L>=110 or (sat>=45 and L>=60): return (r,g,b)   # light colours and accents stay
    v=max(0.0,(L-28)*factor)
    if sat<45: return (round(v),)*3
    k=v/max(L,1); return (round(r*k),round(g*k),round(b*k))
# ---- table ----
D=json.load(open('dark.json'))
out=[]
for n,old,c in D:
    r,g,b,a=c
    if n in ('kFillWindowPanel','kFillPanelBackground','kFillGray249','kFillGray249Blue','kFillBlueGray248','kFillBlueGray249','kFillBlueGray248Alt','kFillGray252','kFillAlmostWhite'):
        new=(0,0,0)
    elif n=='kFillSolidWhite': new=(12,12,12)
    elif n.startswith('kBorder') and not (r==g==b==255): new=amo(r,g,b,0.75)
    elif n.startswith('kShadow') and lum(r,g,b)<60: new=(0,0,0)
    elif n.startswith('kText') or r==g==b==255 or (r==g==b==0): new=(r,g,b)          # white/black alpha overlays unchanged
    else: new=amo(r,g,b)
    # OFFSWITCH: same as Dark - switch outline/knob and scrollbar handles light so they show on black
    if n in ('kFillBlackAlpha76','kFillBlackAlpha78'): new,a=(232,232,232),200
    # kFillBlackAlpha77 is the see-through layer message boxes and modal frames paint over the window behind them
    if n=='kFillBlackAlpha77': new,a=(0,0,0),128
    if n=='kFillBlackAlpha89': new,a=(40,40,40),255   # status pill ("Ready"): subtle fill, white text
    if n in ('kFillBlackAlpha64','kFillBlackAlpha115'): new=(255,255,255)
    # remote-session title bar: StarDesk lays this 25% overlay over the window colour. On dark it only made
    # the bar a lighter grey than the main window's title bar, so let the window colour show through.
    if n=='kFillBlackAlpha64': a=0
    # the dimmed backdrop behind dialogs: a dark see-through layer (Windows' smoke layer), not a light one
    if n=='kBorderBlackAlpha204': new,a=(0,0,0),128
    if n=='kFillCoolGray': new=(8,8,8)   # remote-session tab bar background (MuMuPlayer tab bar)
    if n=='kFillDarkGray25': new,a=(40,40,40),255   # the "Ready" status pill (white text on it): dark grey
    out.append([n,old,[*new,a]])
json.dump(out,open('amoled.json','w'))
dd={x[0]:x[2] for x in D}
for n,o,c in out:
    if c!=dd[n]: print(f"{n:28s} dark{tuple(dd[n])} -> amoled{tuple(c)}")
