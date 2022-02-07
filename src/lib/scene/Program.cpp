//
// Created by tomas on 06.02.22.
//

#include <scene/Program.h>

namespace MapGenerator{
    std::vector<int> Program::getShaders() {
        return {vertexShader, fragmentShader, geometryShader, tessControlShader, tessEvaluationShader};
    }
}

