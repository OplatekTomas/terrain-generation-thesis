
//
// Created by tomas on 03.11.21.
//

#include "LandTypeGenerator.h"
#include <iostream>
#include <utility>
#include <omp.h>


namespace MapGenerator {

    LandTypeGenerator::LandTypeGenerator(std::shared_ptr<OSMData> osmData) {
        ySize = std::fabs(osmData->lat1 - osmData->lat2);
        xSize = std::fabs(osmData->lon1 - osmData->lon2);
        yStart = min(osmData->lat1, osmData->lat2);
        xStart = min(osmData->lon1, osmData->lon2);
        this->osmData = std::move(osmData);
    }


    void LandTypeGenerator::prepareAreas(){
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
    }

    shared_ptr<Texture> LandTypeGenerator::generateTexture(int res) {
        this->resolution = res;
        xStep = xSize / resolution;
        yStep = ySize / resolution;
        auto texture = std::vector<unsigned char>(resolution * resolution * 4);
        if(areas.empty()){
            prepareAreas();
        }
        //Get current time
        auto start = std::chrono::system_clock::now();
        int chunkSize = resolution / 8;

        auto threadCount = omp_get_max_threads();
        if(threadCount > 3){
            threadCount -= 2; //2 for main thread and other processes on the system
        }else{
            threadCount = 1;
        }
#pragma omp parallel for default(none) shared(texture, chunkSize) num_threads(threadCount)
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
                    area->getMetadata(&texture.at(index), &texture.at(index + 1), &texture.at(index + 2),
                                      &texture.at(index + 3));
                }
            }
        }
        //get current time
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Time to render map with resolution " << resolution << ": " << elapsed_seconds.count() << "s\n";
        auto tex = std::make_shared<Texture>(resolution, resolution, texture);
        return tex;
    }


}

