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
            vertices.push_back(vertex);
        }
        return vertices;
    }

    bool VegetationGenerator::shouldRender(const std::shared_ptr<Texture> &texture, int resolution, PointF point) {
        auto min = (1 - scale) / 2;
        auto max = 1 - min;
        PointF coord;
        if (!updateZ) {
            if(point.y < min || point.y > max) {
                return false;
            }
            coord = PointF(point.x, (point.y*scale) + ((1 - scale) / 2));

        } else {
            if(point.x < min || point.x > max) {
                return false;
            }
            coord = PointF((point.x * scale) + ((1 - scale) / 2), point.y);
        }
        PointI pointI = PointI((int) (coord.x * resolution), (int) (coord.y * resolution));
        auto color = texture->getPixel(pointI.x, pointI.y);
        //TODO change according to requested biome
        return color.x == 1.0;
    }

    std::shared_ptr<Model> VegetationGenerator::getVegetation(const std::shared_ptr<Texture> &texture, int resolution) {
        auto treeHeightInM = 20.0;
        auto treeDistanceInM = 5.0;
        auto treeHeight = ((treeHeightInM - heightData->getNormalizedMin()) /
                           (heightData->getNormalizedMax() + heightData->getNormalizedMin()));
        auto treeDistance = ((treeDistanceInM - heightData->getNormalizedMin()) /
                             (heightData->getNormalizedMax() + heightData->getNormalizedMin()));
        auto kRadius = (float) treeDistance;
        auto kXMin = std::array<float, 2>{{0, 0}};
        auto kXMax = std::array<float, 2>{{1, 1}};
        auto floatPositions = thinks::PoissonDiskSampling(kRadius, kXMin, kXMax);
        auto result = std::make_shared<Model>();
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


