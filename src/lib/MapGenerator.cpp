//
// Created by tomas on 08.10.21.
//

#include <MapGenerator.h>

#include <fmt/printf.h>
#include <boolinq.h>
#include <Helper.h>

using namespace boolinq;

namespace MapGenerator {
    MapGenerator::MapGenerator(Config *config) {
        this->config = config;
        bing = std::make_unique<BingApi>(config->keys[0].key);
        osm = std::make_unique<OpenStreetMapApi>("");
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
        auto data = std::make_shared<VertexData>((resolution + 1) * (resolution + 1) * 3, resolution * resolution);
        for (int x = 0; x <= resolution; x++) {
            for (int y = 0; y <= resolution; y++) {
                auto index = (x * (resolution + 1) + y);
                data->addVertex(
                        ((float) y / (float) resolution),
                        (float) elevation[index] - 0.2f,
                        (((float) x / (float) resolution)) * (float) scale
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


    std::shared_ptr<MetadataResult> MapGenerator::getMetadata(double lat1, double long1, double lat2, double long2) {
        if (lat1 > lat2) {
            std::swap(lat1, lat2);
        }
        if (long1 > long2) {
            std::swap(long1, long2);
        }
        auto result = osm->getMetadata(lat1, long1, lat2, long2);
        std::vector<std::string> tagTypes = {"landuse", "highways", "waterways"};
        // C#'s Linq like stuff. Works surprisingly well.
        auto ways = from(result->elements)
            .where([](const element &x) {
                return x.type == nodeType::way && x.tags != nullptr;
            }).where([&](const element &x) {
                return from(tagTypes).any([&](const std::string &t) { return mapContainsKey(*x.tags, t); });
            }).toStdVector();
        auto nodes = from(result->elements).where([](const element &x) { return x.type == nodeType::node; });

        return result;
    }

}


