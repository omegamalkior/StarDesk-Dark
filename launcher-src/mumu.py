import json,re,colorsys
def lum(r,g,b): return r*0.299+g*0.587+b*0.114
def is_blue(r,g,b):
    h,l,s=colorsys.rgb_to_hls(r/255,g/255,b/255); return (max(r,g,b)-min(r,g,b))>=45 and 0.55<=h<=0.68
def cyan(r,g,b):
    h,l,s=colorsys.rgb_to_hls(r/255,g/255,b/255)
    if l<0.30:  # dark blue tints -> dark cyan tints
        rr,gg,bb=colorsys.hls_to_rgb(194/360,l*0.9,s*0.55)
    else:
        rr,gg,bb=colorsys.hls_to_rgb(194/360,min(0.85,l*0.88),1.0)
    return (round(rr*255),round(gg*255),round(bb*255))
def gray_fill(L): v=round(max(30,min(130,50+(L-45)*2.4))); return (v,v,v)
def gray_border(L): v=round(max(50,min(110,60+(L-45)*1.3))); return (v,v,v)
def mumu_rgb(r,g,b,kind='fill'):
    L=lum(r,g,b); sat=max(r,g,b)-min(r,g,b)
    if is_blue(r,g,b): return cyan(r,g,b)
    if sat<45 and 24<=L<110: return gray_border(L) if kind=='border' else gray_fill(L)
    return (r,g,b)
# ---- table ----
D=json.load(open('dark.json'))
special={'kBorderGray240':(36,36,36),'kFillWindowPanel':(50,50,50),'kFillPanelBackground':(55,55,55),'kFillSolidWhite':(70,70,70)}
M=[]
for n,o,c in D:
    r,g,b,a=c
    if n in special: new=special[n]
    elif n.startswith('kText'): new=cyan(r,g,b) if is_blue(r,g,b) else (r,g,b)
    elif r==g==b==255 or r==g==b==0: new=(r,g,b)
    else: new=mumu_rgb(r,g,b,'border' if n.startswith('kBorder') else 'fill')
    # OFFSWITCH: switch outline/knob and scrollbar handles use these; light so they show on dark (like MuMu)
    if n in ('kFillBlackAlpha76','kFillBlackAlpha78'): new,a=(232,232,232),200
    # kFillBlackAlpha77 is the see-through layer message boxes and modal frames paint over the window behind them
    if n=='kFillBlackAlpha77': new,a=(0,0,0),128
    # the device status pill ("Ready") sits on this one with white text: keep it a dark translucent pill
    if n=='kFillBlackAlpha89': new,a=(86,86,86),255   # status pill ("Ready"): Windows-style subtle fill, white text
    if n in ('kFillBlackAlpha64','kFillBlackAlpha115'): new=(255,255,255)
    # remote-session title bar: StarDesk lays this 25% overlay over the window colour. On dark it only made
    # the bar a lighter grey than the main window's title bar, so let the window colour show through.
    if n=='kFillBlackAlpha64': a=0
    # the dimmed backdrop behind dialogs: a dark see-through layer (Windows' smoke layer), not a light one
    if n=='kBorderBlackAlpha204': new,a=(0,0,0),128
    if n=='kFillCoolGray': new=(50,50,50)   # remote-session tab bar background (MuMuPlayer tab bar)
    if n=='kFillDarkGray25': new,a=(78,78,78),255   # the "Ready" status pill (white text on it): dark grey
    M.append([n,o,[*new,a]])
json.dump(M,open('mumu.json','w'))
for (n,o,c),(_,_,m) in zip(D,M):
    if c!=m and n in ('kFillBrand','kFillBrandHover','kTextBrand','kBorderBrand','kFillBrandLight','kFillWindowPanel','kFillSolidWhite','kBorderWhite','kBorderGray240','kFillGray240','kTextInversePrimary','kBorderSelectedBlue','kFillBrandAlpha204'):
        print(f"{n:24s}{tuple(c)} -> {tuple(m)}")
