//
// Created by tomas on 14.10.21.
//

#pragma once

#include <api/ApiBase.h>

namespace MapGenerator{
    class OpenStreetMapApi : ApiBase {
    public:
        OpenStreetMapApi(std::string key);
        std::string getBaseAddress() override;
    };
}




