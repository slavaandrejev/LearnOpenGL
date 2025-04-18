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

#include <glbinding/gl/gl.h>

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
    std::unique_ptr<Shader> lightCubeShader;

    gl::GLuint VBO = {};
    gl::GLuint cubeVAO = {};
    gl::GLuint lightCubeVAO = {};
    gl::GLuint diffuseMap = {};
    gl::GLuint specularMap = {};

    glm::vec3 lightPos{1.2f, 1.0f, 2.0f};
    std::vector<glm::vec3> cubePositions;

    bool mouseGrabbed = false;
    Camera camera{
        {-1.82f, -1.05f, 4.19f}
      , {0.0f, 1.0f, 0.0f}
      , {0.41f, 0.21f, -0.89f}
      };

    gint64 startTime = -1;
    float  curTime   = -1.0f;
};
