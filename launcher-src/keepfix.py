# Icons the generic SVG rules got wrong (found by the full audit):
#  REVERT - keep StarDesk's original artwork: things drawn over the remote screen (cursors), Windows-style
#           warning triangles (black "!" on yellow), and pieces that were already dark (status pills, the
#           pressed pin, the screensaver upload buttons).
#  TILE   - icons with a white tile behind a glyph: the glyph went white, so the tile must go dark too.
# Each entry replaces the whole resource, so the generic rules don't touch it again.
import json,re,struct,zlib,sys
sys.path.insert(0,'/tmp/aud2')
from getsvg import raw,R
E=open('stardesk/bin/StarDesk.exe','rb').read()
T0=open('/tmp/t0.bin','rb').read(); T1=open('/tmp/t1.bin','rb').read()
names=list(R)
REVERT=[n for n in names if n.startswith('/control/viewing_cursor_')]
REVERT+=['/common/message_box/warning_icon.svg','/device/alldevice/error_icon.svg','/multi_screen/mac_no_support_warning.svg',
         '/notify_center/file_transfer_warning.svg','/device/desktop/device_online.svg','/device/desktop/device_occupied.svg',
         '/device/desktop/device_connected.svg','/device/common/pinned_clicked.svg']
REVERT+=[n for n in names if n.startswith('/control/privacy_screen/upload_custom_screensaver_')]
TILE=[n for n in names if re.match(r'/control/ball_(hover|clicked|collapse_\w+_(hover|clicked))\.svg$',n)]
TILE+=[n for n in names if n.startswith('/common/popup/popup_')]+['/apps_manager/forbidden.svg','/safe_lock/client_lock_on_normal.svg']
def tile_colour(n,theme):
    hov='hover' in n
    return (('#555','#444')[not hov]) if theme==0 else (('#222','#111')[not hov])
def fix_tiles(s,col):
    out=[]; pos=0
    # leave clipPath/mask definitions alone
    for m in re.finditer(r'<(clipPath|mask)\b.*?</\1>',s,re.S):
        out.append(re.sub(r'(<rect\b[^>]*?)fill="white"',lambda k:k.group(1)+('fill="%s"'%col).ljust(12),s[pos:m.start()])); out.append(m.group(0)); pos=m.end()
    out.append(re.sub(r'(<rect\b[^>]*?)fill="white"',lambda k:k.group(1)+('fill="%s"'%col).ljust(12),s[pos:]))
    return ''.join(out)
P=[]; seen=set()
for n in REVERT+TILE:
    if n not in R or R[n][1]&1: continue
    o=raw(E,n,rva=False)
    if o in seen: continue
    seen.add(o)
    if n in REVERT: d=a=o
    elif n=='/safe_lock/client_lock_on_normal.svg':   # the lock-screen box: same grey as its Off state
        d=raw(T0,n).decode().replace('fill="white"/>','fill="#555" />',1).encode(); a=raw(T1,n).decode().replace('fill="white"/>','fill="#222" />',1).encode()
    else:
        d=fix_tiles(raw(T0,n).decode(),tile_colour(n,0)).encode(); a=fix_tiles(raw(T1,n).decode(),tile_colour(n,1)).encode()
    assert len(d)==len(o)==len(a),n
    P.append([o.hex(),d.hex(),a.hex()]); print(('revert ' if n in REVERT else 'tile   ')+n)
json.dump(P,open('keepfix.json','w')); print(len(P),'resources')
