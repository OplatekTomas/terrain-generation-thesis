//
// Created by tomas on 07.01.22.
//

#include <scene/Texture.h>
#include <stdexcept>

namespace MapGenerator {

    Texture::Texture(int width, int height) {
        this->width = width;
        this->height = height;
        this->data = std::make_shared<std::vector<unsigned char>>();
        this->data->resize(width * height * 4);
        this->index = 0;
    }

    unsigned char * Texture::getData() const {
        return data->data();
    }



    int Texture::getWidth() const {
        return width;
    }

    int Texture::getHeight() const {
        return height;
    }


    void Texture::removePixel() {
        data->at(index + 0) = 0;
        data->at(index + 1) = 0;
        data->at(index + 2) = 0;
        data->at(index + 3) = 0;
        index -= 4;
    }

    void Texture::addPixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
        data->at(index + 0) = r;
        data->at(index + 1) = g;
        data->at(index + 2) = b;
        data->at(index + 3) = a;
        index += 4;
    }

    void Texture::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            throw std::out_of_range("Pixel out of range");
        }
        int idx = (y * width + x) * 4;
        data->at(idx + 0) = r;
        data->at(idx + 1) = g;
        data->at(idx + 2) = b;
        data->at(idx + 3) = a;
    }

    Texture::Texture(int width, int height, const std::vector<unsigned char> &data) {
        this->width = width;
        this->height = height;
        this->data = std::make_shared<std::vector<unsigned char>>(data);
    }

    void Texture::setPixel(int x, int y, float f) {
        auto const * p = reinterpret_cast<unsigned char const *>(&f);
        setPixel(x, y, p[0], p[1], p[2], p[3]);
    }

    Vertex Texture::getPixel(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return {};
        }
        int idx = (y * width + x) * 4;
        return Vertex(data->at(idx + 0), data->at(idx + 1), data->at(idx + 2));
    }


}
