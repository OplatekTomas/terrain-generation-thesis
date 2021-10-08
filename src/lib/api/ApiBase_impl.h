//
// Created by tomas on 30.09.21.
//

#pragma once

#include <api/ApiBase.h>
#include <cpr/cpr.h>

namespace MapGenerator {
    template<class T>
    std::shared_ptr<T> ApiBase::sendRequest(std::string uri) {
        auto url = cpr::Url{uri};
        auto session = new cpr::Session();
        session->SetHeader({{"Content-Type", "application/json"}});
        session->SetUrl(url);
        auto result = session->Get();
        delete session;
        if (result.status_code != 200) {
            return nullptr;
        }
        auto jsonObj = nlohmann::json::parse(result.text);
        auto obj = new T();
        nlohmann::from_json(jsonObj, *obj);
        auto sharedPtr = std::shared_ptr<T>(obj);
        return sharedPtr;
    }
}
