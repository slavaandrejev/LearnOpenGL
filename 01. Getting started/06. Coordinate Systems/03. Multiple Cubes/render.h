#pragma once

#include <memory>
#include <vector>

#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/glarea.h>

#include <glbinding/gl/gl.h>

#include <glm/glm.hpp>

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

    std::unique_ptr<Shader> renderingProgram;

    gl::GLuint VBO = {};
    gl::GLuint VAO = {};
    gl::GLuint texture[2] = {};

    std::vector<glm::vec3> cubePositions;
};
