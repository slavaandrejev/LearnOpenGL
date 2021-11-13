#pragma once

#include <vector>

#include <giomm/resource.h>

#include <epoxy/gl.h>
#include <epoxy/glx.h>

#include <boost/hana/functional/fix.hpp>

class Shader {
public:
    template <typename ...Args>
    Shader(Args ...args) {
        namespace hana = boost::hana;

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
            glAttachShader(id, shader);
            if constexpr (0 < sizeof...(args)) {
                attshdr(args...);
            }
        });

        attachShaders(args...);
        glLinkProgram(id);
        for (auto shader : shaders) {
            glDeleteShader(shader);
        }
    }

    ~Shader() {
        glDeleteProgram(id);
    }

    auto Id() const { return id; }
    void use() { glUseProgram(id); }
    void set(const std::string &name, float v) {
        glUniform1f(glGetUniformLocation(id, name.c_str()), v);
    }
    void set(const std::string &name, GLint v) {
        glUniform1i(glGetUniformLocation(id, name.c_str()), v);
    }

private:
    GLuint id = glCreateProgram();
};
