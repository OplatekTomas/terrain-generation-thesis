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

    Vertex BuildingsGenerator::checkBounds(Vertex vertex){
        return vertex;
        //TODO fix this function - right now it is not working and i dont have time to fix it
        if(vertex.x < data->lat2 && vertex.x > data->lat1 && vertex.z < data->lon2 && vertex.z > data->lon1) {
            return vertex;
        }
        Vertex result = vertex;
        if(vertex.x < data->lat1) {
            result.x = data->lat1;
        }
        if(vertex.x > data->lat2) {
            result.x = data->lat2;
        }
        if(vertex.z < data->lon1) {
            result.z = data->lon1;
        }
        if(vertex.z > data->lon2) {
            result.z = data->lon2;
        }
        return result;
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
            vertices.emplace_back(checkBounds(Vertex(p1->x, 0, p1->y)));
            vertices.emplace_back(checkBounds(Vertex(p2->x, 0, p2->y)));
            vertices.emplace_back(checkBounds(Vertex(p3->x, 0, p3->y)));
            //vertices.emplace_back(p2->x, 0, p2->y);
            //vertices.emplace_back(p3->x, 0, p3->y);
        }
        //Create the height offset from the height data
        auto min = std::numeric_limits<float>::max();
        auto origVertices = std::vector<Vertex>();
        for (auto &vertex: vertices) {
            //Normalize the coordinates for the height data
            origVertices.emplace_back(vertex);
            vertex = normaliseVertex(vertex);
            //Calculate the index of the height data

        }
        int numberOfFloors = 1;
        auto heightInM = 420 / 100;

        if (mapContainsKey(*building.tags, "building:levels")) {
            numberOfFloors = std::stoi(building.tags->at("building:levels"));
        }
        auto height = ((heightInM - heightData->getNormalizedMin()) /
                       (heightData->getNormalizedMax() + heightData->getNormalizedMin())) * numberOfFloors;
        for (int i = 0; i < origVertices.size(); i++) {
            auto orig = origVertices[i];
            auto v = vertices[i];
            Vertex upperV = {v.x, (float) (height), v.z};
            std::tuple<double, float> key = {orig.x, orig.z};
            this->buildingVertices[key] = {v, upperV};
            vertices.emplace_back(upperV);
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
                Vertex normal = {0, 1, 0};
                model->addVertex(v, normal);
                model->addIndex(index);
                index++;
            }
            for (int i = 0; i < building.geometry->size() - 1; i++) {
                auto firstPos = std::make_tuple((float) building.geometry->at(i).lat,
                                                (float) building.geometry->at(i).lon);
                auto secondPos = std::make_tuple((float) building.geometry->at(i + 1).lat,
                                                 (float) building.geometry->at(i + 1).lon);
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
