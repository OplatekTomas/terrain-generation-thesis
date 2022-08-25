#include "MeshGenerators.h"
#include <array>
#include <glm/geometric.hpp>
#include <vector>

namespace icosahedron {
    // Code inside this namespace is yoinked (and lightly edited) from following website:
    // https://schneide.blog/2016/07/15/generating-an-icosphere-in-c/

    struct Triangle {
        int vertex[3];
    };

    const float X = .525731112119133606f;
    const float Z = .850650808352039932f;
    const float N = 0.f;

    static const std::vector<glm::vec3> vertices = {
        {-X, N, Z}, {X, N, Z}, {-X, N, -Z}, {X, N, -Z}, {N, Z, X}, {N, Z, -X}, {N, -Z, X}, {N, -Z, -X}, {Z, X, N}, {-Z, X, N}, {Z, -X, N}, {-Z, -X, N}};

    static const std::vector<Triangle> triangles =
        {{0, 4, 1}, {0, 9, 4}, {9, 5, 4}, {4, 5, 8}, {4, 8, 1}, {8, 10, 1}, {8, 3, 10}, {5, 3, 8}, {5, 2, 3}, {2, 7, 3}, {7, 10, 3}, {7, 6, 10}, {7, 11, 6}, {11, 0, 6}, {0, 1, 6}, {6, 1, 10}, {9, 0, 11}, {9, 11, 2}, {9, 2, 5}, {7, 2, 11}};

    using Lookup = std::map<std::pair<int, int>, int>;

    int vertex_for_edge(Lookup& lookup, std::vector<glm::vec3>& vertices, int first, int second) {
        Lookup::key_type key(first, second);
        if (key.first > key.second)
            std::swap(key.first, key.second);

        auto inserted = lookup.insert({key, vertices.size()});
        if (inserted.second) {
            auto& edge0 = vertices[first];
            auto& edge1 = vertices[second];
            auto point = glm::normalize(edge0 + edge1);
            vertices.push_back(point);
        }

        return inserted.first->second;
    }

    std::vector<Triangle> subdivide(std::vector<glm::vec3>& vertices, std::vector<Triangle> triangles) {
        Lookup lookup;
        std::vector<Triangle> result;

        for (auto&& each : triangles) {
            std::array<int, 3> mid;
            for (int edge = 0; edge < 3; ++edge) {
                mid[edge] = vertex_for_edge(lookup, vertices, each.vertex[edge], each.vertex[(edge + 1) % 3]);
            }

            result.push_back({each.vertex[0], mid[0], mid[2]});
            result.push_back({each.vertex[1], mid[1], mid[0]});
            result.push_back({each.vertex[2], mid[2], mid[1]});
            result.push_back({mid[0], mid[1], mid[2]});
        }

        return result;
    }

    std::pair<std::vector<glm::vec3>, std::vector<Triangle>> make_icosphere(uint subdivisions) {
        std::vector<glm::vec3> vertices = icosahedron::vertices;
        std::vector<Triangle> triangles = icosahedron::triangles;
        for (uint i = 0; i < subdivisions; ++i) {
            triangles = subdivide(vertices, triangles);
        }
        return {vertices, triangles};
    }

} // namespace icosahedron

std::shared_ptr<Mesh> MeshGenerators::createIcoSphere(float radius, uint subdivisions, const std::string& name) {
    auto [vertices, indices] = icosahedron::make_icosphere(subdivisions);

    // todo build normals

    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(name);
    for (int i = 0; i < vertices.size(); ++i) {
        mesh->addVertex(vertices[i] * radius);
    }
    for (int i = 0; i < indices.size(); ++i) {
        mesh->addTriangleIndex(indices[i].vertex[0], indices[i].vertex[1], indices[i].vertex[2]);
    }
    return mesh;
}

std::shared_ptr<Mesh> MeshGenerators::createPlane(float width, float height, const std::string& name) {
    auto mesh = std::make_shared<Mesh>(name);
    mesh->addVertex(glm::vec3(-width, 0.f, -height));
    mesh->addVertex(glm::vec3(width, 0.f, -height));
    mesh->addVertex(glm::vec3(width, 0.f, height));
    mesh->addVertex(glm::vec3(-width, 0.f, height));
    mesh->addTriangleIndex(0, 1, 2);
    mesh->addTriangleIndex(0, 2, 3);
    return mesh;
}
