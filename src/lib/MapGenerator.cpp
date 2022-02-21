//
// Created by tomas on 08.10.21.
//

#include <MapGenerator.h>
#include <generators/buildings/BuildingsGenerator.h>
#include <fmt/printf.h>
#include <shaders/Shaders.h>
#include <boolinq.h>
#include <generators/surface/SurfaceGenerator.h>

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
        auto surface = generateSurface();
        auto surfaceId = scene->addModel(surface);

        //Set up the shaders required for the terrain.
        auto vertexId = scene->addShader(std::make_shared<Shader>(Shaders::GroundVertexShader(), Shader::VERTEX));
        auto fragmentId = scene->addShader(std::make_shared<Shader>(Shaders::GroundFragmentShader(), Shader::FRAGMENT));
        auto tcsId = scene->addShader(
                std::make_shared<Shader>(Shaders::GroundTessellationControlShader(), Shader::TESS_CONTROL));
        auto tesId = scene->addShader(
                std::make_shared<Shader>(Shaders::GroundTessellationEvaluationShader(), Shader::TESS_EVALUATION));

        //Set up the program for the shaders.
        auto program = std::make_shared<Program>();
        program->vertexShader = vertexId;
        program->fragmentShader = fragmentId;
        program->tessControlShader = tcsId;
        program->tessEvaluationShader = tesId;
        auto programId = scene->createProgram(program);
        scene->bindProgram(surfaceId, programId);

        generateHeightMap(surfaceId, vertexId, tcsId);

        //Start async texture generation.
        if (true) {
            std::thread t1(&MapGenerator::runAsyncGenerators, this, programId);
            t1.detach();
        } else {
            runAsyncGenerators(programId);
        }

        return scene;
    };

    void MapGenerator::generateHeightMap(int surfaceId, int vertexShaderId, int tcsShaderId) {
        int resolution = 512;
        //Set up the terrain height map.
        auto heightMap = std::make_shared<Texture>(resolution, resolution);
        heightTextureId = scene->addTexture(heightMap);
        auto program = std::make_shared<Program>();
        program->vertexShader = vertexShaderId;
        program->fragmentShader = scene->addShader(
                std::make_shared<Shader>(Shaders::HeightmapFragmentShader(), Shader::FRAGMENT));
        program->tessControlShader = tcsShaderId;
        program->tessEvaluationShader = scene->addShader(
                std::make_shared<Shader>(Shaders::HeightmapEvaluationShader(), Shader::TESS_EVALUATION));
        program->drawTarget = Program::DRAW_TO_TEXTURE;
        program->drawTextureResolution = resolution;
        program->drawTexture = heightTextureId;
        program->maxDrawCount = -1;
        auto programId = scene->createProgram(program);
        scene->bindProgram(programId, surfaceId);
    }

    void MapGenerator::runAsyncGenerators(int surfaceProgramId) {
        //Grab OSM metadata
        if (options.lat1 > options.lat2) {
            std::swap(options.lat1, options.lat2);
        }
        if (options.lon1 > options.lon2) {
            std::swap(options.lon1, options.lon2);
        }
        osmData = osm->getMetadata(options.lat1, options.lon1, options.lat2, options.lon2);
        if (osmData == nullptr) {
            std::cerr << "Well fuck. There is no data";
            return;
        }
        generateBuildings();
        generateSurfaceTextures(surfaceProgramId);
    }

    std::shared_ptr<Model> MapGenerator::generateSurface() {
        elevationData = bing->getElevationNormalized(options.lat1, options.lon1, options.lat2, options.lon2,
                                                     options.terrainResolution);
        auto surfaceGenerator = std::make_shared<SurfaceGenerator>(elevationData, options);
        return surfaceGenerator->getSurface();
    }


    void MapGenerator::generateBuildings() {
        auto surfaceGenerator = std::make_shared<BuildingsGenerator>(osmData, elevationData, options);
        auto model = surfaceGenerator->generate();
        auto modelId = scene->addModel(model);
        auto fragShader = std::make_shared<Shader>(Shaders::BuildingsFragmentShader(), Shader::FRAGMENT);
        auto vertexShader = std::make_shared<Shader>(Shaders::BuildingsVertexShader(), Shader::VERTEX);
        auto program = std::make_shared<Program>();
        program->vertexShader = scene->addShader(vertexShader);
        program->fragmentShader = scene->addShader(fragShader);
        auto programId = scene->createProgram(program);
        scene->bindTexture(heightTextureId, programId);
        scene->bindProgram(programId, modelId);
    }


    void MapGenerator::generateSurfaceTextures(int programId) {
        int prevId = -1;
        int currentResolution = options.minTextureResolution;
        while (currentResolution <= options.maxTextureResolution) {
            if (osmData == nullptr) {
                return;
            }
            if (textureGenerator == nullptr) {
                textureGenerator = std::make_shared<LandTypeGenerator>(osmData);
            }
            auto texture = textureGenerator->generateTexture(currentResolution);
            auto texId = scene->addTexture(texture);
            if (prevId != -1) {
                scene->unbindTexture(prevId, programId);
            }
            scene->bindTexture(texId, programId);
            prevId = texId;
            currentResolution = currentResolution * options.textureResolutionStep;
        }
    }

}





