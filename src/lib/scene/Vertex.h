//
// Created by tomas on 06.02.22.
//

#pragma once
#include <cmath>

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
    inline Vector3 &operator-=(const Vector3 &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    inline Vector3 &operator*=(float f) {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }
    inline Vector3 &operator/=(float f) {
        x /= f;
        y /= f;
        z /= f;
        return *this;
    }
    inline float length() const {
        return sqrtf(x * x + y * y + z * z);
    }
    inline float lengthSquared() const {
        return x * x + y * y + z * z;
    }
    [[nodiscard]] inline Vector3 normalize() const {
        float l = length();
        return {x / l, y / l, z / l};
    }
    [[nodiscard]] inline float dot(const Vector3 &v) const {
        return x * v.x + y * v.y + z * v.z;
    }
    [[nodiscard]] inline Vector3 cross(const Vector3 &v) const {
        return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
    }

} Vertex;

Vertex cross(const Vertex &a, const Vertex &b);
Vertex normalize(const Vertex &v);

