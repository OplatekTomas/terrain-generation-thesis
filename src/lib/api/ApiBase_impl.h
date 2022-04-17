//
// Created by tomas on 30.09.21.
//

#pragma once

#include <api/ApiBase.h>
#include <cpr/cpr.h>
#include <iomanip>
#include <utility>
#include <iostream>

namespace MapGenerator {
    template<class T>
    std::shared_ptr<T> ApiBase::sendRequest(std::string uri, int *size) {
        auto url = cpr::Url{std::move(uri)};
        auto session = new cpr::Session();
        session->SetHeader({{"Content-Type", "application/json"}});
        session->SetUrl(url);
        auto result = session->Get();
        delete session;
        if (result.status_code != 200) {
            std::cerr << "Error: " << result.status_code << std::endl;
            return nullptr;
        }
        *size = result.downloaded_bytes;
        auto jsonObj = nlohmann::json::parse(result.text);
        auto obj = new T();
        nlohmann::from_json(jsonObj, *obj);
        auto sharedPtr = std::shared_ptr<T>(obj);
        //requestCache.insert(std::make_pair(h, sharedPtr));
        return sharedPtr;

    }

    template<class T>
    std::shared_ptr<T> ApiBase::postRequest(std::string uri, const std::string &bodyType, const std::string &body) {
        auto url = cpr::Url{std::move(uri)};
        auto session = new cpr::Session();
        session->SetHeader({{"Content-Type", bodyType}});
        session->SetUrl(url);
        session->SetBody(body);
        auto result = session->Post();
        delete session;
        if (result.status_code != 200) {
            return nullptr;
        }
        auto jsonObj = nlohmann::json::parse(result.text);
        auto obj = new T();
        nlohmann::from_json(jsonObj, *obj);
        auto sharedPtr = std::shared_ptr<T>(obj);
        //requestCache.insert(std::make_pair(h, sharedPtr));
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
