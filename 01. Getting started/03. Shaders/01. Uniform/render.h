#pragma once

#include <memory>

#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/glarea.h>

#include <glbinding/gl/gl.h>

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

    gl::GLuint VBO = {};
    gl::GLuint VAO = {};

    gint64 startTime = -1;
    float  curTime   = -1.0f;
};
