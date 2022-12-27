# Learn OpenGL

This is a rewrite of the original code from the [Learn
OpenGL](https://learnopengl.com) book by Joey de Vries. The original code is
available [here](https://github.com/JoeyDeVries/LearnOpenGL). The goal was to
integrate the OpenGL code in a more powerful GUI framework, and rewrite using
more contemporary DSA syntax for OpenGL. The project uses GTK 4 with C++
bindings (gtkmm). Old OpenGL syntax was left as is in the first example, but
the DSA starts on from the first exercise.

In each example or exercise, `render.cpp` contains the code relevant to OpenGL
and has just a minimum of the matter related to the GUI framework boilerplate.
All logic is concentrated in the three methods: `on_realize`, `on_render`, and
`on_unrealize`.

The original code is licensed under the terms of the CC BY-NC 4.0 license.

## Instructions to build and run on Linux

To build you need CMake, Meson, Boost, libepoxy, fmt, glm , and GTK 4 C++
bindings (gtkmm). You have to consult with your distro how to install them. For
ArchLinux you can do this:
```
$ sudo pacman -S cmake meson boost libepoxy fmt glm gtkmm-4.0
```
Then `cd` to where you cloned this repository and
```
$ CC=gcc CXX=g++ meson setup --backend ninja --buildtype debug build/debug
$ meson compile -C build/debug
```
That's it.

## Instructions to build and run on Windows

Download and install MSYS2 following [these
instructions](https://www.msys2.org/). MSYS2 gives you several options for the
[environment](https://www.msys2.org/docs/environments/) to run. The following
instructions assume UCRT environment, you have to click the correspondent link
installed by MSYS2 to open it. Then, from there, first, install the necessary
packages:
```
$ pacman -S mingw-w64-ucrt-x86_64-gcc \
            mingw-w64-ucrt-x86_64-meson \
            mingw-w64-ucrt-x86_64-cmake \
            mingw-w64-ucrt-x86_64-boost \
            mingw-w64-ucrt-x86_64-glm \
            mingw-w64-ucrt-x86_64-fmt \
            mingw-w64-ucrt-x86_64-libepoxy \
            mingw-w64-ucrt-x86_64-gtkmm-4.0
```
Now change to the directory where you cloned this repository and issue the
following command:
```
$ CC=gcc CXX=g++ meson setup --backend ninja --buildtype debug build/debug
```
This will make Meson to check if all dependencies are installed. Now we can
compile:
```
$ meson compile -C build/debug
```
Finally, you can run somethiing:
```
$ 'build/debug/02. Lighting/04. Lighting Maps/03. Exercises/003/ex-003.exe'
```
You will see this:

![cube lighting with emission man](https://user-images.githubusercontent.com/59517790/209611287-d385d087-27d7-499f-b14d-8cde651a29bc.png)
