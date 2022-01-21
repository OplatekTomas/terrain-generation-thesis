//
// Created by tomas on 08.10.21.
//

#include <MapGenerator.h>

#include <fmt/printf.h>
#include <boolinq.h>

using namespace boolinq;

namespace MapGenerator {

    MapGenerator::MapGenerator(const LibConfig& config, const GeneratorOptions &options) {
        this->config = config;
        this->options = options;
        bing = std::make_unique<BingApi>(config.keys[0].key);
        osm = std::make_unique<OpenStreetMapApi>("");
    }

    /// This method will start the generation process. The data will be made available in the scene when ready.
    /// \return
    std::shared_ptr<class Scene> MapGenerator::generateMap() {
        return std::shared_ptr<struct Scene>();
    }


    std::shared_ptr<VertexData>
    MapGenerator::getVertices(double lat1, double long1, double lat2, double long2, int resolution) {
        resolution++;
        auto[elevation, latDist, longDist] = bing->getElevationNormalized(lat1, long1, lat2, long2, &resolution);
        resolution--;
        if (elevation.empty()) {
            return nullptr;
        }
        auto scale = longDist / latDist;
        auto data = std::make_shared<VertexData>((resolution + 1) * (resolution + 1) * 5, resolution * resolution);
        for (int x = 0; x <= resolution; x++) {
            for (int y = 0; y <= resolution; y++) {
                auto index = (x * (resolution + 1) + y);
                data->addVertex(
                        ((float) y / (float) resolution),
                        (float) elevation[index] - 0.2f,
                        (((float) x / (float) resolution)) * (float) scale,
                        ((float) y / (float) resolution),
                        (((float) x / (float) resolution))
                );
            }
        }
        //Setting up indices
        for (int j = 0; j < resolution; ++j) {
            for (int i = 0; i < resolution; ++i) {
                int row1 = j * (resolution + 1);
                int row2 = (j + 1) * (resolution + 1);
                data->addIndex(row1 + i, row1 + i + 1, row2 + i + 1);
                data->addIndex(row1 + i, row2 + i + 1, row2 + i);
            }
        }
        return data;
    }


    std::shared_ptr<std::vector<float>>
    MapGenerator::getMetadata(double lat1, double long1, double lat2, double long2, int resolution) {
        if (textureGenerator != nullptr) {
            return textureGenerator->generateTexture(resolution);
        }
        if (lat1 > lat2) {
            std::swap(lat1, lat2);
        }
        if (long1 > long2) {
            std::swap(long1, long2);
        }
        auto data = osm->getMetadata(lat1, long1, lat2, long2);
        if (data == nullptr) {
            return {};
        }
        textureGenerator = std::make_shared<LandTypeGenerator>(lat1, long1, lat2, long2, data);
        auto tex = textureGenerator->generateTexture(resolution);
        return tex;
    }
}


