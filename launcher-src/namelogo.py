# "STARDESK" wordmark next to the star (Contact us, About): black letters -> white, the star keeps its colours.
import io,json,struct,colorsys
import numpy as np
from PIL import Image
d=open('stardesk/bin/StarDesk.exe','rb').read()
R=json.load(open('/tmp/rcc.json'))
pairs=[]
from PIL import ImageFilter
DARK=Image.open('dark256.png').convert('RGBA')               # the app icon's dark star (with outline)
REF=np.asarray(Image.open('orig256.png').convert('RGBA'))[...,3]>128
ry,rx=np.where(REF); rb=(rx.min(),ry.min(),rx.max()+1,ry.max()+1)
def make(im,white):
    a=np.asarray(im.convert('RGBA')).astype(np.float32)
    rgb=a[...,:3]; al=a[...,3].copy(); sat=rgb.max(-1)-rgb.min(-1)
    star=(sat>=60)&(al>60)
    out=rgb.copy(); out[~star]=white                       # letters: white, anti-aliasing rides on alpha
    k=5 if im.size[0]<500 else 9                            # take the old star out, soft edge included
    gone=np.asarray(Image.fromarray((star*255).astype(np.uint8)).filter(ImageFilter.MaxFilter(k)))>0
    sy,sx=np.where(star); sb=(sx.min(),sy.min(),sx.max()+1,sy.max()+1)
    gone[:, sb[2]+k:]=False                                 # never touch the letters
    al[gone]=0
    base=Image.fromarray(np.dstack([out,al]).astype(np.uint8),'RGBA')
    s=((sb[2]-sb[0])/(rb[2]-rb[0])+(sb[3]-sb[1])/(rb[3]-rb[1]))/2
    size=round(256*s); icon=DARK.resize((size,size),Image.LANCZOS)
    base.alpha_composite(icon,(round(sb[0]-rb[0]*s),round(sb[1]-rb[1]*s)))
    return base
for nm in ['/stardesk/about_logo_en_new.png','/stardesk/logo_with_name.png']:
    off,fl=R[nm]; L=struct.unpack_from('>I',d,off)[0]; old=d[off+4:off+4+L]
    im=Image.open(io.BytesIO(old))
    vs=[]
    for w in ((242,242,242),(255,255,255)):
        b=io.BytesIO(); make(im,np.array(w,np.float32)).save(b,'PNG',optimize=True); vs.append(b.getvalue())
    n=max(map(len,vs))
    if n>=L:   # fall back to an 8-bit palette to fit
        vs=[]
        for w in ((242,242,242),(255,255,255)):
            b=io.BytesIO(); make(im,np.array(w,np.float32)).quantize(256,method=Image.Quantize.FASTOCTREE).save(b,'PNG',optimize=True); vs.append(b.getvalue())
        n=max(map(len,vs))
    assert n<L,(nm,n,L)
    vd,va=[v+bytes(n-len(v)) for v in vs]
    assert d.count(old[:n])==1
    pairs.append([old[:n].hex(),vd.hex(),va.hex()]); print(nm,L,'->',n)
    Image.open(io.BytesIO(vd)).save('/tmp/namelogo_check_%d.png'%len(pairs))
json.dump(pairs,open('namelogopatch.json','w'))
