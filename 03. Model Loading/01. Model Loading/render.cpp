#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/glarea.h>

#include <epoxy/gl.h>

#include "render.h"

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
}

bool OpenGLRender::on_render(const Glib::RefPtr<Gdk::GLContext>& context) {
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

    model->Draw(*shader);

    return true;
}

void OpenGLRender::on_realize() {
    Gtk::GLArea::on_realize();
    make_current();

    shader = std::make_unique<Shader>(
        "/model-loading-vs.glsl", GL_VERTEX_SHADER,
        "/model-loading-fs.glsl", GL_FRAGMENT_SHADER);
    model = std::make_unique<Model>("resources/objects/backpack/backpack.obj");

    auto modelMtx = glm::mat4(1.0f);
    shader->set("model", modelMtx);

    tick_callback_id = add_tick_callback(sigc::mem_fun(*this, &OpenGLRender::timer_event));
}

void OpenGLRender::on_unrealize() {
    model.reset();
    shader.reset();
    remove_tick_callback(tick_callback_id);

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
