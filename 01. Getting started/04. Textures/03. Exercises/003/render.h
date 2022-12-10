#pragma once

#include <memory>

#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/glarea.h>

#include <epoxy/gl.h>

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

    Glib::RefPtr<Gtk::EventControllerKey> keyEvents;

    std::unique_ptr<Shader> renderingProgram;

    GLuint VBO = {};
    GLuint VAO = {};
    GLuint EBO = {};
    GLuint texture[2] = {};

    float mixLevel = 0.2f;
    static auto constexpr mixDelta = 0.01f;
};
