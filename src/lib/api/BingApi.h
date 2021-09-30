//
// Created by tomas on 30.09.21.
//

#ifndef VUT_BP_BINGAPI_H
#define VUT_BP_BINGAPI_H

#pragma once

#include "ApiBase.h"
#include <cpr/cpr.h>


class BingApi : ApiBase {
public:
    explicit BingApi(std::string key);
private:
    std::string getBaseAddress() override;

};


#endif //VUT_BP_BINGAPI_H
