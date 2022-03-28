//
// Created by tomas on 28.03.22.
//

#include <generators/vegetation//ObjectLoader.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace MapGenerator {

    std::shared_ptr<Model> ObjectLoader::load(const std::string &path) {
        Assimp::Importer importer;
        auto scene = importer.ReadFile(path, aiProcess_Triangulate);
        if (scene == nullptr) {
            return {};
        }
        auto model = std::make_shared<Model>();
        auto mesh = scene->mMeshes;
        //Load vertices and their normals and texture coordinates
        auto vertices = std::vector<Vertex>();
        for (int i = 0; i < mesh[0]->mNumVertices; i++) {
            auto assVertex = mesh[0]->mVertices[i]; //TODO find better name
            vertices.emplace_back(assVertex.x, assVertex.y, assVertex.z);
        }
        auto scale = findScale(vertices);
        for (int i = 0; i < mesh[0]->mNumVertices; i++) {
            auto vertex = vertices[i] * scale;
            auto normal = mesh[0]->mNormals[i];
            auto textureCoordinate = mesh[0]->mTextureCoords[0][i];
            model->addVertex(vertex, {normal.x, normal.y, normal.z}, {textureCoordinate.x,textureCoordinate.y});
        }
        //Load indices
        for (int i = 0; i < mesh[0]->mNumFaces; i++) {
            auto face = mesh[0]->mFaces[i];
            for (int j = 0; j < face.mNumIndices; j++) {
                model->addIndex(face.mIndices[j]);
            }
        }
        return model;
    }

    float ObjectLoader::findScale(const std::vector<Vertex> &vertices) {
        float max = 0;
        float min = 9999;
        for (auto vertex: vertices) {
            if (vertex.x > max) {
                max = vertex.x;
            }
            if (vertex.y > max) {
                max = vertex.y;
            }
            if (vertex.z > max) {
                max = vertex.z;
            }
            if (vertex.x < min) {
                min = vertex.x;
            }
            if (vertex.y < min) {
                min = vertex.y;
            }
            if (vertex.z < min) {
                min = vertex.z;
            }
        }
        return 1.0f / (max - min);
    }


}
