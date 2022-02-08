//
// Created by tomas on 07.01.22.
//

#pragma once

#include <memory>
#include <vector>
#include <scene/Vertex.h>

namespace MapGenerator {
    class Model {
    public:
        Model();
        void addVertex(Vertex v, Vertex n = {0,0,0}, float t1 = 0, float t2 = 0);

        void addIndex(int i1, int i2, int i3);

        std::vector<float> vertices;
        std::vector<int> indices;

    };
}




