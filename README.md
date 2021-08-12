## Getting started 
``` sh
git clone --recursive https://github.com/HeizerSpider/graphics_visualisation_project.git 
```

Make sure to pass the `--recursive` flag to grab all the dependencies. If you forgot, then you can `git submodule update --init` instead.

### Run application directly
``` sh
python3 display.py 50
```

``` sh
python3 display.py 50 -p resources/test.png
```

### Build cpp
``` sh
cd graphics_visualisation_project
cd build
cmake ..
make -j8
cd Display_Matrix
./Display_Matrix
```

### Video prep
``` sh
ffmpeg -i <<filename>> -filter_complex [0:v]scale=100x100 -c:a copy <<filename>>-resized.<<ext>>
```

