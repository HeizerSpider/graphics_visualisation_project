import sys
import os
import argparse
from sys import platform
from PIL import Image

def parseArguments():
    # Create argument parser
    parser = argparse.ArgumentParser()
    # Positional mandatory arguments
    parser.add_argument("size", help="display grid size", type=int)
    # Optional arguments
    parser.add_argument("-p", "--file", help="filepath", type=str, default="resources/windows.png")
    # Parse arguments
    args = parser.parse_args()
    return args

args = parseArguments()

# total arguments
size = args.size
image_filename = args.file

print("Reading", image_filename)
print("Size: {}x{}".format(size,size))

im = Image.open(image_filename, "r")
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

if platform == "linux":
    os.system("cd src/ && make -f Makefile.linux && ./../bin/sphere")
elif platform == "darwin":
    os.system("cd src/ && make -f Makefile.mac && ./../bin/sphere")
elif platform == "win32":
    os.system("cd src/ && make -f Makefile.windows && ./../bin/sphere")
