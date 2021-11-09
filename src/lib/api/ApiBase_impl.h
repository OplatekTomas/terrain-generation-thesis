//
// Created by tomas on 30.09.21.
//

#pragma once

#include <api/ApiBase.h>
#include <cpr/cpr.h>
#include <iomanip>

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

    template<class T>
    std::shared_ptr<T> ApiBase::readData(std::string path) {
        std::ifstream t(path);
        std::stringstream buffer;
        buffer << t.rdbuf();
        auto jsonObj = nlohmann::json::parse(buffer.str());
        auto obj = new T();
        nlohmann::from_json(jsonObj, *obj);
        auto sharedPtr = std::shared_ptr<T>(obj);
        return sharedPtr;
    }

}
