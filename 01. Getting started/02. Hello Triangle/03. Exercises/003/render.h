#pragma once

#include <array>
#include <memory>

#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/glarea.h>

#include <glbinding/gl/gl.h>

#include <shader.h>

// Two triangles next to each other using two VAO, two VBO, and two fragment
// shaders.
class OpenGLRender : public Gtk::GLArea
{
public:
    OpenGLRender(BaseObjectType* cobject,
                 const Glib::RefPtr<Gtk::Builder>& refBuilder);
private:
    bool on_render(const Glib::RefPtr<Gdk::GLContext>& context) override;
    void on_realize() override;
    void on_unrealize() override;

    std::array<std::unique_ptr<Shader>, 2> renderingProgram;

    gl::GLuint VBO[2] = {};
    gl::GLuint VAO[2] = {};
};
