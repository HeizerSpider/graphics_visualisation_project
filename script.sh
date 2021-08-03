#!/bin/sh

echo "Please enter grid size (int):"
read size

python3 get_rgba.py $size
cd src/
make -f Makefile.mac && ../bin/sphere