import zlib,struct,re,json
d=open('stardesk/bin/StarDesk.exe','rb').read()
P=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in json.load(open('allpatch.json'))]
txt=[(o,n,a) for o,n,a in P if len(o)<1000]
pat=re.compile(b'|'.join(re.escape(o) for o,n,a in sorted(txt,key=lambda x:-len(x[0]))))
Mn={o:n for o,n,a in txt}; Ma={o:a for o,n,a in txt}
def pack(x):
    best=None
    for lvl in (9,8,7,6,5):
        for ml in (9,8,7):
            for strat in (zlib.Z_DEFAULT_STRATEGY,zlib.Z_FILTERED):
                c=zlib.compressobj(lvl,zlib.DEFLATED,15,ml,strat); z=c.compress(x)+c.flush()
                if best is None or len(z)<len(best): best=z
    return best
def short3(m):
    h=m.group(1).decode(); return ('"#'+''.join('%x'%round(int(h[i:i+2],16)/17) for i in (0,2,4))+'"').encode()
def variants(x):
    yield x
    x=x.replace(b'"white"',b'"#fff"'); yield x
    x=re.sub(rb'>\s+<',b'><',x); yield x
    x=re.sub(rb'\s+/>',b'/>',x); yield x
    x=re.sub(rb'"#([0-9A-Fa-f]{6})"',short3,x); yield x
out=[]
for off,tot,usz in json.load(open('zres.json')):
    blk=d[off:off+4+tot]; raw=zlib.decompress(blk[8:])
    if b'<svg' not in raw[:300]: continue
    nd=pat.sub(lambda m:Mn[m.group(0)],raw); na=pat.sub(lambda m:Ma[m.group(0)],raw)
    # common_box_bk.svg: the white top half of the box background showed through as a light bar
    hdr=b'V70H41V48Z" fill="white"'
    nd=nd.replace(hdr,b'V70H41V48Z" fill="#464646"'); na=na.replace(hdr,b'V70H41V48Z" fill="#0c0c0c"')
    if nd==raw and na==raw: continue
    room=tot-4
    def build(x):
        if x==raw: return blk
        for v in variants(x):
            z=pack(v)
            if len(z)<=room:
                b2=struct.pack('>I',tot)+struct.pack('>I',len(v))+z+bytes(room-len(z))
                assert len(b2)==len(blk) and zlib.decompressobj().decompress(b2[8:])==v
                return b2
    bd,ba=build(nd),build(na)
    if bd and ba: out.append([blk.hex(),bd.hex(),ba.hex()])
    else: print('TOO BIG',hex(off))
json.dump(out,open('zpatch.json','w')); print(len(out),'compressed icons')
