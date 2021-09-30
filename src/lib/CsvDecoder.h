//
// Created by tomas on 29.09.21.
//

#ifndef VUT_BP_CSVDECODER_H
#define VUT_BP_CSVDECODER_H
#include <string>
#include <vector>
#include <unordered_map>

class CsvDecoder {
public:
    static std::tuple<bool, CsvDecoder *> create(char* fileName);
    std::vector<std::string> getValues(const std::string& columnName, const std::string& columnValue);
private:
    explicit CsvDecoder(std::string csv);
    void decode();
    std::string csvData;

    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> data;


    void decodeHeader(const std::string &header);

};


#endif //VUT_BP_CSVDECODER_H
