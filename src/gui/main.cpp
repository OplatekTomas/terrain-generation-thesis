#include <QApplication>
#include <QPushButton>
#include <iostream>
#include <fstream>

#ifdef DEBUG
#include "windows/DevelopWindow.h"
#else
#include "windows/MainWindow.h"
#endif

int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    if(argc != 2){
        std::cout << "You need to pass the path to config file as argument" << std::endl;
        return 1;
    }
    std::string path(argv[1]);
    //check if file exists
    std::ifstream file(path);
    if(!file.good()) {
        std::cout << "File " << path << " does not exist" << std::endl;
        return 1;
    }
    QApplication a(argc, argv);
    //MapGenerator::Renderer window;
#ifdef DEBUG
    DevelopWindow window;
#else
    MainWindow window;
#endif
    window.resize(1600, 900);
    window.show();
    window.setWindowTitle(QApplication::translate("toplevel", "EarthGamel"));
    return QApplication::exec();
}
