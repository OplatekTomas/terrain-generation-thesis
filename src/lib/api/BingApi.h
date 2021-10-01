//
// Created by tomas on 30.09.21.
//

#ifndef VUT_BP_BINGAPI_H
#define VUT_BP_BINGAPI_H

#pragma once

#include "ApiBase.h"
#include <cpr/cpr.h>
#include "../models/bing/ElevationResult.h"

class BingApi : ApiBase {
public:
    explicit BingApi(std::string key);
    quicktype::ElevationResult * getElevation();
private:
    std::string getBaseAddress() override;

};


#endif //VUT_BP_BINGAPI_H
