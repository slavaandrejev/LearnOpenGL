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
