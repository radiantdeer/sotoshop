#include <QApplication>
#include <QLabel>
#include <QWidget>
#include "file_handling/ImageLoaderFactory.hpp"

int main (int argc, char *argv[]) {
    QApplication app(argc, argv);
    QLabel hello("<center>Hello, World!</center>");
    ImageLoaderFactory loaderFactory;
    ImageLoader * imageLoader = loaderFactory.getInstance("test.raw");
    
    hello.setWindowTitle("Hello, World! Title App!");
    hello.resize(400, 400);
    hello.show();

    return app.exec();
}