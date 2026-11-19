# Independent check: build the expected patched image in Python and compare byte-for-byte with the harness output.
import sys,struct,re,json
exe,out,theme=sys.argv[1],sys.argv[2],int(sys.argv[3]); pal_i=int(sys.argv[4]) if len(sys.argv)>4 else -1
PALS=[[(0x99,0xEB,0xFF),(0x4C,0xC2,0xFF),(0x00,0x91,0xF8),(0x00,0x78,0xD4),(0x00,0x67,0xC0),(0x00,0x3E,0x92),(0x00,0x1A,0x68)],
      [(0xFF,0xB3,0xD9),(0xFF,0x8C,0xC6),(0xF2,0x5C,0xAB),(0xE3,0x00,0x8C),(0xB8,0x00,0x71),(0x8A,0x00,0x55),(0x5C,0x00,0x38)]]
d=open(exe,'rb').read(); nt=struct.unpack_from('<I',d,0x3c)[0]
soi,soh=struct.unpack_from('<II',d,nt+24+56); ns,opt=struct.unpack_from('<HH',d,nt+6)[0],struct.unpack_from('<H',d,nt+20)[0]
img=bytearray(soi); img[:soh]=d[:soh]; secs=[]
for i in range(ns):
    s=nt+24+opt+40*i; name=d[s:s+8]; vs,va,rs,ro=struct.unpack_from('<IIII',d,s+8)
    img[va:va+rs]=d[ro:ro+rs]; secs.append((name,va,vs))
H=open('palette.h').read(); A=open('accent.h').read()
arr=lambda src,n: [int(x,0) for x in re.search(n+r'\[[^\]]*\]=\{([^}]*)\}',src).group(1).replace('u','').split(',')]
kL,kD,kA=arr(H,'kLight'),arr(H,'kDark'),arr(H,'kAmoled')
shd=arr(A,'kAccentShadeDark' if theme==0 else 'kAccentShadeAmoled'); lin=arr(A,'kLin')
fixes=[tuple(int(v) for v in m) for m in re.findall(r'\{(\d+),(\d+),(\d+),(\d+)\}',A.split('kAccentFix')[1])]
lum=lambda c:(2126*lin[c[0]]+7152*lin[c[1]]+722*lin[c[2]])//10000
def shade(code):
    p=PALS[pal_i]
    if code<10: return p[6 if code==7 else code]
    i=2
    while i<6 and lum(p[i])>183333: i+=1
    return p[min(6,i+code-10)]
P=[(bytes.fromhex(o),bytes.fromhex(n),bytes.fromhex(a)) for o,n,a in json.load(open('allpatch.json'))]
new=[]
for j,(o,n,a) in enumerate(P):
    b=bytearray(a if theme else n)
    if pal_i>=0:
        for pj,off,sh,th in fixes:
            if pj==j and th==theme:
                c=shade(sh); up=any(65<=x<=70 for x in b[off:off+6])
                b[off:off+6]=(('%02X%02X%02X' if up else '%02x%02x%02x')%c).encode()
    new.append(bytes(b))
# table
tb=struct.pack('<8I',*kL[:8])
exp=bytearray(img)
for name,va,vs in secs:
    if not name.startswith(b'.rdata'): continue
    k=exp.find(tb,va,va+vs)
    if k>=0:
        t=list(struct.unpack_from('<%dI'%len(kL),exp,k)); nw=[]
        for i,v in enumerate(t):
            if v==kL[i]:
                x=(kA if theme else kD)[i]
                if pal_i>=0 and shd[i]>=0:
                    c=shade(shd[i]); x=(x&0xFF000000)|(c[0]<<16)|(c[1]<<8)|c[2]
                nw.append(x)
            else: nw.append(v)
        struct.pack_into('<%dI'%len(kL),exp,k,*nw)
rx=re.compile(b'|'.join(re.escape(o) for o,n,a in P)); idx={o:j for j,(o,n,a) in reversed(list(enumerate(P)))}
for name,va,vs in secs:
    if not (name.startswith(b'.rdata') or name.startswith(b'.rsrc') or name.rstrip(b'\0')==b'.data'): continue
    seg=bytes(exp[va:va+vs]); exp[va:va+vs]=rx.sub(lambda m:new[idx[m.group(0)]],seg)
for o,n,a in json.load(open('codepatch.json')):
    o=bytes.fromhex(o); k=bytes(exp).find(o)
    if k>=0: exp[k:k+len(o)]=bytes.fromhex(a if theme else n)
got=open(out,'rb').read()
diff=sum(1 for i in range(0,len(got),4096) if got[i:i+4096]!=exp[i:i+4096])
changed=sum(1 for i in range(0,len(got),4096) if got[i:i+4096]!=img[i:i+4096])
print(f'theme {theme} accent {pal_i}: pages changed {changed}, pages differing from expected {diff}')
import os
if os.environ.get('SHOWDIFF'):
    for i in range(0,len(got),4096):
        if got[i:i+4096]!=exp[i:i+4096]:
            j=next(k for k in range(i,i+4096) if got[k]!=exp[k]); print('diff page',hex(i),'first byte',hex(j))
