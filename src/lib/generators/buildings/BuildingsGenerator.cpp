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
        this->updateZ = heightData->getScale() > 1;

        this->scale = this->updateZ ? 1 / heightData->getScale() : heightData->getScale();
        this->options = options;
        for (auto &node: data->getWays()) {
            if (!mapContainsKey(*node.tags, "building")) {
                continue;
            }
            buildings.push_back(node);
        }

    }


    Vertex BuildingsGenerator::normaliseVertex(Vertex vertex) {
        auto latDelta = options.lat2 - options.lat1;
        auto lonDelta = options.lon2 - options.lon1;
        auto x = (float) ((vertex.x - options.lat2) / latDelta);
        auto z = (float) ((vertex.z - options.lon2) / lonDelta);
        Vertex v;
        if (updateZ) {
            v = {(-z * scale) + (scale / 4), vertex.y, x + 1};
        } else {
            v = {-z, vertex.y, ((x + 1) * scale) + (scale / 4)};
        }
        return v;
    }


    std::vector<Vertex> BuildingsGenerator::generateBuilding(const Node &building) {
        auto poly = std::vector<p2t::Point *>();
        for (auto point: *building.geometry) {
            poly.push_back(new p2t::Point(point.lat, point.lon));
        }
        poly.pop_back();
        auto cdt = std::make_shared<p2t::CDT>(poly);
        try {
            cdt->Triangulate();
        } catch (std::exception) {
            //The triangulation failed - set some kind of error flag TODO
            return {};
        }
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
        //Create the height offset from the height data
        auto min = std::numeric_limits<float>::max();
        auto origVertices = std::vector<Vertex>();
        for (auto &vertex: vertices) {
            //Normalize the coordinates for the height data
            origVertices.emplace_back(vertex);
            vertex = normaliseVertex(vertex);
            //Calculate the index of the height data
            auto row = static_cast<int>(vertex.z * options.terrainResolution);
            auto col = static_cast<int>(vertex.x * options.terrainResolution);
            //Get the height data
            auto height = (float) heightData->getAt(row, col);
            min = std::min(min, height);
        }
        for (auto &vertex: vertices) {
            vertex.y = min;
        }

        int numberOfFloors = 1;
        auto heightInM = 420 / 100;

        if (mapContainsKey(*building.tags, "building:levels")) {
            numberOfFloors = std::stoi(building.tags->at("building:levels"));
        }
        auto height = ((heightInM - heightData->getNormalizedMin()) /
                       (heightData->getNormalizedMax() + heightData->getNormalizedMin())) * numberOfFloors;
        for(int i = 0; i < origVertices.size(); i++){
            auto orig = origVertices[i];
            auto v = vertices[i];
            Vertex upperV = {v.x, (float)(v.y + height), v.z};
            std::tuple<double, float> key = {orig.x, orig.z};
            this->buildingVertices[key] = {v, upperV};
            vertices.emplace_back(upperV);
            //this->buildingVertices.emplace(key, {v, {v.x, v.y + height, v.z}});
            printf("\n");
        }
        return vertices;
    }

#pragma GCC push_options
#pragma GCC optimize ("O0")
    std::shared_ptr<Model> BuildingsGenerator::generate() {
        auto model = std::make_shared<Model>();
        int index = 0;
        for (auto &building: buildings) {
            if (building.nodes->size() > 30) {
                continue;
            }
            //This will output the data into the buildingVertices map and return the geometry of a floor and a roof
            auto ground = generateBuilding(building);
            for (auto &v: ground) {
                model->addVertex(v, {0, 1, 0});
                model->addIndex(index);
                index++;
            }

            for (int i = 0; i < building.geometry->size() - 1; i++) {
                auto pos1 = Vertex((float)building.geometry->at(i).lat, 0, (float)building.geometry->at(i).lon);
                auto[v1, v2] = this->buildingVertices[{pos1.x, pos1.z}];
                auto pos2 = Vertex((float)building.geometry->at(i + 1).lat, 0, (float)building.geometry->at(i + 1).lon);
                auto[v3, v4] = this->buildingVertices[{pos2.x, pos2.z}];
                auto n1 = (v2 - v1).cross(v3 - v1).normalize();
                auto n2 = (v1 - v3).cross(v4 - v3).normalize();
                model->addVertex(v1, n1);
                model->addVertex(v2, n1);
                model->addVertex(v3, n1);
                model->addVertex(v2, n2);
                model->addVertex(v3, n2);
                model->addVertex(v4, n2);
                model->addIndex(index);
                model->addIndex(index + 1);
                model->addIndex(index + 2);
                model->addIndex(index + 3);
                model->addIndex(index + 4);
                model->addIndex(index + 5);
                index += 6;
                printf("\n");
            }
        }
        return model;
    }
#pragma GCC pop_options

}

