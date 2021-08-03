from PIL import Image
im = Image.open("windows.png", "r")
px = list(im.getdata())

print(im)

foo = im.resize((2,2),Image.ANTIALIAS)
px = list(foo.getdata())

def tuple_to_string(tup):
    s = ''
    for item in tup:
        s = s + str(item) + '\n'
    return s

converted = []
for pixel in px:
    norm = tuple(val/255 for val in pixel)
    # converted.append(norm)
    converted.append(tuple_to_string(norm))

file1 = open('rgba.txt', 'w')
file1.writelines(converted)
file1.close()

