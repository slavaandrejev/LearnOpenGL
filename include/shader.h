#pragma once

#include <vector>

#include <giomm/resource.h>

#include <epoxy/gl.h>
#include <epoxy/glx.h>

#include <boost/hana/functional/fix.hpp>

#include <fmt/printf.h>

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
            auto success = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (0 == success) {
                auto infoLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
                auto info = std::vector<GLchar>(infoLength, 0);
                glGetShaderInfoLog(shader, infoLength, nullptr, &info[0]);
                fmt::print("{}\n", &info[0]);
            }

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
        auto success = 0;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (0 == success) {
            auto infoLength = 0;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLength);
            auto info = std::vector<GLchar>(infoLength, 0);
            glGetProgramInfoLog(id, infoLength, nullptr, &info[0]);
            fmt::print("{}\n", &info[0]);
        }
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
