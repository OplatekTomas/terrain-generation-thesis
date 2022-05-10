//
// Created by tomas on 28.03.22.
//

#include <generators/vegetation/VegetationGenerator.h>
#include <generators/vegetation/ObjectLoader.h>
#include <poisson_disk_sampling.h>
#include "scene/Point.h"
#include <iostream>
#include <filesystem>
#include <Logger.h>


namespace fs = std::filesystem;
namespace MapGenerator {
    VegetationGenerator::VegetationGenerator(GeneratorOptions options, std::shared_ptr<ElevationData> heightData) {
        currentModels = {};

        this->options = options;
        this->heightData = heightData;
        this->updateZ = heightData->getScale() > 1;
        this->scale = this->updateZ ? 1 / heightData->getScale() : heightData->getScale();
        //Initialize a c++11 random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        this->randomGenerator = std::mt19937(rd());
        this->randomDistribution = std::uniform_real_distribution<double>(0, 1);
        loadModels();
    }

    VegetationGenerator::~VegetationGenerator() {

    }

    void VegetationGenerator::loadModels() {
        auto base = "lib/assets/models/";
        std::vector<std::string> types = {"coniferous_trees", "deciduous_trees", "grass", "crops"};
        loadModels(base + types[0], coniferousTrees);
        loadModels(base + types[1], deciduousTrees);
        loadModels(base + types[0], mixedTrees);
        loadModels(base + types[1], mixedTrees);
        loadModels(base + types[2], grass);
        loadModels(base + types[3], crops);

    }

    void VegetationGenerator::loadModels(const std::string &directory, std::vector<std::shared_ptr<Model>> &location) {
        //Get all files inside the directory
        std::vector<std::string> files;
        for (auto &p: fs::directory_iterator(directory)) {
            files.push_back(p.path().string());
        }
        //Load all files
        auto models = ObjectLoader::loadAll(files);
        //Add all models to the location vector
        for (auto &model: models) {
            location.push_back(model);
        }
    }


    std::vector<Vertex> VegetationGenerator::getModelVertices(std::shared_ptr<Model> model, double scale) {
        std::vector<Vertex> vertices;
        for (int i = 0; i < model->vertices.size(); i += 11) {
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

    bool VegetationGenerator::shouldRender(const std::shared_ptr<Texture> &texture, int resolution, PointF point,
                                           VegetationType type) {
        PointI pointI = PointI((int) (point.x * resolution), (int) (point.y * resolution));
        auto color = texture->getPixel(pointI.x, pointI.y);
        auto requestedBiomeId = 0.0f;
        switch (type) {
            case VegetationType::ConiferousForest:
                requestedBiomeId = 1.0f;
                break;
            case VegetationType::DeciduousForest:
                requestedBiomeId = 8.0f;
                break;
            case VegetationType::MixedForest:
                requestedBiomeId = 9.0f;
                break;
            case VegetationType::Field:
                requestedBiomeId = 3.0f;
                break;
            case VegetationType::Grassland:
                requestedBiomeId = 6.0f;
                break;
        }
        return color.x == requestedBiomeId;
    }

    void VegetationGenerator::prepareModels(VegetationType type) {
        auto modelHeight = 0.0;
        currentModelsScaled.clear();
        switch (type) {
            case VegetationType::MixedForest:
                modelHeight = 15.0;
                currentModels = mixedTrees;
                break;
            case VegetationType::DeciduousForest:
                modelHeight = 15.0;
                currentModels = deciduousTrees;
                break;
            case VegetationType::ConiferousForest:
                modelHeight = 15.0;
                currentModels = coniferousTrees;
                break;
            case VegetationType::Field:
                modelHeight = 2.0;
                currentModels = crops;
                break;
            case VegetationType::Grassland:
                modelHeight = 0.2;
                currentModels = grass;
                break;
        }
        modelHeight *= (1 / scale);
        auto modelScale = ((modelHeight - heightData->getNormalizedMin()) /
                           (heightData->getNormalizedMax() + heightData->getNormalizedMin()));
        for (auto &model: currentModels) {
            currentModelsScaled.push_back(getModelVertices(model, modelScale));
        }

    }

    double VegetationGenerator::getDistance(VegetationType type) {
        double distance;
        switch (type) {
            case VegetationType::MixedForest:
            case VegetationType::DeciduousForest:
            case VegetationType::ConiferousForest:
                distance = 6;
                break;
            case VegetationType::Field:
                distance = 4.0;
                break;
            case VegetationType::Grassland:
                distance = 0.5;
                break;
        }
        distance *= (1 / scale);
        return ((distance - heightData->getNormalizedMin()) /
                (heightData->getNormalizedMax() + heightData->getNormalizedMin()));
    }

    std::vector<std::shared_ptr<Model>>
    VegetationGenerator::getVegetation(const std::shared_ptr<Texture> &texture, int resolution, VegetationType type) {
        //Start time
        auto start = std::chrono::high_resolution_clock::now();
        Logger::log("Generating vegetation...");

        std::vector<std::shared_ptr<Model>> result;
        PointF min = {0.005, 0.005};
        PointF max = {0.995, 0.995};
        auto kXMin = std::array<float, 2>{min.x, min.y};
        auto kXMax = std::array<float, 2>{max.x, max.y};
        prepareModels(type);
        if (currentModels.empty()) {
            return {};
        }
        auto scaleX = std::make_shared<Uniform>();
        scaleX->name = "scaleX";
        scaleX->type = Uniform::Type::FLOAT;
        scaleX->valueInt = (int) this->updateZ;
        auto scaleZ = std::make_shared<Uniform>();
        scaleZ->name = "scaleZ";
        scaleZ->type = Uniform::Type::FLOAT;
        scaleZ->valueFloat = this->scale;
        if (!updateZ) {
            scaleZ->valueFloat = scale;
            scaleX->valueFloat = 1.0;
        } else {
            scaleX->valueFloat = scale;
            scaleZ->valueFloat = 1.0;
        }
        for (int i = 0; i < currentModels.size(); i++) {
            auto model = std::make_shared<Model>("vegetation");
            auto current = currentModelsScaled[i];
            auto orig = currentModels[i];
            auto origV = orig->vertices;
            for (int j = 0; j < currentModelsScaled[i].size(); j++) {
                auto idx = j * 11;
                auto vertex = current[j];
                model->addVertex(vertex, {origV[idx + 3], origV[idx + 4], origV[idx + 5]},
                                 {origV[idx + 6], origV[idx + 7]});
            }
            for (int j = 0; j < orig->indices.size(); j++) {
                model->addIndex(orig->indices[j]);
            }
            model->uniforms.push_back(scaleZ);
            model->uniforms.push_back(scaleX);
            model->isInstanced = true;
            result.push_back(model);
        }
        auto floatPositions = thinks::PoissonDiskSampling((float) getDistance(type), kXMin, kXMax);
        for (auto &position: floatPositions) {
            auto point = PointF(position[0], position[1]);
            if (!shouldRender(texture, resolution, point, type)) {
                continue;
            }
            auto idx = (int) (randomDistribution(randomGenerator) * currentModels.size());
            result[idx]->instanceData.push_back(point.x);
            result[idx]->instanceData.push_back(point.y);
        }

        //End time
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        Logger::log("Vegetation generated in " + std::to_string(duration) + "s");
        return result;

    }


}


