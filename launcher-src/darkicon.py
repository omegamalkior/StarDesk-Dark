from PIL import Image, ImageFilter, ImageChops
src=Image.open('orig256.png').convert('RGBA')
W=256
px=src.load(); out=Image.new('RGBA',(W,W)); po=out.load()
blue=(30,40,64); cyan=(58,74,104)
for y in range(W):
    for x in range(W):
        r,g,b,a=px[x,y]
        if a==0: continue
        t=max(0,min(1,(g-118)/(211-118)))
        shade=0.85+0.25*((r+g+b)/3/255)
        c=[min(255,int((blue[i]+(cyan[i]-blue[i])*t)*shade)) for i in range(3)]
        po[x,y]=(*c,a)
# soft light rim so it shows on dark backgrounds
alpha=src.getchannel('A')
grown=alpha.filter(ImageFilter.MaxFilter(7))
rim=ImageChops.subtract(grown,alpha).filter(ImageFilter.GaussianBlur(0.8))
rimimg=Image.new('RGBA',(W,W),(132,156,204,0)); rimimg.putalpha(rim.point(lambda v:int(v*0.9)))
final=Image.alpha_composite(rimimg,out)
# small inner highlight edge along the cyan swoosh edges for depth
final.save('dark256.png')
for bgc,name in [((32,32,32,255),'v_dark.png'),((243,243,243,255),'v_light.png')]:
    bg=Image.new('RGBA',(W+40,W+40),bgc); bg.alpha_composite(final,(20,20)); bg.save(name)
sizes=[256,128,64,48,40,32,24,20,16]
final.save('stardesk_dark.ico',sizes=[(s,s) for s in sizes])
# preview strip
strip=Image.new('RGBA',(sum(sizes)+10*len(sizes),270),(32,32,32,255)); x=5
for s in sizes:
    strip.alpha_composite(final.resize((s,s),Image.LANCZOS),(x,5)); x+=s+10
strip.save('strip.png')
