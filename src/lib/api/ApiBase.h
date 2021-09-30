//
// Created by tomas on 29.09.21.
//

#ifndef VUT_BP_APIBASE_H
#define VUT_BP_APIBASE_H

#include <string>
#include <utility>

class ApiBase {
public:
    ApiBase(std::string apiKey){
        this->apiKey = std::move(apiKey);
    }
private:
    virtual std::string getBaseAddress() = 0;
    std::string apiKey;
};


#endif //VUT_BP_APIBASE_H
