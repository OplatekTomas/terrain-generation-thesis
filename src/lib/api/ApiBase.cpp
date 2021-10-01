//
// Created by tomas on 30.09.21.
//

#include "ApiBase.h"



ApiBase::ApiBase(std::string apiKey) {
    this->apiKey = std::move(apiKey);
}

