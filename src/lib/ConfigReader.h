//
// Created by tomas on 04.10.21.
//

#pragma once


#include <string>

namespace MapGenerator {
    struct Config;

    class ConfigReader {
    public:
        static struct MapGenerator::Config read(std::string path, bool* isError);
    };
}



