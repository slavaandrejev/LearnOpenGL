#include <gtk/gtk.hpp>

#include <glbinding/gl/gl.h>

#include "render.h"

using namespace gl;

bool OpenGLRender::render_(Gdk::GLContext context) noexcept {
    const GLfloat color[] = {0.2f, 0.3f, 0.3f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, color);

    renderingProgram->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    return true;
}

void OpenGLRender::realize_() noexcept {
    GlBoundGlArea::realize_();

    renderingProgram = std::make_unique<Shader>(
        "/program.vs.glsl", GL_VERTEX_SHADER,
        "/program.fs.glsl", GL_FRAGMENT_SHADER);

    float vertices[] = {
       -0.9f, -0.9f, 0.0f,
       -0.1f, -0.9f, 0.0f,
       -0.1f,  0.9f, 0.0f,
        0.1f, -0.9f, 0.0f,
        0.9f, -0.9f, 0.0f,
        0.9f,  0.9f, 0.0f
    };

    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);

    // Named buffer allows to configure VAO without binding VBO to the context.
    glNamedBufferStorage(VBO, sizeof(vertices), &vertices[0], GL_NONE_BIT);
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 3 * sizeof(float));
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);
    glEnableVertexArrayAttrib(VAO, 0);
}

void OpenGLRender::unrealize_() noexcept {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    renderingProgram.reset();

    GlBoundGlArea::unrealize_();
}
