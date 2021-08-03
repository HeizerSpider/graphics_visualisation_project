import sys
from PIL import Image

# total arguments
n = len(sys.argv)
print("Total arguments passed:", n)

size = 10
if (n > 1):
    size = int(sys.argv[1])

image_filename = "windows.png"

print("Reading", image_filename)
print("Size: {}x{}".format(size,size))

im = Image.open("windows.png", "r")
px = list(im.getdata())

compressed = im.resize((size,size),Image.ANTIALIAS)
px = list(compressed.getdata())

print(compressed)

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
file1.write(str(size)+'\n')
file1.writelines(converted)
file1.close()

