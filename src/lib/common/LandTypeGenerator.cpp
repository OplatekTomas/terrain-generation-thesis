
//
// Created by tomas on 03.11.21.
//

#include <common/LandTypeGenerator.h>
#include <iostream>
#include <utility>


namespace MapGenerator {

    LandTypeGenerator::LandTypeGenerator(double lat1, double lon1, double lat2, double lon2, int resolution,
                                         std::shared_ptr<OSMData> osmData) {

        this->osmData = std::move(osmData);
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
        auto xEnd = xStart + xSize;
        auto yEnd = yStart + ySize;
        for (auto way: this->osmData->getWays()) {
            auto area = AreaOnMap::create(yStart, yEnd, xStart, xEnd, way);
            if (area == nullptr) {
                continue;
            }
            areas.push_back(area);
        }
        for (auto way: this->osmData->getRelations()) {
            auto area = AreaOnMap::create(yStart, yEnd, xStart, xEnd, way);
            if (area == nullptr) {
                continue;
            }
            areas.push_back(area);
        }
        areas = from(areas).orderBy([](const std::shared_ptr<AreaOnMap> &area) {
            return area->getArea();
        }).orderBy([](const std::shared_ptr<AreaOnMap> &x) {
            return -x->getPriority();
        }).toStdVector();
#pragma omp parallel for default(none) shared(texture)
            for (int x = 0; x < resolution; x++) {
                auto lon = xStart + x * xStep;
                auto lineData = from(areas).where([lon](const std::shared_ptr<AreaOnMap> &area) {
                    return area->isInsideLon(lon);
                }).toStdVector();
                for (int y = 0; y < resolution; y++) {
                    auto lat = yStart + y * yStep;
                    auto minArea = from(lineData).where([&](const std::shared_ptr<AreaOnMap> &x) {
                        return x->isInsideArea(lat, lon);
                    }).firstOrDefault();
                    if (minArea == nullptr) {
                        continue;
                    }
                    auto index = (y * resolution + (resolution - x - 1)) * 4;
                    minArea->getColor(&texture->at(index), &texture->at(index + 1), &texture->at(index + 2),
                                      &texture->at(index + 3));
                }
            }
            return texture;
        }
    }

