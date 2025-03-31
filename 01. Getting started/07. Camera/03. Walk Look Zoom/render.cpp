#include <gdk/gdkkeysyms.h>
#include <gdkmm/frameclock.h>
#include <gdkmm/pixbuf.h>
#include <giomm/resource.h>
#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/eventcontrollermotion.h>
#include <gtkmm/eventcontrollerscroll.h>
#include <gtkmm/glarea.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>
#include <glbinding/getProcAddress.h>

#include "render.h"

using namespace gl;

OpenGLRender::OpenGLRender(BaseObjectType* cobject,
                           const Glib::RefPtr<Gtk::Builder>& refBuilder)
  : Gtk::GLArea(cobject)
  , keyEvents(Gtk::EventControllerKey::create())
  , mouseMoveEvents(Gtk::EventControllerMotion::create())
  , scrollEvents(Gtk::EventControllerScroll::create())
{
    glbinding::initialize(glbinding::getProcAddress, true);

    add_controller(keyEvents);
    keyEvents->signal_key_pressed().connect(sigc::mem_fun(*this, &OpenGLRender::on_key_pressed), true);
    keyEvents->signal_key_released().connect(sigc::mem_fun(*this, &OpenGLRender::on_key_released), true);

    add_controller(mouseMoveEvents);
    mouseMoveEvents->signal_enter().connect(sigc::mem_fun(*this, &OpenGLRender::on_enter));
    mouseMoveEvents->signal_motion().connect(sigc::mem_fun(*this, &OpenGLRender::on_motion));
    mouseMoveEvents->signal_leave().connect(sigc::mem_fun(*this, &OpenGLRender::on_leave));

    add_controller(scrollEvents);
    scrollEvents->signal_scroll().connect(sigc::mem_fun(*this, &OpenGLRender::on_scroll), true);
    scrollEvents->set_flags(Gtk::EventControllerScroll::Flags::VERTICAL);

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

    auto display = get_display();
    auto seat = display->get_default_seat();

}

bool OpenGLRender::on_render(const Glib::RefPtr<Gdk::GLContext>& context) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTextureUnit(0, texture[0]);
    glBindTextureUnit(1, texture[1]);

    renderingProgram->use();

    auto view       = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    auto projection = glm::perspective(glm::radians(fov), float(get_width()) / get_height(), 0.1f, 100.0f);

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
    glCreateBuffers(1, &VBO);

    glNamedBufferStorage(VBO, sizeof(vertices), &vertices[0], GL_NONE_BIT);

    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(VAO, 0);

    glVertexArrayAttribBinding(VAO, 1, 0);
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glEnableVertexArrayAttrib(VAO, 1);

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(float));

    tickCallbackId = add_tick_callback(sigc::mem_fun(*this, &OpenGLRender::timer_event));
}

void OpenGLRender::on_unrealize() {
    remove_tick_callback(tickCallbackId);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(2, &texture[0]);
    renderingProgram.reset();

    Gtk::GLArea::on_unrealize();
}

bool OpenGLRender::on_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state) {
    auto processed = true;
    switch (keyval) {
        case GDK_KEY_w: {
            moveKeysPressed.forward = 1;
            break;
        }
        case GDK_KEY_s: {
            moveKeysPressed.back = 1;
            break;
        }
        case GDK_KEY_a: {
            moveKeysPressed.left = 1;
            break;
        }
        case GDK_KEY_d: {
            moveKeysPressed.right = 1;
            break;
        }
        default: {
            processed = false;
            break;
        }
    }

    if (processed) {
        cameraSpeedDir =
            float(moveKeysPressed.forward) * cameraFront
          - float(moveKeysPressed.back) * cameraFront
          + float(moveKeysPressed.right) * glm::normalize(glm::cross(cameraFront, cameraUp))
          - float(moveKeysPressed.left) * glm::normalize(glm::cross(cameraFront, cameraUp));
    }

    return processed;
}

void OpenGLRender::on_key_released(guint keyval, guint keycode, Gdk::ModifierType state) {
    auto processed = true;
    switch (keyval) {
        case GDK_KEY_w: {
            moveKeysPressed.forward = 0;
            break;
        }
        case GDK_KEY_s: {
            moveKeysPressed.back = 0;
            break;
        }
        case GDK_KEY_a: {
            moveKeysPressed.left = 0;
            break;
        }
        case GDK_KEY_d: {
            moveKeysPressed.right = 0;
            break;
        }
        default: {
            processed = false;
            break;
        }
    }

    if (processed) {
        cameraSpeedDir =
            float(moveKeysPressed.forward) * cameraFront
          - float(moveKeysPressed.back) * cameraFront
          + float(moveKeysPressed.right) * glm::normalize(glm::cross(cameraFront, cameraUp))
          - float(moveKeysPressed.left) * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
}

void OpenGLRender::on_enter(double x, double y) {
    startMousepos = {x, y};
}

void OpenGLRender::on_motion(double x, double y) {
    auto offset = double(mouseSensitivity) * (glm::dvec2{x, y} - startMousepos);

    yaw   = lastYaw + offset.x;
    pitch = lastPitch - offset.y;
    if (89.0f < pitch) {
        pitch = 89.0f;
    }
    if (-89.0f > pitch) {
        pitch = -89.0f;
    }
    cameraFront = glm::normalize(glm::vec3{
        std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch))
      , std::sin(glm::radians(pitch))
      , std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch))
      });
    queue_draw();
}

void OpenGLRender::on_leave() {
    lastYaw = yaw;
    lastPitch = pitch;
}

bool OpenGLRender::on_scroll(double x, double y) {
    fov -= y;

    if (1.0f > fov) {
        fov = 1.0f;
    }
    if (45.0f < fov) {
        fov = 45.0f;
    }
    queue_draw();

    return true;
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
            if (moveKeysPressed.is_moving()) {
                cameraPos += deltaTime * cameraSpeed * cameraSpeedDir;
                queue_draw();
            }
        }
    }

    return true;
}
