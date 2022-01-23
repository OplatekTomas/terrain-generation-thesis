//
// Created by tomas on 07.01.22.
//

#pragma once

#include <memory>
#include <vector>

namespace MapGenerator {
    class Texture {
    public:
        Texture(int width, int height);
        Texture(int width, int height, const std::vector<unsigned char> &data);

        unsigned char * getData() const;

        int getWidth() const;

        int getHeight() const;

        void addPixel(unsigned char r,unsigned char g,unsigned char b,unsigned char a);

        void setPixel(int x, int y,unsigned char r,unsigned char g,unsigned char b,unsigned char a);

        void removePixel();


    private:
        int width;
        int height;
        int index;
        int maxIndex;
        std::shared_ptr<std::vector<unsigned char>> data;
    };
}




