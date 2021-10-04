//
// Created by tomas on 30.09.21.
//

#ifndef VUT_BP_APIBASE_IMPL_H
#define VUT_BP_APIBASE_IMPL_H

#endif //VUT_BP_APIBASE_IMPL_H

#include "ApiBase.h"
#include <cpr/cpr.h>
namespace MapGenerator {
    template<class T> bool ApiBase::sendRequest(std::string uri, T& obj) {
        auto url = cpr::Url{uri};
        auto session = new cpr::Session();
        session->SetHeader({{"Content-Type", "application/json"}});
        session->SetUrl(url);
        auto result = session->Get();
        delete session;
        if(result.status_code != 200){
            return false;
        }
        auto jsonObj = nlohmann::json::parse(result.text);
        nlohmann::from_json(jsonObj, obj);
        return true;
    }
}
