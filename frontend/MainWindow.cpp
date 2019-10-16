#include "MainWindow.hpp"

#include <iostream>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QPaintEvent>
#include <QRegion>
#include <QUrl>
#include <sstream>
#include "../spdlog/spdlog.h"
#include "../utilities/Convolution.hpp"
#include "../utilities/CommonConvolutions.hpp"

MainWindow::MainWindow() : QMainWindow() {
    this->setWindowTitle("SotoShop");

    QMenu * fileMenu = this->menuBar()->addMenu("File");
    loadAction = fileMenu->addAction("Load");
    saveAction = fileMenu->addAction("Save");

    QMenu * editMenu = this->menuBar()->addMenu("Edit");
    negativeImageAction = editMenu->addAction("Negative Image");
    convertToGrayscaleAction = editMenu->addAction("Convert Image to Grayscale");
    editMenu->addSeparator();
    brightenAction = editMenu->addAction("Brightening");
    editMenu->addSeparator();
    moveAction = editMenu->addAction("Move image");
    QMenu * rotateMenu = editMenu->addMenu("Rotate");
    rotateCWAction = rotateMenu->addAction("90\370 CW (clock-wise)");
    rotateCCWAction = rotateMenu->addAction("90\370 CCW (counterclock-wise)");
    QMenu * flipMenu = editMenu->addMenu("Flip");
    flipHAction = flipMenu->addAction("Horizontal");
    flipVAction = flipMenu->addAction("Vertical");
    zoomAction = editMenu->addAction("Zoom");
    editMenu->addSeparator();
    QMenu * arithmeticMenu = editMenu->addMenu("Arithmetic Operations");
    additionAction = arithmeticMenu->addAction("Addition");
    substractAction = arithmeticMenu->addAction("Substract");
    multiplyAction = arithmeticMenu->addAction("Multiply");
    QMenu * booleanMenu = editMenu->addMenu("Boolean Operations");
    andAction = booleanMenu->addAction("AND");
    orAction = booleanMenu->addAction("OR");
    notAction = booleanMenu->addAction("NOT");

    QMenu * histogramMenu = this->menuBar()->addMenu("Histogram");
    histogramAction = histogramMenu->addAction("Show");
    equalizeAction = histogramMenu->addAction("Equalize");
    specifyHistAction = histogramMenu->addAction("Specify");

    QMenu * convolutionMenu = this->menuBar()->addMenu("Convolution");
    meanFilter = convolutionMenu->addAction("Mean Filter");

    connectActionsToControllers();

    drawSurface = new DrawSurface(this);
    this->setCentralWidget(drawSurface);
    histDialog = nullptr;
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

void MainWindow::brightenImage() {
    if (drawSurface->isImageLoaded()) {
        spdlog::debug("Prompting the user to input deltaX");
        int delta = promptValue("Values needed", "Enter value : ");
        spdlog::debug("User entered delta = {}", delta);
        spdlog::info("MainWindow::convertToGrayscaleImage: Brightening image...");
        drawSurface->acquireLockImage();
        drawSurface->getActiveImage()->adjustBrightness(delta);
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

void MainWindow::addImage() {
    if (drawSurface->isImageLoaded()) {
        spdlog::debug("MainWindow::addImage: Asking for other image...");
        std::string url = getOpenFileUrl("Please load another image.");
        if (url != "") {
            spdlog::info("MainWindow::addImage: Adding newly loaded image to current image...");
            ImageLoader * imageLoader = ImageLoaderFactory::getImageLoader(url);
            Image * secondImage = imageLoader->load(url);
            delete imageLoader;
            drawSurface->acquireLockImage();
            drawSurface->getActiveImage()->add(*secondImage);
            drawSurface->releaseLockImage();
            drawSurface->update();
        } else {
            spdlog::info("MainWindow::addImage: Operation cancelled");
        }
     } else {
        spdlog::warn("MainWindow::addImage: Please load an image first!");
    }
}

void MainWindow::substractImage() {
    if (drawSurface->isImageLoaded()) {
        spdlog::debug("MainWindow::substractImage: Asking for other image...");
        std::string url = getOpenFileUrl("Please load another image");
        if (url != "") {
            spdlog::info("MainWindow::substractImage: Substracting newly loaded image to current image...");
            ImageLoader * imageLoader = ImageLoaderFactory::getImageLoader(url);
            Image * secondImage = imageLoader->load(url);
            delete imageLoader;
            drawSurface->acquireLockImage();
            drawSurface->getActiveImage()->substract(*secondImage);
            drawSurface->releaseLockImage();
            drawSurface->update();
        } else {
            spdlog::info("MainWindow::substractImage: Operation cancelled");
        }
    } else {
        spdlog::warn("MainWindow::substractImage: Please load an image first!");
    }
}

void MainWindow::multiplyImage() {
    if (drawSurface->isImageLoaded()) {
        spdlog::debug("MainWindow::substractImage: Asking for other image...");
        std::string url = getOpenFileUrl("Please load another image");
        if (url != "") {
            spdlog::info("MainWindow::multiplyImage: Multiplying current image with newly loaded image...");
            ImageLoader * imageLoader = ImageLoaderFactory::getImageLoader(url);
            Image * secondImage = imageLoader->load(url);
            delete imageLoader;
            drawSurface->acquireLockImage();
            drawSurface->getActiveImage()->multiply(*secondImage);
            drawSurface->releaseLockImage();
            drawSurface->update();
        } else {
            spdlog::info("MainWindow::multiplyImage: Operation cancelled");
        }
    } else {
        spdlog::warn("MainWindow::multiplyImage: Please load an image first!");
    }
}

void MainWindow::operateAndImage() {
    if (drawSurface->isImageLoaded()) {
        spdlog::debug("MainWindow::operateAndImage: Asking for other image...");
        std::string url = getOpenFileUrl("Please load another image");
        if (url != "") {
            spdlog::info("MainWindow::operateAndImage: Running AND operation...");
            ImageLoader * imageLoader = ImageLoaderFactory::getImageLoader(url);
            Image * secondImage = imageLoader->load(url);
            delete imageLoader;
            drawSurface->acquireLockImage();
            drawSurface->getActiveImage()->and_op(*secondImage);
            drawSurface->releaseLockImage();
            drawSurface->update();
        } else {
            spdlog::info("MainWindow::operateAndImage: Operation cancelled");
        }
    } else {
        spdlog::warn("MainWindow::operateAndImage: Please load an image first!");
    }
}

void MainWindow::operateOrImage() {
    if (drawSurface->isImageLoaded()) {
        spdlog::debug("MainWindow::operateOrImage: Asking for other image...");
        std::string url = getOpenFileUrl("Please load another image");
        if (url != "") {
            spdlog::info("MainWindow::operateOrImage: Running OR operation...");
            ImageLoader * imageLoader = ImageLoaderFactory::getImageLoader(url);
            Image * secondImage = imageLoader->load(url);
            delete imageLoader;
            drawSurface->acquireLockImage();
            drawSurface->getActiveImage()->or_op(*secondImage);
            drawSurface->releaseLockImage();
            drawSurface->update();
        } else {
            spdlog::info("MainWindow::operateOrImage: Operation cancelled");
        }
    } else {
        spdlog::warn("MainWindow::operateOrImage: Please load an image first!");
    }
}

void MainWindow::operateNotImage() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::operateNotImage: Running NOT operation...");
        drawSurface->acquireLockImage();
        drawSurface->getActiveImage()->not_op();
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::operateNotImage: Please load an image first!");
    }
}

void MainWindow::equalizeImageHist() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::equalizeImageHist: Equalizing image histogram...");
        drawSurface->acquireLockImage();
        drawSurface->getActiveImage()->histogramEqualization();
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::equalizeImageHist: Please load an image first!");
    }
}

void MainWindow::specifyHist() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::specifyHist: Specifying image histogram...");
        std::string url = getOpenFileUrl("Load Specification Image");
        if (url != "") {
            ImageLoader * imageLoader = ImageLoaderFactory::getImageLoader(url);
            Image * specImage = imageLoader->load(url);
            drawSurface->acquireLockImage();
            drawSurface->getActiveImage()->histogramSpecification(*specImage);
            drawSurface->releaseLockImage();
            drawSurface->update();
            delete specImage;
            delete imageLoader;
        } else {
            spdlog::info("Specification cancelled");
        }
    } else {
        spdlog::warn("MainWindow::specifyHist: Please load an image first!");
    }
}

void MainWindow::showHistogram() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::showHistogram: Showing histogram...");
        std::vector<std::vector<int>> hist = drawSurface->getActiveImage()->histogram();
        for (int i = 0; i < 256; i+=255) {
            spdlog::info(hist.at(0).at(i));
        }
        if (histDialog != nullptr) {
            delete histDialog;
        }
        histDialog = new HistogramDialog(hist);
        histDialog->show();
    } else {
        spdlog::warn("MainWindow::showHistogram: Please load an image first!");
    }
}

void MainWindow::doMeanFilterImage() {
    if (drawSurface->isImageLoaded()) {
        bool padded = askForPadding();
        spdlog::info("MainWindow::doMeanFilterImage: Convolving with mean filter...");
        Image * newImage = Convolution::convolve(drawSurface->getActiveImage(), CommonConvolutions::Average, padded);
        drawSurface->acquireLockImage();
        drawSurface->purgeImage();
        drawSurface->setActiveImage(newImage);
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::doMeanFilterImage: Please load an image first!");
    }
}

void MainWindow::connectActionsToControllers() {
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

    connect(negativeImageAction, &QAction::triggered, this, &MainWindow::makeNegativeImage);
    connect(convertToGrayscaleAction, &QAction::triggered, this, &MainWindow::convertToGrayscaleImage);

    connect(brightenAction, &QAction::triggered, this, &MainWindow::brightenImage);

    connect(moveAction, &QAction::triggered, this, &MainWindow::moveImage);
    connect(rotateCWAction, &QAction::triggered, this, &MainWindow::rotateImageCW);
    connect(rotateCCWAction, &QAction::triggered, this, &MainWindow::rotateImageCCW);
    connect(flipHAction, &QAction::triggered, this, &MainWindow::flipImageHorizontal);
    connect(flipVAction, &QAction::triggered, this, &MainWindow::flipImageVertical);
    connect(zoomAction, &QAction::triggered, this, &MainWindow::zoomImage);

    connect(additionAction, &QAction::triggered, this, &MainWindow::addImage);
    connect(substractAction, &QAction::triggered, this, &MainWindow::substractImage);
    connect(multiplyAction, &QAction::triggered, this, &MainWindow::multiplyImage);
    connect(andAction, &QAction::triggered, this, &MainWindow::operateAndImage);
    connect(orAction, &QAction::triggered, this, &MainWindow::operateOrImage);
    connect(notAction, &QAction::triggered, this, &MainWindow::operateNotImage);

    connect(equalizeAction, &QAction::triggered, this, &MainWindow::equalizeImageHist);
    connect(specifyHistAction, &QAction::triggered, this, &MainWindow::specifyHist);

    connect(meanFilter, &QAction::triggered, this, &MainWindow::doMeanFilterImage);
    connect(histogramAction, &QAction::triggered, this, &MainWindow::showHistogram);
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

bool MainWindow::askForPadding() {
    QMessageBox::StandardButton response = QMessageBox::question(this, "Convolution Padding", "Do you want to pad the image before convolution?");
    if (response == QMessageBox::Yes) {
        return true;
    } else {
        return false;
    }
}

void MainWindow::setActiveImage(Image * image) {
    drawSurface->setActiveImage(image);
    drawSurface->setImageLoaded(true);
}

void MainWindow::refresh() {
    QRegion tempRegion;
    drawSurface->paintEvent(new QPaintEvent(tempRegion));
}
