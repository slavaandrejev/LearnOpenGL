#pragma once

#include <memory>

#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/glarea.h>

#include <epoxy/gl.h>
#include <epoxy/glx.h>

#include <shader.h>

// Two triangles next to each other.
class OpenGLRender : public Gtk::GLArea
{
public:
    OpenGLRender(BaseObjectType* cobject,
                 const Glib::RefPtr<Gtk::Builder>& refBuilder);
private:
    bool on_render(const Glib::RefPtr<Gdk::GLContext>& context) override;
    void on_realize() override;
    void on_unrealize() override;

    std::unique_ptr<Shader> renderingProgram;

    GLuint VBO = 0;
    GLuint VAO = 0;
};