//
// Created by tomas on 08.10.21.
//

#pragma once

#include <config/LibConfig.h>
#include <api/BingApi.h>
#include <VertexData.h>
#include <common/LandTypeGenerator.h>
#include <api/OpenStreetMapApi.h>
#include <models/openstreetmap/MetadataResult.h>
#include <scene/Scene.h>
#include <common/GeneratorOptions.h>

namespace MapGenerator {
    class MapGenerator {
    public:
        explicit MapGenerator(const LibConfig &config, const GeneratorOptions &options);

        std::shared_ptr<class Scene> generateMap();

        std::shared_ptr<VertexData>
        getVertices(double lat1, double long1, double lat2, double long2, int resolution = 20);

        std::shared_ptr<std::vector<float>>
        getMetadata(double lat1, double long1, double lat2, double long2, int resolution);

    private:
        LibConfig config;
        GeneratorOptions options;
        std::unique_ptr<BingApi> bing;
        std::unique_ptr<OpenStreetMapApi> osm;
        std::shared_ptr<LandTypeGenerator> textureGenerator;

    };
}




