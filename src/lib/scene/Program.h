//
// Created by tomas on 06.02.22.
//

#include <vector>

#pragma once

namespace MapGenerator {
    class Program {
    public:
        enum DrawTarget {
            DRAW_TO_SCREEN,
            DRAW_TO_TEXTURE,
            DRAW_DEPTH_TO_TEXTURE
        };

        Program() = default;

        std::vector<int> getShaders();

        int vertexShader = -1;
        int fragmentShader = -1;
        int geometryShader = -1;
        int computeShader = -1;
        int tessControlShader = -1;
        int tessEvaluationShader = -1;
        DrawTarget drawTarget = DRAW_TO_SCREEN;
        int drawTexture = -1;
        int drawTextureResolution = 0;
        // -1 represent infinite drawing
        int maxDrawCount = -1;
    };

}



