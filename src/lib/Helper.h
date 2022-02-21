//
// Created by tomas on 31.10.21.
//

#pragma once


constexpr uint32_t hash(const char* data, size_t const size) noexcept{
    uint32_t hash = 5381;
    for (const char *c = data; c < data + size; ++c){
        hash = ((hash << 5) + hash) + (unsigned char) *c;
    }
    return hash;
}


template<typename A, typename B>
inline bool mapContainsKey(const std::map <A, B> &m, const std::string &str) {
    return m.find(str) != m.end();
}

template<typename A, typename B>
inline bool mapContainsKey(const std::map <A, B> &m, const A&key) {
    return m.find(key) != m.end();
}

template<typename A, typename B>
inline bool mapContainsKeyAndValue(const std::map <A, B> &m, const std::string& a, const std::string &b) {
    return m.find(a) != m.end() && m.find(a)->second == b;
}

template<typename A, typename B>
inline bool mapContainsKeyAndValue(const std::map <A, B> &m, const A& a, const B& b) {
    return m.find(a) != m.end() && m.find(a)->second == b;
}

//Calculate hash for string
inline uint32_t hash(const std::string &str) {
    int hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}


