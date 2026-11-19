import re,zlib,json,struct
d=open('stardesk/bin/StarDesk.exe','rb').read()
RD=0x223ac00; rd=d[RD:RD+0x4832379]
P=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in json.load(open('allpatch.json')) if len(bytes.fromhex(o))<1000]
pat=re.compile(b'|'.join(re.escape(o) for o,n,a in sorted(P,key=lambda x:-len(x[0]))))
Mn={o:n for o,n,a in P}; Ma={o:a for o,n,a in P}
COL={'dark':b'#555','amoled':b'#222'}
def box_recolor(svg,theme):
    s=pat.sub(lambda m:(Mn if theme=='dark' else Ma)[m.group(0)],svg)
    changed=False
    def rect(m):
        nonlocal changed
        t=m.group(0)
        w=re.search(rb'\bwidth="([\d.]+)"',t); h=re.search(rb'\bheight="([\d.]+)"',t)
        if not (w and h and float(w.group(1))>=40 and float(h.group(1))>=16): return t
        if not re.search(rb'fill="white"',t): return t
        op=re.search(rb'fill-opacity="([\d.]+)"',t)
        if op and float(op.group(1))<0.5: return t
        changed=True
        t=t.replace(b'fill="white"',b'fill="'+COL[theme]+b'" ')
        return t
    s=re.sub(rb'<rect\b[^>]*>',rect,s)
    return s,changed
def fit_len(new,old_len):
    # shrink "0.5" style numbers if needed, then pad with spaces before </svg>
    while len(new)>old_len:
        m=re.search(rb'="0\.(\d)"',new)
        if not m: return None
        new=new[:m.start()]+b'=".'+m.group(1)+b'"'+new[m.end():]
    i=new.rfind(b'</svg>')
    return new[:i]+b' '*(old_len-len(new))+new[i:]
out=[]; zout=[]
# plain svgs
for m in re.finditer(rb'<svg[^>]*>',rd):
    e=rd.find(b'</svg>',m.start())+6; svg=rd[m.start():e]
    nd,cd=box_recolor(svg,'dark'); na,ca=box_recolor(svg,'amoled')
    if not cd: continue
    nd2=fit_len(nd,len(svg)); na2=fit_len(na,len(svg))
    if nd2 is None or na2 is None: print('no fit',hex(m.start())); continue
    out.append([svg.hex(),nd2.hex(),na2.hex()])
print('plain box pictures:',len(out))
# compressed svgs
def pack(x):
    best=None
    for lvl in (9,8,7,6,5):
        for ml in (9,8,7):
            for strat in (zlib.Z_DEFAULT_STRATEGY,zlib.Z_FILTERED):
                c=zlib.compressobj(lvl,zlib.DEFLATED,15,ml,strat); z=c.compress(x)+c.flush()
                if best is None or len(z)<len(best): best=z
    return best
Z=json.load(open('zpatch.json')); zmap={bytes.fromhex(o):(bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in Z}
for off,tot,usz in json.load(open('zres.json')):
    blk=d[off:off+4+tot]; raw=zlib.decompress(blk[8:])
    if b'<svg' not in raw[:300]: continue
    nd,cd=box_recolor(raw,'dark'); na,ca=box_recolor(raw,'amoled')
    if not cd: continue
    def build(x):
        for v in (x,re.sub(rb'>\s+<',b'><',x)):
            z=pack(v)
            if len(z)<=tot-4: return struct.pack('>I',tot)+struct.pack('>I',len(v))+z+bytes(tot-4-len(z))
    bd,ba=build(nd),build(na)
    if bd and ba: zmap[blk]=(bd,ba); print('compressed box picture',hex(off))
    else: print('compressed no fit',hex(off))
json.dump(out,open('boxpatch.json','w'))
json.dump([[o.hex(),n.hex(),a.hex()] for o,(n,a) in zmap.items()],open('zpatch.json','w'))
