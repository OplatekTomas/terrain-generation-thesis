#include <QApplication>
#include <QPushButton>
#include "MainWindow.h"
int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    QApplication a(argc, argv);
    //MapGenerator::Renderer window;
    MainWindow window;
    window.resize(1600, 900);
    window.show();
    window.setWindowTitle(QApplication::translate("toplevel", "EarthGamel"));
    return QApplication::exec();
}
