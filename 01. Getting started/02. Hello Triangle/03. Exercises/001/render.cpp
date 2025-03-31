#include <giomm/resource.h>
#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/glarea.h>

#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>
#include <glbinding/getProcAddress.h>

#include "render.h"

using namespace gl;

OpenGLRender::OpenGLRender(BaseObjectType* cobject,
                           const Glib::RefPtr<Gtk::Builder>& refBuilder)
  : Gtk::GLArea(cobject)
{
    glbinding::initialize(glbinding::getProcAddress, true);
}

bool OpenGLRender::on_render(const Glib::RefPtr<Gdk::GLContext>& context) {
    const GLfloat color[] = {0.2f, 0.3f, 0.3f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, color);

    renderingProgram->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    return true;
}

void OpenGLRender::on_realize() {
    Gtk::GLArea::on_realize();
    make_current();

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

void OpenGLRender::on_unrealize() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    renderingProgram.reset();

    Gtk::GLArea::on_unrealize();
}
