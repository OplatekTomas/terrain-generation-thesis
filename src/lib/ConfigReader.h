//
// Created by tomas on 04.10.21.
//

#ifndef VUT_BP_CONFIGREADER_H
#define VUT_BP_CONFIGREADER_H

#include <string>

namespace MapGenerator {
    struct Config;

    class ConfigReader {
    public:
        static struct MapGenerator::Config read(std::string path, bool* isError);
    };
}



#endif //VUT_BP_CONFIGREADER_H
