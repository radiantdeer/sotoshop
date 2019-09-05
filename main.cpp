#include <QApplication>
#include "MainWindow.hpp"
#include "file_handling/loader/ImageLoaderFactory.hpp"


int main (int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow rootWindow;
    rootWindow.setWindowTitle("SotoShop");
    rootWindow.setFixedSize(800,600); 

    //ImageLoader * imageLoader = ImageLoaderFactory::getImageLoader("test.ppm");

    rootWindow.show();

    return app.exec();
}