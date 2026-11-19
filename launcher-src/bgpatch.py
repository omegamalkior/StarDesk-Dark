# Window/panel background pictures that are still a light panel (with a soft shadow) in dark mode:
# recolour the panel to the dark surface colour, keep the shadow and any decoration (darkened in proportion).
# Only the first len(new) bytes of each resource are replaced (the new, smaller PNG ends with IEND).
import io,json,struct,zlib
import numpy as np
from PIL import Image
d=open('stardesk/bin/StarDesk.exe','rb').read()
R=json.load(open('/tmp/rcc.json'))
NAMES=['/cloud_device/cloud_device_control_running_state_bg.png','/cloud_device/cloud_device_control_running_state_bg@2x.png',
'/cloud_device/my_device_control_running_state_bg.png','/cloud_device/my_device_control_running_state_bg@2x.png',
'/common/message_box/message_box_bk.png','/common/message_box/message_box_bk@2x.png',
'/common/message_box/network_message_menu.png','/common/message_box/network_message_menu@2x.png',
'/control/controlled/controlled_bk.png','/control/controlled/controlled_bk@2x.png',
'/control/menu/new_menu_bk.png','/control/menu/new_menu_bk@2x.png',
'/device/alldevice/guest_guide_bk@2x.png','/device/alldevice/guest_guide_bk_win10@2x.png',
'/device/alldevice/user_guide_bk@2x.png','/device/alldevice/user_guide_bk_win10@2x.png',
'/device/setup_guide/setup_guide_wizard_bg@2x.png',
'/file_transfer/file_transfer_combo_view.png','/file_transfer/file_transfer_combo_view@2x.png',
'/file_transfer/file_transfer_controlled_bg.png','/file_transfer/file_transfer_controlled_bg@2x.png',
'/login/login_bk_img_rect_upgrade.png','/login/login_bk_img_rect_upgrade@2x.png',
'/login/new_login_bk@4x.png','/login/new_login_bk_win10@4x.png',
'/notify_center/frame_bk@2x.png',
'/second_screen/second_screen_bg.png','/second_screen/second_screen_bg@2x.png',
'/second_screen/virtual_display_messagebox_bk.png','/second_screen/virtual_display_messagebox_bk@2x.png',
'/safe_lock/safe_locked.png','/safe_lock/safe_unlock.png',
'/stardesk/sdlogin_bk@4x.png','/stardesk/sdlogin_bk_win10@4x.png',
'/upgrade/login_bk_img_upgrade.png','/upgrade/login_bk_img_upgrade@2x.png',
'/recharge/background.png']
def make(im,surf):
    a=np.asarray(im.convert('RGBA')).astype(np.float32)
    rgb=a[...,:3]; al=a[...,3]
    lum=rgb@np.array([.299,.587,.114],np.float32)
    m=(al>200)&(lum>200)
    q=rgb[m].astype(int); key=(q[:,0]<<16)|(q[:,1]<<8)|q[:,2]
    v=np.bincount(key).argmax(); ref=np.array([v>>16,(v>>8)&255,v&255],np.float32)
    t=np.clip(rgb/np.maximum(ref,1),0,1)
    out=t*np.array(surf,np.float32)
    return Image.fromarray(np.dstack([np.round(out),al]).astype(np.uint8),'RGBA')
def enc(im):
    best=None
    for q in (None,256,128):
        b=io.BytesIO()
        (im if q is None else im.quantize(colors=q,method=Image.Quantize.FASTOCTREE)).save(b,'PNG',optimize=True)
        v=b.getvalue()
        if best is None or len(v)<len(best): best=v
    return best
def pack(x):
    best=None
    for lvl in (9,):
        c=zlib.compressobj(lvl); z=c.compress(x)+c.flush()
        if best is None or len(z)<len(best): best=z
    return best
pairs=[]
for nm in NAMES:
    off,fl=R[nm]; L=struct.unpack_from('>I',d,off)[0]; blk=d[off+4:off+4+L]
    if fl&1:
        raw=zlib.decompressobj().decompress(blk[4:]); im=Image.open(io.BytesIO(raw))
        vd=enc(make(im,(54,54,54))); va=enc(make(im,(9,9,9)))
        zd=struct.pack('>I',len(vd))+pack(vd); za=struct.pack('>I',len(va))+pack(va)
        n=max(len(zd),len(za)); assert n<L,nm
        zd+=bytes(n-len(zd)); za+=bytes(n-len(za))
        pairs.append([blk[:n].hex(),zd.hex(),za.hex()]); print(nm,L,'->',n,'(compressed)')
    else:
        im=Image.open(io.BytesIO(blk))
        vd=enc(make(im,(54,54,54))); va=enc(make(im,(9,9,9)))
        n=max(len(vd),len(va)); assert n<L,nm
        vd+=bytes(n-len(vd)); va+=bytes(n-len(va))
        pairs.append([blk[:n].hex(),vd.hex(),va.hex()]); print(nm,L,'->',n)
for o,n,a in pairs: assert d.count(bytes.fromhex(o))==1, o[:40]
json.dump(pairs,open('bgpatch.json','w'))
