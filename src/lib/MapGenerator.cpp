//
// Created by tomas on 08.10.21.
//

#include <MapGenerator.h>

MapGenerator::MapGenerator::MapGenerator(Config* config) {
    this->config = config;
    bing = std::make_unique<BingApi>(config->keys[0].key);

}

void MapGenerator::MapGenerator::getVertices() {
    auto elevation = bing->getElevationNormalized(49.210677743172724, 16.62863105170431, 49.21309576479339, 16.625380048112635);
}
