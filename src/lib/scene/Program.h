//
// Created by tomas on 06.02.22.
//

#include <vector>

#pragma once

namespace MapGenerator{
    class Program {
    public:
        Program() = default;
        std::vector<int> getShaders();
        int vertexShader = -1;
        int fragmentShader = -1;
        int geometryShader = -1;
        int computeShader = -1;
        int tessControlShader = -1;
        int tessEvaluationShader = -1;
    };

}



