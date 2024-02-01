#pragma once

#include <memory>
#include <vector>

#include <gdkmm/frameclock.h>
#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/eventcontrollermotion.h>
#include <gtkmm/eventcontrollerscroll.h>
#include <gtkmm/gestureclick.h>
#include <gtkmm/glarea.h>

#include <epoxy/gl.h>

#include <glm/glm.hpp>

#include <camera.h>
#include <shader.h>

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

    guint tickCallbackId{};
    bool timer_event(const Glib::RefPtr<Gdk::FrameClock>&);

    std::unique_ptr<Shader> lightingShader;

    GLuint VBO = {};
    GLuint cubeVAO = {};
    GLuint diffuseMap = {};
    GLuint specularMap = {};

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
