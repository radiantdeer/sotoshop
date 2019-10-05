#include "MainWindow.hpp"

#include <iostream>
#include <QFileDialog>
#include <QInputDialog>
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
    QMenu * rotateMenu = editMenu->addMenu("Rotate");
    rotateCWAction = rotateMenu->addAction("90\370 CW (clock-wise)");
    rotateCCWAction = rotateMenu->addAction("90\370 CCW (counterclock-wise)");
    QMenu * flipMenu = editMenu->addMenu("Flip");
    flipHAction = flipMenu->addAction("Horizontal");
    flipVAction = flipMenu->addAction("Vertical");
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
        spdlog::info("MainWindow::loadFile: Loading from file {}", url);
        if (drawSurface->isImageLoaded()) {
            drawSurface->purgeImage();
        }
        ImageLoader * imageLoader = ImageLoaderFactory::getImageLoader(url);
        Image * loadedImage = imageLoader->load(url);
        drawSurface->acquireLockImage();
        drawSurface->setActiveImage(loadedImage);
        drawSurface->releaseLockImage();
        delete imageLoader;
        spdlog::info("MainWindow::loadFile: Image {} loaded successfully.", url);
        drawSurface->update();

        std::string windowTitle = "SotoShop [" + url + "]";
        this->setWindowTitle(windowTitle.c_str());

    } else {
        spdlog::info("MainWindow::loadFile: File loading cancelled");
    }
}

void MainWindow::saveFile() {
    using namespace std;
    if (drawSurface->isImageLoaded()) {
        string url = getSaveFileUrl("Save Image");
        if (url != "") {
            spdlog::info("MainWindow::saveFile: Saving to file {}", url);
            ImageSaver * imageSaver = ImageSaverFactory::getImageSaver(url);
            drawSurface->acquireLockImage();
            Image * imageToBeSaved = drawSurface->getActiveImage();
            imageSaver->save(*imageToBeSaved, url);
            drawSurface->releaseLockImage();
            delete imageSaver;
            spdlog::info("MainWindow::saveFile: File {} successfully saved!", url);
        } else {
            spdlog::info("MainWindow::saveFile: File saving cancelled.");
        }
    } else {
        spdlog::warn("MainWindow::save: SotoShop cannot save nothing! Load an image first!");
    }
}

void MainWindow::makeNegativeImage() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::makeNegativeImage: Creating negative image...");
        drawSurface->acquireLockImage();
        drawSurface->getActiveImage()->invert();
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::makeNegativeImage: Please load an image first!");
    }
}

void MainWindow::convertToGrayscaleImage() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::convertToGrayscaleImage: Creating grayscale image...");
        drawSurface->acquireLockImage();
        drawSurface->getActiveImage()->grayscale();
        drawSurface->releaseLockImage();
        drawSurface->update();

    } else {
        spdlog::warn("MainWindow::convertToGrayscaleImage: Please load an image first!");
    }
}

void MainWindow::moveImage() {
    if (drawSurface->isImageLoaded()) {
        spdlog::debug("Prompting the user to input deltaX");
        int deltaX = promptValue("Values needed", "Enter delta X (how further the image is moved horizontally) : ");
        spdlog::debug("User entered deltaX = {}", deltaX);

        spdlog::debug("Prompting the user to input deltaY");
        int deltaY = promptValue("Values needed", "Enter delta Y (how further the image is moved vertically) : ");
        spdlog::debug("User entered deltaY = {}", deltaY);

        spdlog::info("MainWindow::moveImage: Now moving the image...");
        drawSurface->acquireLockImage();
        drawSurface->getActiveImage()->translate(deltaX, deltaY);
        drawSurface->releaseLockImage();
        drawSurface->update();

    } else {
        spdlog::warn("MainWindow::moveImage: Please load an image first!");
    }
}

void MainWindow::rotateImageCW() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::rotateImageCW: Rotating image clockwise...");
        drawSurface->acquireLockImage();
        Image * oldImage = drawSurface->getActiveImage();
        Image * newImage = oldImage->rotate90CW();
        drawSurface->setActiveImage(newImage);
        drawSurface->releaseLockImage();
        drawSurface->update();
        delete oldImage;
    } else {
        spdlog::warn("MainWindow::rotateImageCW: Please load an image first!");
    }
}

void MainWindow::rotateImageCCW() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::rotateImageCCW: Rotating image counter-clockwise...");
        drawSurface->acquireLockImage();
        Image * oldImage = drawSurface->getActiveImage();
        Image * newImage = oldImage->rotate90CCW();
        drawSurface->setActiveImage(newImage);
        drawSurface->releaseLockImage();
        drawSurface->update();
        delete oldImage;
    } else {
        spdlog::warn("MainWindow::rotateImageCCW: Please load an image first!");
    }
}

void MainWindow::flipImageHorizontal() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::flipImageHorizontal: Flipping image horizontally...");
        drawSurface->acquireLockImage();
        drawSurface->getActiveImage()->flipH();
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::flipImageHorizontal: Please load an image first!");
    }
}

void MainWindow::flipImageVertical() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::flipImageVertical: Flipping image vertically...");
        drawSurface->acquireLockImage();
        drawSurface->getActiveImage()->flipV();
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::flipImageVertical: Please load an image first!");
    }
}

void MainWindow::zoomImage() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::zoomImage: Zooming image...");
        spdlog::info("MainWindow::zoomImage: stub function");
    } else {
        spdlog::warn("MainWindow::zoomImage: Please load an image first!");
    }
}

void MainWindow::connectActionsToControllers() {
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

    connect(negativeImageAction, &QAction::triggered, this, &MainWindow::makeNegativeImage);
    connect(convertToGrayscaleAction, &QAction::triggered, this, &MainWindow::convertToGrayscaleImage);

    connect(moveAction, &QAction::triggered, this, &MainWindow::moveImage);
    connect(rotateCWAction, &QAction::triggered, this, &MainWindow::rotateImageCW);
    connect(rotateCCWAction, &QAction::triggered, this, &MainWindow::rotateImageCCW);
    connect(flipHAction, &QAction::triggered, this, &MainWindow::flipImageHorizontal);
    connect(flipVAction, &QAction::triggered, this, &MainWindow::flipImageVertical);
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

int MainWindow::promptValue(std::string promptTitle, std::string promptText) {
    int value = QInputDialog::getInt(this, promptTitle.c_str(), promptText.c_str(), 0);
    return value;
}

void MainWindow::setActiveImage(Image * image) {
    drawSurface->setActiveImage(image);
    drawSurface->setImageLoaded(true);
}

void MainWindow::refresh() {
    QRegion tempRegion;
    drawSurface->paintEvent(new QPaintEvent(tempRegion));
}