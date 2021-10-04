//
// Created by tomas on 29.09.21.
//

#ifndef VUT_BP_APIBASE_H
#define VUT_BP_APIBASE_H

#include <string>
#include <utility>
#include "nlohmann/json.hpp"

namespace MapGenerator{
    class ApiBase {
    public:
        ApiBase(std::string apiKey);
        template<class T> bool sendRequest(std::string uri, T& obj);
    private:
        virtual std::string getBaseAddress() = 0;
    protected:
        std::string apiKey;

    };
};


#include "ApiBase_impl.h"


#endif //VUT_BP_APIBASE_H
