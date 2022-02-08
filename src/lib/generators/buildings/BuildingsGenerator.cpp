//
// Created by tomas on 08.02.22.
//

#include <generators/buildings/BuildingsGenerator.h>
#include <Helper.h>
#include <poly2tri/poly2tri.h>

namespace MapGenerator {

    BuildingsGenerator::BuildingsGenerator(const std::shared_ptr<OSMData> &metadata,
                                           const std::shared_ptr<ElevationData> &heightData, GeneratorOptions options) {
        this->heightData = heightData;
        this->data = metadata;
        this->options = options;

        for (auto &node: data->getRelations()) {
            if (!mapContainsKey(*node.tags, "building")) {
                continue;
            }
            buildings.push_back(node);
        }
        for (auto &node: data->getWays()) {
            if (!mapContainsKey(*node.tags, "building")) {
                continue;
            }
            buildings.push_back(node);
        }

    }

    std::vector<Vertex> generateBuilding(const Node &building) {
        auto poly = std::vector<p2t::Point *>();
        for (auto point: *building.geometry) {
            poly.push_back(new p2t::Point(point.lat, point.lon));
        }
        poly.pop_back();
        auto cdt = std::make_shared<p2t::CDT>(poly);
        cdt->Triangulate();
        auto triangles = cdt->GetTriangles();
        std::vector<Vertex> vertices;
        for (auto tri: triangles) {
            auto p1 = tri->GetPoint(0);
            auto p2 = tri->GetPoint(1);
            auto p3 = tri->GetPoint(2);
            vertices.emplace_back(p1->x, 0, p1->y);
            vertices.emplace_back(p2->x, 0, p2->y);
            vertices.emplace_back(p3->x, 0, p3->y);
        }
        return vertices;
    }


    Vertex BuildingsGenerator::normaliseVertex(Vertex vertex) {
        auto latDelta = options.lat2 - options.lat1;
        auto lonDelta = options.lon2 - options.lon1;
        auto x = (float)((vertex.x - options.lon1) * lonDelta);
        auto z = (float)((vertex.z - options.lat1) * latDelta);
        return {x, 0, z};
    }

    std::shared_ptr<Model> MapGenerator::BuildingsGenerator::generate() {
        auto model = std::make_shared<Model>();
        int index = 0;

        for (auto &building: buildings) {
            auto buildingModel = generateBuilding(building);
            for (auto &vertex: buildingModel) {
                vertex = normaliseVertex(vertex);
                model->addVertex(vertex);
                model->addIndex(index, index + 1, index + 2);
                index += 3;
            }
        }

        return model;
    }


}

