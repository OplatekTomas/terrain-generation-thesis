//
// Created by tomas on 07.01.22.
//

#pragma once

#include <memory>
#include <vector>
#include <scene/Vertex.h>
#include <scene/Point.h>
#include <scene/Uniform.h>

namespace MapGenerator {
    class Model {
    public:
        Model();

        void
        addVertex(const Vertex &v, const Vertex &n = {0, 0, 0}, const PointF &uv = {0, 0}, const Vertex &t = {0, 0, 0});

        void addIndex(int i1, int i2, int i3);

        void addIndex(int i1);

        void calculateTangents();

        bool tangentsCalculated() const { return m_tangentsCalculated; }

        std::vector<float> vertices;
        std::vector<int> indices;

        bool isInstanced = false;
        std::vector<float> instanceData;
        std::vector<std::shared_ptr<class Uniform>> uniforms;

    private:
        Vertex calculateTangents(Vertex v1, Vertex v2, Vertex v3, PointF uv1, PointF uv2, PointF uv3);

        bool m_tangentsCalculated = false;

    };
}




