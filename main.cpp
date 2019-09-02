#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include "file_handling/ImageLoaderFactory.hpp"

int main (int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget rootWindow;
    rootWindow.setWindowTitle("Hello, World!");
    rootWindow.setFixedSize(800,600);

    QLabel hello("Hello, World!", &rootWindow);
    hello.setGeometry(10, 10, 100, 15);
    QPushButton loadButton("Load File", &rootWindow);
    loadButton.setToolTip("Load an image");
    loadButton.setGeometry(10, 30, 100, 20);

    ImageLoader * imageLoader = ImageLoaderFactory::getImageLoader("test.ppm");

    rootWindow.show();

    return app.exec();
}