#pragma once

#include <array>
#include <memory>

#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/glarea.h>

#include <boost/hana/functional/fix.hpp>
#include <boost/hana/functional/overload.hpp>

#include <epoxy/gl.h>
#include <epoxy/glx.h>

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

    std::array<std::unique_ptr<Shader>, 2> renderingProgram;

    GLuint VBO[2] = {};
    GLuint VAO[2] = {};
};
