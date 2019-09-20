#include <QApplication>
#include "frontend/MainWindow.hpp"

int main (int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow rootWindow;
    rootWindow.setFixedSize(800,600); 
    rootWindow.show();

    /* STUB IMAGE DATA */
    Image * image = new Image(500, 400);
    rootWindow.setActiveImage(image); 

    return app.exec();
}