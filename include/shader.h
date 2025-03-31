#pragma once

#include <vector>

#include <giomm/resource.h>

#include <glbinding/gl/gl.h>

#include <boost/hana/functional/fix.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fmt/format.h>
#include <fmt/printf.h>

class Shader {
public:
    template <typename ...Args>
    Shader(Args ...args) {
        namespace hana = boost::hana;

        auto compileFromResource = [](auto path, auto shaderType) -> gl::GLuint {
            auto bytes = Gio::Resource::lookup_data_global(path);
            auto size = gsize{};
            auto data = reinterpret_cast<const char*>(bytes->get_data(size));

            auto shader = gl::glCreateShader(shaderType);
            gl::glShaderSource(shader, 1, &data, nullptr);
            gl::glCompileShader(shader);
            auto success = 0;
            gl::glGetShaderiv(shader, gl::GL_COMPILE_STATUS, &success);
            if (0 == success) {
                auto infoLength = 0;
                gl::glGetShaderiv(shader, gl::GL_INFO_LOG_LENGTH, &infoLength);
                auto info = std::vector<gl::GLchar>(infoLength, 0);
                gl::glGetShaderInfoLog(shader, infoLength, nullptr, &info[0]);
                fmt::print("{}\n", &info[0]);
            }

            return shader;
        };

        auto shaders = std::vector<gl::GLuint>{};
        auto attachShaders = hana::fix([&](auto attshdr, auto path, auto type, auto ...args) {
            auto shader = compileFromResource(path, type);
            shaders.push_back(shader);
            gl::glAttachShader(id, shader);
            if constexpr (0 < sizeof...(args)) {
                attshdr(args...);
            }
        });

        attachShaders(args...);
        gl::glLinkProgram(id);
        auto success = 0;
        gl::glGetProgramiv(id, gl::GL_LINK_STATUS, &success);
        if (0 == success) {
            auto infoLength = 0;
            gl::glGetProgramiv(id, gl::GL_INFO_LOG_LENGTH, &infoLength);
            auto info = std::vector<gl::GLchar>(infoLength, 0);
            gl::glGetProgramInfoLog(id, infoLength, nullptr, &info[0]);
            fmt::print("{}\n", &info[0]);
        }
        for (auto shader : shaders) {
            gl::glDeleteShader(shader);
        }
    }

    ~Shader() {
        gl::glDeleteProgram(id);
    }

    auto Id() const { return id; }
    void use() { gl::glUseProgram(id); }
    void set(const char *name, float v) {
        gl::glProgramUniform1f(id, gl::glGetUniformLocation(id, name), v);
    }
    void set(const char *name, gl::GLint v) {
        gl::glProgramUniform1i(id, gl::glGetUniformLocation(id, name), v);
    }
    void set(const char *name, gl::GLuint v) {
        gl::glProgramUniform1ui(id, gl::glGetUniformLocation(id, name), v);
    }
    void set(const char *name, const glm::vec3 &v) {
        gl::glProgramUniform3f(id, gl::glGetUniformLocation(id, name), v[0], v[1], v[2]);
    }
    void set(const char *name, const glm::vec4 &v) {
        gl::glProgramUniform4f(id, gl::glGetUniformLocation(id, name), v[0], v[1], v[2], v[3]);
    }
    void set(const char *name, const glm::mat3 &m) {
        gl::glProgramUniformMatrix3fv(id, gl::glGetUniformLocation(id, name), 1, gl::GL_FALSE, glm::value_ptr(m));
    }
    void set(const char *name, const glm::mat4 &m) {
        gl::glProgramUniformMatrix4fv(id, gl::glGetUniformLocation(id, name), 1, gl::GL_FALSE, glm::value_ptr(m));
    }
    void set(const char *array_name, size_t index, const char *const_name, float v) {
        gl::glProgramUniform1f(
            id
          , gl::glGetUniformLocation(
                id
              , fmt::format("{}[{}].{}", array_name, index, const_name).c_str()
              )
          , v);
    }
    void set(const char *array_name, size_t index, const char *const_name, const glm::vec3 &v) {
        gl::glProgramUniform3f(
            id
          , gl::glGetUniformLocation(
                id
              , fmt::format("{}[{}].{}", array_name, index, const_name).c_str()
              )
          , v[0]
          , v[1]
          , v[2]
          );
    }

private:
    gl::GLuint id = gl::glCreateProgram();
};
