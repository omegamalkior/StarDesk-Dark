# Writes full-quality dark / AMOLED versions of every recoloured picture into the developer package,
# at the same resource path as the original (under assets/dark and assets/amoled).
import io,json,os,struct,zlib,sys
import numpy as np
from PIL import Image
DEV='/home/claude/pr/repo/src/client_ui/resources/theme'
d=open('stardesk/bin/StarDesk.exe','rb').read()
R=json.load(open('/tmp/rcc.json')); byoff={v[0]:k for k,v in R.items()}
def load(nm):
    off,fl=R[nm]; L=struct.unpack_from('>I',d,off)[0]; b=d[off+4:off+4+L]
    if fl&1: b=zlib.decompressobj().decompress(b[4:])
    return Image.open(io.BytesIO(b))
def panel(im,surf):               # light panel -> surface colour, shadow and decoration kept
    a=np.asarray(im.convert('RGBA')).astype(np.float32); rgb=a[...,:3]; al=a[...,3]
    lum=rgb@np.array([.299,.587,.114],np.float32); m=(al>200)&(lum>200)
    q=rgb[m].astype(int); key=(q[:,0]<<16)|(q[:,1]<<8)|q[:,2]
    v=np.bincount(key).argmax(); ref=np.array([v>>16,(v>>8)&255,v&255],np.float32)
    out=np.clip(rgb/np.maximum(ref,1),0,1)*np.array(surf,np.float32)
    return Image.fromarray(np.dstack([np.round(out),al]).astype(np.uint8),'RGBA')
modal=[0x4a7e2a7,0x24a798b,0x3426202,0x38da7df,0x3a72da1,0x3a7ad01,0x3a83323,0x3a8f615,0x3a9a574,0x3b3d370,0x5824e56]
names=[byoff[o] for o in modal]
import re
src=open('bgpatch.py').read(); exec(src.split('def make')[0].split('R=json.load')[1].split('\n',1)[1].split('def ')[0]) if False else None
bg=re.search(r"NAMES=\[(.*?)\]\n",src,re.S).group(1); names+=re.findall(r"'([^']+)'",bg)
written=0
for nm in names:
    im=load(nm)
    for mode,surf in (('dark',(54,54,54)),('amoled',(9,9,9))):
        p=os.path.join(DEV,mode,nm.lstrip('/')); os.makedirs(os.path.dirname(p),exist_ok=True)
        panel(im,surf).save(p,optimize=True); written+=1
# QR-code logo: dark tile with the dark star (same method as the splash screens)
sys.argv=['x']; exec(open('splash.py').read().split('pairs=[]')[0])
for nm in [byoff[0x3d3ea75],byoff[0x3d27f13]]:
    im=load(nm)
    for mode,bgc in (('dark',(44,44,44)),('amoled',(16,16,16))):
        p=os.path.join(DEV,mode,nm.lstrip('/')); os.makedirs(os.path.dirname(p),exist_ok=True)
        make(im,bgc).save(p,optimize=True); written+=1
print('written',written,'files for',len(names)+2,'pictures')
