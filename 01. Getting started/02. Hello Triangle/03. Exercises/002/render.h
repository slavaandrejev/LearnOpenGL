#pragma once

#include <memory>

#include <gtk/gtk.hpp>

#include <glbinding/gl/gl.h>

#include <glboundglarea.h>
#include <gnamespaces.h>
#include <shader.h>

// Two triangles next to each other using two VAO and two VBO.
class OpenGLRender : public GlBoundGlArea
{
    friend struct WidgetClassDef::TypeInitData;
    friend struct GLAreaClassDef::TypeInitData;
public:
    OpenGLRender(const InitData &id)
      : GlBoundGlArea(id, "OpenGLRender")
    {}

    static GType get_type_() {
        return register_type_<OpenGLRender>("OpenGLRender", 0, {}, {}, {});
    }

private:
    bool render_(Gdk::GLContext context) noexcept override;
    void realize_() noexcept override;
    void unrealize_() noexcept override;

    std::unique_ptr<Shader> renderingProgram;

    gl::GLuint VBO[2] = {};
    gl::GLuint VAO[2] = {};
};
