//
// Created by tomas on 06.02.22.
//

#include <scene/Vertex.h>
#include <cmath>

Vertex cross(const Vertex &a, const Vertex &b) {
    Vertex c;
    c.x = a.y * b.z - a.z * b.y;
    c.y = a.z * b.x - a.x * b.z;
    c.z = a.x * b.y - a.y * b.x;
    return c;
}

Vertex normalize(const Vertex &v) {
    Vertex n;
    n.x = v.x / std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    n.y = v.y / std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    n.z = v.z / std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return n;
}

