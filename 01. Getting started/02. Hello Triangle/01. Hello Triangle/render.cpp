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
    glDrawArrays(GL_TRIANGLES, 0, 3);

    return true;
}

void OpenGLRender::on_realize() {
    Gtk::GLArea::on_realize();
    make_current();

    renderingProgram = CompileShaders();

    float vertices[] = {
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered
    // VBO as the vertex attribute's bound vertex buffer object so afterwards we
    // can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // You can unbind the VAO afterwards so other VAO calls won't accidentally
    // modify this VAO, but this rarely happens. Modifying other VAOs requires a
    // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
    // VBOs) when it's not directly necessary.
    glBindVertexArray(0);
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
