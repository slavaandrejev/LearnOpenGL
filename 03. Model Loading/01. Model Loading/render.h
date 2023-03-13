#pragma once

#include <memory>

#include <gdkmm/frameclock.h>
#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/eventcontrollermotion.h>
#include <gtkmm/eventcontrollerscroll.h>
#include <gtkmm/gestureclick.h>
#include <gtkmm/glarea.h>

#include <epoxy/gl.h>

#include <camera.h>
#include <model.h>

class OpenGLRender : public Gtk::GLArea
{
public:
    OpenGLRender(BaseObjectType* cobject,
                 const Glib::RefPtr<Gtk::Builder>& refBuilder);
private:
    bool on_render(const Glib::RefPtr<Gdk::GLContext>& context) override;
    void on_realize() override;
    void on_unrealize() override;

    bool on_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
    void on_key_released(guint keyval, guint keycode, Gdk::ModifierType state);
    Glib::RefPtr<Gtk::EventControllerKey> keyEvents;

    Glib::RefPtr<Gtk::EventControllerMotion> mouseMoveEvents;
    Glib::RefPtr<Gtk::EventControllerScroll> scrollEvents;
    Glib::RefPtr<Gtk::GestureClick> clickEvents;

    bool timer_event(const Glib::RefPtr<Gdk::FrameClock>&);
    guint tick_callback_id;

    std::unique_ptr<Model>  model;
    std::unique_ptr<Shader> shader;

    bool mouseGrabbed = false;
    Camera camera{
        {4.07f, -0.02f, -4.43f}
      , {0.0f, 1.0f, 0.0f}
      , {-0.72f, 0.06f, 0.69f}
      };

    gint64 startTime = -1;
    float  curTime   = -1.0f;
};
