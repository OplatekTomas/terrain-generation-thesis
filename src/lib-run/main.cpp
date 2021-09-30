#include <iostream>
#include "CsvDecoder.h"
#include "api/BingApi.h"
using namespace std;

int main(int argc, char *argv[]) {
    if(argc == 1){
        cout << "Please enter a keys.csv file as an arg" << endl;
        return 1;
    }
    auto data = CsvDecoder::create(argv[1]);
    auto success = get<bool>(data);
    if(!success){
        cout << "CSV Parsing failed!" << endl;
        return 1;
    }
    auto reader = get<CsvDecoder*>(data);
    auto key = reader->getValues("name", "bing");

    auto api = new BingApi(key[1]);


    return 0;
}
