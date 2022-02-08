//
// Created by tomas on 09.01.22.
//

#pragma once

namespace MapGenerator{
    typedef struct GeneratorOptions_T {
        // Min or max requested latitude
        double lat1{};
        // Max or min requested latitude
        double lat2{};
        // Min or max requested longitude
        double lon1{};
        // Max or min requested longitude
        double lon2{};
        // Specifies resolution of the terrain. When the value cannot be devised by 32 it will be rounded up
        int terrainResolution = 32;
        // Min resolution of the terrain texture ()
        int minTextureResolution = 512;
        // Max resolution of the terrain texture
        int maxTextureResolution = 8192;
        // How much does the texture scale up with each iteration
        int textureResolutionStep = 2;
        // Right now unused
        int seed{};
    } GeneratorOptions;
}
