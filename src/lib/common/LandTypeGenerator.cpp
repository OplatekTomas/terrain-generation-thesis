
//
// Created by tomas on 03.11.21.
//

#include <common/LandTypeGenerator.h>
#include <iostream>
#include <utility>
#include <omp.h>


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

    shared_ptr<vector<float>> LandTypeGenerator::generateTexture() {
        auto texture = std::make_shared<std::vector<float>>(resolution * resolution * 4);
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

        areas = from(areas).orderBy([](const std::shared_ptr<AreaOnMap> &x) {
            return x->getArea();
        }).orderBy([&](const std::shared_ptr<AreaOnMap> &x) {
            return -x->getPriority();
        }).toStdVector();
        //Get current time
        auto start = std::chrono::system_clock::now();
        int chunkSize = resolution / 8;
#pragma omp parallel for default(none) shared(texture, chunkSize)
        for (int x = 0; x < resolution; x++) {
            auto lon = xStart + x * xStep;
            auto lineData = from(areas).where([lon](const std::shared_ptr<AreaOnMap> &area) {
                return area->isInsideLon(lon);
            }).toStdVector();
            for (int chunk = 0; chunk < resolution; chunk += chunkSize) {

                auto latStart = yStart + chunk * yStep;
                auto latEnd = latStart + chunkSize * yStep;
                auto chunkAreas = from(lineData).where([latStart, latEnd](const std::shared_ptr<AreaOnMap> &area) {
                    return area->isInsideLatRange(latStart, latEnd);
                }).toStdVector();
                for (int y = chunk; y < chunk + chunkSize; y++) {
                    auto lat = yStart + y * yStep;
                    auto area = from(chunkAreas).firstOrDefault([lat, lon](const std::shared_ptr<AreaOnMap> &area) {
                        return area->isInsideArea(lat, lon);
                    });
                    if (area == nullptr) {
                        continue;
                    }
                    auto index = (y * resolution + (resolution - x - 1)) * 4;
                    area->getMetadata(&texture->at(index), &texture->at(index + 1), &texture->at(index + 2),
                                      &texture->at(index + 3));
                }
            }
        }
        //get current time
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Time to render map: " << elapsed_seconds.count() << "s\n";
        return texture;
    }
}

