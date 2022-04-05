//
// Created by tomas on 05.04.22.
//

#include <scene/TextureArray.h>

namespace MapGenerator {


    std::shared_ptr<Texture> TextureArray::getTexture(int index) {
        if (index < 0 || index >= textures.size()) {
            return nullptr;
        }
        return textures[index];
    }

    void TextureArray::removeTexture(int index) {
        //remove from index
        textures.erase(textures.begin() + index);
    }

    void TextureArray::addTexture(std::shared_ptr<Texture> texture) {
        if (texture->getHeight() != height || texture->getWidth() != width) {
            return;
        }
        textures.push_back(texture);
    }

    TextureArray::TextureArray(std::vector<std::shared_ptr<Texture>> textures) {
        if (!textures.empty()) {
            this->textures = textures;
            this->height = textures[0]->getHeight();
            this->width = textures[0]->getWidth();
        }

    }

    TextureArray::TextureArray(int height, int width) {
        this->height = height;
        this->width = width;
    }
}


