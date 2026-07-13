#include <gtk/gtk.hpp>

#include <glbinding/gl/gl.h>

#include "render.h"

using namespace gl;

bool OpenGLRender::render_(Gdk::GLContext context) noexcept {
    const GLfloat color[] = {0.2f, 0.3f, 0.3f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, color);

    renderingProgram->use();
    float greenValue = std::sin(curTime) / 2.0f + 0.5f;
    renderingProgram->set("ourColor", {0.0f, greenValue, 0.0f, 1.0f});

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    return true;
}

void OpenGLRender::realize_() noexcept {
    GlBoundGlArea::realize_();

    renderingProgram = std::make_unique<Shader>(
        "/vs.glsl", GL_VERTEX_SHADER,
        "/fs.glsl", GL_FRAGMENT_SHADER);

    float vertices[] = {
        0.5f, -0.5f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f, // bottom left
        0.0f,  0.5f, 0.0f  // top
    };

    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);

    glNamedBufferStorage(VBO, sizeof(vertices), &vertices[0], GL_NONE_BIT);
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 3 * sizeof(float));
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);
    glEnableVertexArrayAttrib(VAO, 0);

    tickCallbackId = add_tick_callback(gi::mem_fun(&OpenGLRender::timer_event, this));
}

void OpenGLRender::unrealize_() noexcept {
    remove_tick_callback(tickCallbackId);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    renderingProgram.reset();

    GlBoundGlArea::unrealize_();
}

bool OpenGLRender::timer_event(Gtk::Widget, Gdk::FrameClock frame_clock) {
    if (0 > startTime) {
        startTime = frame_clock.get_frame_time();
        curTime   = 0;
    } else {
        curTime = 1e-6f * (frame_clock.get_frame_time() - startTime);
    }
    queue_draw();

    return true;
}
