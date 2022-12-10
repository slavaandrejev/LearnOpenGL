#include <giomm/resource.h>
#include <glibmm/refptr.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/builder.h>
#include <gtkmm/glarea.h>

#include <epoxy/gl.h>

#include "render.h"

OpenGLRender::OpenGLRender(BaseObjectType* cobject,
                           const Glib::RefPtr<Gtk::Builder>& refBuilder)
  : Gtk::GLArea(cobject)
{}

bool OpenGLRender::on_render(const Glib::RefPtr<Gdk::GLContext>& context) {
    const GLfloat color[] = {0.2f, 0.3f, 0.3f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, color);

    glBindTexture(GL_TEXTURE_2D, texture);
    renderingProgram->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    return true;
}

void OpenGLRender::on_realize() {
    Gtk::GLArea::on_realize();
    make_current();

    renderingProgram = std::make_unique<Shader>(
        "/vs.glsl", GL_VERTEX_SHADER,
        "/fs.glsl", GL_FRAGMENT_SHADER);

    auto contImg = Gdk::Pixbuf::create_from_resource("/container.jpg");
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureStorage2D(texture, 1, GL_RGB8, contImg->get_width(), contImg->get_height());
    glTextureSubImage2D(
        texture
      , 0
      , 0
      , 0
      , contImg->get_width()
      , contImg->get_height()
      , GL_RGB
      , GL_UNSIGNED_BYTE
      , contImg->get_pixels());
    glGenerateTextureMipmap(texture);
    contImg.reset();

    float vertices[] = {
        // positions       // texture coords
        0.5f,  0.5f, 0.0f, 0.55f, 0.55f, // top right
        0.5f, -0.5f, 0.0f, 0.55f, 0.45f, // bottom right
       -0.5f, -0.5f, 0.0f, 0.45f, 0.45f, // bottom left
       -0.5f,  0.5f, 0.0f, 0.45f, 0.55f  // top left
    };
    GLuint indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);
    glCreateBuffers(1, &EBO);

    glNamedBufferStorage(VBO, sizeof(vertices), &vertices[0], 0);
    glNamedBufferStorage(EBO, sizeof(indices), &indices[0], 0);

    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(VAO, 0);

    glVertexArrayAttribBinding(VAO, 1, 0);
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glEnableVertexArrayAttrib(VAO, 1);

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(float));

    glVertexArrayElementBuffer(VAO, EBO);
}

void OpenGLRender::on_unrealize() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture);
    renderingProgram.reset();

    Gtk::GLArea::on_unrealize();
}
