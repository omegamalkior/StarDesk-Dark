import struct,sys
d=open(sys.argv[1],'rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]
nsec=struct.unpack_from('<H',d,pe+6)[0]; opt=struct.unpack_from('<H',d,pe+20)[0]
secs=[]
for i in range(nsec):
    o=pe+24+opt+40*i
    name=d[o:o+8].rstrip(b'\0'); vs,va,rs,ro=struct.unpack_from('<IIII',d,o+8)
    secs.append((name,va,vs,ro,rs))
rsrc=[s for s in secs if s[0]==b'.rsrc'][0]
rva0,raw0=rsrc[1],rsrc[3]
def r2o(rva):
    for n,va,vs,ro,rs in secs:
        if va<=rva<va+max(vs,rs): return rva-va+ro
def dir_entries(off):
    nn,ni=struct.unpack_from('<HH',d,raw0+off+12)
    for i in range(nn+ni):
        idn,ptr=struct.unpack_from('<II',d,raw0+off+16+8*i)
        yield idn,ptr
def leaf(ptr):
    while ptr&0x80000000:
        ptr=next(dir_entries(ptr&0x7fffffff))[1]
    rva,size=struct.unpack_from('<II',d,raw0+ptr)
    return d[r2o(rva):r2o(rva)+size]
types={}
for t,p in dir_entries(0): types[t]=p
icons={}
for idn,p in dir_entries(types[3]&0x7fffffff): icons[idn]=leaf(p)
groups=[(idn,leaf(p)) for idn,p in dir_entries(types[14]&0x7fffffff)]
gid,g=groups[0]
res,typ,cnt=struct.unpack_from('<HHH',g,0)
ents=[];datas=[]
for i in range(cnt):
    w,h,cc,rsv,pl,bc,sz,nid=struct.unpack_from('<BBBBHHIH',g,6+14*i)
    ents.append((w,h,cc,rsv,pl,bc)); datas.append(icons[nid])
out=struct.pack('<HHH',0,1,cnt); off=6+16*cnt
for e,dd in zip(ents,datas):
    out+=struct.pack('<BBBBHHII',*e,len(dd),off); off+=len(dd)
out+=b''.join(datas)
open(sys.argv[2],'wb').write(out)
print('groups',len(groups),'images',[(e[0] or 256,e[5],len(dd), dd[:4]) for e,dd in zip(ents,datas)])
