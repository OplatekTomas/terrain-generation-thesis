
#pragma once

#include <cstddef>
#include <geGL/geGL.h>
#include <memory>
#include <renderer/SceneObject.h>
#include <string>
#include <utility>
#include <vector>

namespace MapGenerator::Renderer::SceneObjects {

    class Texture : public SceneObject {
    public:
        enum Type {
            TYPE_2D,
            TYPE_CUBE,
            TYPE_ARRAY
        };

        enum Format {
            RED = GL_RED,
            RG = GL_RG,
            RGB = GL_RGB,
            BGR = GL_BGR,
            RGBA = GL_RGBA,
            BGRA = GL_BGRA
        };

        enum DataType{
            BYTE = GL_BYTE,
            UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
            SHORT = GL_SHORT,
            UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
            INT = GL_INT,
            UNSIGNED_INT = GL_UNSIGNED_INT,
            FLOAT = GL_FLOAT,
            DOUBLE = GL_DOUBLE,

        };

        Texture(Type type = TYPE_2D, Format format = RGBA, DataType dataType = UNSIGNED_BYTE);
        Texture(const std::string& name, Type type = TYPE_2D, Format format = RGBA, DataType dataType = UNSIGNED_BYTE);

        void setDimensions(int width, int height);
        void addArrayLayer();

        void setData(const std::vector<unsigned char>& data, size_t layer);
        void setData(const std::vector<char>& data, size_t layer);
        void setData(const std::vector<unsigned short>& data, size_t layer);
        void setData(const std::vector<short>& data, size_t layer);
        void setData(const std::vector<unsigned int>& data, size_t layer);
        void setData(const std::vector<int>& data, size_t layer);
        void setData(const std::vector<float>& data, size_t layer);
        void setData(const std::vector<double>& data, size_t layer);

        void setData(const std::vector<unsigned char>& data);
        void setData(const std::vector<char>& data);
        void setData(const std::vector<unsigned short>& data);
        void setData(const std::vector<short>& data);
        void setData(const std::vector<unsigned int>& data);
        void setData(const std::vector<int>& data);
        void setData(const std::vector<float>& data);
        void setData(const std::vector<double>& data);

        /**
         * @brief Binds the texture to the given texture unit.
           If the texture type is TYPE_ARRAY, the textures will be bound to unit + layer.
         * 
         * @param unit Unit to bind the texture to.
         */
        void bind(uint unit);
        std::shared_ptr<ge::gl::Texture> glTexture();
        ~Texture();

    private:
        void init();

        std::vector<std::vector<unsigned char>> data;
        std::pair<int, int> dimensions;
        std::shared_ptr<ge::gl::Texture> texture;
        Type type;
        GLenum  glType;
        Format format;
        DataType dataType;

    };
} // namespace MapGenerator::Renderer::SceneObjects
