#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QPainter>
#include <QPaintEvent>
#include <QRegion>
#include "DrawSurface.hpp"
#include "file_handling/loader/ImageLoaderFactory.hpp"

int main (int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMainWindow rootWindow;
    rootWindow.setWindowTitle("SotoShop");
    rootWindow.setFixedSize(800,600);

    QMenu * fileMenu = rootWindow.menuBar()->addMenu("File");
    QAction * loadAction = fileMenu->addAction("Load");
    QAction * saveAction = fileMenu->addAction("Save");

    DrawSurface drawspace(&rootWindow);
    rootWindow.setCentralWidget(&drawspace);
    QRegion tempRegion;
    drawspace.paintEvent(new QPaintEvent(tempRegion));

    //ImageLoader * imageLoader = ImageLoaderFactory::getImageLoader("test.ppm");

    rootWindow.show();

    return app.exec();
}