//
// Created by tomas on 07.02.22.
//

#include <generators/surface/SurfaceGenerator.h>
#include <iostream>
#include <Logger.h>
#include <chrono>

namespace MapGenerator{

    SurfaceGenerator::SurfaceGenerator(std::shared_ptr<ElevationData> elevationData, GeneratorOptions options) {
        this->data = elevationData;
        this->options = options;
    }

    std::shared_ptr<Model> SurfaceGenerator::getSurface() {
        if (data->getData()->empty()) {
            return nullptr;
        }
        //get start time
        auto start = std::chrono::high_resolution_clock::now();
        Logger::log("Generating surface...");

        auto model = std::make_shared<Model>("Surface");
        auto res = options.terrainResolution - 1;
        //Let's create the model
        auto fRes = (float) res;
        auto changeY = data->getScale() > 1;
        auto scale = changeY ? 1 / data->getScale() : data->getScale();

        auto vertices = std::vector<Vertex>();
        auto verticesNormals = std::vector<std::vector<Vertex>>();
        auto verticesTextureCoords = std::vector<PointF>();
        for (auto x = 0; x <= res; x++) {
            for (auto y = 0; y <= res; y++) {
                auto index = (x * (res + 1) + y);
                auto yPos = (float) y / fRes;
                auto xPos = (float) x / fRes;
                auto height = (float) data->getData()->at(index);
                // the (scale / 4) is there to put it at the center of the 0-1 range
                Vertex v;
                if (changeY) {
                    v = {(yPos * scale) + ((1-scale)/2), height, xPos};
                } else {
                    v = {yPos, height, (xPos * scale) + ((1-scale)/2)};
                }
                vertices.push_back(v);
                verticesNormals.emplace_back();
                verticesTextureCoords.emplace_back(yPos, xPos);
            }
        }
        //And add the indices
        std::vector<int> indices;
        for (int i = 0; i < res; ++i) {
            for (int j = 0; j < res; ++j) {
                int row1 = i * (res + 1);
                int row2 = (i + 1) * (res + 1);
                indices.push_back(row1 + j);
                indices.push_back(row1 + j + 1);
                indices.push_back(row2 + j + 1);
                indices.push_back(row1 + j);
                indices.push_back(row2 + j + 1);
                indices.push_back(row2 + j);
                model->addIndex(row1 + j, row1 + j + 1, row2 + j + 1);
                model->addIndex(row1 + j, row2 + j + 1, row2 + j);
            }
        }
        //Go through the vertices and add the normals
        for (auto i = 0; i < indices.size(); i += 3) {
            auto index = indices[i];
            auto index2 = indices[i + 1];
            auto index3 = indices[i + 2];
            auto v1 = vertices[index];
            auto v2 = vertices[index2];
            auto v3 = vertices[index3];
            auto normal = (v3 - v1).cross(v2 - v1).normalize();
            verticesNormals[index].push_back(normal);
            verticesNormals[index2].push_back(normal);
            verticesNormals[index3].push_back(normal);
        }
        //Add the vertices and normals to the model
        for (auto i = 0; i < vertices.size(); i++) {
            //Average the normals
            auto normals = verticesNormals[i];
            auto normal = Vertex(0, 0, 0);
            for (auto n: normals) {
                normal += n;
            }
            normal = normal.normalize();
            model->addVertex(vertices[i], normal, verticesTextureCoords[i]);
        }

        //get end time
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        Logger::log("Surface generated in " + std::to_string(duration.count()) + "s");
        return model;
    }


}
