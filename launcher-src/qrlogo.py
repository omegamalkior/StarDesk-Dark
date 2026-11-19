# The logo in the middle of the "Complete Verification" QR code (qrcode_logo.png / qrcode_logo_en.png):
# a white app tile with the bright star -> dark tile with the dark star used for the app icon.
import io,json,struct,sys
sys.argv=['x']
src=open('splash.py').read().split('pairs=[]')[0]      # reuse make()/fit() from the splash-screen patch
exec(src)
DS=0x2248c20
ents=[0x3d3ea75,0x3d27f13]
pairs=[]
for p in ents:
    L=struct.unpack_from('>I',d,p)[0]; old=d[p+4:p+4+L]; im=Image.open(io.BytesIO(old))
    vd=fit(make(im,(44,44,44)),L); va=fit(make(im,(16,16,16)),L)
    assert vd and va
    n=max(len(vd),len(va)); vd+=bytes(n-len(vd)); va+=bytes(n-len(va))
    pairs.append([old[:n].hex(),vd.hex(),va.hex()])
    Image.open(io.BytesIO(vd)).save('/tmp/qrlogo_%d.png'%L)
    print(hex(p),L,'->',n)
json.dump(pairs,open('qrlogopatch.json','w'))
