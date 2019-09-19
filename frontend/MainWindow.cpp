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

    QMenu * editMenu = this->menuBar()->addMenu("Edit");
    negativeImageAction = editMenu->addAction("Negative Image");
    convertToGrayscaleAction = editMenu->addAction("Convert Image to Grayscale");
    editMenu->addSeparator();
    moveAction = editMenu->addAction("Move image");
    rotateAction = editMenu->addAction("Rotate");
    flipAction = editMenu->addAction("Flip");
    zoomAction = editMenu->addAction("Zoom");

    connectActionsToControllers();

    drawSurface = new DrawSurface(this);
    this->setCentralWidget(drawSurface);
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
        Image * loadedImage = imageLoader->load(url);
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

void MainWindow::makeNegativeImage() {
    if (drawSurface->isImageLoaded()) {
        std::cout << "negative image" << std::endl;
    } else {
        std::cout << "Please load an image first!" << std::endl;
    }
}

void MainWindow::convertToGrayscaleImage() {
    if (drawSurface->isImageLoaded()) {
        std::cout << "make grayscale image" << std::endl;
    } else {
        std::cout << "Please load an image first!" << std::endl;
    }
}

void MainWindow::moveImage() {
    if (drawSurface->isImageLoaded()) {
        std::cout << "move image" << std::endl;
    } else {
        std::cout << "Please load an image first!" << std::endl;
    }
}

void MainWindow::rotateImage() {
    if (drawSurface->isImageLoaded()) {
        std::cout << "rotate image" << std::endl;
    } else {
        std::cout << "Please load an image first!" << std::endl;
    }
}

void MainWindow::flipImage() {
    if (drawSurface->isImageLoaded()) {
        std::cout << "flip image" << std::endl;
    } else {
        std::cout << "Please load an image first!" << std::endl;
    }
}

void MainWindow::zoomImage() {
    if (drawSurface->isImageLoaded()) {
        std::cout << "zoom image" << std::endl;
    } else {
        std::cout << "Please load an image first!" << std::endl;
    }
}


void MainWindow::connectActionsToControllers() {
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

    connect(negativeImageAction, &QAction::triggered, this, &MainWindow::makeNegativeImage);
    connect(convertToGrayscaleAction, &QAction::triggered, this, &MainWindow::convertToGrayscaleImage);
    connect(moveAction, &QAction::triggered, this, &MainWindow::moveImage);
    connect(rotateAction, &QAction::triggered, this, &MainWindow::rotateImage);
    connect(flipAction, &QAction::triggered, this, &MainWindow::flipImage);
    connect(zoomAction, &QAction::triggered, this, &MainWindow::zoomImage);
}

std::string MainWindow::getFileUrl(std::string dialogTitle) {
    QUrl tempFileUrl = QFileDialog::getOpenFileUrl(this, dialogTitle.c_str(), *(new QUrl()), "Image Files (*.raw , *.pbm , *.pgm , *.ppm , *.bmp);; All Files (*)");
    std::string fileUrl = tempFileUrl.toLocalFile().toUtf8().constData();
    return fileUrl;
}

void MainWindow::setActiveImage(Image * image) {
    drawSurface->setActiveImage(image);
    drawSurface->setImageLoaded(true);
}

void MainWindow::refresh() {
    QRegion tempRegion;
    drawSurface->paintEvent(new QPaintEvent(tempRegion));
}