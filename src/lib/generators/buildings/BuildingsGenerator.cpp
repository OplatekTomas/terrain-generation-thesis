//
// Created by tomas on 08.02.22.
//

#include <generators/buildings/BuildingsGenerator.h>
#include <Helper.h>
#include <iostream>
#include <CDT.h>
#include <Logger.h>
#include <chrono>

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
            v = {(-z * scale) + ((1 - scale) / 2), vertex.y, x + 1};
        } else {
            v = {-z, vertex.y, ((x + 1) * scale) + (1 - scale) / 2};
        }
        return v;
    }

    int BuildingsGenerator::isOutsideBounds(GeoPoint point) {
        if (point.lat > data->lat2) {
            return 1;
        }
        if (point.lat < data->lat1) {
            return 2;
        }
        if (point.lon > data->lon2) {
            return 3;
        }
        if (point.lon < data->lon1) {
            return 4;
        }
        return 0;
    }

    std::vector<std::tuple<double, double>> BuildingsGenerator::clipBuilding(const Node &building) {
        auto result = std::vector<std::tuple<double, double>>();
        for (int i = 0; i < building.geometry->size() - 1; i++) {
            auto p1 = building.geometry->at(i);
            auto p2 = building.geometry->at(i + 1);
            auto p1Outside = isOutsideBounds(p1);
            auto p2Outside = isOutsideBounds(p2);
            if (p1Outside && p2Outside) {
                continue;
            }
            if (!p1Outside && !p2Outside) {
                result.emplace_back(p1.lat, p1.lon);
                continue;
            }
            //This means that first point is inside the bounds and must be added. If the second point is inside,
            //it will be added later with the next iteration.
            if (!p1Outside) {
                result.emplace_back(p1.lat, p1.lon);
            }
            // Calculate general equation of line from p1 to p2
            auto m = (p2.lat - p1.lat) / (p2.lon - p1.lon);
            auto b = p1.lat - m * p1.lon;
            auto outResult = p1Outside + p2Outside;
            //About 1 meter in lat and lon. This is used to make sure the height map is not too small.
            auto offset = 0.00001;
            double x = 0.0;
            double y = 0.0;
            switch (outResult) {
                case 1:
                    y = data->lat2 - offset;
                    x = (y - b) / m;
                    if (std::isnan(x)) {
                        x = p2.lon - offset;
                    }
                    break;
                case 2:
                    y = data->lat1 + offset;
                    x = ((data->lat1) - b) / m;
                    if (std::isnan(x)) {
                        x = p1.lon + offset;
                    }
                    break;
                case 3:
                    x = data->lon2 - offset;
                    y = m * data->lon2 + b;
                    if (std::isnan(y)) {
                        y = p2.lat - offset;
                    }
                    break;
                case 4:
                    x = data->lon1 + offset;
                    y = m * data->lon1 + b;
                    if (std::isnan(y)) {
                        y = p1.lat + offset;
                    }
                    break;
            }
            result.emplace_back(y, x);
            __asm__("nop");
        }
        __asm__("nop");
        return result;
    }


    /// @brief Generates the building
    /// \param building building to generate the mesh for
    /// \return The first tuple contains ground and roof mesh, the second contains ordered vertices
    std::tuple<std::vector<Vertex>, std::vector<Vertex>> BuildingsGenerator::generateBuilding(const Node &building) {
        auto coords = clipBuilding(building);
        if (coords.size() < 3) {
            return {};
        }
        CDT::Triangulation<double> cdt;

        cdt.insertVertices(coords.begin(), coords.end(), [](const std::tuple<double, double> v) {
            return std::get<0>(v);
        }, [](const std::tuple<double, double> v) {
            return std::get<1>(v);
        });
        std::vector<CDT::Edge> edges;
        for (auto i = 0; i < coords.size() - 1; i++) {
            edges.emplace_back(i, i + 1);
        }
        edges.emplace_back(coords.size() - 1, 0);
        cdt.insertEdges(edges);
        cdt.eraseOuterTrianglesAndHoles();
        auto triangles = cdt.triangles;
        std::vector<Vertex> vertices;
        std::vector<Vertex> positions;
        if (triangles.empty()) {
            return {};
        }
        for (auto t: triangles) {
            auto v1 = Vertex((float) std::get<0>(coords[t.vertices[0]]), 0, (float) std::get<1>(coords[t.vertices[0]]));
            auto v2 = Vertex((float) std::get<0>(coords[t.vertices[1]]), 0, (float) std::get<1>(coords[t.vertices[1]]));
            auto v3 = Vertex((float) std::get<0>(coords[t.vertices[2]]), 0, (float) std::get<1>(coords[t.vertices[2]]));
            vertices.emplace_back(v1);
            vertices.emplace_back(v2);
            vertices.emplace_back(v3);
        }
        //Create the height offset from the height data
        auto origVertices = std::vector<Vertex>();
        for (auto &vertex: vertices) {
            //Normalize the coordinates for the height data
            origVertices.emplace_back(vertex);
            vertex = normaliseVertex(vertex);
        }
        int numberOfFloors = 1;
        auto heightInM = 420.0 / 100.0;

        if (mapContainsKey(*building.tags, "building:levels")) {
            try{
                numberOfFloors = std::stoi(building.tags->at("building:levels"));
            }catch(...){
                numberOfFloors = 1;
            }
        }

        auto height = ((heightInM - heightData->getNormalizedMin()) /
                       (heightData->getNormalizedMax() + heightData->getNormalizedMin())) * numberOfFloors;
        for (int i = 0; i < origVertices.size(); i++) {
            auto orig = origVertices[i];
            auto v = vertices[i];
            Vertex upperV = {v.x, (float) (height), v.z};
            std::tuple<double, double> key = {orig.x, orig.z};
            this->buildingVertices[key] = {v, upperV};
            vertices.emplace_back(upperV);
        }
        for (auto &vertex: coords) {
            auto key = std::tuple<double, double>((double) (float) std::get<0>(vertex),
                                                  (double) (float) std::get<1>(vertex));
            if (!mapContainsKey(this->buildingVertices, key)) {
                continue;
            }

            positions.emplace_back(Vertex(std::get<0>(vertex), 0, std::get<1>(vertex)));
        }
        positions.emplace_back(positions[0]);
        return {vertices, positions};
    }


    std::shared_ptr<Model> BuildingsGenerator::generate() {
        //grab time
        auto start = std::chrono::high_resolution_clock::now();
        Logger::log("Generating buildings...");

        auto model = std::make_shared<Model>("buildings");
        int index = 0;
        std::vector<std::tuple<std::vector<Vertex>, std::vector<Vertex>>> buildingModels;
        for (auto &building: buildings) {
            //This will output the data into the buildingVertices map and return the geometry of a floor and a roof
            auto result = generateBuilding(building);
            if (std::get<0>(result).empty() && std::get<1>(result).empty()) {
                continue;
            }
            buildingModels.emplace_back(result);
        }
        for (auto[ground, positions]: buildingModels) {
            for (auto &v: ground) {
                Vertex normal = {0, 1, 0};
                model->addVertex(v, normal);
                model->addIndex(index);
                index++;

            }
            //Let's build the walls and make compiler pay for it
            for (int i = 0; i < ((int) positions.size()) - 1; i++) {
                auto firstPos = std::make_tuple((double) positions.at(i).x, (double) positions.at(i).z);
                auto secondPos = std::make_tuple((double) positions.at(i + 1).x, (double) positions.at(i + 1).z);

                auto[v0, v1] = buildingVertices[firstPos];
                auto[v2, v3] = buildingVertices[secondPos];
                auto normal = (v2 - v1).cross(v0 - v1);
                model->addVertex(v0, normal);
                model->addVertex(v1, normal);
                model->addVertex(v2, normal);
                model->addVertex(v3, normal);
                model->addIndex(index);
                model->addIndex(index + 1);
                model->addIndex(index + 2);
                model->addIndex(index + 1);
                model->addIndex(index + 3);
                model->addIndex(index + 2);
                index += 4;
            }

        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        Logger::log("Building generation took: " + std::to_string(duration.count()) + "s");
        return model;
    }
}


/*

            */