import sys, PIL.Image
imageName = sys.argv[-1].rsplit( ".", 1 )[ 0 ].replace("-","_")
img = PIL.Image.open(sys.argv[-1]).convert('L')

threshold = 128*[0] + 128*[255]

for y in range(img.size[1]):
    for x in range(img.size[0]):

        old = img.getpixel((x, y))
        new = threshold[old]
        err = (old - new) >> 3 # divide by 8
            
        img.putpixel((x, y), new)
        
        for nxy in [(x+1, y), (x+2, y), (x-1, y+1), (x, y+1), (x+1, y+1), (x, y+2)]:
            try:
                img.putpixel(nxy, img.getpixel(nxy) + err)
            except IndexError:
                pass
f = open('%s.h' % imageName, 'w')
f.write("#ifndef %s\n" % imageName.upper())
f.write("#define %s\n" % imageName.upper())
f.write("#include \"stm32l1xx.h\"\n" )
f.write("const int16_t %s_width = %d;\n" % (imageName , img.size[0]))
f.write("const int16_t %s_height = %d;\n" % (imageName , img.size[1]) )
f.write("const uint8_t %s[] = {\n" % imageName)
for y in range(img.size[1]):
    for x in xrange(0,img.size[0],8):
        outputString = "0x%X, "
        outputHex = ( ((img.getpixel((x+0, y)) if x+0 < img.size[0] else 0) & 1) << 7 ) | \
        ( ((img.getpixel((x+1, y)) if x+1 < img.size[0] else 0) & 1) << 6 ) | \
        ( ((img.getpixel((x+2, y)) if x+2 < img.size[0] else 0) & 1) << 5 ) | \
        ( ((img.getpixel((x+3, y)) if x+3 < img.size[0] else 0) & 1) << 4 ) | \
        ( ((img.getpixel((x+4, y)) if x+4 < img.size[0] else 0) & 1) << 3 ) | \
        ( ((img.getpixel((x+5, y)) if x+5 < img.size[0] else 0) & 1) << 2 ) | \
        ( ((img.getpixel((x+6, y)) if x+6 < img.size[0] else 0) & 1) << 1 ) | \
        ( ((img.getpixel((x+7, y)) if x+7 < img.size[0] else 0) & 1) << 0 )
        outputString = outputString % (~outputHex & 255)
        f.write(outputString)
    f.write("\n")
f.write("};\n")
f.write("#endif")
f.close()
img.save("%s-output.jpg" % imageName)
