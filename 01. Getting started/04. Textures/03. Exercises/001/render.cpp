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

    glBindTextureUnit(0, texture[0]);
    glBindTextureUnit(1, texture[1]);
    renderingProgram->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    return true;
}

void OpenGLRender::on_realize() {
    Gtk::GLArea::on_realize();
    make_current();

    glCreateTextures(GL_TEXTURE_2D, 2, &texture[0]);

    auto contImg = Gdk::Pixbuf::create_from_resource("/container.jpg");
    glTextureParameteri(texture[0], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture[0], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture[0], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture[0], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(texture[0], 1, GL_RGB8, contImg->get_width(), contImg->get_height());
    glTextureSubImage2D(
        texture[0]
      , 0
      , 0
      , 0
      , contImg->get_width()
      , contImg->get_height()
      , GL_RGB
      , GL_UNSIGNED_BYTE
      , contImg->get_pixels());
    glGenerateTextureMipmap(texture[0]);
    contImg.reset();

    auto faceImg = Gdk::Pixbuf::create_from_resource("/awesomeface.png");
    glTexParameteri(texture[1], GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texture[1], GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(texture[1], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(texture[1], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(texture[1], 1, GL_RGBA8, faceImg->get_width(), faceImg->get_height());
    glTextureSubImage2D(
        texture[1]
      , 0
      , 0
      , 0
      , faceImg->get_width()
      , faceImg->get_height()
      , GL_RGBA
      , GL_UNSIGNED_BYTE
      , faceImg->get_pixels());
    glGenerateTextureMipmap(texture[1]);
    faceImg.reset();

    renderingProgram = std::make_unique<Shader>(
        "/vs.glsl", GL_VERTEX_SHADER,
        "/fs.glsl", GL_FRAGMENT_SHADER);

    renderingProgram->set("texture1", 0);
    renderingProgram->set("texture2", 1);

    float vertices[] = {
        // positions       // colors         // texture coords
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
       -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f  // top left
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
    glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glEnableVertexArrayAttrib(VAO, 1);

    glVertexArrayAttribBinding(VAO, 2, 0);
    glVertexArrayAttribFormat(VAO, 2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float));
    glEnableVertexArrayAttrib(VAO, 2);

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 8 * sizeof(float));
    glVertexArrayElementBuffer(VAO, EBO);
}

void OpenGLRender::on_unrealize() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(2, &texture[0]);
    renderingProgram.reset();

    Gtk::GLArea::on_unrealize();
}
