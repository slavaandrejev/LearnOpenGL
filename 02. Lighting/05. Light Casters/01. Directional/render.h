#pragma once

#include <memory>
#include <vector>

#include <gtk/gtk.hpp>

#include <glbinding/gl/gl.h>

#include <glm/glm.hpp>

#include <glboundglarea.h>
#include <gnamespaces.h>
#include <camera.h>
#include <shader.h>

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

    std::unique_ptr<Shader> lightingShader;

    gl::GLuint VBO = {};
    gl::GLuint cubeVAO = {};
    gl::GLuint diffuseMap = {};
    gl::GLuint specularMap = {};

    std::vector<glm::vec3> cubePositions;

    bool mouseGrabbed = false;
    Camera camera{
        {-5.86f, 2.36f, -4.75f}
      , {0.0f, 1.0f, 0.0f}
      , {0.87f, -0.23f, 0.44f}
      };

    gint64 startTime = -1;
    float  curTime   = -1.0f;
};
