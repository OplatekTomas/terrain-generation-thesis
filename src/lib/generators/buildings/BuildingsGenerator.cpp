//
// Created by tomas on 08.02.22.
//

#include <generators/buildings/BuildingsGenerator.h>
#include <Helper.h>
#include <poly2tri/poly2tri.h>
#include <iostream>

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

    int BuildingsGenerator::isOutsideBounds(Point point) {
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

    /// @brief Generates the building
    /// \param building building to generate the mesh for
    /// \return The first tuple contains ground and roof mesh, the second contains ordered vertices
    std::tuple<std::vector<Vertex>, std::vector<Vertex>> BuildingsGenerator::generateBuilding(const Node &building) {
        auto poly = std::vector<p2t::Point *>();
        for (int i = 0; i < building.geometry->size() - 1; i++) {
            auto p1 = building.geometry->at(i);
            auto p2 = building.geometry->at(i + 1);
            auto p1Outside = isOutsideBounds(p1);
            auto p2Outside = isOutsideBounds(p2);
            if (p1Outside && p2Outside) {
                continue;
            }
            if (!p1Outside && !p2Outside) {
                poly.push_back(new p2t::Point(p1.lat, p1.lon));
                continue;
            }
            //This means that first point is inside the bounds and must be added. If the second point is inside,
            //it will be added later with the next iteration.
            if (!p1Outside) {
                poly.push_back(new p2t::Point(p1.lat, p1.lon));
            }

            // Calculate general equation of line from p1 to p2
            auto m = (p2.lat - p1.lat) / (p2.lon - p1.lon);
            auto b = p1.lat - m * p1.lon;
            auto outResult = p1Outside + p2Outside;
            //About 1 meter in lat and lon. This is used to make sure the height map is not too small.
            auto offset = 0.00001;
            p2t::Point newP;
            if (outResult == 1) {
                auto y = data->lat2 - offset;
                auto x = (y - b) / m;
                newP = {y, x};
            } else if (outResult == 2) {
                auto y = data->lat1 + offset;
                auto x = ((data->lat1) - b) / m;
                newP = {y, x};
            }
            if (outResult == 3) {
                auto x = data->lon2 - offset;
                auto y = m * data->lon2 + b;
                newP = {y, x};
            }
            if (outResult == 4) {
                auto x = data->lon1 + offset;
                auto y = m * data->lon1 + b;
                newP = {y, x};
            }
            poly.push_back(new p2t::Point(newP.x, newP.y));
            __asm__("nop");
        }
        auto cdt = std::make_shared<p2t::CDT>(poly);
        try {
            cdt->Triangulate();
        } catch (std::exception &e) {
            //The triangulation failed - set some kind of error flag TODO
            std::cout << e.what() << std::endl;
            return {};
        }
        auto triangles = cdt->GetTriangles();

        std::vector<Vertex> vertices;
        for (auto tri: triangles) {
            auto p1 = tri->GetPoint(0);
            auto p2 = tri->GetPoint(1);
            auto p3 = tri->GetPoint(2);
            vertices.emplace_back(Vertex(p1->x, 0, p1->y));
            vertices.emplace_back(Vertex(p2->x, 0, p2->y));
            vertices.emplace_back(Vertex(p3->x, 0, p3->y));
        }
        //Create the height offset from the height data
        auto min = std::numeric_limits<float>::max();
        auto origVertices = std::vector<Vertex>();
        for (auto &vertex: vertices) {
            //Normalize the coordinates for the height data
            origVertices.emplace_back(vertex);
            vertex = normaliseVertex(vertex);

        }
        int numberOfFloors = 1;
        auto heightInM = 420.0 / 100.0;

        if (mapContainsKey(*building.tags, "building:levels")) {
            numberOfFloors = std::stoi(building.tags->at("building:levels"));
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
        std::vector<Vertex> positions;
        for (auto &vertex: poly) {
            positions.emplace_back(Vertex(vertex->x, 0, vertex->y));
        }
        positions.emplace_back(positions[0]);
        return {vertices, positions};
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
            auto[ground, positions] = generateBuilding(building);
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
        for (int i = 0; i < model->vertices.size(); i += 8) {
            auto x = model->vertices.at(i);
            auto y = model->vertices.at(i + 1);
            auto z = model->vertices.at(i + 2);
            if (x > 1 || x < 0) {
                __asm__("nop");
            }
            if (z > 1 || z < 0) {
                __asm__("nop");
            }
        }
        return model;
    }

#pragma GCC pop_options

}




/*  auto vertices = std::vector<Vertex>();
  auto normals = std::vector<std::vector<Vertex>>((building.geometry->size() - 1) * 2);
  auto indices = std::vector<int>();

  for (int i = 0; i < building.geometry->size() - 1; i++) {
      auto pos1 = Vertex((float) building.geometry->at(i).lat, 0, (float) building.geometry->at(i).lon);
      auto[v1, v2] = this->buildingVertices[{pos1.x, pos1.z}];
      vertices.emplace_back(v1);
      vertices.emplace_back(v2);
      normals[i * 2].push_back({0, 1, 0});
      normals[i * 2 + 1].push_back({0, 1, 0});
  }
  for (int i = 0; i < vertices.size(); i += 2) {
      auto i2 = (i + 1) % vertices.size();
      auto i3 = (i + 2) % vertices.size();
      auto i4 = (i + 3) % vertices.size();
      indices.push_back(i);
      indices.push_back(i2);
      indices.push_back(i3);
      indices.push_back(i3);
      indices.push_back(i2);
      indices.push_back(i4);
  }

  for (int i = 0; i < indices.size(); i += 3) {
      auto v1 = vertices[indices[i]];
      auto v2 = vertices[indices[i + 1]];
      auto v3 = vertices[indices[i + 2]];
      auto normal = (v2 - v1).cross(v3 - v1).normalize();
      normals[indices[i]].push_back(normal);
      normals[indices[i + 1]].push_back(normal);
      normals[indices[i + 2]].push_back(normal);
  }
  for (int i = 0; i < normals.size(); i++) {
      Vertex normal = {0, 0, 0};
      Vertex prevNormal = {0, 0, 0};
      for (int j = 0; j < normals[i].size(); j++) {
          if (prevNormal == normals[i][j]) {
              continue;
          }
          normal += normals[i][j];
          prevNormal = normals[i][j];
      }
      normal = normal.normalize();
      model->addVertex(vertices[i], normal);
  }
  for (auto i: indices) {
      model->addIndex(i + index);
  }
  index += vertices.size();*/
/*if (outResult == 1 || outResult == 2) {
    auto x = (data->lat2 - b) / m;
    auto y = data->lat2;
    if (outResult == 2) {
        y = data->lat1;
        x = (data->lat1 - b) / m;
    }
    poly.push_back(new p2t::Point(y, x));
} else if (outResult == 3 || outResult == 4) {
    auto x = (data->lon2 - b) / m;
    auto y = data->lon2;
    if (outResult == 4) {
        y = data->lon1;
    }
    poly.push_back(new p2t::Point(y, x));
}*/