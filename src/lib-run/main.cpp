#include <iostream>
#include <api/BingApi.h>
#include <boost/format.hpp>
#include "ConfigReader.h"
#include "Config.h"
using namespace std;

int main(int argc, char *argv[]) {
    if(argc == 1){
        cout << "Please enter a keys.csv file as an arg" << endl;
        return 1;
    }
    bool hasError = false;
    auto config = MapGenerator::ConfigReader::read(argv[1], &hasError);
    if(hasError){
        cout << "CSV Parsing failed!" << endl;
        return 1;
    }
    auto api = new MapGenerator::BingApi(config.keys[0].key);
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
