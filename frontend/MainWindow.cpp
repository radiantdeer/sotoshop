#include "MainWindow.hpp"

#include <iostream>
#include <QFileDialog>
#include <QMenuBar>
#include <QPaintEvent>
#include <QRegion>
#include <QUrl>
#include "../spdlog/spdlog.h"

MainWindow::MainWindow() : QMainWindow() {
    this->setWindowTitle("SotoShop");

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
    std::string url = getOpenFileUrl("Load Image");
    if (url != "") {
        spdlog::info("Loading from file {}", url);
        if (drawSurface->isImageLoaded()) {
            drawSurface->purgeImage();
        }
        ImageLoader * imageLoader = ImageLoaderFactory::getImageLoader(url);
        Image * loadedImage = imageLoader->load(url);
        drawSurface->setActiveImage(loadedImage);
        delete imageLoader;
        spdlog::info("Image {} is loaded.", url);
        drawSurface->update();
      
        std::string windowTitle = "SotoShop [" + url + "]";
        this->setWindowTitle(windowTitle.c_str());

    } else {
        spdlog::info("File loading cancelled");        
    }
}

void MainWindow::saveFile() {
    using namespace std;
    if (drawSurface->isImageLoaded()) {
        string url = getSaveFileUrl("Save Image");
        if (url != "") {
            spdlog::info("Saving to file {}", url);
            ImageSaver * imageSaver = ImageSaverFactory::getImageSaver(url);
            drawSurface->acquireLockImage();
            Image * imageToBeSaved = drawSurface->getActiveImage();
            imageSaver->save(*imageToBeSaved, url);
            drawSurface->releaseLockImage();
            delete imageSaver;
            spdlog::info("File {} successfully saved!", url);
        } else {
            spdlog::info("File saving cancelled.");
        }
    } else {
        spdlog::warn("SotoShop cannot save nothing! Load an image first!");
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

std::string MainWindow::getOpenFileUrl(std::string dialogTitle) {
    using namespace std;
    spdlog::debug("Showing up the open file dialog...");
    QUrl tempFileUrl = QFileDialog::getOpenFileUrl(this, dialogTitle.c_str(), *(new QUrl()), "Image Files (*.raw , *.pbm , *.pgm , *.ppm , *.bmp);; All Files (*)");
    string fileUrl = tempFileUrl.toLocalFile().toUtf8().constData();
    return fileUrl;
}

std::string MainWindow::getSaveFileUrl(std::string dialogTitle) {
    using namespace std;
    spdlog::debug("Showing up the save file dialog...");
    QUrl tempFileUrl = QFileDialog::getSaveFileUrl(this, dialogTitle.c_str(), *(new QUrl()), "Raw Image File (*.raw);; PBM Image File (*.pbm);; PGM Image File (*.pgm);; PPM Image File (*.ppm);; Bitmap File (*.bmp)");
    string fileUrl = tempFileUrl.toLocalFile().toUtf8().constData();
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