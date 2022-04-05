//
// Created by tomas on 05.04.22.
//


#pragma once


#include <vector>
#include <scene/Texture.h>
#include <memory>

namespace MapGenerator {
    class TextureArray {
    public:
        TextureArray(int height, int width);

        explicit TextureArray(std::vector<std::shared_ptr<Texture>> textures);

        void addTexture(std::shared_ptr<Texture> texture);

        void removeTexture(int index);

        std::shared_ptr<Texture> getTexture(int index);

        int getSize() { return textures.size(); }

        int getHeight() { return height; }

        int getWidth() { return width; }

    private:
        int height;
        int width;
        std::vector<std::shared_ptr<Texture>> textures;

    };
}


