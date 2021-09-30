//
// Created by tomas on 30.09.21.
//

#include "BingApi.h"

#include <utility>

std::string BingApi::getBaseAddress() {
    return "http://dev.virtualearth.net/REST/v1/";
}

BingApi::BingApi(std::string key) : ApiBase(std::move(key)){
}
