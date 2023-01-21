#include <gdk/gdkkeysyms.h>
#include <gdkmm/frameclock.h>
#include <gdkmm/pixbuf.h>
#include <giomm/resource.h>
#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/eventcontrollermotion.h>
#include <gtkmm/eventcontrollerscroll.h>
#include <gtkmm/gestureclick.h>
#include <gtkmm/glarea.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <epoxy/gl.h>

#include "render.h"

GLuint LoadTextureFromResources(const std::string &path);

OpenGLRender::OpenGLRender(BaseObjectType* cobject,
                           const Glib::RefPtr<Gtk::Builder>& refBuilder)
  : Gtk::GLArea(cobject)
  , keyEvents(Gtk::EventControllerKey::create())
  , mouseMoveEvents(Gtk::EventControllerMotion::create())
  , scrollEvents(Gtk::EventControllerScroll::create())
  , clickEvents(Gtk::GestureClick::create())
{
    add_controller(keyEvents);
    keyEvents->signal_key_pressed().connect(sigc::mem_fun(*this, &OpenGLRender::on_key_pressed), true);
    keyEvents->signal_key_released().connect(sigc::mem_fun(*this, &OpenGLRender::on_key_released), true);

    add_controller(mouseMoveEvents);
    mouseMoveEvents->signal_motion().connect([&](double x, double y) {
        if (mouseGrabbed) {
            camera.OnPointerMotion(x, y);
            queue_draw();
        }
    });

    add_controller(scrollEvents);
    scrollEvents->signal_scroll().connect([&](double x, double y) {
        camera.OnScroll(y);
        queue_draw();

        return true;
    }, true);
    scrollEvents->set_flags(Gtk::EventControllerScroll::Flags::VERTICAL);

    add_controller(clickEvents);
    clickEvents->signal_pressed().connect([&](int n, double x, double y) {
        camera.OnPointerEnter(x, y);
        mouseGrabbed = true;
    });
    clickEvents->signal_released().connect([&](int n, double x, double y) {
        camera.OnPointerLeave();
        mouseGrabbed = false;
    });

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
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTextureUnit(0, diffuseMap);
    glBindTextureUnit(1, specularMap);

    auto view       = camera.GetViewMatrix();
    auto projection = glm::perspective(
        glm::radians(camera.GetZoom())
      , float(get_width()) / get_height()
      , 0.1f
      , 100.0f);

    lightingShader->use();
    lightingShader->set("view", view);
    lightingShader->set("projection", projection);
    lightingShader->set("viewPos", camera.GetPosition());

    glBindVertexArray(cubeVAO);
    for (auto i = size_t{}; cubePositions.size() > i; ++i) {
        auto model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
        auto angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        auto normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        lightingShader->set("model", model);
        lightingShader->set("normalMatrix", normalMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    lightCubeShader->use();
    lightCubeShader->set("view", view);
    lightCubeShader->set("projection", projection);

    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    return true;
}

void OpenGLRender::on_realize() {
    Gtk::GLArea::on_realize();
    make_current();

    diffuseMap = LoadTextureFromResources("/container2.png");
    specularMap = LoadTextureFromResources("/container2_specular.png");

    lightingShader = std::make_unique<Shader>(
        "/point-vs.glsl", GL_VERTEX_SHADER,
        "/point-fs.glsl", GL_FRAGMENT_SHADER);
    lightCubeShader = std::make_unique<Shader>(
        "/light-cube-vs.glsl", GL_VERTEX_SHADER,
        "/light-cube-fs.glsl", GL_FRAGMENT_SHADER);

    lightingShader->set("material.diffuse", 0);
    lightingShader->set("material.specular", 1);
    lightingShader->set("material.shininess", 64.0f);

    lightingShader->set("light.ambient", {0.2f, 0.2f, 0.2f});
    lightingShader->set("light.diffuse", {0.5f, 0.5f, 0.5f});
    lightingShader->set("light.specular", {1.0f, 1.0f, 1.0f});
    lightingShader->set("light.position", lightPos);
    lightingShader->set("light.constant",  1.0f);
    lightingShader->set("light.linear",    0.09f);
    lightingShader->set("light.quadratic", 0.032f);

    auto lightModel = glm::translate(glm::mat4(1.0f), lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));
    lightCubeShader->set("model", lightModel);

    float vertices[] = {
        // positions         // normals          // texture coords
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
    };

    glCreateVertexArrays(1, &cubeVAO);
    glCreateVertexArrays(1, &lightCubeVAO);
    glCreateBuffers(1, &VBO);

    glNamedBufferStorage(VBO, sizeof(vertices), &vertices[0], 0);

    glVertexArrayAttribBinding(cubeVAO, 0, 0);
    glVertexArrayAttribFormat(cubeVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(cubeVAO, 0);

    glVertexArrayAttribBinding(cubeVAO, 1, 0);
    glVertexArrayAttribFormat(cubeVAO, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glEnableVertexArrayAttrib(cubeVAO, 1);

    glVertexArrayAttribBinding(cubeVAO, 2, 0);
    glVertexArrayAttribFormat(cubeVAO, 2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float));
    glEnableVertexArrayAttrib(cubeVAO, 2);

    glVertexArrayVertexBuffer(cubeVAO, 0, VBO, 0, 8 * sizeof(float));

    glVertexArrayAttribBinding(lightCubeVAO, 0, 0);
    glVertexArrayAttribFormat(lightCubeVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(lightCubeVAO, 0);
    glVertexArrayVertexBuffer(lightCubeVAO, 0, VBO, 0, 8 * sizeof(float));

    add_tick_callback(sigc::mem_fun(*this, &OpenGLRender::timer_event));
}

void OpenGLRender::on_unrealize() {
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    lightingShader.reset();

    Gtk::GLArea::on_unrealize();
}

bool OpenGLRender::on_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state) {
    auto processed = true;
    switch (keyval) {
        case GDK_KEY_w:
        case GDK_KEY_W:         { camera.StartForward(); break; }
        case GDK_KEY_s:
        case GDK_KEY_S:         { camera.StartBack(); break; }
        case GDK_KEY_a:
        case GDK_KEY_A:         { camera.StartLeft(); break; }
        case GDK_KEY_d:
        case GDK_KEY_D:         { camera.StartRight(); break; }
        case GDK_KEY_space:     { camera.StartUp(); break; }
        case GDK_KEY_Control_L: { camera.StartDown(); break; }
        case GDK_KEY_Shift_L:   { camera.HiSpeed(); break; }
        default: { processed = false; break; }
    }

    return processed;
}

void OpenGLRender::on_key_released(guint keyval, guint keycode, Gdk::ModifierType state) {
    switch (keyval) {
        case GDK_KEY_w:
        case GDK_KEY_W:         { camera.StopForward(); break; }
        case GDK_KEY_s:
        case GDK_KEY_S:         { camera.StopBack(); break; }
        case GDK_KEY_a:
        case GDK_KEY_A:         { camera.StopLeft(); break; }
        case GDK_KEY_d:
        case GDK_KEY_D:         { camera.StopRight(); break; }
        case GDK_KEY_space:     { camera.StopUp(); break; }
        case GDK_KEY_Control_L: { camera.StopDown(); break; }
        case GDK_KEY_Shift_L:   { camera.LowSpeed(); break; }
    }
}

bool OpenGLRender::timer_event(const Glib::RefPtr<Gdk::FrameClock>& frameClock) {
    if (0 > startTime) {
        startTime = frameClock->get_frame_time();
        curTime   = 0;
    } else {
        auto frameTime = frameClock->get_frame_time();
        curTime = 1e-6f * (frameTime - startTime);

        auto frame = frameClock->get_frame_counter();
        auto historyStart = frameClock->get_history_start();
        auto histLen = frame - historyStart;
        if (0 < histLen) {
            auto prevTimings = frameClock->get_timings(frame - histLen);
            auto prevTime = prevTimings->get_frame_time();
            auto deltaTime = 1e-6f * (frameTime - prevTime);
            if (camera.IsMoving()) {
                camera.TimeTick(deltaTime);
                queue_draw();
            }
        }
    }

    return true;
}

GLuint LoadTextureFromResources(const std::string &path) {
    auto img = Gdk::Pixbuf::create_from_resource(path);
    auto tex = GLuint{};

    glCreateTextures(GL_TEXTURE_2D, 1, &tex);
    auto format         = GLenum{};
    auto internalFormat = GLenum{};
    switch (img->get_n_channels()) {
        case 1: {
            format =  GL_RED;
            internalFormat = GL_R8;
            break;
        }
        case 3: {
            format =  GL_RGB;
            internalFormat = GL_RGB8;
            break;
        }
        case 4: {
            format =  GL_RGBA;
            internalFormat = GL_RGBA8;
            break;
        }
    }
    glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(tex, 1, internalFormat, img->get_width(), img->get_height());
    glTextureSubImage2D(
        tex
      , 0
      , 0
      , 0
      , img->get_width()
      , img->get_height()
      , format
      , GL_UNSIGNED_BYTE
      , img->get_pixels());
    glGenerateTextureMipmap(tex);

    return tex;
}
