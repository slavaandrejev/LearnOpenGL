#include <giomm/resource.h>
#include <glibmm/refptr.h>
#include <gdkmm/frameclock.h>
#include <gtkmm/builder.h>
#include <gtkmm/glarea.h>

#include <fmt/printf.h>

#include "render.h"

OpenGLRender::OpenGLRender(BaseObjectType* cobject,
                           const Glib::RefPtr<Gtk::Builder>& refBuilder)
  : Gtk::GLArea(cobject)
{}

bool OpenGLRender::on_render(const Glib::RefPtr<Gdk::GLContext>& context) {
    const GLfloat color[] = {0.2f, 0.3f, 0.3f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, color);

    glUseProgram(renderingProgram);
    float greenValue = std::sin(curTime) / 2.0f + 0.5f;
    int vertexColorLocation = glGetUniformLocation(renderingProgram, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    return true;
}

void OpenGLRender::on_realize() {
    Gtk::GLArea::on_realize();
    make_current();

    renderingProgram = CompileShaders(
        "/vs.glsl", GL_VERTEX_SHADER,
        "/fs.glsl", GL_FRAGMENT_SHADER);

    float vertices[] = {
        0.5f, -0.5f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f, // bottom left
        0.0f,  0.5f, 0.0f  // top
    };

    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);

    glNamedBufferStorage(VBO, sizeof(vertices), &vertices[0], 0);
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 3 * sizeof(float));
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);
    glEnableVertexArrayAttrib(VAO, 0);

    add_tick_callback(sigc::mem_fun(*this, &OpenGLRender::timer_event));
}

void OpenGLRender::on_unrealize() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(renderingProgram);

    Gtk::GLArea::on_unrealize();
}

bool OpenGLRender::timer_event(const Glib::RefPtr<Gdk::FrameClock>& frameClock) {
    if (0 > startTime) {
        startTime = frameClock->get_frame_time();
        curTime   = 0;
    } else {
        curTime = 1e-6f * (frameClock->get_frame_time() - startTime);
    }
    queue_draw();

    return true;
}
