## Getting started 
``` sh
git clone --recursive https://github.com/HeizerSpider/graphics_visualisation_project.git 
```

Make sure to pass the `--recursive` flag to grab all the dependencies. If you forgot, then you can `git submodule update --init` instead.

``` sh
cd graphics_visualisation_project
cd build
cmake ..
make 
cd Display_Matrix
./Display_Matrix
```

