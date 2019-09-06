#include "MainWindow.hpp"

#include <iostream>
#include <QFileDialog>
#include <QMenuBar>
#include <QPaintEvent>
#include <QRegion>
#include <QUrl>

MainWindow::MainWindow() : QMainWindow() {
    QMenu * fileMenu = this->menuBar()->addMenu("File");
    loadAction = fileMenu->addAction("Load");
    saveAction = fileMenu->addAction("Save");  
    drawSurface = new DrawSurface(this);
    this->setCentralWidget(drawSurface);
    QRegion tempRegion;
    drawSurface->paintEvent(new QPaintEvent(tempRegion));

    connect(loadAction, &QAction::triggered, this, &MainWindow::loadFile);
}

QAction * MainWindow::getLoadAction() {
    return loadAction;
}

QAction * MainWindow::getSaveAction() {
    return saveAction;
}

DrawSurface * MainWindow::getDrawSurface() {
    return drawSurface;
}


void MainWindow::loadFile() {
    std::string url = getFileUrl("Load Image");
    std::cout << url << std::endl;
    if (url != "") {
        ImageLoader * imageLoader = ImageLoaderFactory::getImageLoader(url);
        //drawSurface->setActiveImage(imageLoader->load(url));
        delete imageLoader;
    }
}

std::string MainWindow::getFileUrl(std::string dialogTitle) {
    QUrl tempFileUrl = QFileDialog::getOpenFileUrl(this, dialogTitle.c_str(), *(new QUrl()), "Image Files (*.raw, *.pbm, *.pgm, *.ppm, *.bmp);; All Files (*)");
    std::string fileUrl = tempFileUrl.toLocalFile().toUtf8().constData();
    return fileUrl;
}
