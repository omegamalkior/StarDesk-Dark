# The common modal-dialog background (:/uikit/common_modal_bk@4x.png and common_modal_round_bk@4x.png) is a
# light panel with a soft shadow. Dialogs draw their content 10px inside it, so in dark mode a light rim showed.
# Recolour the panel to the dialog's own surface colour (kFillSolidWhite) and keep the shadow.
# Only the first len(new) bytes are replaced: the new, smaller PNG ends with IEND and the rest is ignored.
import io,json
import numpy as np
from PIL import Image
d=open('stardesk/bin/StarDesk.exe','rb').read()
# every dialog background picture with that light panel (found by scanning the exe's PNGs)
offs=[0x4a7e2a7,0x24a798b,0x3426202,0x38da7df,0x3a72da1,0x3a7ad01,0x3a83323,0x3a8f615,0x3a9a574,0x3b3d370,0x5824e56]
REF=np.array([248,251,253],np.float32)
def make(im,surf):
    a=np.asarray(im.convert('RGBA')).astype(np.float32)
    t=np.clip(a[...,:3]/REF,0,1)
    rgb=t*np.array(surf,np.float32)
    return Image.fromarray(np.dstack([np.round(rgb),a[...,3]]).astype(np.uint8),'RGBA')
def enc(im):
    best=None
    for q in (None,128,64):
        b=io.BytesIO()
        (im if q is None else im.quantize(colors=q,method=Image.Quantize.FASTOCTREE)).save(b,'PNG',optimize=True)
        v=b.getvalue()
        if best is None or len(v)<len(best): best=v
    return best
pairs=[]
for o in offs:
    L=int.from_bytes(d[o:o+4],'big'); old=d[o+4:o+4+L]; im=Image.open(io.BytesIO(old))
    vd=enc(make(im,(54,54,54))); va=enc(make(im,(9,9,9)))
    n=max(len(vd),len(va)); assert n<L
    vd+=bytes(n-len(vd)); va+=bytes(n-len(va))
    pairs.append([old[:n].hex(),vd.hex(),va.hex()])
    print(hex(o),L,'->',n)
    Image.open(io.BytesIO(vd)).convert('RGBA').save('/tmp/modal_dark.png')
json.dump(pairs,open('modalpatch.json','w'))
