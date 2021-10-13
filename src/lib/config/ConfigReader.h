//
// Created by tomas on 04.10.21.
//

#pragma once

#include <config/Config.h>
#include <string>

namespace MapGenerator {

    class ConfigReader {

    public:
        static Config read(std::string path, bool* isError);
    };
}



