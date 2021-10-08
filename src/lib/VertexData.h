

#pragma once
#include <memory>
#include <vector>
namespace MapGenerator{
    class VertexData {
    public:
        VertexData(int vertexCount, int indexCount);

        void addVertex(float v1, float v2, float v3);
        void addIndex(int i1, int i2,int i3);
        std::shared_ptr<std::vector<float>> vertices;
        std::shared_ptr<std::vector<int>> indices;
        bool isUpdated = false;
    };
}




