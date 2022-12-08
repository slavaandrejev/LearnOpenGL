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
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    return true;
}

void OpenGLRender::on_realize() {
    Gtk::GLArea::on_realize();
    make_current();

    renderingProgram = CompileShaders();

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
    glNamedBufferStorage(VBO, sizeof(vertices), &vertices[0], 0);
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 3 * sizeof(float));
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);
    glEnableVertexArrayAttrib(VAO, 0);
}

void OpenGLRender::on_unrealize() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(renderingProgram);

    Gtk::GLArea::on_unrealize();
}

GLuint OpenGLRender::CompileShaders() {
    auto compileFromResource = [](auto path, auto shaderType) -> GLuint {
        auto bytes = Gio::Resource::lookup_data_global(path);
        auto size = gsize{};
        auto data = reinterpret_cast<const char*>(bytes->get_data(size));

        auto shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &data, nullptr);
        glCompileShader(shader);

        return shader;
    };

    auto vertexShader   = compileFromResource("/program.vs.glsl", GL_VERTEX_SHADER);
    auto fragmentShader = compileFromResource("/program.fs.glsl", GL_FRAGMENT_SHADER);

    auto program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}
