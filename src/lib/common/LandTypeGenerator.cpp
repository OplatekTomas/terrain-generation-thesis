
//
// Created by tomas on 03.11.21.
//

#include <common/LandTypeGenerator.h>


namespace MapGenerator {

    LandTypeGenerator::LandTypeGenerator(double lat1, double lon1, double lat2, double lon2, int resolution,
                                         std::shared_ptr<OSMData> osmData) {
        vector<string> landTypes;
        ySize = std::fabs(lat1 - lat2);
        xSize = std::fabs(lon1 - lon2);
        this->resolution = resolution;
        xStep = xSize / resolution;
        yStep = ySize / resolution;
        yStart = min(lat1, lat2);
        xStart = min(lon1, lon2);
    }

    shared_ptr<vector<unsigned char>> LandTypeGenerator::generateTexture() {
        auto texture = std::make_shared<std::vector<unsigned char>>(resolution * resolution * 4);
        for (const auto &way: this->osmData->getWays()) {
            AreaOnMap area;
            for (auto nodeId: *way.nodes) {
                auto node = this->osmData->getNode(nodeId);
                area.addNode(node);
            }
            areas.emplace(way, area);
            std::cout << endl;
        }
        for (int x = 0; x < resolution; x++) {
            for (int y = 0; y < resolution; y++) {
                auto lon = xStart + x * xStep;
                auto lat = yStart + y * yStep;
                auto area = from(areas).where(
                        [&](pair<const element, AreaOnMap> x) { return x.second.isInsideBounds(lat, lon); });

                std::cout << std::endl;

                /*if(area.isInsideArea(lat, lon)){
                    continue;
                }
                int index = (y * resolution + (resolution - x)) * 4;
                area.getColor(&texture->at(index), &texture->at(index + 1), &texture->at(index + 2), &texture->at(index + 3));*/
            }
        }
        return texture;
    }
}

