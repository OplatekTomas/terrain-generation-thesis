
#pragma once

#include <memory>
#include <renderer/scene_objects/Mesh.h>

using namespace MapGenerator::Renderer::SceneObjects;



class MeshGenerators {
    
public:
    static std::shared_ptr<Mesh> createIcoSphere(float radius, uint subdivisions, const std::string& name);

    static std::shared_ptr<Mesh> createPlane(float width, float height, const std::string& name);
};
