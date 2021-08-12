## Getting started 
``` sh
git clone --recursive https://github.com/HeizerSpider/graphics_visualisation_project.git 
```

Make sure to pass the `--recursive` flag to grab all the dependencies. If you forgot, then you can `git submodule update --init` instead.

### Run application directly
``` sh
python3 display.py 100
```
This defaults to resources/windows.png . You can change the grid_size by changing the number from 100 to 200 for instance.

``` sh
python3 display.py 100 -p resources/test.png
```
You can use another image file using the -p argument

``` sh
python3 display.py 100 -p resources/bad-apple-resized-short.mp4
```
This is the command to play the simple and quick sample video 

If other video are used - they need to be added to /resources . The build process helps to move these files to build/Display_Matrix. However, manual copying into that folder might be required since that copying seems to only be done when the whole project is rebuilt. 

### Build cpp
``` sh
cd graphics_visualisation_project
cd build
cmake ..
make -j8
cd Display_Matrix
./Display_Matrix
```
These steps used to run the c++ project. These steps called in the python script automatically

### Video prep
``` sh
ffmpeg -i <<filename>> -filter_complex [0:v]scale=100x100 -c:a copy <<filename>>-resized.<<ext>>
```

This is the command we used to separately prepare our video input
