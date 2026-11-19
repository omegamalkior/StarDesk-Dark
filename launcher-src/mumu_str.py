import json,re
exec(open('mumu.py').read().split('# ---- table ----')[0])
d=open('stardesk/bin/StarDesk.exe','rb').read()
rd=d[0x223ac00:0x223ac00+0x4832379]
def hexmap(b):
    def rep(m):
        h=m.group(1).decode(); a=h[:2] if len(h)==8 else ''; rgb=h[-6:]
        r,g,bb=int(rgb[0:2],16),int(rgb[2:4],16),int(rgb[4:6],16)
        return ('#'+a+'%02X%02X%02X'%mumu_rgb(r,g,bb)).encode()
    return re.sub(rb'#([0-9A-Fa-f]{8}|[0-9A-Fa-f]{6})(?![0-9A-Fa-f])',rep,b)
S=json.load(open('strpatch_dark.json'))
out=[[o,hexmap(n.encode()).decode('latin1'),'SAME_AS_DARKSRC'] for o,n in S]
olds={o.encode() for o,n in S}
def argb(a,rgb,width): s='#%02X%02X%02X%02X'%(a,*rgb); return s+' '*(width-len(s))
extra=[]
# white-text-on-blue buttons -> dark text on cyan (whole declaration, so the white text is only changed here)
for m in set(re.findall(rb'color: ?rgba\(255, 255, 255, (\d+)\);\s*background-color: ?rgba\(58, 123, 252, (\d+)\)',rd)):
    pass
for m in re.finditer(rb'color: ?rgba\(255, 255, 255, (\d+)\);\s*background-color: ?rgba\(58, 123, 252, (\d+)\)',rd):
    o=m.group(0); ta,ba=int(m.group(1)),int(m.group(2))
    pre=o[:o.index(b'rgba')]; mid=o[o.index(b')')+1:o.rindex(b'rgba')]
    t1=b'rgba(255, 255, 255, %d)'%ta; t2=b'rgba(58, 123, 252, %d)'%ba
    new=pre+argb(ta,(20,36,42),len(t1)).encode()+mid+argb(ba,cyan(58,123,252),len(t2)).encode()
    if o not in olds: extra.append((o,new)); olds.add(o)
# remaining hard-coded brand blues in styles
for m in set(re.findall(rb'rgba\((58|11|36|39|10), ?(123|102|111|91|94), ?(252|221|254|214|204), ?(\d+)\)',rd)):
    o=b'rgba(%s, %s, %s, %s)'%m
    for oo in (o,o.replace(b', ',b',')):
        if oo in rd and oo not in olds:
            r,g,b,a=(int(x) for x in m); extra.append((oo,argb(a,cyan(r,g,b),len(oo)).encode())); olds.add(oo)
for o in [b'rgb(58,123,252)',b'color:#3A7BFC',b'color:#0B66DD',b'color:#1740B0']:
    if o in rd and o not in olds:
        pre=o[:o.index(b'#')] if b'#' in o else b''
        if o.startswith(b'rgb('): extra.append((o,('#%02X%02X%02X'%cyan(58,123,252)).ljust(len(o)).encode()))
        else:
            h=o[-6:].decode(); extra.append((o,pre+('#%02X%02X%02X'%cyan(int(h[0:2],16),int(h[2:4],16),int(h[4:6],16))).encode()))
        olds.add(o)
# blue artwork colours in pictures (fill / stroke / stop-color)
from collections import Counter
for (attr,h),n in Counter(re.findall(rb'(fill|stroke|stop-color)="(#[0-9A-Fa-f]{6})"',rd)).items():
    r,g,b=int(h[1:3],16),int(h[3:5],16),int(h[5:7],16)
    o=attr+b'="'+h+b'"'
    if is_blue(r,g,b) and o not in olds:
        extra.append((o,attr+b'="'+('#%02X%02X%02X'%cyan(r,g,b)).encode()+b'"')); olds.add(o)
for o,n in extra: assert len(o)==len(n),(o,n)
# no pattern may be a prefix of another (keeps the launcher's scan and the test identical)
allo=[o.encode('latin1') if isinstance(o,str) else o for o,_,_ in out]+[o for o,n in extra]
for a_ in allo:
    for b_ in allo:
        if a_!=b_ and b_.startswith(a_): print('PREFIX CLASH',a_[:40],b_[:40])
json.dump({'base':out,'extra':[[o.hex(),n.hex()] for o,n in extra]},open('strpatch_mumu.json','w'))
print(len(extra),'new blue patterns'); 
for o,n in extra[:12]: print(' ',o[:70],'->',n[:70])
