//
// Created by tomas on 07.01.22.
//

#include <scene/Model.h>

namespace MapGenerator {
    Model::Model() {
        vertices = std::vector<float>();
        indices = std::vector<int>();
    }

    void Model::addVertex(const Vertex &v, const Vertex &n, const PointF &uv, const Vertex &t) {
        vertices.push_back(v.x);
        vertices.push_back(v.y);
        vertices.push_back(v.z);
        vertices.push_back(n.x);
        vertices.push_back(n.y);
        vertices.push_back(n.z);
        vertices.push_back(uv.x);
        vertices.push_back(uv.y);
        vertices.push_back(t.x);
        vertices.push_back(t.y);
        vertices.push_back(t.z);

    }

    void Model::addIndex(int i1, int i2, int i3) {
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    }

    void Model::addIndex(int i) {
        indices.push_back(i);
    }

    Vertex Model::calculateTangents(Vertex v1, Vertex v2, Vertex v3, PointF uv1, PointF uv2, PointF uv3) {
        Vertex tangent;
        float deltaX1 = v2.x - v1.x;
        float deltaX2 = v3.x - v1.x;
        float deltaY1 = v2.y - v1.y;
        float deltaY2 = v3.y - v1.y;
        float deltaZ1 = v2.z - v1.z;
        float deltaZ2 = v3.z - v1.z;
        float deltaU1 = uv2.x - uv1.x;
        float deltaU2 = uv3.x - uv1.x;
        float deltaV1 = uv2.y - uv1.y;
        float deltaV2 = uv3.y - uv1.y;
        float dividend = (deltaU1 * deltaV2 - deltaU2 * deltaV1);
        float f = dividend == 0.0f ? 0.0f : 1.0f / dividend;
        tangent.x = f * (deltaV2 * deltaX1 - deltaV1 * deltaX2);
        tangent.y = f * (deltaV2 * deltaY1 - deltaV1 * deltaY2);
        tangent.z = f * (deltaV2 * deltaZ1 - deltaV1 * deltaZ2);
        return tangent;
    }


    void Model::calculateTangents() {
        for (int i = 0; i < this->indices.size(); i += 3) {
            auto i1 = this->indices[i] * 11;
            auto i2 = this->indices[i + 1] * 11;
            auto i3 = this->indices[i + 2] * 11;
            auto v1 = Vertex(this->vertices[i1], this->vertices[i1 + 1], this->vertices[i1 + 2]);
            auto v2 = Vertex(this->vertices[i2], this->vertices[i2 + 1], this->vertices[i2 + 2]);
            auto v3 = Vertex(this->vertices[i3], this->vertices[i3 + 1], this->vertices[i3 + 2]);
            auto uv1 = PointF(this->vertices[i1 + 6], this->vertices[i1 + 7]);
            auto uv2 = PointF(this->vertices[i2 + 6], this->vertices[i2 + 7]);
            auto uv3 = PointF(this->vertices[i3 + 6], this->vertices[i3 + 7]);
            auto t1 = this->calculateTangents(v1, v2, v3, uv1, uv2, uv3);
            auto t2 = this->calculateTangents(v2, v3, v1, uv2, uv3, uv1);
            auto t3 = this->calculateTangents(v3, v1, v2, uv3, uv1, uv2);
            this->vertices[i1 + 8] = t1.x;
            this->vertices[i1 + 9] = t1.y;
            this->vertices[i1 + 10] = t1.z;
            this->vertices[i2 + 8] = t2.x;
            this->vertices[i2 + 9] = t2.y;
            this->vertices[i2 + 10] = t2.z;
            this->vertices[i3 + 8] = t3.x;
            this->vertices[i3 + 9] = t3.y;
            this->vertices[i3 + 10] = t3.z;
        }
        this->m_tangentsCalculated = true;
    }
}

