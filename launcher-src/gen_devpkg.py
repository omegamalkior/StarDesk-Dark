# Refresh the developer-integration sources from the current palettes and patterns.
import json,re,csv,io
DEV='/home/claude/pkg/2 - Developer Integration'
D=json.load(open('mumu.json')); A=json.load(open('amoled.json'))
argb=lambda c:(c[3]<<24)|(c[0]<<16)|(c[1]<<8)|c[2]
lightv=lambda o:(o[0]<<24)|(o[1]<<16)|(o[2]<<8)|o[3]
acc=open('accent.h').read()
roles=lambda n:[int(x) for x in re.search(n+r'\[TOKEN_COUNT\]=\{([^}]*)\}',acc).group(1).split(',')]
RD,RA=roles('kAccentShadeDark'),roles('kAccentShadeAmoled')
h=open(DEV+'/src/theme/theme_palettes.h').read()
head=h.split('inline constexpr QRgb kLight')[0]
def arr(name,vals): return 'inline constexpr QRgb %s[kTokenCount] = {\n'%name+''.join('    0x%08Xu,  // %s\n'%(v,n) for v,(n,o,c) in zip(vals,D))+'};\n\n'
out=head+arr('kLight',[lightv(o) for n,o,c in D])+arr('kDark',[argb(c) for n,o,c in D])+arr('kAmoled',[argb(c) for n,o,c in A])
out+='''// Accent roles, for following the Windows accent colour (ThemeManager::setUseSystemAccent).
// -1 = not an accent colour. 0..6 = Windows AccentPalette shade (0 Light3, 1 Light2, 2 Light1,
// 3 Base, 4 Dark1, 5 Dark2, 6 Dark3); 7 = deep tint for backgrounds (uses Dark3).
// Accent buttons follow the Windows dark theme: rest = Light2, hover = Light1, pressed = Base
// (AccentFillColorDefault / Secondary / Tertiary), with dark text on them (TextOnAccentFillColorPrimary).
inline constexpr signed char kAccentRoleDark[kTokenCount] = {%s};
inline constexpr signed char kAccentRoleAmoled[kTokenCount] = {%s};

}}} // namespace client::theme::palette
'''%(','.join(map(str,RD)),','.join(map(str,RA)))
open(DEV+'/src/theme/theme_palettes.h','w').write(out)
with open(DEV+'/data/token_colors.csv','w',newline='') as f:
    w=csv.writer(f); w.writerow(['token','light_argb','dark_argb','amoled_argb','accent_role_dark','accent_role_amoled'])
    for (n,o,c),(n2,o2,c2),rd,ra in zip(D,A,RD,RA): w.writerow([n,'#%08X'%lightv(o),'#%08X'%argb(c),'#%08X'%argb(c2),rd,ra])
P={bytes.fromhex(o).decode('latin1'):(bytes.fromhex(n).decode('latin1').rstrip(),bytes.fromhex(a).decode('latin1').rstrip()) for o,n,a in json.load(open('allpatch.json')) if all(32<=b<127 for b in bytes.fromhex(o))}
m=open(DEV+'/src/theme/dark_color_maps.h').read(); cnt=0
def fixline(mm):
    global cnt
    fr=mm.group(1).encode().decode('unicode_escape')
    if fr in P and mm.group(0).count('","')==2:
        nd,na=P[fr]; esc=lambda s:s.replace('\\','\\\\').replace('"','\\"')
        new='{"%s","%s","%s"}'%(mm.group(1),esc(nd),esc(na))
        if new!=mm.group(0): cnt+=1
        return new
    return mm.group(0)
m=re.sub(r'\{"((?:[^"\\]|\\.)*)","(?:[^"\\]|\\.)*","(?:[^"\\]|\\.)*"\}',fixline,m)
open(DEV+'/src/theme/dark_color_maps.h','w').write(m)
print('palettes written; stylesheet/svg swaps updated:',cnt)
