#include <gdkmm/pixbuf.h>
#include <giomm/resource.h>
#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/glarea.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <epoxy/gl.h>

#include "render.h"

OpenGLRender::OpenGLRender(BaseObjectType* cobject,
                           const Glib::RefPtr<Gtk::Builder>& refBuilder)
  : Gtk::GLArea(cobject)
{
    // note that after setting this property it's not necessary to call
    // glEnable(GL_DEPTH_TEST);
    set_has_depth_buffer();

    cubePositions = {
        { 0.0f,  0.0f,   0.0f}
      , { 2.0f,  5.0f, -15.0f}
      , {-1.5f, -2.2f,  -2.5f}
      , {-3.8f, -2.0f, -12.3f}
      , { 2.4f, -0.4f,  -3.5f}
      , {-1.7f,  3.0f,  -7.5f}
      , { 1.3f, -2.0f,  -2.5f}
      , { 1.5f,  2.0f,  -2.5f}
      , { 1.5f,  0.2f,  -1.5f}
      , {-1.3f,  1.0f,  -1.5f}
      };
}

bool OpenGLRender::on_render(const Glib::RefPtr<Gdk::GLContext>& context) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTextureUnit(0, texture[0]);
    glBindTextureUnit(1, texture[1]);

    renderingProgram->use();

    auto view       = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    auto projection = glm::perspective(glm::radians(45.0f), float(get_width()) / get_height(), 0.1f, 100.0f);

    renderingProgram->set("view", view);
    renderingProgram->set("projection", projection);

    glBindVertexArray(VAO);

    for (auto i = size_t{}; cubePositions.size() > i; ++i) {
        auto model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
        auto angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        renderingProgram->set("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    return true;
}

void OpenGLRender::on_realize() {
    Gtk::GLArea::on_realize();
    make_current();

    glCreateTextures(GL_TEXTURE_2D, 2, &texture[0]);

    auto contImg = Gdk::Pixbuf::create_from_resource("/container.jpg");
    glTextureParameteri(texture[0], GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture[0], GL_TEXTURE_WRAP_T, GL_REPEAT);
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
    glTextureParameteri(texture[1], GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture[1], GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture[1], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture[1], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

    renderingProgram->use();
    renderingProgram->set("texture1", 0);
    renderingProgram->set("texture2", 1);

    float vertices[] = {
        // positions        // texture coords
       -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
    };

    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glCreateBuffers(1, &VBO);

    glNamedBufferStorage(VBO, sizeof(vertices), &vertices[0], 0);

    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(VAO, 0);

    glVertexArrayAttribBinding(VAO, 1, 0);
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glEnableVertexArrayAttrib(VAO, 1);

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(float));
}

void OpenGLRender::on_unrealize() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(2, &texture[0]);
    renderingProgram.reset();

    Gtk::GLArea::on_unrealize();
}
