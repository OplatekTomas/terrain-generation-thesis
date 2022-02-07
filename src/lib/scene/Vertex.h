//
// Created by tomas on 06.02.22.
//

#pragma once

typedef struct Vector3 {
    float x, y, z;
    inline Vector3() : x(0), y(0), z(0) {}
    inline Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    inline Vector3(const Vector3 &v)  = default;
    inline Vector3 &operator=(const Vector3 &v) = default;
    inline Vector3 operator+(const Vector3 &v) const {
        return {x + v.x, y + v.y, z + v.z};
    }
    inline Vector3 operator-(const Vector3 &v) const {
        return {x - v.x, y - v.y, z - v.z};
    }
    inline Vector3 operator*(float f) const {
        return {x * f, y * f, z * f};
    }
    inline Vector3 operator/(float f) const {
        return {x / f, y / f, z / f};
    }
    inline Vector3 &operator+=(const Vector3 &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

} Vertex;

Vertex cross(const Vertex &a, const Vertex &b);
Vertex normalize(const Vertex &v);

