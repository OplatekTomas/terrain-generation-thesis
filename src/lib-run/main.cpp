#include <iostream>
#include <MapGenerator.h>
#include <config/Config.h>
#include <config/ConfigReader.h>

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
    auto generator = new MapGenerator::MapGenerator(&config);
    generator->getVertices();
    cout << endl;

    return 0;
}
