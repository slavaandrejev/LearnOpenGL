#include <gtk/gtk.hpp>

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

    shader->use();

    shader->set("view", view);
    shader->set("projection", projection);
    shader->set("viewPos", camera.GetPosition());

    model->Draw(*shader);

    return true;
}

void OpenGLRender::realize_() noexcept {
    GlBoundGlArea::realize_();

    shader = std::make_unique<Shader>(
        "/model-loading-vs.glsl", GL_VERTEX_SHADER,
        "/model-loading-fs.glsl", GL_FRAGMENT_SHADER);
    model = std::make_unique<Model>("/resources/objects/backpack/backpack.obj");

    auto modelMtx  = glm::mat4(1.0f);
    auto normalMtx = glm::transpose(glm::inverse(glm::mat3(modelMtx)));
    shader->set("model", modelMtx);
    shader->set("normalMatrix", normalMtx);

    shader->set("material.shininess", 32.0f);

    shader->set("pointLight.ambient", {0.05f, 0.05f, 0.05f});
    shader->set("pointLight.diffuse", {0.8f, 0.8f, 0.8f});
    shader->set("pointLight.specular", {1.0f, 1.0f, 1.0f});
    shader->set("pointLight.position", { 0.7f, 0.2f, 2.0f});
    shader->set("pointLight.constant", 1.0f);
    shader->set("pointLight.linear", 0.09f);
    shader->set("pointLight.quadratic", 0.032f);

    tickCallbackId = add_tick_callback(gi::mem_fun(&OpenGLRender::timer_event, this));
}

void OpenGLRender::unrealize_() noexcept {
    remove_tick_callback(tickCallbackId);
    model.reset();
    shader.reset();

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
