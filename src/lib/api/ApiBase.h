//
// Created by tomas on 29.09.21.
//

#ifndef VUT_BP_APIBASE_H
#define VUT_BP_APIBASE_H

#include <string>
#include <utility>

class ApiBase {
public:
    ApiBase(std::string apiKey);
    template <class T> T sendRequest(std::string uri);
private:
    virtual std::string getBaseAddress() = 0;
protected:
    std::string apiKey;

};

#include "ApiBase_impl.h"


#endif //VUT_BP_APIBASE_H
