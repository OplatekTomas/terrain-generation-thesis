
//
// Created by tomas on 03.11.21.
//

#include <common/LandTypeGenerator.h>
#include <iostream>


namespace MapGenerator {

    LandTypeGenerator::LandTypeGenerator(double lat1, double lon1, double lat2, double lon2, int resolution,
                                         std::shared_ptr<OSMData> osmData) {



        this->osmData = osmData;
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
            areas.push_back(area);
        }
        areas = from(areas).orderBy([](AreaOnMap area) {
            return area.getArea();
        }).toStdVector();

#pragma omp parallel for default(none) shared(texture, cout)
        for (int x = 0; x < resolution; x++) {
            auto lon = xStart + x * xStep;
            auto lineData = from(areas).where([lon](AreaOnMap area) {
                return area.isInsideLon(lon);
            }).toStdVector();
            for (int y = 0; y < resolution; y++) {
                auto start = std::chrono::high_resolution_clock::now();
                auto lat = yStart + y * yStep;
                auto minArea = from(lineData).where(
                        [&](AreaOnMap x) {
                            return x.isInsideArea(lat, lon);
                        }).firstOrDefault();
                if (minArea.getArea() == 0) {
                    continue;
                }
                auto index = (y * resolution + (resolution - x - 1)) * 4;
                minArea.getColor(&texture->at(index), &texture->at(index + 1), &texture->at(index + 2),
                                 &texture->at(index + 3));
            }
        }
        return texture;
    }
}

