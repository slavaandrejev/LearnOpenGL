#pragma once

#include <memory>

#include <gtk/gtk.hpp>

#include <glboundglarea.h>
#include <gnamespaces.h>
#include <camera.h>
#include <model.h>

class OpenGLRender : public GlBoundGlArea
{
    friend struct WidgetClassDef::TypeInitData;
    friend struct GLAreaClassDef::TypeInitData;
public:
    OpenGLRender(const InitData &id);

    static GType get_type_() {
        return register_type_<OpenGLRender>("OpenGLRender", 0, {}, {}, {});
    }

private:
    bool render_(Gdk::GLContext context) noexcept override;
    void realize_() noexcept override;
    void unrealize_() noexcept override;

    bool on_key_pressed(Gtk::EventControllerKey, guint keyval, guint keycode, Gdk::ModifierType state);
    void on_key_released(Gtk::EventControllerKey, guint keyval, guint keycode, Gdk::ModifierType state);
    Gtk::EventControllerKey keyEvents;

    Gtk::EventControllerMotion mouseMoveEvents;
    Gtk::EventControllerScroll scrollEvents;
    Gtk::GestureClick clickEvents;

    guint tickCallbackId{};
    bool timer_event(Gtk::Widget, Gdk::FrameClock frame_clock);

    std::unique_ptr<Model>  model;
    std::unique_ptr<Shader> shader;

    bool mouseGrabbed = false;
    Camera camera{
        {-2.14f, -0.07f, 5.36f}
      , {0.0f, 1.0f, 0.0f}
      , {0.33f, 0.08f, -0.94f}
      };

    gint64 startTime = -1;
    float  curTime   = -1.0f;
};
