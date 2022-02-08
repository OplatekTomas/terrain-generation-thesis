//
// Created by tomas on 08.10.21.
//

#include <MapGenerator.h>

#include <fmt/printf.h>
#include <shaders/Shaders.h>
#include <boolinq.h>
#include "generators/surface/SurfaceGenerator.h"

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

        //Create an empty scene.
        scene = std::make_shared<Scene>();

        //Add the surface mesh.
        auto surface = createSurface();
        auto surfaceId = scene->addModel(surface);

        //Set up the shaders required for the terrain.
        auto vertexId = scene->addShader(std::make_shared<Shader>(Shaders::MainVertexShader(), Shader::VERTEX));
        auto fragmentId = scene->addShader(std::make_shared<Shader>(Shaders::GroundFragmentShader(), Shader::FRAGMENT));
        auto tcsId = scene->addShader(std::make_shared<Shader>(Shaders::GroundTessellationControlShader(), Shader::TESS_CONTROL));
        auto tesId = scene->addShader(std::make_shared<Shader>(Shaders::GroundTessellationEvaluationShader(), Shader::TESS_EVALUATION));

        //Set up the program for the shaders.
        auto program = std::make_shared<Program>();
        program->vertexShader = vertexId;
        program->fragmentShader = fragmentId;
        program->tessControlShader = tcsId;
        program->tessEvaluationShader = tesId;
        auto programId = scene->createProgram(program);
        scene->bindProgram(surfaceId, programId);

        //Start async texture generation.
        std::thread t1(&MapGenerator::generateTexturesAsync, this, surfaceId);
        t1.detach();
        return scene;
    };

    std::shared_ptr<Model> MapGenerator::createSurface() {
        auto heightData = bing->getElevationNormalized(options.lat1, options.lon1, options.lat2, options.lon2, options.terrainResolution);
        auto surfaceGenerator = std::make_shared<SurfaceGenerator>(heightData, options);
        return surfaceGenerator->getSurface();
    }

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

    std::shared_ptr<Texture> MapGenerator::createLandTexture(int resolution) {
        //Texture generator already exists and has all metadata pulled from the API - no need to do it again.
        if (textureGenerator != nullptr) {
            return textureGenerator->generateTexture(resolution);
        }
        if (options.lat1 > options.lat2) {
            std::swap(options.lat1, options.lat2);
        }
        if (options.lon1 > options.lon2) {
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





