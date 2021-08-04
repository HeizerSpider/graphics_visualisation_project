### Run application directly
``` sh
python3 display.py 100
```

``` sh
python3 display.py 100 -p resources/test.png
```

### C++ internals

##### Build
```sh
cd src
# change mac to windows/linux depending on your OS
make -f Makefile.mac clean
make -f Makefile.mac
```

##### Run
```sh
cd bin
./sphere # for mac and linux, for windows just run .exec file
```
