#pragma once

#include <memory>

#include <gtk/gtk.hpp>

#include <glbinding/gl/gl.h>

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

    guint tickCallbackId{};
    bool timer_event(Gtk::Widget, Gdk::FrameClock frame_clock);

    std::unique_ptr<Shader> renderingProgram;

    gl::GLuint VBO = {};
    gl::GLuint VAO = {};
    gl::GLuint texture[2] = {};

    gint64 startTime = -1;
    float  curTime   = -1.0f;
};
