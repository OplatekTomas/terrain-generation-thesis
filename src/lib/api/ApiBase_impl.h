//
// Created by tomas on 30.09.21.
//

#ifndef VUT_BP_APIBASE_IMPL_H
#define VUT_BP_APIBASE_IMPL_H

#endif //VUT_BP_APIBASE_IMPL_H

#include <cpr/cpr.h>
#include "ApiBase.h"

template<class T> T ApiBase::sendRequest(std::string uri) {
    auto result = cpr::Get(cpr::Url{uri});
    return nullptr;
}