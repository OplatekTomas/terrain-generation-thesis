//
// Created by tomas on 08.10.21.
//

#include <MapGenerator.h>

#include <fmt/printf.h>
#include <shaders/Shaders.h>
#include <boolinq.h>

using namespace boolinq;

namespace MapGenerator {

    MapGenerator::MapGenerator(const LibConfig &config, GeneratorOptions options) {
        this->config = config;
        this->options = options;
        bing = std::make_unique<BingApi>(config.keys[0].key);
        osm = std::make_unique<OpenStreetMapApi>("");
        while (options.terrainResolution % 32 != 0) {
            options.terrainResolution++;
        }
        if (options.lat1 > options.lat2) {
            std::swap(options.lat1, options.lat2);
        }
        if (options.lon1 > options.lon2) {
            std::swap(options.lon1, options.lon2);
        }
    }

    /// This method will start the generation process. The data will be made available in the scene when ready.
    /// \return
    std::shared_ptr<class Scene> MapGenerator::generateMap() {
        scene = std::make_shared<Scene>();
        auto surface = createSurface();
        auto surfaceId = scene->addModel(surface);
        auto vertexId = scene->addShader(std::make_shared<Shader>(Shaders::VertexSource(), Shader::VERTEX));
        auto fragmentId = scene->addShader(std::make_shared<Shader>(Shaders::FragmentSource(), Shader::FRAGMENT));
        auto programId = scene->createProgram(vertexId, fragmentId);
        scene->bindProgram(surfaceId, programId);
        //generateTexturesAsync(surfaceId);

        std::thread t1(&MapGenerator::generateTexturesAsync, this, surfaceId);
        t1.detach();
        return scene;
    };
    void MapGenerator::generateTexturesAsync(int surfaceId) {
        int prevId = -1;
        int currentResolution = options.minTextureResolution;
        while (currentResolution <= options.maxTextureResolution) {
            auto texture = createLandTexture(currentResolution);
            auto texId = scene->addTexture(texture);
            if (prevId != -1) {
                scene->unbindTexture(prevId, surfaceId);
            }
            scene->bindTexture(texId, surfaceId);
            prevId = texId;
            currentResolution = currentResolution * options.textureResolutionStep;
        }

    }

    std::shared_ptr<Model> MapGenerator::createSurface() {
        auto data = bing->getElevationNormalized(options.lat1, options.lon1, options.lat2, options.lon2,
                                                 options.terrainResolution);
        if (data->getData()->empty()) {
            return nullptr;
        }
        auto model = std::make_shared<Model>();
        auto res = options.terrainResolution - 1;
        //Let's create the model
        auto fRes = (float) res;
        auto iRes = res;
        for (auto x = 0; x <= res; x++) {
            for (auto y = 0; y <= res; y++) {
                auto index = (x * (res + 1) + y);
                auto yPos = (float) y / fRes;
                auto xPos = (float) x / fRes;
                auto height = (float) data->getData()->at(index);
                // the (data scale / 4) is there to put it at the center of the 0-1 range
                model->addVertex(yPos, height, (xPos * data->getScale()) , yPos, xPos);
            }
        }

        //And add the indices
        for (int i = 0; i < iRes; ++i) {
            for (int j = 0; j < iRes; ++j) {
                int row1 = i * (iRes + 1);
                int row2 = (i + 1) * (iRes + 1);
                model->addIndex(row1 + j, row1 + j + 1, row2 + j + 1);
                model->addIndex(row1 + j, row2 + j + 1, row2 + j);
            }
        }
        return model;
    }

    std::shared_ptr<Texture> MapGenerator::createLandTexture(int resolution) {
        //Texture generator already exists and has all metadata pulled from the API - no need to do it again.
        if (textureGenerator != nullptr) {
            return textureGenerator->generateTexture(resolution);
        }
        if(options.lat1 > options.lat2) {
            std::swap(options.lat1, options.lat2);
        }
        if(options.lon1 > options.lon2) {
            std::swap(options.lon1, options.lon2);
        }
        auto data = osm->getMetadata(options.lat1, options.lon1, options.lat2, options.lon2);
        if (data == nullptr) {
            return {};
        }
        textureGenerator = std::make_shared<LandTypeGenerator>(data);
        auto tex = textureGenerator->generateTexture(resolution);
        return tex;
    }

}





