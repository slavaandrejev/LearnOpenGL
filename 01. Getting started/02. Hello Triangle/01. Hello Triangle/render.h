#pragma once

#include <gtk/gtk.hpp>

#include <glbinding/gl/gl.h>

#include <glboundglarea.h>
#include <gnamespaces.h>

// First triangle.
// Code: https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.1.hello_triangle/hello_triangle.cpp
// Text: https://learnopengl.com/Getting-started/Hello-Triangle
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

    gl::GLuint CompileShaders();

    gl::GLuint renderingProgram  = 0;
    gl::GLuint VBO = 0;
    gl::GLuint VAO = 0;
};
