//
// Created by tomas on 31.10.21.
//

#pragma once

template<typename T>
struct identity {
    typedef T type;
};

template<typename A, typename B>
inline bool mapContainsKey(const std::map <A, B> &m, const std::string &str) {
    return m.find(str) != m.end();
}