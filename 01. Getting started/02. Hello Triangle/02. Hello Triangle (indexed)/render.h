#pragma once

#include <gtk/gtk.hpp>

#include <glbinding/gl/gl.h>

#include <glboundglarea.h>
#include <gnamespaces.h>

// First triangle using EBO.
// Code: https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.2.hello_triangle_indexed/hello_triangle_indexed.cpp
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
    gl::GLuint EBO = 0;
    gl::GLuint VBO = 0;
    gl::GLuint VAO = 0;
};
