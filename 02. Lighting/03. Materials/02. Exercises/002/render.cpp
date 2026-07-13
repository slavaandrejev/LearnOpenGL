#include <gtk/gtk.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/gl.h>

#include "render.h"

using namespace gl;

OpenGLRender::OpenGLRender(const InitData &id)
  : GlBoundGlArea(id, "OpenGLRender")
  , keyEvents(Gtk::EventControllerKey::new_())
  , mouseMoveEvents(Gtk::EventControllerMotion::new_())
  , scrollEvents(Gtk::EventControllerScroll::new_(Gtk::EventControllerScrollFlags::VERTICAL_))
  , clickEvents(Gtk::GestureClick::new_())
{
    add_controller(keyEvents);
    keyEvents.signal_key_pressed().connect(gi::mem_fun(&OpenGLRender::on_key_pressed, this));
    keyEvents.signal_key_released().connect(gi::mem_fun(&OpenGLRender::on_key_released, this));

    add_controller(mouseMoveEvents);
    mouseMoveEvents.signal_motion().connect([&](Gtk::EventControllerMotion, gdouble x, gdouble y) {
        if (mouseGrabbed) {
            camera.OnPointerMotion(x, y);
            queue_draw();
        }
    });

    add_controller(scrollEvents);
    scrollEvents.signal_scroll().connect([&](Gtk::EventControllerScroll, gdouble x, gdouble y) {
        camera.OnScroll(y);
        queue_draw();
        return true;
    });

    add_controller(clickEvents);
    clickEvents.signal_pressed().connect([&](Gtk::GestureClick, int n, gdouble x, gdouble y) {
        camera.OnPointerEnter(x, y);
        mouseGrabbed = true;
        return true;
    });
    clickEvents.signal_released().connect([&](Gtk::GestureClick, int n, gdouble x, gdouble y) {
        camera.OnPointerLeave();
        mouseGrabbed = false;
        return true;
    });

    set_has_depth_buffer(TRUE);

    cubes = {
        {
            .pos       = {0.0f, 0.0f, 0.0f}
          , .ambient   = {0.0215f, 0.1745f, 0.0215f}
          , .diffuse   = {0.07568f, 0.61424f, 0.07568f}
          , .specular  = {0.633f, 0.727811f, 0.633f}
          , .shininess = 0.6f * 128
          }
      , {
            .pos       = {0.0f, 1.1f, 0.0f}
          , .ambient   = {0.135f, 0.2225f, 0.1575}
          , .diffuse   = {0.54f, 0.89f, 0.63f}
          , .specular  = {0.316228f, 0.316228f, 0.316228}
          , .shininess = 0.1f * 128
          }
      , {
            .pos       = {0.0f, -1.1f, 0.0f}
          , .ambient   = {0.05375f, 0.05f, 0.06625f}
          , .diffuse   = {0.18275f, 0.17f, 0.22525f}
          , .specular  = {0.332741f, 0.328634f, 0.346435f}
          , .shininess = 0.3f * 128
          }
      , {
            .pos       = {-1.1f, -1.1f, 0.0f}
          , .ambient   = {0.25f, 0.20725f, 0.20725f}
          , .diffuse   = {1.0f, 0.829f, 0.829f}
          , .specular  = {0.296648f, 0.296648f, 0.296648f}
          , .shininess = 0.088f * 128
          }
      , {
            .pos       = {1.1f, -1.1f, 0.0f}
          , .ambient   = {0.1745f, 0.01175f, 0.01175f}
          , .diffuse   = {0.61424f, 0.04136f, 0.04136f}
          , .specular  = {0.727811f, 0.626959f, 0.626959f}
          , .shininess = 0.6f * 128
          }
      , {
            .pos       = {1.1f, 1.1f, 0.0f}
          , .ambient   = {0.1f, 0.18725f, 0.1745f}
          , .diffuse   = {0.396f, 0.74151f, 0.69102f}
          , .specular  = {0.297254f, 0.30829f, 0.306678f}
          , .shininess = 0.1f * 128
          }
      , {
            .pos       = {-1.1f, 1.1f, 0.0f}
          , .ambient   = {0.329412f, 0.223529f, 0.027451f}
          , .diffuse   = {0.780392f, 0.568627f, 0.113725f}
          , .specular  = {0.992157f, 0.941176f, 0.807843f}
          , .shininess = 0.21794872f * 128
          }
      , {
            .pos       = {-1.1f, 0.0f, 0.0f}
          , .ambient   = {0.2125f, 0.1275f, 0.054f}
          , .diffuse   = {0.714f, 0.4284f, 0.18144f}
          , .specular  = {0.393548f, 0.271906f, 0.166721f}
          , .shininess = 0.2f * 128
          }
      , {
            .pos       = {1.1f, 0.0f, 0.0f}
          , .ambient   = {0.25f, 0.25f, 0.25f}
          , .diffuse   = {0.4f, 0.4f, 0.4f}
          , .specular  = {0.774597f, 0.774597f, 0.774597f}
          , .shininess = 0.6f * 128
          }
      };
}

bool OpenGLRender::render_(Gdk::GLContext context) noexcept {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    for (auto &&cube : cubes) {
        lightingShader->set("material.ambient", cube.ambient);
        lightingShader->set("material.diffuse", cube.diffuse);
        lightingShader->set("material.specular", cube.specular);
        lightingShader->set("material.shininess", cube.shininess);

        auto cubeModel = glm::translate(glm::mat4(1.0f), cube.pos);
        auto normalMatrix = glm::transpose(glm::inverse(glm::mat3(cubeModel)));
        lightingShader->set("model", cubeModel);
        lightingShader->set("normalMatrix", normalMatrix);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    lightCubeShader->use();
    lightCubeShader->set("view", view);
    lightCubeShader->set("projection", projection);
    lightCubeShader->set("lightColor", {1.0f, 1.0f, 1.0f});

    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    return true;
}

void OpenGLRender::realize_() noexcept {
    GlBoundGlArea::realize_();

    lightingShader = std::make_unique<Shader>(
        "/ex-002-vs.glsl", GL_VERTEX_SHADER,
        "/ex-002-fs.glsl", GL_FRAGMENT_SHADER);
    lightCubeShader = std::make_unique<Shader>(
        "/light-cube-vs.glsl", GL_VERTEX_SHADER,
        "/light-cube-fs.glsl", GL_FRAGMENT_SHADER);

    lightingShader->set("light.specular", {1.0f, 1.0f, 1.0f});
    lightingShader->set("light.ambient", {1.0f, 1.0f, 1.0f});
    lightingShader->set("light.diffuse", {1.0f, 1.0f, 1.0f});
    lightingShader->set("light.position", lightPos);

    auto lightModel = glm::translate(glm::mat4(1.0f), lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));
    lightCubeShader->set("model", lightModel);

    float vertices[] = {
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    glCreateVertexArrays(1, &cubeVAO);
    glCreateVertexArrays(1, &lightCubeVAO);
    glCreateBuffers(1, &VBO);

    glNamedBufferStorage(VBO, sizeof(vertices), &vertices[0], GL_NONE_BIT);

    glVertexArrayAttribBinding(cubeVAO, 0, 0);
    glVertexArrayAttribFormat(cubeVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(cubeVAO, 0);

    glVertexArrayAttribBinding(cubeVAO, 1, 0);
    glVertexArrayAttribFormat(cubeVAO, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glEnableVertexArrayAttrib(cubeVAO, 1);

    glVertexArrayVertexBuffer(cubeVAO, 0, VBO, 0, 6 * sizeof(float));

    glVertexArrayAttribBinding(lightCubeVAO, 0, 0);
    glVertexArrayAttribFormat(lightCubeVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(lightCubeVAO, 0);
    glVertexArrayVertexBuffer(lightCubeVAO, 0, VBO, 0, 6 * sizeof(float));

    tickCallbackId = add_tick_callback(gi::mem_fun(&OpenGLRender::timer_event, this));
}

void OpenGLRender::unrealize_() noexcept {
    remove_tick_callback(tickCallbackId);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    lightingShader.reset();

    GlBoundGlArea::unrealize_();
}

bool OpenGLRender::on_key_pressed(Gtk::EventControllerKey, guint keyval, guint keycode, Gdk::ModifierType state) {
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

void OpenGLRender::on_key_released(Gtk::EventControllerKey, guint keyval, guint keycode, Gdk::ModifierType state) {
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

bool OpenGLRender::timer_event(Gtk::Widget, Gdk::FrameClock frame_clock) {
    if (0 > startTime) {
        startTime = frame_clock.get_frame_time();
        curTime   = 0;
    } else {
        auto frameTime = frame_clock.get_frame_time();
        curTime = 1e-6f * (frameTime - startTime);

        auto frame = frame_clock.get_frame_counter();
        auto historyStart = frame_clock.get_history_start();
        auto histLen = frame - historyStart;
        if (0 < histLen) {
            auto prevTimings = frame_clock.get_timings(frame - histLen);
            auto prevTime = prevTimings.get_frame_time();
            auto deltaTime = 1e-6f * (frameTime - prevTime);
            if (camera.IsMoving()) {
                camera.TimeTick(deltaTime);
                queue_draw();
            }
        }
    }

    return true;
}
