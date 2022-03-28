//
// Created by tomas on 28.03.22.
//

#include <generators/vegetation/VegetationGenerator.h>
#include <generators/vegetation/ObjectLoader.h>
#include <poisson_disk_sampling.h>
#include "scene/Point.h"
#include <iostream>

namespace MapGenerator {
    VegetationGenerator::VegetationGenerator(GeneratorOptions options, std::shared_ptr<ElevationData> heightData) {
        defaultTree = ObjectLoader::load("../lib/models/kokotree.obj");
        this->options = options;
        this->heightData = heightData;

        this->updateZ = heightData->getScale() > 1;

        this->scale = this->updateZ ? 1 / heightData->getScale() : heightData->getScale();
    }

    VegetationGenerator::~VegetationGenerator() {

    }


    std::vector<Vertex> VegetationGenerator::getModelVertices(std::shared_ptr<Model> model, double scale) {
        std::vector<Vertex> vertices;
        for (int i = 0; i < model->vertices.size(); i += 8) {
            auto v1 = model->vertices[i];
            auto v2 = model->vertices[i + 1];
            auto v3 = model->vertices[i + 2];
            auto vertex = Vertex(v1, v2, v3) * scale;
            if (!updateZ) {
                vertex.x = (vertex.x * this->scale);
            } else {
                vertex.z = (vertex.z * this->scale);
            }
            vertex.y = (vertex.y * this->scale);
            vertices.push_back(vertex);
        }
        return vertices;
    }

    bool VegetationGenerator::shouldRender(const std::shared_ptr<Texture> &texture, int resolution, PointF point) {
        PointI pointI = PointI((int) (point.x * resolution), (int) (point.y * resolution));
        auto color = texture->getPixel(pointI.x, pointI.y);
        //TODO change according to requested biome
        //return true;
        return color.x == 1.0;
    }

    std::shared_ptr<Model> VegetationGenerator::getVegetation(const std::shared_ptr<Texture> &texture, int resolution) {
        auto result = std::make_shared<Model>();
        PointF min;
        PointF max;
        if (!updateZ) {
            min = PointF(0, (1 - scale) / 2);
            max = PointF(1, 1 - min.y);
        } else {
            min = PointF((1 - scale) / 2, 0);
            max = PointF(1 - min.x, 1);
        }
        min = {0.005, 0.005};
        max = {0.995, 0.995};

        auto treeHeightInM = 20.0;
        auto treeDistanceInM = 4.0;
        auto treeHeight = ((treeHeightInM - heightData->getNormalizedMin()) /
                           (heightData->getNormalizedMax() + heightData->getNormalizedMin()));
        auto treeDistance = ((treeDistanceInM - heightData->getNormalizedMin()) /
                             (heightData->getNormalizedMax() + heightData->getNormalizedMin()));
        auto kXMin = std::array<float, 2>{min.x, min.y};
        auto kXMax = std::array<float, 2>{max.x, max.y};
        auto floatPositions = thinks::PoissonDiskSampling((float) treeDistance, kXMin, kXMax);

        auto scaledModel = getModelVertices(defaultTree, treeHeight);
        auto model = defaultTree;
        for (int i = 0; i < floatPositions.size(); i++) {
            //TODO dont add if not in forrest
            auto xOffset = floatPositions[i][0];
            auto zOffset = floatPositions[i][1];
            if (!shouldRender(texture, resolution, PointF(xOffset, zOffset))) continue;

            for (int j = 0; j < scaledModel.size(); j++) {
                auto origV = &defaultTree->vertices;
                auto vertex = Vertex(scaledModel[j].x + xOffset, scaledModel[j].y, scaledModel[j].z + zOffset);
                if (!updateZ) {
                    vertex.z = (vertex.z * scale) + ((1 - scale) / 2);
                    //vertex.x = vertex.x * scale;
                } else {
                    vertex.x = (vertex.x * scale) + ((1 - scale) / 2);
                    //vertex.z = (vertex.z * scale
                }
                auto normal = Vertex((*origV)[j * 8 + 3], (*origV)[j * 8 + 4], (*origV)[j * 8 + 5]);
                auto uv = PointF((*origV)[j * 8 + 6], (*origV)[j * 8 + 7]);
                result->addVertex(vertex, normal, uv);
            }
            for (int j = 0; j < model->indices.size(); j++) {
                result->addIndex(model->indices[j] + (scaledModel.size() * i));
            }
        }
        return result;
    }


}


