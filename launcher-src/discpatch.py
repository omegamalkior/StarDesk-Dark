# Light round artwork that sits on the dark window: the remote-boot power button, the cloud-device
# "turn on" button, the guest avatar. Neutral/pale pixels become dark greys (lighter rim, darker face, as in
# MuMu's dark buttons); coloured pixels (the blue power symbol, the spinner) are kept so they still stand out.
# The trial popup background is a light panel: recoloured like the other panel pictures.
import io,json,struct,colorsys
import numpy as np
from PIL import Image
_b=open('bgpatch.py').read()
exec(_b.split("NAMES=[")[0])                                  # d, R
exec('def make(im,surf):'+_b.split('def make(im,surf):')[1].split('pairs=[]')[0])   # make, enc, pack
DISCS=['/remote_boot/remote_boot_normal@4x.png','/remote_boot/remote_boot_bk@4x.png','/remote_boot/remote_boot_loading@4x.png',
       '/cloud_device/turn_on_cloud_device_icon_normal@4x.png','/settings/account/guest_account_header_pic@4x.png']
PANELS=['/recharge/trial/trial_bk.png','/recharge/trial/trial_bk@2x.png']
def disc(im,lo,hi):
    a=np.asarray(im.convert('RGBA')).astype(np.float32); rgb=a[...,:3]; al=a[...,3]
    mx=rgb.max(-1); mn=rgb.min(-1); sat=mx-mn; lum=rgb@np.array([.299,.587,.114],np.float32)
    pale=(sat<70)&(lum>100)
    # pale: map luminance 100..255 -> hi..lo (the brightest face becomes the darkest grey, rims a bit lighter)
    t=np.clip((lum-100)/155,0,1); g=hi+(lo-hi)*t
    out=rgb.copy(); out[pale]=np.stack([g,g,g],-1)[pale]
    return Image.fromarray(np.dstack([np.round(out),al]).astype(np.uint8),'RGBA')
def avatar(im,face):
    # white circle with a grey figure -> dark grey circle with a light grey figure
    a=np.asarray(im.convert('RGBA')).astype(np.float32); rgb=a[...,:3]; al=a[...,3]
    lum=rgb@np.array([.299,.587,.114],np.float32); g=face+(255-lum)*0.95
    return Image.fromarray(np.dstack([np.round(np.stack([g,g,g],-1)),al]).astype(np.uint8),'RGBA')
pairs=[]
def add(nm,vd,va,blk):
    n=max(len(vd),len(va)); assert n<len(blk),(nm,n,len(blk))
    vd+=bytes(n-len(vd)); va+=bytes(n-len(va)); pairs.append([blk[:n].hex(),vd.hex(),va.hex()]); print(nm,len(blk),'->',n)
for nm in DISCS+PANELS:
    off,fl=R[nm]; assert not fl&1; L=struct.unpack_from('>I',d,off)[0]; blk=d[off+4:off+4+L]
    im=Image.open(io.BytesIO(blk))
    if nm.endswith('guest_account_header_pic@4x.png'): vd=enc(avatar(im,72)); va=enc(avatar(im,28))
    elif nm in DISCS: vd=enc(disc(im,58,96)); va=enc(disc(im,14,44))
    else: vd=enc(make(im,(54,54,54))); va=enc(make(im,(9,9,9)))
    add(nm,vd,va,blk)
for o,n,a in pairs: assert d.count(bytes.fromhex(o))==1, o[:40]
json.dump(pairs,open('discpatch.json','w'))
# full-quality copies for the developer package (pull request)
import os
if os.environ.get('EXPORT'):
    DEV='/home/claude/pr/repo/src/client_ui/resources/theme'; n=0
    for nm in DISCS+PANELS:
        off,fl=R[nm]; L=struct.unpack_from('>I',d,off)[0]; im=Image.open(io.BytesIO(d[off+4:off+4+L]))
        for mode in ('dark','amoled'):
            if nm.endswith('guest_account_header_pic@4x.png'): out=avatar(im,72 if mode=='dark' else 28)
            elif nm in DISCS: out=disc(im,*((58,96) if mode=='dark' else (14,44)))
            else: out=make(im,(54,54,54) if mode=='dark' else (9,9,9))
            p=os.path.join(DEV,mode,nm.lstrip('/')); os.makedirs(os.path.dirname(p),exist_ok=True); out.save(p,optimize=True); n+=1
    print('exported',n)
