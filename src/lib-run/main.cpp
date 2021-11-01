#include <iostream>
#include <MapGenerator.h>
#include <config/Config.h>
#include <config/ConfigReader.h>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        cout << "Please enter a keys.csv file as an arg" << endl;
        return 1;
    }
    bool hasError = false;
    auto config = MapGenerator::ConfigReader::read(argv[1], &hasError);
    if (hasError) {
        cout << "CSV Parsing failed!" << endl;
        return 1;
    }
    auto generator = new MapGenerator::MapGenerator(&config);
    std::vector<double> posHradec = {
            49.870291704376214, 17.860090558506485,
            49.888954501165955, 17.88689223413519,
    };
    auto draw = posHradec;
    auto data = generator->getVertices(draw[0], draw[1], draw[2], draw[3], 40);
    cout << endl;

    return 0;
}
