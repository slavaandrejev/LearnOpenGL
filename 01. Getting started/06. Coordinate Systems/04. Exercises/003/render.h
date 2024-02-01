#pragma once

#include <memory>
#include <vector>

#include <gdkmm/frameclock.h>
#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/glarea.h>

#include <epoxy/gl.h>

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

    guint tickCallbackId{};
    bool timer_event(const Glib::RefPtr<Gdk::FrameClock>&);

    std::unique_ptr<Shader> renderingProgram;

    GLuint VBO = {};
    GLuint VAO = {};
    GLuint texture[2] = {};

    std::vector<glm::vec3> cubePositions;

    gint64 startTime = -1;
    float  curTime   = -1.0f;
};
