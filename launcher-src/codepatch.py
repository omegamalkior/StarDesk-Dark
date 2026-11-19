# Small code patches: which colour token the remote-session tab bar paints with.
# StarDesk paints the selected tab with kTextInversePrimary (white) and hovered tabs with kTextGray182;
# those tokens must stay light for text, so the tab painter is pointed at surface tokens instead.
import json
exe='stardesk/bin/StarDesk.exe'
d=open(exe,'rb').read()
N=[n for n,o,c in json.load(open('mumu.json'))]
SEL=bytes.fromhex('40 53 48 83 ec 20 ba 02 00 00 00 48 8b d9 e8 ae ee 8a ff 48 8b c3 48 83 c4 20 5b c3')
HOV=bytes.fromhex('40 53 48 83 ec 20 ba dd 00 00 00 48 8b d9 e8 9e ef 8a ff 48 8b c3 48 83 c4 20 5b c3')
def with_tok(p,name): b=bytearray(p); b[7]=N.index(name); return bytes(b)
P=[(SEL,with_tok(SEL,'kFillSolidWhite'),with_tok(SEL,'kBorderGray240')),     # selected tab: Dark #464646 / AMOLED #161616
   (HOV,with_tok(HOV,'kFillPanelBackground'),with_tok(HOV,'kFillSolidWhite'))] # hovered tab
import struct
from_rva=lambda r: r   # helper for readability
# --- Off-switch knob: StarDesk draws it with kFillBlackAlpha153 (a dark overlay colour, also used elsewhere). Route the knob's
#     colour lookup through a tiny stub that swaps that one token for kFillDarkGray25 (light grey, like
#     MuMuPlayer's and Windows' Off knob). The stub lives in unused padding right after the function.
GETTER=0x52941; SITE=0x896ea6; CAVE=0x896f50
site_old=bytes.fromhex('00480f44d08b1432e8')+struct.pack('<i',GETTER-(SITE+5))
site_new=bytes.fromhex('00480f44d08b1432e8')+struct.pack('<i',CAVE-(SITE+5))
stub=bytes.fromhex('81fa')+struct.pack('<I',N.index('kFillBlackAlpha153'))+bytes.fromhex('7505ba')+struct.pack('<I',N.index('kTextGray140'))+b'\xe9'+struct.pack('<i',GETTER-(CAVE+18))
cave_old=bytes.fromhex('00498bcae9c52680ffc3')+b'\xcc'*26
cave_new=bytes.fromhex('00498bcae9c52680ffc3')+b'\xcc'*3+stub+b'\xcc'*(26-3-len(stub))
assert len(stub)==18
P.append((site_old,site_new,site_new)); P.append((cave_old,cave_new,cave_new))
# --- Video toasts ("Optimizing route..."): fill with the Windows dark flyout grey instead of the card grey
TOAST=bytes.fromhex('ff15a6307a06ba22000000488d4dc8e891578aff48')
def tok_at(p,i,name): b=bytearray(p); b[i]=N.index(name); return bytes(b)
P.append((TOAST,tok_at(TOAST,7,'kBorderGray240'),tok_at(TOAST,7,'kBorderGray240')))
# --- Popups (menus, About, tips, toasts) use Windows' Mica backdrop. Following Microsoft's guidance for
#     dark themes in Win32 apps, tell Windows each such window is dark (DWMWA_USE_IMMERSIVE_DARK_MODE = 20)
#     right before StarDesk turns Mica on, so the backdrop is drawn dark instead of light.
IAT_DWMSET=0x6f55340
MSITE=0x874c87; MCAVE=0x8851c0; MRET=0x874c95   # body lives in the big spare area at 0x885160 (with the button stubs)
msite_old=bytes.fromhex('41b9040000004c8d442420488bcb')
msite_new=b'\xe9'+struct.pack('<i',MCAVE-(MSITE+5))+b'\x90'*9
def ripcall(at): return b'\xff\x15'+struct.pack('<i',IAT_DWMSET-(at+6))
def mica_body(caption):
    # Windows draws its own title-bar colour where StarDesk leaves the window see-through (the
    # remote-session tab bar). Give it StarDesk's title-bar colour (DWMWA_CAPTION_COLOR = 35) as well.
    c=bytearray()
    c+=bytes.fromhex('4889c6')                    # mov rsi, rax      (keep the Windows-version pointer)
    c+=bytes.fromhex('c744242001000000')          # mov dword [rsp+20h], 1
    c+=bytes.fromhex('ba14000000')                # mov edx, 20 (DWMWA_USE_IMMERSIVE_DARK_MODE)
    c+=bytes.fromhex('488bcb')                    # mov rcx, rbx      (the window)
    c+=bytes.fromhex('4c8d442420')                # lea r8, [rsp+20h]
    c+=bytes.fromhex('41b904000000')              # mov r9d, 4
    c+=ripcall(MCAVE+len(c))                      # call DwmSetWindowAttribute
    c+=bytes.fromhex('c7442420')+struct.pack('<I',caption)   # mov dword [rsp+20h], COLORREF
    c+=bytes.fromhex('ba23000000')                # mov edx, 35 (DWMWA_CAPTION_COLOR)
    c+=bytes.fromhex('488bcb')                    # mov rcx, rbx
    c+=bytes.fromhex('4c8d442420')                # lea r8, [rsp+20h]
    c+=bytes.fromhex('41b904000000')              # mov r9d, 4
    c+=ripcall(MCAVE+len(c))                      # call DwmSetWindowAttribute
    c+=bytes.fromhex('4889f0')                    # mov rax, rsi
    c+=msite_old                                  # the three original instructions
    c+=b'\xe9'+struct.pack('<i',MRET-(MCAVE+len(c)+5))   # jmp back
    return bytes(c)
MICA_DARK=mica_body(0x00323232); MICA_AMOLED=mica_body(0x00000000)
P.append((msite_old,msite_new,msite_new))
# --- Icon+text buttons ("Continue with Google / Email"): their label colours are fixed QColors (black at 90/60/45/35%
#     for the outlined and plain styles, white for the blue style), formatted as "color:rgba(r,g,b,a)". On the
#     dark button faces the black text vanished. The formatter's red()/green()/blue() reads now go through a
#     tiny stub that returns 255-x, so black text becomes white (same opacity) and white text on the accent
#     button becomes black (Windows' dark-theme accent buttons use dark text). Nothing else uses this formatter.
ITB_CAVE=0x885160
ITB={0x885380:0x6f50368,0x8853bd:0x6f50360,0x8853fa:0x6f50358}   # call site -> IAT slot (red, green, blue)
def foff(rva):
    for nm,va,vs,ro,rs in SECS:
        if va<=rva<va+rs: return rva-va+ro
pe_=struct.unpack_from('<I',d,0x3c)[0]; ns_=struct.unpack_from('<H',d,pe_+6)[0]; so_=struct.unpack_from('<H',d,pe_+20)[0]
SECS=[]
for i_ in range(ns_):
    o_=pe_+24+so_+40*i_; vs_,va_,rs_,ro_=struct.unpack_from('<IIII',d,o_+8); SECS.append((d[o_:o_+8],va_,vs_,ro_,rs_))
stubs=b''; cur=ITB_CAVE
for site,iat in ITB.items():
    f=foff(site); old=d[f-5:f+6]; assert old[5:7]==b'\xff\x15',hex(site)
    new=old[:5]+b'\xe8'+struct.pack('<i',cur-(site+5))+b'\x90'
    P.append((old,new,new))
    st=bytes.fromhex('4883ec28')+b'\xff\x15'+struct.pack('<i',iat-(cur+4+6))+bytes.fromhex('4883c428')+bytes.fromhex('35ff000000')+b'\xc3'
    stubs+=st; cur+=len(st)
f=foff(ITB_CAVE); g=f-36; end=foff(MCAVE)+len(MICA_DARK)+4
cave_old=d[g:end]; assert cave_old[36:]==b'\xcc'*(end-f), cave_old.hex()
assert d.count(cave_old)==1, 'cave pattern not unique'
assert ITB_CAVE+len(stubs)<=MCAVE
def fill(body):
    b=bytearray(cave_old); b[36:36+len(stubs)]=stubs; o=36+(MCAVE-ITB_CAVE); b[o:o+len(body)]=body; return bytes(b)
P.append((cave_old,fill(MICA_DARK),fill(MICA_AMOLED)))
# --- Small round icon buttons (hover/pressed fill): hover used kFillBlackAlpha64, now see-through for the
#     remote-session title bar; use the pressed colour (kFillBlackAlpha51, white 20%) for hover too.
HOVBTN=bytes.fromhex('55c8488d4dc0ff1562fb730690eb6183f802752fba30000000e8d92084ff488bd041')
assert d.count(HOVBTN)==1
P.append((HOVBTN,tok_at(HOVBTN,21,'kFillBlackAlpha51'),tok_at(HOVBTN,21,'kFillBlackAlpha51')))
# --- Acrylic windows (the remote-session window's title strip): StarDesk tints Windows' acrylic blur with
#     kFillSolidWhite at 56% opacity, so whatever is behind the window (bright pages) showed through as a light
#     grey bar. Dark themes: tint with the main window's title-bar colour (kFillWindowPanel) at full opacity.
ACR_OPA=bytes.fromhex('ffc745300400000081cb00000090895d3841'); assert d.count(ACR_OPA)==1
o=bytearray(ACR_OPA); o[13]=0xff; P.append((ACR_OPA,bytes(o),bytes(o)))
ACR_TOK=bytes.fromhex('40534883ec20ba22000000488bd9e83ee2'); assert d.count(ACR_TOK)==1
P.append((ACR_TOK,tok_at(ACR_TOK,7,'kFillWindowPanel'),tok_at(ACR_TOK,7,'kFillWindowPanel')))
# --- Mica: Windows draws its Mica backdrop itself (light, whatever StarDesk's colours), and it showed through
#     the remote-session window's see-through title strip. Dark themes: ask for no system backdrop
#     (DWMWA_SYSTEMBACKDROP_TYPE = 1, DWMSBT_NONE) instead of Mica (2); the window colour shows instead.
MICA_SET=bytes.fromhex('ba26000000c744242002000000ff1574066e'); assert d.count(MICA_SET)==1
o=bytearray(MICA_SET); assert o[9]==2; o[9]=1; P.append((MICA_SET,bytes(o),bytes(o)))
# --- Remote-session title bar: VideoContentTitle fills itself with kBorderCoolGray (a border colour, light grey in
#     dark themes). On HDR displays that window is shown brighter than the main window, so Dark uses a slightly
#     deeper grey (kBorderGray240, #242424) to look like the main title bar; AMOLED uses black (kFillWindowPanel).
VCT=bytes.fromhex('20ff150928980690ba90000000488d4c2440e86a4d'); assert d.count(VCT)==1 and VCT[8]==0xba and VCT[9]==N.index('kBorderCoolGray')
P.append((VCT,tok_at(VCT,9,'kBorderGray240'),tok_at(VCT,9,'kFillWindowPanel')))
for o,n,a in P: assert d.count(o)==1, o.hex()
with open('codepatch.h','w') as f:
    f.write('#define NCODE %d\n#define CODELEN %d\n'%(len(P),max(len(o) for o,n,a in P)))
    f.write('static const unsigned char kCodeLen[NCODE]={%s};\nstatic const unsigned char kCodeOld[NCODE][CODELEN]={%s};\n'%(','.join(str(len(o)) for o,n,a in P),','.join('{%s}'%','.join(map(str,o)) for o,n,a in P)))
    f.write('static const unsigned char kCodeNew[NCODE][CODELEN]={%s};\n'%','.join('{%s}'%','.join(map(str,n)) for o,n,a in P))
    f.write('static const unsigned char kCodeNewA[NCODE][CODELEN]={%s};\n'%','.join('{%s}'%','.join(map(str,a)) for o,n,a in P))
json.dump([[o.hex(),n.hex(),a.hex()] for o,n,a in P],open('codepatch.json','w'))
print('code patches',len(P))
