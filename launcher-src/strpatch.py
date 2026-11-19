import re, colorsys
from collections import Counter
d=open('stardesk/bin/StarDesk.exe','rb').read()
rd=d[0x223ac00:0x223ac00+0x4832379]
P={}   # old -> new (bytes, same length)
def add(o,n):
    o=o.encode() if isinstance(o,str) else o; n=n.encode() if isinstance(n,str) else n
    assert len(o)==len(n),(o,n); P[o]=n
def lum(r,g,b): return r*0.299+g*0.587+b*0.114
def hx(c): return '#%02X%02X%02X'%c
def dark_surface(r,g,b):
    L=lum(r,g,b); h,l,s=colorsys.rgb_to_hls(r/255,g/255,b/255)
    t=max(26,min(80,30+(255-L)*0.55))/255
    rr,gg,bb=colorsys.hls_to_rgb(h,t,min(1,s*0.35))
    return (round(rr*255),round(gg*255),round(bb*255))
def light_text(r,g,b):
    L=lum(r,g,b); v=int(max(150,min(240,255-L*0.6))); return (v,v,v)
# ---- SVG artwork ----
add('fill="black"','fill="white"'); add('stroke="black"','stroke="white"')
add('#F9F9F9" fill-opacity="0.6"','#2B2C31" fill-opacity="0.8"')
add('#F8F8F8" fill-opacity="0.85"','#1E1F23" fill-opacity="0.85"')
# the frosted bar under the device picture (600x52, white gradient over a blur)
i=rd.find(b'<linearGradient id="paint0_linear_2424_141285"')
j=rd.find(b'</linearGradient>', rd.find(b'paint1_linear_2424_141285" x1'))+len(b'</linearGradient>')
if i>=0: blk=rd[i:j]; add(blk, blk.replace(b'stop-color="white"', b'stop-color="#222" '))
svghex=Counter(re.findall(rb'(fill|stroke|stop-color)="(#[0-9A-Fa-f]{6})"',rd))
for (attr,h),n in svghex.items():
    r,g,b=int(h[1:3],16),int(h[3:5],16),int(h[5:7],16)
    s=max(r,g,b)-min(r,g,b); L=lum(r,g,b)
    if L>=200 and not (r>200 and b<160): new=dark_surface(r,g,b)
    elif L<90 and s<30: new=light_text(r,g,b)
    else: continue
    add(attr+b'="'+h+b'"', attr+b'="'+hx(new).encode()+b'"')
# ---- hardcoded stylesheet colors ----
def argb(a,r,g,b,width): s='#%02X%02X%02X%02X'%(a,r,g,b); return s+' '*(width-len(s))
for m in set(re.findall(rb'rgba\(0, ?0, ?0, ?([0-9.]+)\)',rd)):
    v=m.decode(); a=round(float(v)*255) if '.' in v else int(v)
    for sep in (', ',','):
        o='rgba(0%s0%s0%s%s)'%(sep,sep,sep,v)
        if o.encode() in rd: add(o,argb(a,255,255,255,len(o)))
qss={'color:#000000':'color:#EEEEEE','rgb(0, 0, 0)':'#FFFFFF     ',
     'rgb(220, 220, 220)':'#3A3C42'.ljust(18),'rgb(217, 217, 217)':'#48494E'.ljust(18),'rgb(233, 236, 241)':'#3A3C40'.ljust(18),
     'rgb(249, 251, 253)':'#1E1F23'.ljust(18),'rgba(252,252,252,217)':argb(217,30,31,35,21),
     'rgba(255, 255, 255, 153)':argb(153,38,39,43,24),'rgba(255, 255, 255, 245)':argb(245,48,49,54,24),
     'rgba(248, 248, 253, 0.95)':argb(242,30,31,35,25),
     'rgba(218, 218, 218, 230)':argb(230,72,74,80,24),'rgba(218, 218, 218, 77)':argb(77,90,92,98,23),'rgba(218, 218, 218, 51)':argb(51,90,92,98,23),
     'rgba(222, 234, 255, 255)':argb(255,33,50,79,24),'rgba(181, 211, 255, 255)':argb(255,40,62,102,24),'rgba(140, 184, 255, 255)':argb(255,48,76,128,24),
     'color:rgba(10, 94, 204, 255)':'color:'+argb(255,130,178,255,22),
     'color:#0B66DD':'color:#82B2FF','color:#1740B0':'color:#5087F0',
     'rgba(11, 102, 221, 255)':argb(255,130,178,255,23),'rgba(11, 102, 221, 128)':argb(128,130,178,255,23)}
for o,n in qss.items():
    if o.encode() in rd: add(o,n)
tot=0
for o,n in sorted(P.items()):
    c=rd.count(o); tot+=c
    print(c,o.decode(),'->',n.decode())
print('patterns',len(P),'occurrences',tot)
import json; json.dump([[o.decode(),n.decode()] for o,n in P.items()],open('strpatch.json','w'))
