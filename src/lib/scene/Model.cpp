//
// Created by tomas on 07.01.22.
//

#include <scene/Model.h>

namespace MapGenerator {
    Model::Model() {
        vertices = std::vector<float>();
        indices = std::vector<int>();
    }
    void Model::addVertex(float v1, float v2, float v3, float t1, float t2, float n1, float n2, float n3) {
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);
        vertices.push_back(t1);
        vertices.push_back(t2);
        vertices.push_back(n1);
        vertices.push_back(n2);
        vertices.push_back(n3);
    }

    void Model::addIndex(int i1, int i2, int i3) {
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    }
}

