//
// Created by tomas on 29.09.21.
//

#include <utility>
#include <vector>
#include <sstream>
#include <iostream>
#include "boost/filesystem.hpp"
#include "boost/algorithm/string.hpp"
#include "CsvDecoder.h"

CsvDecoder::CsvDecoder(std::string csv) {
    csvData = csv;
}




std::tuple<bool, CsvDecoder*> CsvDecoder::create(char *fileName) {
    if (!boost::filesystem::exists(fileName)) {
        std::cout << "Can't find file!" << std::endl;
        return {false, nullptr};
    }
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cout << "Can't open file!" << std::endl;
        return {false, nullptr};
    }
    std::string csv;
    std::stringstream buffer;
    buffer << file.rdbuf();
    csv = buffer.str();
    auto decoder = new CsvDecoder(csv);
    decoder->decode();
    return {true, decoder};

}

void CsvDecoder::decode() {

    std::vector<std::string> lines;
    boost::split(lines,csvData,boost::is_any_of("\n"));
    if(lines.size() <= 1){
        return;
    }
    decodeHeader(lines[0]);
    for(int i = 1; i < lines.size(); i++){
        std::vector<std::string> parts;
        boost::split(parts,lines[i],boost::is_any_of(";"));
        data.push_back(parts);
    }

}

void CsvDecoder::decodeHeader(const std::string& header) {
    std::vector<std::string> parts;
    boost::split(parts,header,boost::is_any_of(";"));
    headers = parts;
}

std::vector<std::string> CsvDecoder::getValues(const std::string& columnName, const std::string& columnValue) {
    auto iter = std::find(headers.begin(), headers.end(), columnName);
    if(iter == std::end(headers)){
        return {};
    }
    auto index= std::distance(headers.begin(), iter);
    for(auto & item : data){
        if(item[index] != columnValue){
            continue;
        }
        return item;
    }
    return {};
}


