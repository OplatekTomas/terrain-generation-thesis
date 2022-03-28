//
// Created by tomas on 28.03.22.
//


#pragma once

namespace MapGenerator{
    template <typename T> struct Point{
        T x;
        T y;
        Point(T x, T y) : x(x), y(y){}
        Point() : x(0), y(0){}
        Point(const Point &p) : x(p.x), y(p.y){}
        Point &operator=(const Point &p){
            x = p.x;
            y = p.y;
            return *this;
        }
        bool operator==(const Point &p) const{
            return x == p.x && y == p.y;
        }
        bool operator!=(const Point &p) const{
            return x != p.x || y != p.y;
        }
        Point operator+(const Point &p) const{
            return Point(x + p.x, y + p.y);
        }
        Point operator-(const Point &p) const{
            return Point(x - p.x, y - p.y);
        }
        Point operator*(const Point &p) const{
            return Point(x * p.x, y * p.y);
        }
        Point operator/(const Point &p) const{
            return Point(x / p.x, y / p.y);
        }
        Point operator*(const T &p) const{
            return Point(x * p, y * p);
        }
        Point operator/(const T &p) const{
            return Point(x / p, y / p);
        }
        Point &operator+=(const Point &p){
            x += p.x;
            y += p.y;
            return *this;
        }
        Point &operator-=(const Point &p){
            x -= p.x;
            y -= p.y;
            return *this;
        }

        Point &operator*=(const Point &p){
            x *= p.x;
            y *= p.y;
            return *this;
        }
        Point &operator/=(const Point &p){
            x /= p.x;
            y /= p.y;
            return *this;
        }
        Point &operator*=(const T &p){
            x *= p;
            y *= p;
            return *this;
        }
        Point &operator/=(const T &p){
            x /= p;
            y /= p;
            return *this;
        }
        Point operator-() const{
            return Point(-x, -y);
        }
        Point operator+() const{
            return Point(x, y);
        }
        T length() const{
            return sqrt(x * x + y * y);
        }

    };

    typedef Point<int> PointI;
    typedef Point<float> PointF;
}


