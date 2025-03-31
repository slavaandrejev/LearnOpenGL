#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include <glbinding/gl/gl.h>

#include <fmt/format.h>

#include <glm/glm.hpp>

#include <shader.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture {
    gl::GLuint  id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<gl::GLuint> indices, std::vector<Texture> textures)
      : vertices(std::move(vertices))
      , indices(std::move(indices))
      , textures(std::move(textures))
    {
        SetupMesh();
    }

    // To avoid complications of shared ownership of OpenGL objects, let's just
    // disable copy.
    Mesh(const Mesh &m) = delete;
    Mesh & operator=(const Mesh &) = delete;

    Mesh & operator=(Mesh &&m) noexcept(
        noexcept(std::declval<std::vector<Vertex>>() = std::declval<std::vector<Vertex>&&>()) &&
        noexcept(std::declval<std::vector<gl::GLuint>>() = std::declval<std::vector<gl::GLuint>&&>()) &&
        noexcept(std::declval<std::vector<Texture>>() = std::declval<std::vector<Texture>&&>()))
    {
        if (&m != this) {
            vertices = std::move(m.vertices);
            indices  = std::move(m.indices);
            textures = std::move(m.textures);

            VAO = m.VAO;
            VBO = m.VBO;
            EBO = m.EBO;

            m.VAO = m.VBO = m.EBO = 0;
        }

        return *this;
    }

    Mesh(Mesh &&m) noexcept(
        noexcept(std::declval<Mesh>() = std::declval<Mesh&&>()))
    {
        *this = std::move(m);
    }

    ~Mesh() {
        if (0 != VAO) {
            gl::glDeleteVertexArrays(1, &VAO);
        }
        if (0 != VBO) {
            gl::glDeleteBuffers(1, &VBO);
        }
        if (0 != EBO) {
            gl::glDeleteBuffers(1, &EBO);
        }
        for (auto &&t : textures) {
            gl::glDeleteTextures(1, &t.id);
        }
    }

    void Draw(Shader &shader) const {
        auto diffuseNr = 0;
        auto specularNr = 0;
        for (auto i = size_t{}; textures.size() > i; ++i) {
            auto number = 0;
            if ("diffuse" == textures[i].type) {
                number = diffuseNr++;
            } else if ("specular" == textures[i].type) {
                number = specularNr++;
            }
            // Note that the sampler name has type `GLint`.
            shader.set(fmt::format("material.{}[{}]", textures[i].type, number).c_str(), static_cast<gl::GLint>(i));
            gl::glBindTextureUnit(i, textures[i].id);
        }

        // draw mesh
        gl::glBindVertexArray(VAO);
        gl::glDrawElements(gl::GL_TRIANGLES, indices.size(), gl::GL_UNSIGNED_INT, nullptr);
        gl::glBindVertexArray(0);
    }

private:
    std::vector<Vertex>     vertices;
    std::vector<gl::GLuint> indices;
    std::vector<Texture>    textures;

    gl::GLuint VAO{};
    gl::GLuint VBO{};
    gl::GLuint EBO{};

    void SetupMesh() {
        gl::glCreateVertexArrays(1, &VAO);
        gl::glCreateBuffers(1, &VBO);
        gl::glCreateBuffers(1, &EBO);

        gl::glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(Vertex));
        gl::glVertexArrayElementBuffer(VAO, EBO);

        gl::glNamedBufferStorage(VBO, sizeof(vertices[0]) * vertices.size(), &vertices[0], gl::GL_NONE_BIT);
        gl::glNamedBufferStorage(EBO, sizeof(indices[0]) * indices.size(), &indices[0], gl::GL_NONE_BIT);

        // vertex positions
        gl::glVertexArrayAttribBinding(VAO, 0, 0);
        gl::glVertexArrayAttribFormat(VAO, 0, 3, gl::GL_FLOAT, gl::GL_FALSE, 0);
        gl::glEnableVertexArrayAttrib(VAO, 0);

        // vertex normals
        gl::glVertexArrayAttribBinding(VAO, 1, 0);
        gl::glVertexArrayAttribFormat(VAO, 1, 3, gl::GL_FLOAT, gl::GL_FALSE, offsetof(Vertex, normal));
        gl::glEnableVertexArrayAttrib(VAO, 1);

        // vertex texture coords
        gl::glVertexArrayAttribBinding(VAO, 2, 0);
        gl::glVertexArrayAttribFormat(VAO, 2, 2, gl::GL_FLOAT, gl::GL_FALSE, offsetof(Vertex, texCoords));
        gl::glEnableVertexArrayAttrib(VAO, 2);
    }
};
