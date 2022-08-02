//
// Created by tomas on 03.04.22.
//

#pragma once

#include <GL/gl.h>
#include <string>
#include <vector>

float lerp(float a, float b, float f);

std::vector<unsigned char> readPng(const std::string& path);

void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);