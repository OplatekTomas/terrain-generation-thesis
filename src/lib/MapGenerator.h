//
// Created by tomas on 08.10.21.
//

#pragma once

#include <config/LibConfig.h>
#include <api/BingApi.h>
#include <generators/landtype/LandTypeGenerator.h>
#include <api/OpenStreetMapApi.h>
#include <api/models/openstreetmap/MetadataResult.h>
#include <scene/Scene.h>
#include <config/GeneratorOptions.h>
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
        std::shared_ptr<Scene> scene;
        std::shared_ptr<OSMData> osmData;
        std::shared_ptr<ElevationData> elevationData;

        void runAsyncGenerators(int surfaceModelId);

        std::shared_ptr<Model> generateSurface();

        void generateSurfaceTextures(int id);

        void generateBuildings();
    };
}




