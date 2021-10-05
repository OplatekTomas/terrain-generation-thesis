//
// Created by tomas on 30.09.21.
//

#include <api/ApiBase.h>

namespace MapGenerator{
    ApiBase::ApiBase(std::string apiKey) {
        this->apiKey = std::move(apiKey);
    }
}



