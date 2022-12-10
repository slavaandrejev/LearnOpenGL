#pragma once

#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/glarea.h>

#include <epoxy/gl.h>

// First triangle using EBO.
// Code: https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.2.hello_triangle_indexed/hello_triangle_indexed.cpp
// Text: https://learnopengl.com/Getting-started/Hello-Triangle
class OpenGLRender : public Gtk::GLArea
{
public:
    OpenGLRender(BaseObjectType* cobject,
                 const Glib::RefPtr<Gtk::Builder>& refBuilder);
private:
    bool on_render(const Glib::RefPtr<Gdk::GLContext>& context) override;
    void on_realize() override;
    void on_unrealize() override;

    GLuint CompileShaders();

    GLuint renderingProgram  = 0;
    GLuint EBO = 0;
    GLuint VBO = 0;
    GLuint VAO = 0;
};
