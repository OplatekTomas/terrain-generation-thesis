//
// Created by tomas on 17.04.22.
//

#pragma once

#include <functional>
#include <string>

namespace MapGenerator {
    class Logger {
    public:
        static void log(const std::string &message);

        static void init(bool logToConsole = false);

        static void setTargetFn(const std::function<void(const std::string &)>& target);

        static bool logToConsole;

    private:
        Logger(){};
        static bool isInitialized;
        static std::function<void(const std::string &)> targetFn;
    };
}





