//
// Created by tomas on 28.03.22.
//

#include <generators/vegetation/ObjectLoader.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace MapGenerator {

    std::vector<Vertex> ObjectLoader::readVertices(const aiMesh *mesh) {
        std::vector<Vertex> vertices;
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        }
        return vertices;
    }

    std::vector<std::shared_ptr<Model>> ObjectLoader::load(const std::string &path) {
        Assimp::Importer importer;
        std::vector<Vertex> allVertices;
        std::vector<std::vector<Vertex>> modelVertices;
        std::vector<std::vector<Vertex>> modelNormals;
        std::vector<std::vector<PointF>> modelUVs;
        std::vector<std::vector<unsigned int>> modelIndices;
        auto scene = importer.ReadFile(path, aiProcess_Triangulate);
        if (scene == nullptr) {
            return {};
        }
        for (int i = 0; i < scene->mNumMeshes; i++) {
            modelVertices.emplace_back();
            modelNormals.emplace_back();
            modelUVs.emplace_back();

            auto mesh = scene->mMeshes[i];
            //Load vertices and their normals and texture coordinates
            for (int j = 0; j < mesh->mNumVertices; j++) {
                auto vertex = mesh->mVertices[j];
                auto normal = mesh->mNormals[j];
                auto textureCoordinate = mesh->mTextureCoords[0][j];
                allVertices.emplace_back(vertex.x, vertex.y, vertex.z);
                modelVertices.back().emplace_back(vertex.x, vertex.y, vertex.z);
                modelNormals.back().emplace_back(normal.x, normal.y, normal.z);
                modelUVs.back().emplace_back(textureCoordinate.x, textureCoordinate.y);
            }
            modelIndices.emplace_back();
            for (int j = 0; j < mesh->mNumFaces; j++) {
                auto face = mesh->mFaces[j];
                for (int k = 0; k < face.mNumIndices; k++) {
                    modelIndices.back().emplace_back(face.mIndices[k]);
                }
            }
        }
        auto scale = findScale(allVertices);
        std::vector<std::shared_ptr<Model>> models;
        for (int i = 0; i < modelVertices.size(); i++) {
            auto model = std::make_shared<Model>();
            for (int j = 0; j < modelVertices[i].size(); j++) {
                model->addVertex(modelVertices[i][j] * scale, modelNormals[i][j], modelUVs[i][j]);
            }
            for (unsigned int j: modelIndices[i]) {
                model->addIndex(j);
            }
            models.emplace_back(model);
        }
        return models;
    }

    std::vector<std::shared_ptr<Model>> ObjectLoader::loadAll(const std::vector<std::string> &paths) {
        std::vector<Vertex> allVertices;
        std::vector<std::vector<Vertex>> modelVertices;
        std::vector<std::vector<Vertex>> modelNormals;
        std::vector<std::vector<PointF>> modelUVs;
        std::vector<std::vector<unsigned int>> modelIndices;
        Assimp::Importer importer;
        for (auto path: paths) {
            auto scene = importer.ReadFile(path, aiProcess_Triangulate);
            if (scene == nullptr) {
                continue;
            }
            for (int i = 0; i < scene->mNumMeshes; i++) {
                modelVertices.emplace_back();
                modelNormals.emplace_back();
                modelUVs.emplace_back();
                auto mesh = scene->mMeshes[i];
                //Load vertices and their normals and texture coordinates
                for (int j = 0; j < mesh->mNumVertices; j++) {
                    auto vertex = mesh->mVertices[j];
                    auto normal = mesh->mNormals[j];
                    PointF textureCoordinate = mesh->mTextureCoords[0] ?
                                               PointF(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y) :
                                               PointF(0, 0);
                    allVertices.emplace_back(vertex.x, vertex.y, vertex.z);
                    modelVertices.back().emplace_back(vertex.x, vertex.y, vertex.z);
                    modelNormals.back().emplace_back(normal.x, normal.y, normal.z);
                    modelUVs.back().emplace_back(textureCoordinate.x, textureCoordinate.y);
                }
                modelIndices.emplace_back();
                for (int j = 0; j < mesh->mNumFaces; j++) {
                    auto face = mesh->mFaces[j];
                    for (int k = 0; k < face.mNumIndices; k++) {
                        modelIndices.back().emplace_back(face.mIndices[k]);
                    }
                }
            }
        }
        auto scale = findScale(allVertices);
        std::vector<std::shared_ptr<Model>> models;
        for (int i = 0; i < modelVertices.size(); i++) {
            auto model = std::make_shared<Model>();
            for (int j = 0; j < modelVertices[i].size(); j++) {
                model->addVertex(modelVertices[i][j] * scale, modelNormals[i][j], modelUVs[i][j]);
            }
            for (unsigned int j: modelIndices[i]) {
                model->addIndex(j);
            }
            models.emplace_back(model);
        }
        return models;

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
