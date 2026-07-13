#include <gtk/gtk.hpp>
#include <gdkpixbuf/gdkpixbuf.hpp>

#include <glbinding/gl/gl.h>

#include "render.h"

using namespace gl;

bool OpenGLRender::render_(Gdk::GLContext context) noexcept {
    const GLfloat color[] = {0.2f, 0.3f, 0.3f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, color);

    glBindTexture(GL_TEXTURE_2D, texture);
    renderingProgram->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    return true;
}

void OpenGLRender::realize_() noexcept {
    GlBoundGlArea::realize_();

    renderingProgram = std::make_unique<Shader>(
        "/vs.glsl", GL_VERTEX_SHADER,
        "/fs.glsl", GL_FRAGMENT_SHADER);

    auto contImg = Gdk::Pixbuf::new_from_resource("/container.jpg");
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(texture, 1, GL_RGB8, contImg.get_width(), contImg.get_height());
    glTextureSubImage2D(
        texture
      , 0
      , 0
      , 0
      , contImg.get_width()
      , contImg.get_height()
      , GL_RGB
      , GL_UNSIGNED_BYTE
      , contImg.get_pixels().data());
    glGenerateTextureMipmap(texture);

    float vertices[] = {
        // positions       // colors         // texture coords
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
       -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    };
    GLuint indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);
    glCreateBuffers(1, &EBO);

    glNamedBufferStorage(VBO, sizeof(vertices), &vertices[0], GL_NONE_BIT);
    glNamedBufferStorage(EBO, sizeof(indices), &indices[0], GL_NONE_BIT);

    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(VAO, 0);

    glVertexArrayAttribBinding(VAO, 1, 0);
    glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glEnableVertexArrayAttrib(VAO, 1);

    glVertexArrayAttribBinding(VAO, 2, 0);
    glVertexArrayAttribFormat(VAO, 2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float));
    glEnableVertexArrayAttrib(VAO, 2);

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 8 * sizeof(float));

    glVertexArrayElementBuffer(VAO, EBO);
}

void OpenGLRender::unrealize_() noexcept {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture);
    renderingProgram.reset();

    GlBoundGlArea::unrealize_();
}
