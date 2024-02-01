#pragma once

#include <memory>

#include <gdkmm/frameclock.h>
#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
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

    guint tickCallbackId{};
    bool timer_event(const Glib::RefPtr<Gdk::FrameClock>&);

    std::unique_ptr<Shader> renderingProgram;

    GLuint VBO = {};
    GLuint VAO = {};
    GLuint EBO = {};
    GLuint texture[2] = {};

    gint64 startTime = -1;
    float  curTime   = -1.0f;
};
