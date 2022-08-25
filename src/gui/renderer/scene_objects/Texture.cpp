#include "renderer/SceneObject.h"
#include <cassert>
#include <renderer/scene_objects/Texture.h>

using namespace MapGenerator::Renderer::SceneObjects;

Texture::Texture(Type type, Format format, DataType dataType) : SceneObject(), type(type), format(format), dataType(dataType) {
    init();
}

Texture::Texture(const std::string& name, Type type, Format format, DataType dataType) : SceneObject(name), type(type), format(format), dataType(dataType) {
    init();
}

void Texture::init() {
    if (type == TYPE_2D) {
        dimensions.push_back(std::make_pair(0, 0));
    } else if (type == TYPE_CUBE) {
        dimensions.push_back(std::make_pair(0, 0));
        for (int i = 0; i < 6; i++) {
            data.push_back({});
        }
    }
}

void Texture::setDimensions(int width, int height) {
    assert(width > 0 && height > 0);
    assert(type == TYPE_2D || type == TYPE_CUBE);
    dimensions[0] = std::make_pair(width, height);
}

void Texture::addArray(int width, int height) {
    assert(width > 0 && height > 0);
    assert(type == TYPE_ARRAY);
    dimensions.push_back(std::make_pair(width, height));
}

void Texture::setArrayDimension(int width, int height, int layer) {
    assert(width > 0 && height > 0);
    assert(type == TYPE_ARRAY);
    assert(layer < dimensions.size());
    dimensions[layer] = std::make_pair(width, height);
}

void Texture::setData(const std::vector<unsigned char>& data, size_t layer) {
    assert(dataType == UNSIGNED_BYTE);
    assert(data.size() > 0);
    assert(layer < dimensions.size());
    this->data[layer] = data;
}

void Texture::setData(const std::vector<char>& data, size_t layer) {
    assert(dataType == BYTE);
    assert(data.size() > 0);
    assert(layer < dimensions.size());
    this->data[layer] = std::vector<unsigned char>((unsigned char*)data.data(), (unsigned char*)data.data() + data.size());
}

void Texture::setData(const std::vector<unsigned short>& data, size_t layer) {
    assert(dataType == UNSIGNED_SHORT);
    assert(data.size() > 0);
    assert(layer < dimensions.size());
    this->data[layer] = std::vector<unsigned char>((unsigned char*)data.data(), (unsigned char*)data.data() + data.size() * sizeof(unsigned short));
}

void Texture::setData(const std::vector<short>& data, size_t layer) {
    assert(dataType == SHORT);
    assert(data.size() > 0);
    assert(layer < dimensions.size());
    this->data[layer] = std::vector<unsigned char>((unsigned char*)data.data(), (unsigned char*)data.data() + data.size() * sizeof(short));
}

void Texture::setData(const std::vector<unsigned int>& data, size_t layer) {
    assert(dataType == UNSIGNED_INT);
    assert(data.size() > 0);
    assert(layer < dimensions.size());
    this->data[layer] = std::vector<unsigned char>((unsigned char*)data.data(), (unsigned char*)data.data() + data.size() * sizeof(unsigned int));
}

void Texture::setData(const std::vector<int>& data, size_t layer) {
    assert(dataType == INT);
    assert(data.size() > 0);
    assert(layer < dimensions.size());
    this->data[layer] = std::vector<unsigned char>((unsigned char*)data.data(), (unsigned char*)data.data() + data.size() * sizeof(int));
}

void Texture::setData(const std::vector<float>& data, size_t layer) {
    assert(dataType == FLOAT);
    assert(data.size() > 0);
    assert(layer < dimensions.size());
    this->data[layer] = std::vector<unsigned char>((unsigned char*)data.data(), (unsigned char*)data.data() + data.size() * sizeof(float));
}

void Texture::setData(const std::vector<double>& data, size_t layer) {
    assert(dataType == DOUBLE);
    assert(data.size() > 0);
    assert(layer < dimensions.size());
    this->data[layer] = std::vector<unsigned char>((unsigned char*)data.data(), (unsigned char*)data.data() + data.size() * sizeof(double));
}

void Texture::setData(const std::vector<unsigned char>& data){
    setData(data, 0);
}

void Texture::setData(const std::vector<char>& data){
    setData(data, 0);
}

void Texture::setData(const std::vector<unsigned short>& data){
    setData(data, 0);
}

void Texture::setData(const std::vector<short>& data){
    setData(data, 0);
}

void Texture::setData(const std::vector<unsigned int>& data){
    setData(data, 0);
}

void Texture::setData(const std::vector<int>& data){
    setData(data, 0);
}

void Texture::setData(const std::vector<float>& data){
    setData(data, 0);
}

void Texture::setData(const std::vector<double>& data){
    setData(data, 0);
}


Texture::~Texture() {
}