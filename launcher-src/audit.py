# WCAG 2.x contrast audit of the Dark / AMOLED palettes against the surfaces text actually sits on.
import json,sys
def lin(c):
    c/=255; return c/12.92 if c<=0.03928 else ((c+0.055)/1.055)**2.4
def L(rgb): r,g,b=rgb; return 0.2126*lin(r)+0.7152*lin(g)+0.0722*lin(b)
def cr(a,b):
    la,lb=L(a),L(b); return (max(la,lb)+0.05)/(min(la,lb)+0.05)
def over(fg,bg):
    r,g,b,a=fg; t=a/255; return tuple(round(fg[i]*t+bg[i]*(1-t)) for i in range(3))
TINT=(255,255,255,13)            # the faint whole-window tint StarDesk lays on top (~5%)
def seen(c): return over(TINT,c)
def surfaces(P):
    g=lambda n: tuple(P[n][:3])
    return {'sidebar':seen(g('kBorderGray240')),'window':seen(g('kFillWindowPanel')),'panel':seen(g('kFillPanelBackground')),'card':seen(g('kFillSolidWhite'))}
def audit(path,label,report=True):
    P={n:c for n,o,c in json.load(open(path))}
    S=surfaces(P); fails=[]
    for n,c in P.items():
        if not n.startswith('kText') or c[3]==0: continue
        if any(k in n for k in ('Inverse','WhiteAlpha','TooltipYellow','BrandLight')): continue   # text on photos / on accent / tooltips
        if n in ('kTextGray208','kTextGray233'): continue   # near-white in the light theme: StarDesk uses these as pill/badge backgrounds, not text
        worst=min((cr(over(c,s),s),k) for k,s in S.items())
        need=3.0 if n in ('kTextBlackAlpha89','kTextBlackAlpha77','kTextGray208','kTextGray233') else 4.5   # disabled-style text: WCAG exempts, keep >=3 anyway
        if worst[0]<need: fails.append((n,c,round(worst[0],2),worst[1],need))
    if report:
        print(label,'surfaces as seen:',S)
        for f in fails: print('  FAIL',f)
        print(' ',len(fails),'text colours below target')
    return fails
if __name__=='__main__':
    audit('mumu.json','DARK'); audit('amoled.json','AMOLED')
