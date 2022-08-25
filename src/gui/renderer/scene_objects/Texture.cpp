#include "renderer/SceneObject.h"
#include <cassert>
#include <renderer/scene_objects/Texture.h>

using namespace MapGenerator::Renderer::SceneObjects;

Texture::Texture(Type type, Format format, DataType dataType)
        : SceneObject(), type(type), format(format), dataType(dataType) {
    init();
}

Texture::Texture(const std::string &name, Type type, Format format, DataType dataType)
        : SceneObject(name), type(type), format(format), dataType(dataType) {
    init();
}

void Texture::init() {
    if (type == TYPE_2D) {
        dimensions = {0, 0};
        data.emplace_back();
    } else if (type == TYPE_CUBE) {
        dimensions = {0, 0};
        for (int i = 0; i < 6; i++) {
            data.emplace_back();
        }
    }
    switch (type) {
        case TYPE_2D:
            glType = GL_TEXTURE_2D;
            break;
        case TYPE_CUBE:
            glType = GL_TEXTURE_CUBE_MAP;
            break;
        case TYPE_ARRAY:
            glType = GL_TEXTURE_2D_ARRAY;
            break;
    }
}


void Texture::setDimensions(int width, int height) {
    assert(width > 0 && height > 0);
    dimensions = std::make_pair(width, height);
}

void Texture::addArrayLayer() {
    assert(type == TYPE_ARRAY);
    data.emplace_back();
}

void Texture::setData(const std::vector<unsigned char> &data, size_t layer) {
    assert(dataType == UNSIGNED_BYTE);
    assert(data.size() > 0);
    assert(layer < data.size());
    this->data[layer] = data;
}

void Texture::setData(const std::vector<char> &data, size_t layer) {
    assert(dataType == BYTE);
    assert(data.size() > 0);
    assert(layer < data.size());
    auto ptr = (unsigned char *) data.data();
    this->data[layer] = std::vector<unsigned char>(ptr, ptr + data.size());
}

void Texture::setData(const std::vector<unsigned short> &data, size_t layer) {
    assert(dataType == UNSIGNED_SHORT);
    assert(data.size() > 0);
    assert(layer < data.size());
    auto ptr = (unsigned char *) data.data();
    this->data[layer] = std::vector<unsigned char>(ptr, ptr + data.size() * 2);
}

void Texture::setData(const std::vector<short> &data, size_t layer) {
    assert(dataType == SHORT);
    assert(data.size() > 0);
    assert(layer < data.size());
    auto ptr = (unsigned char *) data.data();
    this->data[layer] = std::vector<unsigned char>(ptr, ptr + data.size() * sizeof(short));
}

void Texture::setData(const std::vector<unsigned int> &data, size_t layer) {
    assert(dataType == UNSIGNED_INT);
    assert(data.size() > 0);
    assert(layer < data.size());
    auto ptr = (unsigned char *) data.data();
    this->data[layer] = std::vector<unsigned char>(ptr, ptr + data.size() * sizeof(unsigned int));
}

void Texture::setData(const std::vector<int> &data, size_t layer) {
    assert(dataType == INT);
    assert(data.size() > 0);
    assert(layer < data.size());
    auto ptr = (unsigned char *) data.data();
    this->data[layer] = std::vector<unsigned char>(ptr, ptr + data.size() * sizeof(int));
}

void Texture::setData(const std::vector<float> &data, size_t layer) {
    assert(dataType == FLOAT);
    assert(data.size() > 0);
    assert(layer < data.size());
    auto ptr = (unsigned char *) data.data();
    this->data[layer] = std::vector<unsigned char>(ptr, ptr + data.size() * sizeof(float));
}

void Texture::setData(const std::vector<double> &data, size_t layer) {
    assert(dataType == DOUBLE);
    assert(data.size() > 0);
    assert(layer < data.size());
    auto ptr = (unsigned char *) data.data();
    this->data[layer] = std::vector<unsigned char>(ptr, ptr + data.size() * sizeof(double));
}

void Texture::setData(const std::vector<unsigned char> &data) {
    setData(data, 0);
}

void Texture::setData(const std::vector<char> &data) {
    setData(data, 0);
}

void Texture::setData(const std::vector<unsigned short> &data) {
    setData(data, 0);
}

void Texture::setData(const std::vector<short> &data) {
    setData(data, 0);
}

void Texture::setData(const std::vector<unsigned int> &data) {
    setData(data, 0);
}

void Texture::setData(const std::vector<int> &data) {
    setData(data, 0);
}

void Texture::setData(const std::vector<float> &data) {
    setData(data, 0);
}

void Texture::setData(const std::vector<double> &data) {
    setData(data, 0);
}


Texture::~Texture() {
}

void Texture::bind(uint unit) {
    if (texture != nullptr) {
        texture->bind(unit);
        return;
    }
    auto [w, h] = dimensions;
    this->texture = std::make_shared<ge::gl::Texture>(glType, format, 0, w, h);
    switch (type) {
        case TYPE_2D:
            texture->setData2D(data[0].data(), format, dataType, 0, glType, 0, 0, w, h);
            break;
        case TYPE_ARRAY: { // Thanks for the redundant braces, C++
            auto flat = std::vector<unsigned char>(w * h * data.size());
            for (int i = 0; i < data.size(); i++) {
                std::copy(data[i].begin(), data[i].end(), flat.begin() + i * w * h);
            }
            texture->setData3D(flat.data(), format, dataType, 0, glType, 0, 0, 0, w, h, data.size());
            break;
        }
        case TYPE_CUBE:
            gl->glBindTexture(GL_TEXTURE_CUBE_MAP, texture->getId());
            for (int i = 0; i < 6; i++) {
                gl->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, w, h, 0, format, dataType,
                                 data[i].data());
            }
            break;
    }
    texture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->texParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture->texParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture->texParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture->texParameteri(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    GLfloat value, max_anisotropy = 8.0f; /* don't exceed this value...*/
    gl->glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &value);
    value = (value > max_anisotropy) ? max_anisotropy : value;
    gl->glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, value);
    texture->bind(unit);


}

std::shared_ptr<ge::gl::Texture> Texture::glTexture() {
    return texture;
}

