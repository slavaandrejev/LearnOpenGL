#pragma once

#include <memory>
#include <vector>

#include <gdkmm/frameclock.h>
#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/eventcontrollermotion.h>
#include <gtkmm/eventcontrollerscroll.h>
#include <gtkmm/glarea.h>

#include <epoxy/gl.h>

#include <glm/glm.hpp>

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

    void on_enter(double x, double y);
    void on_motion(double x, double y);
    void on_leave();
    Glib::RefPtr<Gtk::EventControllerMotion> mouseMoveEvents;

    bool on_scroll(double x, double y);
    Glib::RefPtr<Gtk::EventControllerScroll> scrollEvents;

    bool timer_event(const Glib::RefPtr<Gdk::FrameClock>&);

    std::unique_ptr<Shader> renderingProgram;

    GLuint VBO = {};
    GLuint VAO = {};
    GLuint texture[2] = {};

    std::vector<glm::vec3> cubePositions;

    float fov = 45.0f;
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    struct MoveKeysPressed {
        uint8_t forward:1    = 0;
        uint8_t back:1  = 0;
        uint8_t left:1  = 0;
        uint8_t right:1 = 0;

        bool is_moving() const {
            return 0 != forward || 0 != back || 0 != left || 0 != right;
        }
    };
    MoveKeysPressed moveKeysPressed;

    float cameraSpeed = 0.05f;
    glm::vec3 cameraSpeedDir = glm::vec3(0.0f, 0.0f, 0.0f);

    static constexpr float mouseSensitivity = 0.1f;
    glm::dvec2 startMousepos;
    float yaw       = -90.0f;
    float pitch     = 0.0f;
    float lastYaw   = -90.0f;
    float lastPitch = 0.0f;

    gint64 startTime = -1;
    float  curTime   = -1.0f;
};
