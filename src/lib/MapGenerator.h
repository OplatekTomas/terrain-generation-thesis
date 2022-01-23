//
// Created by tomas on 08.10.21.
//

#pragma once

#include <config/LibConfig.h>
#include <api/BingApi.h>
#include <common/LandTypeGenerator.h>
#include <api/OpenStreetMapApi.h>
#include <models/openstreetmap/MetadataResult.h>
#include <scene/Scene.h>
#include <common/GeneratorOptions.h>
#include <iostream>
#include <future>
#include <thread>
#include <chrono>


namespace MapGenerator {
    class MapGenerator {
    public:
        explicit MapGenerator(const LibConfig &config, GeneratorOptions options);

        std::shared_ptr<class Scene> generateMap();

    private:
        LibConfig config;
        GeneratorOptions options;
        std::unique_ptr<BingApi> bing;
        std::unique_ptr<OpenStreetMapApi> osm;
        std::shared_ptr<LandTypeGenerator> textureGenerator;

        std::shared_ptr<Model> createSurface();
        std::shared_ptr<Texture> createLandTexture(int resolution);
        //std::shared_ptr<std::vector<float>> getMetadata(double lat1, double long1, double lat2, double long2, int resolution);

    };
}




