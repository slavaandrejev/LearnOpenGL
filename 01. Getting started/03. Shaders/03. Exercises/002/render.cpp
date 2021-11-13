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

    renderingProgram->use();
    renderingProgram->set("offset", 0.5);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    return true;
}

void OpenGLRender::on_realize() {
    Gtk::GLArea::on_realize();
    make_current();

    renderingProgram = std::make_unique<Shader>(
        "/vs.glsl", GL_VERTEX_SHADER,
        "/fs.glsl", GL_FRAGMENT_SHADER);

    float vertices[] = {
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // top
    };

    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);

    glNamedBufferStorage(VBO, sizeof(vertices), &vertices[0], 0);

    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(VAO, 0);

    glVertexArrayAttribBinding(VAO, 1, 0);
    glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glEnableVertexArrayAttrib(VAO, 1);

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 6 * sizeof(float));
}

void OpenGLRender::on_unrealize() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    renderingProgram.reset();

    Gtk::GLArea::on_unrealize();
}
