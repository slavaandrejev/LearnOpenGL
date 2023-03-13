#pragma once

#include <algorithm>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <boost/filesystem.hpp>

#include <fmt/format.h>

#include <gdkmm/pixbuf.h>

#include <load_texture.h>
#include <mesh.h>

class Model {
    using Path = boost::filesystem::path;
public:
    // model data
    std::vector<Texture> textures_loaded;
    std::vector<Mesh>    meshes;
    Path                 directory;

    Model(std::string_view path) {
        LoadModel(path);
    }

    void Draw(Shader &shader) {
        for (auto &&m : meshes) {
            m.Draw(shader);
        }
    }

private:
    void LoadModel(std::string_view path) {
        auto import = Assimp::Importer{};
        auto scene = import.ReadFile(std::string{path}, aiProcess_Triangulate);

        if (nullptr == scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || nullptr == scene->mRootNode) {
            throw std::runtime_error(fmt::format("ERROR::ASSIMP::{}", import.GetErrorString()));
        }
        directory = Path(std::string{path}).parent_path();
        ProcessNode(scene->mRootNode, scene);
    }

    void ProcessNode(aiNode *node, const aiScene *scene) {
        for (decltype(node->mNumMeshes) i{}; node->mNumMeshes > i; ++i) {
            auto mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.emplace_back(ProcessMesh(mesh, scene));
        }

        for(decltype(node->mNumChildren) i{}; node->mNumChildren > i; ++i) {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene) {
        auto vertices = std::vector<Vertex>{};
        auto indices  = std::vector<GLuint>{};
        auto textures = std::vector<Texture>{};

        for (decltype(mesh->mNumVertices) i{}; mesh->mNumVertices > i; ++i) {
            auto texCoords = glm::vec2{};
            if (nullptr != mesh->mTextureCoords[0]) {
                texCoords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            } else {
                texCoords = {0.0f, 0.0f};
            }

            vertices.push_back({
                .position = {
                    mesh->mVertices[i].x
                  , mesh->mVertices[i].y
                  , mesh->mVertices[i].z}
              , .normal = {
                    mesh->mNormals[i].x
                  , mesh->mNormals[i].y
                  , mesh->mNormals[i].z}
              , .texCoords = texCoords
              });
        }

        for (decltype(mesh->mNumFaces) i{}; mesh->mNumFaces > i; ++i) {
            auto face = mesh->mFaces[i];
            for (decltype(face.mNumIndices) j{}; face.mNumIndices > j; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }

        if (0 <= mesh->mMaterialIndex) {
            auto material = scene->mMaterials[mesh->mMaterialIndex];
            auto diffuseMaps = LoadMaterialTextures(
                material
              , aiTextureType_DIFFUSE
              , "diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            auto specularMaps = LoadMaterialTextures(
                material
              , aiTextureType_SPECULAR
              , "specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return {vertices, indices, textures};
    }

    std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string_view typeName) {
        auto textures = std::vector<Texture>{};

        auto numTex = mat->GetTextureCount(type);
        for(decltype(numTex) i{}; numTex > i; ++i) {
            auto path = aiString{};
            mat->GetTexture(type, i, &path);
            auto loaded = std::find_if(textures_loaded.cbegin(), textures_loaded.cend(), [&](auto &&v) {
                return v.path == path.C_Str();
            });
            if (textures_loaded.cend() == loaded) {
                auto texture = Texture{
                    .id = LoadTextureFromPixbuf(
                        *Gdk::Pixbuf::create_from_file((directory / path.C_Str()).string())
                      )
                  , .type = std::string{typeName}
                  , .path = path.C_Str()
                  };
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            } else {
                textures.push_back(*loaded);
            }
        }

        return textures;
    }
};
