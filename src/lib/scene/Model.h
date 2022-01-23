//
// Created by tomas on 07.01.22.
//

#pragma once

#include <memory>
#include <vector>

namespace MapGenerator {
    class Model {
    public:
        Model();
        void addVertex(float v1, float v2, float v3, float t1 = 0, float t2 = 0, float n1 = 0, float n2 = 0, float n3 = 0);

        void addIndex(int i1, int i2, int i3);

        std::shared_ptr<std::vector<float>> vertices;
        std::shared_ptr<std::vector<int>> indices;
    };
}




