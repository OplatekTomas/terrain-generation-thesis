//
// Created by tomas on 03.04.22.
//

#pragma once

#include <GL/gl.h>
#include <string>
#include <typeinfo>
#include <vector>

#define YEET(x) throw new std::runtime_error(x)

std::string demangle(const char* name);
template <class T> std::string type(const T& t) {
    return demangle(typeid(t).name());
}

float lerp(float a, float b, float f);

std::vector<unsigned char> readPng(const std::string& path);

void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);