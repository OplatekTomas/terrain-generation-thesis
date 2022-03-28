#include <QApplication>
#include <QPushButton>
#include "MainWindow.h"
int main(int argc, char *argv[]) {
    std::srand(std::time(nullptr));
    QApplication a(argc, argv);
    MapGenerator::Renderer window;
    window.resize(1280, 720);
    window.show();
    //window.setWindowTitle(QApplication::translate("toplevel", "gui"));
    return QApplication::exec();
}
