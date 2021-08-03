from PIL import Image
im = Image.open("windows.png", "r")
px = list(im.getdata())

print(im)

foo = im.resize((2,2),Image.ANTIALIAS)
px = list(foo.getdata())

def tupleToString(tup):
    s = ''
    for item in tup:
        s = s + str(item) + ' '
    return s[:-1]+'\n'

converted = []
for pixel in px:
    norm = tuple(val/255 for val in pixel)
    # converted.append(norm)
    converted.append(tupleToString(norm))

print(converted)

file1 = open('output.txt', 'w')
file1.writelines(converted)
file1.close()

