//
// Created by tomas on 06.04.22.
//
#include <string>

#pragma once

namespace MapGenerator {
    class Uniform {
    public:
        enum Type{
            FLOAT,
            INT,
            VEC2,
            VEC3,
            VEC4,
        };

        Uniform();
        Uniform(const std::string& name, Type type, void* value);



        std::string name;
        enum Type type;
        float valueFloat;
        int valueInt;
        float valueVec2[2];
        float valueVec3[3];
        float valueVec4[4];

    };


}




