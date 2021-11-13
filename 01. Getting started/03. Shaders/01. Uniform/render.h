#pragma once

#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/glarea.h>

#include <boost/hana/functional/fix.hpp>

#include <epoxy/gl.h>
#include <epoxy/glx.h>

class OpenGLRender : public Gtk::GLArea
{
public:
    OpenGLRender(BaseObjectType* cobject,
                 const Glib::RefPtr<Gtk::Builder>& refBuilder);
private:
    bool on_render(const Glib::RefPtr<Gdk::GLContext>& context) override;
    void on_realize() override;
    void on_unrealize() override;

    bool timer_event(const Glib::RefPtr<Gdk::FrameClock>&);

    template <typename ...Args>
    GLuint CompileShaders(Args ...args) {
        namespace hana = boost::hana;
        auto program = glCreateProgram();

        auto compileFromResource = [](auto path, auto shaderType) -> GLuint {
            auto bytes = Gio::Resource::lookup_data_global(path);
            auto size = gsize{};
            auto data = reinterpret_cast<const char*>(bytes->get_data(size));

            auto shader = glCreateShader(shaderType);
            glShaderSource(shader, 1, &data, nullptr);
            glCompileShader(shader);

            return shader;
        };

        auto shaders = std::vector<GLuint>{};
        auto attachShaders = hana::fix([&](auto attshdr, auto path, auto type, auto ...args) {
            auto shader = compileFromResource(path, type);
            shaders.push_back(shader);
            glAttachShader(program, shader);
            if constexpr (0 < sizeof...(args)) {
                attshdr(args...);
            }
        });

        attachShaders(args...);
        glLinkProgram(program);
        for (auto shader : shaders) {
            glDeleteShader(shader);
        }

        return program;
    }

    GLuint renderingProgram  = {};

    GLuint VBO = {};
    GLuint VAO = {};

    gint64 startTime = -1;
    float  curTime   = -1.0f;
};
