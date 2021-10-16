//
// Created by tomas on 04.10.21.
//

#include <config/ConfigReader.h>
#include <config/Config.h>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace MapGenerator{
    Config ConfigReader::read(std::string path, bool *isError) {
        std::error_code error;
        Config c;

        if (!std::filesystem::exists(path, error)) {
            *isError = true;
            return c;
        }
        std::ifstream file(path);
        if(!file.good()){
            *isError = true;
            return c;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        auto json = buffer.str();
        c = nlohmann::json::parse(json);
        *isError = false;
        return c;
    }
}
