#pragma once

#include <memory>
#include <vector>

#include <gtk/gtk.hpp>

#include <glbinding/gl/gl.h>

#include <glm/glm.hpp>

#include <glboundglarea.h>
#include <gnamespaces.h>
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

    guint tickCallbackId{};
    bool timer_event(Gtk::Widget, Gdk::FrameClock frame_clock);

    std::unique_ptr<Shader> renderingProgram;

    gl::GLuint VBO = {};
    gl::GLuint VAO = {};
    gl::GLuint texture[2] = {};

    std::vector<glm::vec3> cubePositions;

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

    gint64 startTime = -1;
    float  curTime   = -1.0f;
};
