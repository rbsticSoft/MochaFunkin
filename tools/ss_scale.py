from PIL import Image
import sys
import xml
import xml.etree.ElementTree as ET

ratio = 0.5
ss = Image.open(sys.argv[1])
sxml = ET.parse(sys.argv[2])

root = sxml.getroot()
for sub in root.findall("SubTexture"):
    x = int(sub.get("x")) * ratio
    y = int(sub.get("y")) * ratio
    w = int(sub.get("width")) * ratio
    h = int(sub.get("height")) * ratio
    fx = sub.get("frameX")
    fy = sub.get("frameY")
    fw = sub.get("frameWidth")
    fh = sub.get("frameHeight")
    if fw != None:
        fx = int(fx)
        fw = int(fw)
        fx *= ratio
        fw *= ratio
        sub.set("frameX", str(fx))
        sub.set("frameWidth", str(fw))
    if fh != None:
        fy = int(fx)
        fh = int(fw)
        fy *= ratio
        fh *= ratio
        sub.set("frameY", str(fy))
        sub.set("frameHeight", str(fh))

    sub.set("x", str(x))
    sub.set("y", str(y))
    sub.set("width", str(w))
    sub.set("height", str(h))

sxml.write("rescaled.xml")

ss = ss.resize((int(ss.width * ratio), int(ss.height * ratio)))
ss.save("rescaled.png")