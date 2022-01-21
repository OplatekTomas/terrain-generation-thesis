//
// Created by tomas on 04.10.21.
//

#pragma once

#include <config/LibConfig.h>
#include <string>

namespace MapGenerator {

    class ConfigReader {

    public:
        static LibConfig read(std::string path, bool* isError);
    };
}



