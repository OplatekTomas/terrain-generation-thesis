//
// Created by tomas on 17.04.22.
//

#include "Logger.h"
#include <iostream>

namespace MapGenerator {

    //Define static variables
    bool Logger::isInitialized;
    std::function<void(const std::string &)> Logger::targetFn;
    bool Logger::logToConsole;


    void Logger::log(const std::string &message) {
            if (!Logger::isInitialized) {
                std::cout << "ERROR LOGGER NOT INITIALIZED" << std::endl;
                std::cout << "CALL Logger::init() AT LEAST ONCE" << std::endl;
                return;
            }
            if (Logger::logToConsole) {
                std::cout << message << std::endl;
            }
            Logger::targetFn(message);
    }

    void Logger::setTargetFn(const std::function<void(const std::string &)> &target) {
        Logger::targetFn = target;
    }

    void Logger::init(bool console) {
        Logger::logToConsole = console;
        Logger::targetFn = [](const std::string &message) {};
        Logger::isInitialized = true;
    }


}

