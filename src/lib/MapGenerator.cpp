//
// Created by tomas on 08.10.21.
//

#include <MapGenerator.h>
#include <generators/buildings/BuildingsGenerator.h>
#include <fmt/printf.h>
#include <filesystem>
#include <shaders/Shaders.h>
#include <boolinq.h>
#include <Helper.h>
#include <generators/surface/SurfaceGenerator.h>
#include <Logger.h>
using namespace boolinq;
namespace fs = std::filesystem;


namespace MapGenerator {

    MapGenerator::MapGenerator(const LibConfig &config) {
        heightTextureId = 0;
        this->canceled = false;
        this->config = config;
        this->options = config.options;
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
        this->canceled = false;
        //Create an empty scene.
        scene = std::make_shared<Scene>();
        //Start async texture generation.
        if (true) { //used for debug purposes where async is worse to use.
            std::thread t1(&MapGenerator::runAsyncGenerators, this);
            t1.detach();
        } else {
            runAsyncGenerators();
        }
        return scene;
    };


    void MapGenerator::generateHeightMap(int surfaceId, int vertexShaderId, int tcsShaderId) {
        int resolution = 2048;
        //Set up the terrain height map.
        auto heightMap = std::make_shared<Texture>(resolution, resolution);
        //Copy the original height data into the texture.
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
        auto programId = scene->addProgram(program);
        scene->bindProgram(programId, surfaceId);
    }


    void MapGenerator::runAsyncGenerators() {
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
        auto programId = scene->addProgram(program);
        loadTexturesForSurface(programId);

        scene->bindProgram(programId, surfaceId);
        vegetationGenerator = std::make_shared<VegetationGenerator>(options, elevationData);
        generateHeightMap(surfaceId, vertexId, tcsId);
        buildingsDone = false;
        //Grab OSM metadata
        if (options.lat1 > options.lat2) {
            std::swap(options.lat1, options.lat2);
        }
        if (options.lon1 > options.lon2) {
            std::swap(options.lon1, options.lon2);
        }
        osmData = osm->getMetadata(options.lat1, options.lon1, options.lat2, options.lon2);
        gotMetadata = true;
        if (osmData == nullptr) {
            Logger::log("OSM returned no data. Please go back and try again");
            return;
        }
        int prevId = -1;
        int currentResolution = options.minTextureResolution;
        bool vegetationDone = false;
        while (currentResolution <= options.maxTextureResolution) {
            if (osmData == nullptr) {
                return;
            }
            if (textureGenerator == nullptr) {
                textureGenerator = std::make_shared<LandTypeGenerator>(osmData);
            }
            if(canceled) {
                return;
            }
            auto texture = textureGenerator->generateTexture(currentResolution);
            if(canceled) {
                return;
            }
            auto texId = scene->addTexture(texture);
            if (prevId != -1) {
                scene->unbindTexture(prevId, programId);
            }
            scene->bindTexture(texId, programId);
            prevId = texId;

            //Now we can generate the trees -> when we have at least basic texture
            if(!buildingsDone){
                generateBuildings();

            }
            if (!vegetationDone && currentResolution >= 1024) {
                //Generate all types of vegetation
                generateVegetation(texture, currentResolution, VegetationGenerator::VegetationType::ConiferousForest);
                generateVegetation(texture, currentResolution, VegetationGenerator::VegetationType::DeciduousForest);
                generateVegetation(texture, currentResolution, VegetationGenerator::VegetationType::MixedForest);
                generateVegetation(texture, currentResolution, VegetationGenerator::VegetationType::Field);
                vegetationDone = true;
            }
            currentResolution = currentResolution * options.textureResolutionStep;
        }
    }

    std::shared_ptr<Model> MapGenerator::generateSurface() {
        elevationData = bing->getElevationNormalized(options.lat1, options.lon1, options.lat2, options.lon2,
                                                     options.terrainResolution);
        auto surfaceGenerator = std::make_shared<SurfaceGenerator>(elevationData, options);
        return surfaceGenerator->getSurface();
    }


    void MapGenerator::generateVegetation(const shared_ptr<Texture> &texture, int resolution,
                                          VegetationGenerator::VegetationType type) {
        auto vegetation = vegetationGenerator->getVegetation(texture, resolution, type);
        //Add vegetation to scene -- multiple vegetation types could have been generated, so we need to bind all of them.
        for (auto &i: vegetation) {
            auto vegetationId = scene->addModel(i);
            auto program = std::make_shared<Program>();
            program->vertexShader = scene->addShader(
                    std::make_shared<Shader>(Shaders::TreesVertexShader(), Shader::VERTEX));
            program->fragmentShader = scene->addShader(
                    std::make_shared<Shader>(Shaders::TreesFragmentShader(), Shader::FRAGMENT));
            program->drawTarget = Program::DRAW_TO_SCREEN;
            auto progId = scene->addProgram(program);
            scene->bindTexture(heightTextureId, progId);
            scene->bindProgram(progId, vegetationId);
        }
    }


    void MapGenerator::generateBuildings() {
        //Mostly just connecting all the programs together and making sure everything runs the way it should.
        auto surfaceGenerator = std::make_shared<BuildingsGenerator>(osmData, elevationData, options);
        auto model = surfaceGenerator->generate();
        auto modelId = scene->addModel(model);
        auto fragShader = std::make_shared<Shader>(Shaders::BuildingsFragmentShader(), Shader::FRAGMENT);
        auto vertexShader = std::make_shared<Shader>(Shaders::BuildingsVertexShader(), Shader::VERTEX);
        auto program = std::make_shared<Program>();
        program->vertexShader = scene->addShader(vertexShader);
        program->fragmentShader = scene->addShader(fragShader);
        auto programId = scene->addProgram(program);
        scene->bindTexture(heightTextureId, programId);
        scene->bindProgram(programId, modelId);
        buildingsDone = true;
    }

    void MapGenerator::loadTexturesForSurface(int surfaceId) {
        auto texturePath = "lib/assets/textures/";
        auto textureTypes = {"asphalt", "field", "meadow", "forrest", "water", "footpath", "sand"};
        for (std::string textureType: textureTypes) {
            auto textureArray = std::make_shared<TextureArray>(1024, 1024);
            auto dir = texturePath + textureType;
            //get files inside the folder
            auto files = std::vector<std::string>();
            for (auto &p: fs::directory_iterator(dir)) {
                files.push_back(p.path().string());
            }
            //sort files by name
            std::sort(files.begin(), files.end(), [](const std::string &a, const std::string &b) {
                return fs::path(a).filename() < fs::path(b).filename();
            });
            //load files
            for (auto &file: files) {
                //read the entire file using libpng
                auto data = readPng(file);
                if(data.empty()) {
                    Logger::log("ERROR: Could not read file: " + file);
                    data = std::vector<unsigned char>(1024 * 1024 * 4, 0);
                }
                auto texture = std::make_shared<Texture>(1024, 1024);
                auto stepSize = data.size() == 1024 * 1024 * 4 ? 4 : 3;
                for (int i = 0; i < data.size(); i += stepSize) {
                    texture->addPixel(data[i], data[i + 1], data[i + 2], 255);
                }
                textureArray->addTexture(texture);
            }
            auto id = scene->addTextureArray(textureArray);
            scene->bindTextureArray(id, surfaceId);
        }
    }

    void MapGenerator::cancel() {
        this->canceled = true;
        this->textureGenerator->cancel();
    }



}





