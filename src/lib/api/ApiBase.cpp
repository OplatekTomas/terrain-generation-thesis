//
// Created by tomas on 30.09.21.
//

#include <api/ApiBase.h>
#include <cpr/cpr.h>
#include <iomanip>
#include <string>
#include <iostream>


namespace MapGenerator{
    ApiBase::ApiBase(std::string apiKey) {
        this->apiKey = std::move(apiKey);
    }
    std::string ApiBase::encodeUrl(const std::string& url){
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;

        for (std::string::const_iterator i = url.begin(), n = url.end(); i != n; ++i) {
            std::string::value_type c = (*i);

            // Keep alphanumeric and other accepted characters intact
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
                escaped << c;
                continue;
            }
            // Any other characters are percent-encoded
            escaped << std::uppercase;
            escaped << '%' << std::setw(2) << int((unsigned char) c);
            escaped << std::nouppercase;
        }

        return escaped.str();
    }

    inline uint32_t ApiBase::hash(const std::string &str) {
        int hash = 5381;
        for (char c : str) {
            hash = ((hash << 5) + hash) + c;
        }
        return hash;
    }
}



