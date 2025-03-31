#pragma once

#include <string>

#include <gdkmm/pixbuf.h>

#include <glbinding/gl/gl.h>

inline
gl::GLuint LoadTextureFromPixbuf(const Gdk::Pixbuf &img) {
    auto tex = gl::GLuint{};

    gl::glCreateTextures(gl::GL_TEXTURE_2D, 1, &tex);
    auto format         = gl::GLenum{};
    auto internalFormat = gl::GLenum{};
    switch (img.get_n_channels()) {
        case 1: {
            format         = gl::GL_RED;
            internalFormat = gl::GL_R8;
            break;
        }
        case 3: {
            format         = gl::GL_RGB;
            internalFormat = gl::GL_RGB8;
            break;
        }
        case 4: {
            format         = gl::GL_RGBA;
            internalFormat = gl::GL_RGBA8;
            break;
        }
    }
    gl::glTextureParameteri(tex, gl::GL_TEXTURE_WRAP_S, gl::GL_REPEAT);
    gl::glTextureParameteri(tex, gl::GL_TEXTURE_WRAP_T, gl::GL_REPEAT);
    gl::glTextureParameteri(tex, gl::GL_TEXTURE_MIN_FILTER, gl::GL_LINEAR_MIPMAP_LINEAR);
    gl::glTextureParameteri(tex, gl::GL_TEXTURE_MAG_FILTER, gl::GL_LINEAR);
    gl::glTextureStorage2D(tex, 1, internalFormat, img.get_width(), img.get_height());
    gl::glTextureSubImage2D(
        tex
      , 0
      , 0
      , 0
      , img.get_width()
      , img.get_height()
      , format
      , gl::GL_UNSIGNED_BYTE
      , img.get_pixels());
      gl::glGenerateTextureMipmap(tex);

    return tex;
}
