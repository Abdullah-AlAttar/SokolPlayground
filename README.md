## Description
Sample project using [sokol](https://github.com/floooh/sokol) and [meson](https://mesonbuild.com/index.html) build system.
## Building
```
meson build -Dimgui:default_library=static
ninja 
ninja
./src/sokolsample
```