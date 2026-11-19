import json, colorsys
T=json.load(open('/tmp/table.json'))  # (name,a,r,g,b)

def lum(r,g,b): return (r*0.299+g*0.587+b*0.114)
def tint(r,g,b,L, sat_keep=0.35):
    # keep hue, set new lightness level L (0-255), keep a little saturation
    h,l,s=colorsys.rgb_to_hls(r/255,g/255,b/255)
    s2=min(1,s*sat_keep) if l>0.85 else s*0.6
    rr,gg,bb=colorsys.hls_to_rgb(h,L/255,s2)
    return round(rr*255),round(gg*255),round(bb*255)
def surface(r,g,b):   # light background -> dark background (white = 38)
    L=lum(r,g,b); return tint(r,g,b,max(20,min(90,30+(255-L)*0.55)) if L<255 else 38)
def border(r,g,b):
    L=lum(r,g,b); return tint(r,g,b,max(40,min(110,52+(255-L)*0.5)))
def textgray(r,g,b):
    L=lum(r,g,b); return tint(r,g,b,max(70,min(240,255-L*0.75)))

O={  # hand-picked roles
 'kFillSolidWhite':(38,39,43,255),'kFillWindowPanel':(30,31,35,255),'kFillPanelBackground':(33,35,39,255),
 'kFillAlmostWhite':(36,37,41,255),'kFillGray249':(32,33,37,255),'kFillGray249Blue':(32,33,37,255),
 'kFillBrandLight':(33,50,82,255),'kFillBrandLightHover':(40,62,102,255),'kFillBrandLightPressed':(48,76,128,255),
 'kFillPaleBlue':(30,46,70,255),'kFillPaleBlueAlpha128':(40,70,110,128),'kFillPaleBlueAlpha0':(30,46,70,0),
 'kFillMessageRefreshHover':(42,50,64,255),'kFillMessageRefreshPressed':(48,60,80,255),
 'kFillPrivacySafe':(24,60,47,255),'kFillPrivacyWarning':(72,52,24,255),'kFillLightRed':(92,40,38,255),'kFillPaleRed':(64,32,30,255),
 'kFillTooltipBackground':(52,54,58,255),'kFillTeachingTip':(46,48,54,255),
 'kFillDeepPrimary':(58,62,74,255),'kFillSafeLockGradientStart':(30,48,72,255),'kFillSafeLockGradientEnd':(32,42,58,255),
 'kFillWebViewProgressTrack':(55,56,60,255),'kFillAnimationGray':(44,45,50,255),
 'kTextPrimary':(255,255,255,229),'kTextDeepPrimary':(236,240,246,255),
 'kTextBrand':(98,156,255,255),'kTextBrandHover':(130,178,255,255),'kTextBrandPressed':(80,135,240,255),'kTextBrandAlpha204':(98,156,255,204),
 'kTextBlueGray83':(196,212,232,255),'kTextBlueGray34':(226,230,234,255),'kTextGray34':(228,228,228,255),
 'kTextRed201':(240,110,96,255),'kTextRed230':(245,105,90,255),
 'kBorderWhite':(56,57,62,255),'kBorderWindowPanel':(48,50,55,255),'kBorderHeaderBlue':(42,62,92,255),'kBorderPaleBlue':(42,62,92,255),
 'kBorderDeepPrimary':(200,206,216,255),'kBorderBlack':(170,170,170,255),
 'kShadowGray241':(18,18,20,255),'kShadowGray249':(18,18,20,255),
}
keep=lambda n:( 'Brand' in n and 'Light' not in n) or n.startswith('kTextInverse') or 'WhiteAlpha' in n and n.startswith('kText') \
   or n in ('kFillTransparent','kBorderTransparent','kTextBlackAlpha0','kFillDarkGray25','kFillGray34Alpha153','kFillGray34Alpha179',
            'kBorderGray34Alpha153','kFillRed','kTextBadgeRed','kTextPerformanceWarning','kTextPerformanceDanger','kTextAmber200',
            'kTextTooltipYellow','kFillGreenAlpha77','kBorderGreenAlpha77','kTextBrandLight','kBorderInverseSubtle','kBorderWhiteAlpha20',
            'kFillBlackAlpha255','kFillWhiteAlpha0','kFillWhiteAlpha26','kTextRed240','kTextRed250','kBorderRed201') \
   or n.startswith('kShadowBlack') or any(k in n for k in ('WinBlue','ProgressBlue','AnimationBlue','LoginBrand','LinkHover','SelectedBlue','BlueStrong','BluePressed'))
out=[]
for n,a,r,g,b in T:
    if n in O: c=O[n]
    elif keep(n): c=(r,g,b,a)
    elif r==g==b==0:                      # black with alpha
        if n.startswith('kFill') and a>=64: c=(0,0,0,a)   # dimming masks stay black
        else: c=(255,255,255,a)            # text / hover / borders become white
    elif n.startswith('kText'):
        c=(*textgray(r,g,b),a)
    elif n.startswith('kBorder'):
        c=(*border(r,g,b),a) if lum(r,g,b)>150 else (r,g,b,a)
    elif n.startswith('kShadow'): c=(18,18,20,a)
    else:  # fills
        L=lum(r,g,b)
        if L>=200: c=(*surface(r,g,b),a)
        elif 120<=L<200: c=(*tint(r,g,b,255-L*0.9),a) if 'Gray' in n or 'Cool' in n else (r,g,b,a)
        elif L<120 and 'Gray' in n: c=(*tint(r,g,b,min(200,255-L)),a)
        else: c=(r,g,b,a)
    out.append((n,(a,r,g,b),c))
for n,old,c in out:
    print(f"{n:30s} {old[1:]+(old[0],)} -> {c}")
json.dump([[n,list(old),list(c)] for n,old,c in out],open('dark.json','w'))
