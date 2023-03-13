#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include <epoxy/gl.h>

#include <fmt/format.h>

#include <glm/glm.hpp>

#include <shader.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture {
    GLuint      id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex>  vertices;
    std::vector<GLuint>  indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
      : vertices(std::move(vertices))
      , indices(std::move(indices))
      , textures(std::move(textures))
    {
        SetupMesh();
    }

    Mesh(const Mesh &m) = delete;
    Mesh & operator=(const Mesh &) = delete;
    Mesh & operator=(Mesh &&) = delete;

    Mesh(Mesh &&m)
      : vertices(std::move(m.vertices))
      , indices(std::move(m.indices))
      , textures(std::move(m.textures))
      , VAO(m.VAO)
      , VBO(m.VBO)
      , EBO(m.EBO)
    {
        m.VAO = m.VBO = m.EBO = 0;
    }

    ~Mesh() {
        if (0 != VAO) {
            glDeleteVertexArrays(1, &VAO);
        }
        if (0 != VBO) {
            glDeleteBuffers(1, &VBO);
        }
        if (0 != EBO) {
            glDeleteBuffers(1, &EBO);
        }
        for (auto &&t : textures) {
            glDeleteTextures(1, &t.id);
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
            shader.set(fmt::format("material.{}[{}]", textures[i].type, number).c_str(), static_cast<GLint>(i));
            glBindTextureUnit(i, textures[i].id);
        }

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

private:
    GLuint VAO{};
    GLuint VBO{};
    GLuint EBO{};

    void SetupMesh() {
        glCreateVertexArrays(1, &VAO);
        glCreateBuffers(1, &VBO);
        glCreateBuffers(1, &EBO);

        glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(Vertex));
        glVertexArrayElementBuffer(VAO, EBO);

        glNamedBufferStorage(VBO, sizeof(vertices[0]) * vertices.size(), &vertices[0], 0);
        glNamedBufferStorage(EBO, sizeof(indices[0]) * indices.size(), &indices[0], 0);

        // vertex positions
        glVertexArrayAttribBinding(VAO, 0, 0);
        glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexArrayAttrib(VAO, 0);

        // vertex normals
        glVertexArrayAttribBinding(VAO, 1, 0);
        glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
        glEnableVertexArrayAttrib(VAO, 1);

        // vertex texture coords
        glVertexArrayAttribBinding(VAO, 2, 0);
        glVertexArrayAttribFormat(VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoords));
        glEnableVertexArrayAttrib(VAO, 2);
    }
};
