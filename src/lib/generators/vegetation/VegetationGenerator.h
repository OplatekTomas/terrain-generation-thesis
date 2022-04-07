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
        enum class VegetationType {
            MixedForest,
            DeciduousForest,
            ConiferousForest,
            Field,
            Grassland,
        };

        VegetationGenerator(GeneratorOptions options, std::shared_ptr<ElevationData> heightData);

        ~VegetationGenerator();

        std::vector <std::shared_ptr<Model>>
        getVegetation(const std::shared_ptr<Texture> &texture, int resolution, VegetationType type);

    private:
        GeneratorOptions options;
        std::shared_ptr<ElevationData> heightData;
        std::vector<std::shared_ptr<Model>> coniferousTrees;
        std::vector<std::shared_ptr<Model>> deciduousTrees;
        std::vector<std::shared_ptr<Model>> mixedTrees;
        std::vector<std::shared_ptr<Model>> grass;
        std::vector<std::shared_ptr<Model>> crops;

        std::vector<std::vector<Vertex>> currentModelsScaled;
        std::vector <std::shared_ptr<Model>> currentModels;

        std::vector<Vertex> getModelVertices(std::shared_ptr<Model> model, double scale);

        bool shouldRender(const std::shared_ptr <Texture> &texture, int resolution, PointF point, VegetationType type);

        bool updateZ;
        float scale;


        std::uniform_real_distribution<double> randomDistribution;
        std::mt19937 randomGenerator;

        void loadModels();

        void loadModels(const std::string &directory, std::vector<std::shared_ptr<Model>> &location);

        void prepareModels(VegetationType type);

        double getDistance(VegetationType type);

    };
}




