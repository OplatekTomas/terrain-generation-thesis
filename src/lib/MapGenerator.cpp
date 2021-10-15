//
// Created by tomas on 08.10.21.
//

#include <MapGenerator.h>

#include <fmt/printf.h>

namespace MapGenerator {
    MapGenerator::MapGenerator(Config *config) {
        this->config = config;
        bing = std::make_unique<BingApi>(config->keys[0].key);
    }


    std::shared_ptr<VertexData> MapGenerator::getVertices(double long1, double lat1, double long2, double lat2, int resolution) {
        auto elevation = bing->getElevationNormalized(long1, lat1, long2, lat2, resolution+1);
        auto data = std::make_shared<VertexData>((resolution + 1) * (resolution + 1) * 3, resolution * resolution);
        for (int x = 0; x <= resolution; x++) {
            for (int y = 0; y <= resolution; y++) {
                auto index = (x * (resolution + 1) + y);
                data->addVertex(
                    ((float) x / (float) resolution),
                    (float) elevation[index],
                    ((float) y / (float) resolution)
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

}


