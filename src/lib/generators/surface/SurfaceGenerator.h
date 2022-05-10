//
// Created by tomas on 07.02.22.
//

#pragma once

#include <scene/Model.h>
#include <api/models/bing/ElevationData.h>
#include <memory>
#include <config/LibConfig.h>
#include <scene/Vertex.h>

namespace MapGenerator {
    class SurfaceGenerator {
    public:
        SurfaceGenerator(std::shared_ptr<ElevationData> elevationData, GeneratorOptions options);
        std::shared_ptr<Model> getSurface();

    private:
        std::shared_ptr<ElevationData> data;
        GeneratorOptions options;
    };

}



