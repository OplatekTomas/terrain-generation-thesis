#include <QApplication>
#include <QPushButton>
#include "MainWindow.h"
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow window;
    window.show();
    window.setWindowTitle(
            QApplication::translate("toplevel", "gui"));
    return QApplication::exec();
}
