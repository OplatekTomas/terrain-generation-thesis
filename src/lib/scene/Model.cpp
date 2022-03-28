//
// Created by tomas on 07.01.22.
//

#include <scene/Model.h>

namespace MapGenerator {
    Model::Model() {
        vertices = std::vector<float>();
        indices = std::vector<int>();
    }

    void Model::addVertex(const Vertex& v, const Vertex& n, const PointF& uv) {
        vertices.push_back(v.x);
        vertices.push_back(v.y);
        vertices.push_back(v.z);
        vertices.push_back(n.x);
        vertices.push_back(n.y);
        vertices.push_back(n.z);
        vertices.push_back(uv.x);
        vertices.push_back(uv.y);

    }

    void Model::addIndex(int i1, int i2, int i3) {
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    }

    void Model::addIndex(int i) {
        indices.push_back(i);
    }
}

