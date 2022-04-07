//
// Created by tomas on 06.04.22.
//

#include <scene/Uniform.h>

namespace MapGenerator {
    Uniform::Uniform() {

    }

    Uniform::Uniform(const std::string &name, Uniform::Type type, void *value) {
        this->name = name;
        this->type = type;
        switch (type) {
            case Uniform::Type::INT:
                this->valueInt = *(int *) value;
                break;
            case Uniform::Type::FLOAT:
                this->valueFloat = *(float *) value;
                break;
            case Uniform::Type::VEC2:
                this->valueVec2[0] = *(((float *) value) + 0);
                this->valueVec2[1] = *(((float *) value) + 1);
                break;
            case Uniform::Type::VEC3:
                this->valueVec3[0] = *(((float *) value) + 0);
                this->valueVec3[1] = *(((float *) value) + 1);
                this->valueVec3[2] = *(((float *) value) + 2);
                break;
            case Uniform::Type::VEC4:
                this->valueVec4[0] = *(((float *) value) + 0);
                this->valueVec4[1] = *(((float *) value) + 1);
                this->valueVec4[2] = *(((float *) value) + 2);
                this->valueVec4[3] = *(((float *) value) + 3);
                break;
        }
    }

}