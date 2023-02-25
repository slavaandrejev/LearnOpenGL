#pragma once

#include <string>

#include <gdkmm/pixbuf.h>

#include <epoxy/gl.h>

GLuint LoadTextureFromResources(const std::string &path) {
    auto img = Gdk::Pixbuf::create_from_resource(path);
    auto tex = GLuint{};

    glCreateTextures(GL_TEXTURE_2D, 1, &tex);
    auto format         = GLenum{};
    auto internalFormat = GLenum{};
    switch (img->get_n_channels()) {
        case 1: {
            format =  GL_RED;
            internalFormat = GL_R8;
            break;
        }
        case 3: {
            format =  GL_RGB;
            internalFormat = GL_RGB8;
            break;
        }
        case 4: {
            format =  GL_RGBA;
            internalFormat = GL_RGBA8;
            break;
        }
    }
    glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(tex, 1, internalFormat, img->get_width(), img->get_height());
    glTextureSubImage2D(
        tex
      , 0
      , 0
      , 0
      , img->get_width()
      , img->get_height()
      , format
      , GL_UNSIGNED_BYTE
      , img->get_pixels());
    glGenerateTextureMipmap(tex);

    return tex;
}
