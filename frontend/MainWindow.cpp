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

    connectActionsToControllers();

    drawSurface = new DrawSurface(this);
    this->setCentralWidget(drawSurface);

    /* STUB IMAGE DATA */
    Image * image = new Image(100, 100);

    this->setActiveImage(image);
    this->getDrawSurface()->setImageLoaded(true);

    /*QRegion tempRegion;
    drawSurface->paintEvent(new QPaintEvent(tempRegion));*/
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
    if (url != "") {
        std::cout << "Loading from file " << url << std::endl;
        ImageLoader * imageLoader = ImageLoaderFactory::getImageLoader(url);
        Image loadedImage = imageLoader->load(url);
        drawSurface->setActiveImage(loadedImage);
        drawSurface->setImageLoaded(true);
        delete imageLoader;
    }
}

void MainWindow::saveFile() {
    if (drawSurface->isImageLoaded()) {
        std::string url = getFileUrl("Save Image");
        if (url != "") {
            std::cout << "Saving to file " << url << std::endl;
            Image * imageToBeSaved = drawSurface->getActiveImage();
            ImageSaver * imageSaver = ImageSaverFactory::getImageSaver(imageToBeSaved->getOriginalFormat());
            //imageSaver->save(imageToBeSaved, url);
            delete imageSaver;
        }
    } else {
        std::cout << "SotoShop cannot save nothing! Load an image first!" << std::endl;
    }
}

void MainWindow::connectActionsToControllers() {
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
}

std::string MainWindow::getFileUrl(std::string dialogTitle) {
    QUrl tempFileUrl = QFileDialog::getOpenFileUrl(this, dialogTitle.c_str(), *(new QUrl()), "Image Files (*.raw, *.pbm, *.pgm, *.ppm, *.bmp);; All Files (*)");
    std::string fileUrl = tempFileUrl.toLocalFile().toUtf8().constData();
    return fileUrl;
}

void MainWindow::setActiveImage(Image * image) {
    drawSurface->setActiveImage(image);
}