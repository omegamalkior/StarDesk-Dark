# Dark star icons at every size, downscaled by area-averaging (no sharpening halo).
import io,json,struct,zlib
from PIL import Image
BIG=Image.open('dark256.png').convert('RGBA')
def star(s): return BIG if s==256 else BIG.resize((s,s),Image.BOX)
def dib(im):
    s=im.size[0]; hdr=struct.pack('<IiiHHIIiiII',40,s,s*2,1,32,0,0,0,0,0,0)
    rows=bytearray()
    for y in range(s-1,-1,-1):
        for x in range(s):
            r,g,b,a=im.getpixel((x,y)); rows+=bytes((b,g,r,a))
    return hdr+bytes(rows)+bytes(((s+31)//32)*4*s)
def png(im,limit=None):
    for q in (None,256,192,128):
        b=io.BytesIO(); (im if q is None else im.quantize(colors=q,method=Image.Quantize.FASTOCTREE)).save(b,'PNG',optimize=True)
        if limit is None or len(b.getvalue())<=limit: return b.getvalue()
def ico(sizes):
    imgs=[(s,png(star(s)) if s==256 else dib(star(s))) for s in sizes]
    out=struct.pack('<HHH',0,1,len(imgs)); off=6+16*len(imgs)
    for s,dd in imgs: out+=struct.pack('<BBBBHHII',s%256,s%256,0,0,1,32,len(dd),off); off+=len(dd)
    return out+b''.join(dd for s,dd in imgs)
if __name__=='__main__':
    d=open('stardesk/bin/StarDesk.exe','rb').read()
    # 1. launcher's own icon
    open('stardesk_dark.ico','wb').write(ico([256,128,64,48,40,32,24,20,16]))
    # 2. program icon resources (exact same size and format as the originals)
    old=json.load(open('iconpatch.json')); new=[]
    for o,n in old:
        ob=bytes.fromhex(o)
        if ob[:4]==b'\x89PNG':
            nb=png(star(256),len(ob)); nb=nb+bytes(len(ob)-len(nb))
        else:
            s=struct.unpack_from('<i',ob,4)[0]; nb=dib(star(s)); assert len(nb)==len(ob)
        new.append([o,nb.hex()])
    json.dump(new,open('iconpatch.json','w'))
    # 3. the compressed :/stardesk/stardesk.ico copies (window / taskbar icon)
    out=[]
    for off in (0x264d28e,0x3c269b5):
        tot=struct.unpack('>I',d[off:off+4])[0]; blk=d[off:off+4+tot]; room=tot-4
        for sizes in ([256,128,64,48,40,32,24,20,16],[256,64,48,40,32,24,20,16],[256,48,32,24,16],[64,48,32,24,16]):
            raw=ico(sizes); z=zlib.compress(raw,9)
            if len(z)<=room: break
        assert len(z)<=room, 'does not fit'
        nb=struct.pack('>I',tot)+struct.pack('>I',len(raw))+z+bytes(room-len(z))
        assert len(nb)==len(blk) and zlib.decompress(nb[8:8+len(z)])==raw
        out.append([blk.hex(),nb.hex(),nb.hex()]); print('stardesk.ico at',hex(off),'sizes',sizes,'compressed',len(z),'of',room)
    json.dump(out,open('icopatch.json','w'))
    # preview of the small sizes (tray / taskbar) on a dark taskbar colour, enlarged 4x
    c=Image.new('RGBA',(16+20+24+32+48+10*5,48),(32,32,32,255)); x=5
    for s in (16,20,24,32,48):
        c.alpha_composite(star(s),(x,(48-s)//2)); x+=s+10
    c.resize((c.width*4,c.height*4),Image.NEAREST).save('/tmp/small_icons.png')
