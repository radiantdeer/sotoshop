#include "MainWindow.hpp"

#include <iostream>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QPaintEvent>
#include <QRegion>
#include <QSignalMapper>
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
    moveAction = editMenu->addAction("Move image");
    rotateAction = editMenu->addAction("Rotate");
    flipAction = editMenu->addAction("Flip");
    zoomAction = editMenu->addAction("Zoom");

    histogramAction = this->menuBar()->addAction("Histogram");

    QMenu * convolutionMenu = this->menuBar()->addMenu("Convolution");
    meanFilterAction = convolutionMenu->addAction("Mean Filter");
    medianFilterAction = convolutionMenu->addAction("Median Filter");
    QMenu * highPass = convolutionMenu->addMenu("High-pass Filter");
    highPassFilter1Action = highPass->addAction("Variation 1");
    highPassFilter2Action = highPass->addAction("Variation 2");
    highPassFilter3Action = highPass->addAction("Variation 3");
    highPassFilter4Action = highPass->addAction("Variation 4");

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
        drawSurface->setActiveImage(loadedImage);
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
        spdlog::info("MainWindow::moveImage: Moving image...");
        spdlog::info("MainWindow::moveImage: stub function");
    } else {
        spdlog::warn("MainWindow::moveImage: Please load an image first!");
    }
}

void MainWindow::rotateImage() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::rotateImage: Rotating image...");
        spdlog::info("MainWindow::rotateImage: stub function");
    } else {
        spdlog::warn("MainWindow::rotateImage: Please load an image first!");
    }
}

void MainWindow::flipImage() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::flipImage: Flipping image...");
        spdlog::info("MainWindow::flipImage: stub function");
    } else {
        spdlog::warn("MainWindow::rotateImage: Please load an image first!");
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

void MainWindow::doMeanFilter() {
    if (drawSurface->isImageLoaded()) {
        bool padded = askForPadding();
        spdlog::info("MainWindow::doMeanFilter: Convolving with mean filter...");
        Image * newImage = Convolution::convolve(drawSurface->getActiveImage(), CommonConvolutions::Average, padded);
        drawSurface->acquireLockImage();
        drawSurface->purgeImage();
        drawSurface->setActiveImage(newImage);
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::doMeanFilter: Please load an image first!");
    }
}

void MainWindow::doMedianFilter() {
    if (drawSurface->isImageLoaded()) {
        bool padded = askForPadding();
        spdlog::info("MainWindow::doMedianFilter: Convolving with median filter...");
        Image * newImage = Convolution::medianConvolve(drawSurface->getActiveImage(), 3, 3, padded);
        drawSurface->acquireLockImage();
        drawSurface->purgeImage();
        drawSurface->setActiveImage(newImage);
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::doMedianFilter: Please load an image first!");
    }
}

void MainWindow::doHighPassFilter(int filterVariation) {
    if (drawSurface->isImageLoaded()) {
        bool padded = askForPadding();
        spdlog::info("MainWindow::doHighPassFilter: Convolving with high-pass filter #{}...", filterVariation);

        const ConvolutionMatrix * opMatrix;
        switch(filterVariation) {
            case 1:
                opMatrix = &CommonConvolutions::HighFilter1;
                break;
            case 2:
                opMatrix = &CommonConvolutions::HighFilter2;
                break;
            case 3:
                opMatrix = &CommonConvolutions::HighFilter3;
                break;
            case 4:
                opMatrix = &CommonConvolutions::HighFilter4;
                break;
            default:
                spdlog::warn("MainWindow::doHighPassFilter: Number variation is not recognized, using variation 2 instead.");
                opMatrix = &CommonConvolutions::HighFilter1;
        }
        Image * newImage = Convolution::convolve(drawSurface->getActiveImage(), *opMatrix, padded);
        drawSurface->acquireLockImage();
        drawSurface->purgeImage();
        drawSurface->setActiveImage(newImage);
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::doHighPassFilter: Please load an image first!");
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

void MainWindow::connectActionsToControllers() {
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

    connect(negativeImageAction, &QAction::triggered, this, &MainWindow::makeNegativeImage);
    connect(convertToGrayscaleAction, &QAction::triggered, this, &MainWindow::convertToGrayscaleImage);
    connect(moveAction, &QAction::triggered, this, &MainWindow::moveImage);
    connect(rotateAction, &QAction::triggered, this, &MainWindow::rotateImage);
    connect(flipAction, &QAction::triggered, this, &MainWindow::flipImage);
    connect(zoomAction, &QAction::triggered, this, &MainWindow::zoomImage);

    connect(meanFilterAction, &QAction::triggered, this, &MainWindow::doMeanFilter);
    connect(medianFilterAction, &QAction::triggered, this, &MainWindow::doMedianFilter);
    connect(highPassFilter1Action, &QAction::triggered, this, [this]{doHighPassFilter(1); });
    connect(highPassFilter2Action, &QAction::triggered, this, [this]{doHighPassFilter(2); });
    connect(highPassFilter3Action, &QAction::triggered, this, [this]{doHighPassFilter(3); });
    connect(highPassFilter4Action, &QAction::triggered, this, [this]{doHighPassFilter(4); });

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
