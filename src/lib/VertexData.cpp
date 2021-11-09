//
// Created by tomas on 08.10.21.
//

#include "VertexData.h"

namespace MapGenerator {
    VertexData::VertexData(int vertexCount, int indexCount) {
        vertices = std::make_shared<std::vector<float>>();
        vertices->reserve(vertexCount);
        indices = std::make_shared<std::vector<int>>();
        indices->reserve(indexCount);
    }


    void VertexData::addVertex(float v1, float v2, float v3) {
        isUpdated = true;
        vertices->push_back(v1);
        vertices->push_back(v2);
        vertices->push_back(v3);
        vertices->push_back(v1);
        vertices->push_back(v3);
    }

    void VertexData::addIndex(int i1, int i2, int i3) {
        isUpdated = true;
        indices->push_back(i1);
        indices->push_back(i2);
        indices->push_back(i3);

    }


}

