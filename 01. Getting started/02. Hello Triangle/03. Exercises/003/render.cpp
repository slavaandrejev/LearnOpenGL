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

    renderingProgram[0]->use();
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    renderingProgram[1]->use();
    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    return true;
}

void OpenGLRender::on_realize() {
    Gtk::GLArea::on_realize();
    make_current();

    renderingProgram = {
        std::make_unique<Shader>(
            "/vs.glsl", GL_VERTEX_SHADER
          , "/fs-01.glsl", GL_FRAGMENT_SHADER
          )
      ,
        std::make_unique<Shader>(
            "/vs.glsl", GL_VERTEX_SHADER
          , "/fs-02.glsl", GL_FRAGMENT_SHADER
          )
      };

    float vertices[] = {
       -0.9f, -0.9f, 0.0f,
       -0.1f, -0.9f, 0.0f,
       -0.1f,  0.9f, 0.0f,
        0.1f, -0.9f, 0.0f,
        0.9f, -0.9f, 0.0f,
        0.9f,  0.9f, 0.0f
    };

    glCreateVertexArrays(2, &VAO[0]);
    glCreateBuffers(2, &VBO[0]);

    glNamedBufferStorage(VBO[0], sizeof(vertices) / 2, &vertices[0], GL_NONE_BIT);
    glVertexArrayVertexBuffer(VAO[0], 0, VBO[0], 0, 3 * sizeof(float));
    glVertexArrayAttribFormat(VAO[0], 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO[0], 0, 0);
    glEnableVertexArrayAttrib(VAO[0], 0);

    glNamedBufferStorage(VBO[1], sizeof(vertices) / 2, &vertices[9], GL_NONE_BIT);
    glVertexArrayVertexBuffer(VAO[1], 0, VBO[1], 0, 3 * sizeof(float));
    glVertexArrayAttribFormat(VAO[1], 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO[1], 0, 0);
    glEnableVertexArrayAttrib(VAO[1], 0);
}

void OpenGLRender::on_unrealize() {
    glDeleteVertexArrays(2, &VAO[0]);
    glDeleteBuffers(2, &VBO[0]);
    for (auto &&rp : renderingProgram) {
        rp.reset();
    }

    Gtk::GLArea::on_unrealize();
}
