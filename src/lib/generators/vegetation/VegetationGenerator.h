//
// Created by tomas on 28.03.22.
//


#pragma once

#include <scene/Model.h>
#include <api/models/bing/ElevationData.h>
#include <memory>
#include <config/GeneratorOptions.h>
#include <scene/Vertex.h>
#include <string>
#include <scene/Texture.h>
#include <memory>
#include <random>


namespace MapGenerator {
    class VegetationGenerator {
    public:
        VegetationGenerator(GeneratorOptions options, std::shared_ptr<ElevationData> heightData);

        ~VegetationGenerator();

        std::shared_ptr<Model> getVegetation(const std::shared_ptr<Texture> &texture, int resolution);

    private:
        GeneratorOptions options;
        std::shared_ptr<ElevationData> heightData;
        std::shared_ptr<Model> defaultTree;

        std::vector<Vertex> getModelVertices(std::shared_ptr<Model> model, double scale);

        bool shouldRender(const std::shared_ptr<Texture> &texture, int resolution, PointF point);

        bool updateZ;
        float scale;
        std::shared_ptr<Model> result;

        void addToResult(const std::shared_ptr <Model> &model, const std::vector <Vertex> &scaledData, const PointF &offset);

        std::uniform_real_distribution<double> randomDistribution;
        std::mt19937 randomGenerator;
    };
}




