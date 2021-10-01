#include <iostream>
#include "CsvDecoder.h"
#include <boost/format.hpp>
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
    auto elevation = api->getElevation();
    if(elevation == nullptr){
        return 1;
    }
    cout << "Printing heights: " << endl;
    for(auto height: elevation->getResourceSets()[0].getResources()[0].getElevations()) {
        std::cout << boost::str(boost::format("%.2f") % height) << " ";
    }
    cout << endl;

    return 0;
}
