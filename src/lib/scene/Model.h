//
// Created by tomas on 07.01.22.
//

#pragma once

#include <memory>
#include <vector>
#include <scene/Vertex.h>
#include <scene/Point.h>

namespace MapGenerator {
    class Model {
    public:
        Model();

        void addVertex(const Vertex &v, const Vertex &n = {0, 0, 0}, const PointF &uv = {0, 0});

        void addIndex(int i1, int i2, int i3);

        void addIndex(int i1);

        std::vector<float> vertices;
        std::vector<int> indices;

    };
}




