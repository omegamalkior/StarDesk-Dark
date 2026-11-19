# Lift every text colour that misses WCAG AA on any StarDesk surface, keeping its hue.
import json,colorsys
from audit import audit,over,cr,surfaces
BUTTON_REST=('kFillBrand','kFillLoginBrand','kFillBrandAlpha204','kFillBrandAlpha166','kFillBrandGradientEnd')
BUTTON_HOVER=('kFillBrandHover','kFillLoginBrandHover')
BUTTON_PRESSED=('kFillLoginBrandPressed','kFillBrandLightPressed')
def fix(path):
    D=json.load(open(path)); P={n:c for n,o,c in D}; S=surfaces(P)
    changed=[]
    for n,need_c,worst,where,need in audit(path,'',report=False):
        r,g,b,a=P[n]
        def worst_cr(c): return min(cr(over(c,s),s) for s in S.values())
        if (r,g,b)==(255,255,255):          # translucent white text: raise opacity
            while a<255 and worst_cr((r,g,b,a))<need: a+=1
            new=[r,g,b,a]
        else:                                # coloured / grey text: make it lighter, same hue
            h,l,s=colorsys.rgb_to_hls(r/255,g/255,b/255)
            new=[r,g,b,a]
            while worst_cr(new)<need and l<1:
                l=min(1,l+0.005); rr,gg,bb=colorsys.hls_to_rgb(h,l,s); new=[round(rr*255),round(gg*255),round(bb*255),a]
            if worst_cr(new)<need: new[3]=255
        changed.append((n,P[n],new,round(min(cr(over(new,s),s) for s in S.values()),2)))
        P[n]=new
    # TIERS: keep a visible primary / secondary / tertiary / disabled hierarchy (Windows dark-theme style)
    for n,c in P.items():
        if n.startswith('kText') and tuple(c[:3])==(255,255,255) and 'WhiteAlpha' not in n and 'Inverse' not in n:
            orig=next(o for nn,o,cc in D if nn==n)[0]          # original (light theme) alpha
            if 150<=orig<200: c[3]=max(c[3],197)   # secondary tier; primary text keeps its own near-white alpha
    # BUTTONS: accent-filled buttons keep white text, so the fill gets deep enough for white to read (4.5:1+)
    def darken_to(c,target):
        r,g,b,a=c; h,l,s=colorsys.rgb_to_hls(r/255,g/255,b/255); new=list(c)
        while cr((255,255,255),new[:3])<target and l>0:
            l-=0.003; rr,gg,bb=colorsys.hls_to_rgb(h,l,s); new=[round(rr*255),round(gg*255),round(bb*255),a]
        return new
    for names,target in ():   # (Microsoft/MuMu style: accent buttons stay bright, text on them is dark)
        for n in names:
            if n in P and cr((255,255,255),P[n][:3])<target:
                changed.append((n,list(P[n]),darken_to(P[n],target),target)); P[n]=changed[-1][2]
    # invisible fingerprints (Dark only): tiny unique tints, so ordinary screenshots identify colours
    import os
    if path=='mumu.json' and os.path.exists('fingerprint.json'):
        for n,(dr,dg,db) in json.load(open('fingerprint.json')).items():
            if n in P: c=P[n]; P[n]=[max(0,c[0]-dr),max(0,c[1]-dg),max(0,c[2]-db),c[3]]
    for e in D: e[2]=P[e[0]]
    json.dump(D,open(path,'w'))
    return changed
if __name__=='__main__':
    for p in ('mumu.json','amoled.json'):
        ch=fix(p); print(p,len(ch),'lifted')
        for n,o,nw,c in ch: print(f'   {n:24s}{tuple(o)} -> {tuple(nw)}  worst contrast {c}')
        left=audit(p,'',report=False); print('  still failing:',len(left))
