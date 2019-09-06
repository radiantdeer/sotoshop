#include <QApplication>
#include "frontend/MainWindow.hpp"

int main (int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow rootWindow;
    rootWindow.setWindowTitle("SotoShop");
    rootWindow.setFixedSize(800,600); 

    rootWindow.show();

    return app.exec();
}