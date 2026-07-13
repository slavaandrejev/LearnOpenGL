#include <gtk/gtk.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/gl.h>

#include <load_texture.h>

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
}

bool OpenGLRender::render_(Gdk::GLContext context) noexcept {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTextureUnit(0, diffuseMap);

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
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightCubeShader->use();
    lightCubeShader->set("view", view);
    lightCubeShader->set("projection", projection);

    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    return true;
}

void OpenGLRender::realize_() noexcept {
    GlBoundGlArea::realize_();

    diffuseMap = LoadTextureFromPixbuf(Gdk::Pixbuf::new_from_resource("/container2.png"));

    lightingShader = std::make_unique<Shader>(
        "/diffuse-map-vs.glsl", GL_VERTEX_SHADER,
        "/diffuse-map-fs.glsl", GL_FRAGMENT_SHADER);
    lightCubeShader = std::make_unique<Shader>(
        "/light-cube-vs.glsl", GL_VERTEX_SHADER,
        "/light-cube-fs.glsl", GL_FRAGMENT_SHADER);

    lightingShader->set("material.diffuse", 0);
    lightingShader->set("material.specular", {0.5f, 0.5f, 0.5f});
    lightingShader->set("material.shininess", 64.0f);

    lightingShader->set("light.ambient", {0.2f, 0.2f, 0.2f});
    lightingShader->set("light.diffuse", {0.5f, 0.5f, 0.5f});
    lightingShader->set("light.specular", {1.0f, 1.0f, 1.0f});
    lightingShader->set("light.position", lightPos);

    auto cubeModel = glm::mat4(1.0f);
    auto normalMatrix = glm::transpose(glm::inverse(glm::mat3(cubeModel)));
    lightingShader->set("model", cubeModel);
    lightingShader->set("normalMatrix", normalMatrix);

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

    glNamedBufferStorage(VBO, sizeof(vertices), &vertices[0], GL_NONE_BIT);

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
