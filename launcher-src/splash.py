import io,json
import numpy as np
from PIL import Image, ImageFilter
d=open('stardesk/bin/StarDesk.exe','rb').read()
offs=[0x3bf9924,0x3c98a7b,0x3ca29fd,0x3caf139,0x3c03a1e,0x3c30f4f,0x3c54f14,0x3c78074]
DARK=Image.open('dark256.png').convert('RGBA')               # the desktop icon's dark star (with outline)
REF=np.asarray(Image.open('orig256.png').convert('RGBA'))[...,3]>128   # same star, original colours, same placement
ry,rx=np.where(REF); rb=(rx.min(),ry.min(),rx.max()+1,ry.max()+1)
def make(im,bg,fg=(235,236,238)):
    a=np.asarray(im.convert('RGBA')).astype(np.float32)
    rgb=a[...,:3]; al=a[...,3]; sat=rgb.max(-1)-rgb.min(-1); L=rgb@np.array([.299,.587,.114])
    card=np.array(bg)+(1-L/255.0)[...,None]*(np.array(fg)-np.array(bg))
    w=np.clip((al-120)/120,0,1)[...,None]
    star=(sat>=60)&(al>120)
    out=rgb*(1-w)+card*w
    # remove the original star (and its soft edge), filling with the card colour around it
    k=5 if im.size[0]<500 else 9
    gone=np.asarray(Image.fromarray((star*255).astype(np.uint8)).filter(ImageFilter.MaxFilter(k)))>0
    ring=np.asarray(Image.fromarray((gone*255).astype(np.uint8)).filter(ImageFilter.MaxFilter(k)))>0 & ~gone
    fill=np.median(out[ring & (al>250)],axis=0)
    out[gone]=fill
    base=Image.fromarray(np.dstack([np.clip(out,0,255),al]).astype(np.uint8),'RGBA')
    # paste the high-resolution dark star, scaled so its shape lands exactly where the old star was
    sy,sx=np.where(star); sb=(sx.min(),sy.min(),sx.max()+1,sy.max()+1)
    s=((sb[2]-sb[0])/(rb[2]-rb[0])+(sb[3]-sb[1])/(rb[3]-rb[1]))/2
    size=round(256*s); icon=DARK.resize((size,size),Image.LANCZOS)
    ox=round(sb[0]-rb[0]*s); oy=round(sb[1]-rb[1]*s)
    base.alpha_composite(icon,(ox,oy))
    return base
def fit(im,room):
    for q in (None,256,192,128):
        b=io.BytesIO()
        if q is None: im.save(b,'PNG',optimize=True)
        else: im.quantize(colors=q,method=Image.Quantize.FASTOCTREE).save(b,'PNG',optimize=True)
        v=b.getvalue()
        if len(v)<=room: return v
pairs=[]; prev=[]
for o in offs:
    j=d.find(b'IEND',o)+8; old=d[o:j]; im=Image.open(io.BytesIO(old))
    vd=fit(make(im,(65,65,65)),len(old)); va=fit(make(im,(10,10,10)),len(old))
    assert vd and va, hex(o)
    pairs.append([old.hex(),(vd+bytes(len(old)-len(vd))).hex(),(va+bytes(len(old)-len(va))).hex()])
    if o in (0x3ca29fd,0x3c03a1e): prev.append(Image.open(io.BytesIO(vd)).convert('RGBA'))
json.dump(pairs,open('splashpatch.json','w'))
# preview: 2x version cropped around the logo, at true size
p=prev[1]; p.crop((150,170,560,320)).save('/tmp/splash_logo2x.png')
p=prev[0]; c=p.crop((60,80,300,165)); c.resize((c.width*2,c.height*2),Image.NEAREST).save('/tmp/splash_logo1x.png')
