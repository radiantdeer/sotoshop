#include "MainWindow.hpp"

#include <iostream>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QPaintEvent>
#include <QRegion>
#include <QSignalMapper>
#include <QUrl>
#include <sstream>
#include "../spdlog/spdlog.h"
#include "../utilities/BitPlaneSlicing.hpp"
#include "../utilities/Convolution.hpp"
#include "../utilities/CommonConvolutions.hpp"
#include "../utilities/Fourier.hpp"

MainWindow::MainWindow() : QMainWindow() {
    this->setWindowTitle("SotoShop");

    QMenu * fileMenu = this->menuBar()->addMenu("File");
    loadAction = fileMenu->addAction("Load");
    saveAction = fileMenu->addAction("Save");

    QMenu * editMenu = this->menuBar()->addMenu("Edit");
    negativeImageAction = editMenu->addAction("Negative Image");
    convertToGrayscaleAction = editMenu->addAction("Convert Image to Grayscale");
    editMenu->addSeparator();
    brightenAction = editMenu->addAction("Brighten Image");
    editMenu->addSeparator();
    moveAction = editMenu->addAction("Move image");
    QMenu * rotateMenu = editMenu->addMenu("Rotate");
    rotateCWAction = rotateMenu->addAction("90\370 CW (clock-wise)");
    rotateCCWAction = rotateMenu->addAction("90\370 CCW (counterclock-wise)");
    QMenu * flipMenu = editMenu->addMenu("Flip");
    flipHAction = flipMenu->addAction("Horizontal");
    flipVAction = flipMenu->addAction("Vertical");
    zoomInAction = editMenu->addAction("Zoom In");
    zoomOutAction = editMenu->addAction("Zoom Out");
    grayLevelSlicingAction = editMenu->addAction("Graylevel Slicing");
    editMenu->addSeparator();
    QMenu * arithmeticMenu = editMenu->addMenu("Arithmetic Operations");
    additionAction = arithmeticMenu->addAction("Addition");
    substractAction = arithmeticMenu->addAction("Substract");
    multiplyAction = arithmeticMenu->addAction("Multiply");
    QMenu * booleanMenu = editMenu->addMenu("Boolean Operations");
    andAction = booleanMenu->addAction("AND");
    orAction = booleanMenu->addAction("OR");
    notAction = booleanMenu->addAction("NOT");
    editMenu->addSeparator();
    nthPowerAction = editMenu->addAction("N-Power");
    logAction = editMenu->addAction("Log");
    invLogAction = editMenu->addAction("Inverse Log");

    editMenu->addSeparator();
    QMenu * contrastStretch = editMenu->addMenu("Stretch Contrast");
    contrastStretchingAutoAction = contrastStretch->addAction("Auto");
    contrastStretchingManualAction = contrastStretch->addAction("Manual");

    QMenu * histogramMenu = this->menuBar()->addMenu("Histogram");
    histogramAction = histogramMenu->addAction("Show");
    equalizeAction = histogramMenu->addAction("Equalize");
    specifyHistAction = histogramMenu->addAction("Specify");

    QMenu * convolutionMenu = this->menuBar()->addMenu("Convolution");
    meanFilterAction = convolutionMenu->addAction("Mean Filter");
    medianFilterAction = convolutionMenu->addAction("Median Filter");
    QMenu * highPass = convolutionMenu->addMenu("High-pass Filter");
    highPassFilter1Action = highPass->addAction("Variation 1");
    highPassFilter2Action = highPass->addAction("Variation 2");
    highPassFilter3Action = highPass->addAction("Variation 3 (Laplace)");
    highPassFilter4Action = highPass->addAction("Variation 4");
    unsharpMaskingAction = convolutionMenu->addAction("Unsharp Masking");
    highboostAction = convolutionMenu->addAction("Highboost");

    QMenu * other = this->menuBar()->addMenu("Other");
    bitPlaneAction = other->addAction("Bit Planes");
    fourierAction = other->addAction("Fourier Transform");
    viewFourierSpectrumAction = other->addAction("View Fourier Spectrum");
    inverseFourierAction = other->addAction("Inverse Fourier Transform");

    connectActionsToControllers();

    drawSurface = new DrawSurface(this);
    this->setCentralWidget(drawSurface);
    histDialog = nullptr;
    bitPlaneDialog = nullptr;
    fourierFrequencies = nullptr;
    fourierVisual = nullptr;
    fourierDialog = nullptr;
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
        Image * newImage = drawSurface->getActiveImage()->translate(deltaX, deltaY);
        drawSurface->purgeImage();
        drawSurface->setActiveImage(newImage);
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

void MainWindow::doMeanFilter() {
    if (drawSurface->isImageLoaded()) {
        bool padded = askYesNoQuestion("Convolution Padding", "Do you want to pad the image first?");
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
        bool padded = askYesNoQuestion("Convolution Padding", "Do you want to pad the image first?");
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
        bool padded = askYesNoQuestion("Convolution Padding", "Do you want to pad the image first?");
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

void MainWindow::doUnsharpMasking() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::doUnsharpMasking: Filtering with unsharp masking...");
        Image * result = Convolution::unsharpMasking(drawSurface->getActiveImage());
        drawSurface->acquireLockImage();
        drawSurface->purgeImage();
        drawSurface->setActiveImage(result);
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::doUnsharpMasking: Please load an image first!");
    }
}

void MainWindow::doHighboost() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::doUnsharpMasking: Filtering with highboost...");
        double alpha = QInputDialog::getDouble(this, "Value needed", "Enter alpha");
        Image * result = Convolution::highboost(drawSurface->getActiveImage(), alpha);
        drawSurface->acquireLockImage();
        drawSurface->purgeImage();
        drawSurface->setActiveImage(result);
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::doUnsharpMasking: Please load an image first!");
    }
}

void MainWindow::zoomIn() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::zoomIn: Zooming in image...");
        drawSurface->acquireLockImage();
        drawSurface->setActiveImage(drawSurface->getActiveImage()->magnify2());
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::zoomIn: Please load an image first!");
    }
}

void MainWindow::zoomOut() {
    if (drawSurface->isImageLoaded()) {
        spdlog::info("MainWindow::zoomOut: Zooming out image...");
        drawSurface->acquireLockImage();
        drawSurface->setActiveImage(drawSurface->getActiveImage()->shrink2());
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::zoomOut: Please load an image first!");
    }
}

void MainWindow::nthPower() {
    if (drawSurface->isImageLoaded()) {
        spdlog::debug("Prompting the user to input c");
        double c = QInputDialog::getDouble(this, "Constant", "Enter value: ");
        spdlog::debug("User entered delta = {}", c);
        spdlog::debug("Prompting the user to input n");
        double n = QInputDialog::getDouble(this, "N Power", "Enter value: ");
        spdlog::debug("User entered delta = {}", n);
        spdlog::info("MainWindow::nthPower: N Power");
        drawSurface->acquireLockImage();
        drawSurface->setActiveImage(drawSurface->getActiveImage()->nthPower(n, c));
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::nthPower: Please load an image first!");
    }
}

void MainWindow::grayLevelSlicing() {
    if (drawSurface->isImageLoaded()) {
        spdlog::debug("Prompting the user to input a");
        int a = promptValue("Slicing Start Point", "Insert a value between 0 and 255");
        spdlog::debug("User entered a = {}", a);
        spdlog::debug("Prompting the user to input b");
        int b = promptValue("Slicing End Point", "Insert a value between 0 and 255");
        spdlog::debug("User entered b = {}", b);
        spdlog::debug("Prompting the user to input val");
        int val = promptValue("Slicing Value", "Insert a value between 0 and 255");
        spdlog::debug("User entered val = {}", val);
        spdlog::info("MainWindow::grayLevelSlicing: Slicing Gray Level...");
        drawSurface->acquireLockImage();
        drawSurface->setActiveImage(drawSurface->getActiveImage()->grayLevelSlicing(a,b,val));
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::grayLevelSlicing: Please load an image first!");
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

void MainWindow::showBitPlanes() {
    if (drawSurface->isImageLoaded()) {
        Image * currentImage = drawSurface->getActiveImage();
        if ((currentImage->getOriginalFormat() == "ppm") || (currentImage->getOriginalFormat() == "bmp")) {
            spdlog::warn("MainWindow::showBitPlanes: Bit Plane slicing currently only available to grayscale images");
        } else {
            spdlog::debug("MainWindow::showBitPlanes: Generating bit planes...");
            std::vector<Image> bitPlanes = BitPlaneSlicing::generate(currentImage);
            spdlog::debug("MainWindow::showBitPlanes: Planes generated...");
            if (bitPlaneDialog != nullptr) {
                delete bitPlaneDialog;
            }
            spdlog::info("MainWindow::showBitPlanes: Showing bit planes...");
            bitPlaneDialog = new BitPlaneDialog(bitPlanes);
            bitPlaneDialog->show();
        }
    } else {
        spdlog::warn("MainWindow::showBitPlanes: Please load an image first!");
    }
}

void MainWindow::doFourierTransform() {
    if (drawSurface->isImageLoaded()) {
        if (fourierFrequencies != nullptr) {
            delete fourierFrequencies;
        }
        fourierFrequencies = Fourier::forward(drawSurface->getActiveImage());
    } else {
        spdlog::warn("MainWindow::doFourierTransform: Please load an image first!");
    }
}

void MainWindow::viewFourierSpectrum() {
    if (fourierFrequencies != nullptr) {
        spdlog::info("MainWindow::viewFourierSpectrum: Visualizing Fourier frequencies...");
        if (fourierVisual != nullptr) {
            delete fourierVisual;
        }
        if (fourierDialog != nullptr) {
            delete fourierDialog;
        }
        spdlog::info("MainWindow::viewFourierSpectrum: Shift frequencies first...");
        Fourier::shift(fourierFrequencies);
        Image * fourierVisual = Fourier::visualizeFrequencies(fourierFrequencies);
        fourierDialog = new DrawSurface(nullptr, fourierVisual);
        fourierDialog->show();
        spdlog::info("MainWindow::viewFourierSpectrum: De-shift frequencies...");
        Fourier::shift(fourierFrequencies);
    } else {
        spdlog::warn("MainWindow::viewFourierSpectrum: Please do a Fourier Transform first!");
    }
}

void MainWindow::doInverseFourier() {
    if (fourierFrequencies != nullptr) {
        if (fourierVisual != nullptr) {
            delete fourierVisual;
        }
        if (fourierDialog != nullptr) {
            delete fourierDialog;
        }
        Image * fourierVisual = Fourier::inverse(fourierFrequencies);
        fourierDialog = new DrawSurface(nullptr, fourierVisual);
        fourierDialog->show();
    } else {
        spdlog::warn("MainWindow::doInverseFourier: Please do a Fourier Transform first!");
    }
}


void MainWindow::contrastStretching(bool automatic) {
    if (drawSurface->isImageLoaded()) {
        Image * currentImage = drawSurface->getActiveImage();
        if (automatic) {
            spdlog::info("MainWindow::contrastStretching: Automatically determine lower & upper bounds...");
            drawSurface->acquireLockImage();
            currentImage->contrastStretch();
            drawSurface->releaseLockImage();
        } else {
            spdlog::info("MainWindow::contrastStretching: Prompting lower & upper bound to user...");
            int rrmin = promptValue("Values needed.", "Enter lower bound of red color. (0 - 255)");
            int rrmax = promptValue("Values needed", "Enter upper bound of red color. (0 - 255)");
            int rgmin = promptValue("Values needed", "Minimum green color\n(just enter the same value as red if grayscale)");
            int rgmax = promptValue("Values needed", "Maximum green color\n(just enter the same value as red if grayscale)");
            int rbmin = promptValue("Values needed", "Minimum blue color\n(just enter the same value as red if grayscale)");
            int rbmax = promptValue("Values needed", "Maximum blue color\n(just enter the same value as red if grayscale)");
            drawSurface->acquireLockImage();
            currentImage->contrastStretch(rrmin, rrmax, rgmin, rgmax, rbmin, rbmax);
            drawSurface->releaseLockImage();
        }
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::contrastStretching: Please load an image first!");
    }
}

void MainWindow::logOperation() {
    if (drawSurface->isImageLoaded()) {
        drawSurface->acquireLockImage();
        drawSurface->getActiveImage()->logTrans();
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::logOperation: Please load an image first!");
    }
}

void MainWindow::invLogOperation() {
    if (drawSurface->isImageLoaded()) {
        drawSurface->acquireLockImage();
        drawSurface->getActiveImage()->invLogTrans();
        drawSurface->releaseLockImage();
        drawSurface->update();
    } else {
        spdlog::warn("MainWindow::invLogOperation: Please load an image first!");
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
    connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);
    connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);
    connect(grayLevelSlicingAction, &QAction::triggered, this, &MainWindow::grayLevelSlicing);
    connect(nthPowerAction, &QAction::triggered, this, &MainWindow::nthPower);
    connect(logAction, &QAction::triggered, this, &MainWindow::logOperation);
    connect(invLogAction, &QAction::triggered, this, &MainWindow::invLogOperation);

    connect(additionAction, &QAction::triggered, this, &MainWindow::addImage);
    connect(substractAction, &QAction::triggered, this, &MainWindow::substractImage);
    connect(multiplyAction, &QAction::triggered, this, &MainWindow::multiplyImage);
    connect(andAction, &QAction::triggered, this, &MainWindow::operateAndImage);
    connect(orAction, &QAction::triggered, this, &MainWindow::operateOrImage);
    connect(notAction, &QAction::triggered, this, &MainWindow::operateNotImage);

    connect(contrastStretchingAutoAction, &QAction::triggered, this, [this]{contrastStretching(true); });
    connect(contrastStretchingManualAction, &QAction::triggered, this, [this]{contrastStretching(false); });

    connect(histogramAction, &QAction::triggered, this, &MainWindow::showHistogram);
    connect(equalizeAction, &QAction::triggered, this, &MainWindow::equalizeImageHist);
    connect(specifyHistAction, &QAction::triggered, this, &MainWindow::specifyHist);

    connect(meanFilterAction, &QAction::triggered, this, &MainWindow::doMeanFilter);
    connect(medianFilterAction, &QAction::triggered, this, &MainWindow::doMedianFilter);
    connect(highPassFilter1Action, &QAction::triggered, this, [this]{doHighPassFilter(1); });
    connect(highPassFilter2Action, &QAction::triggered, this, [this]{doHighPassFilter(2); });
    connect(highPassFilter3Action, &QAction::triggered, this, [this]{doHighPassFilter(3); });
    connect(highPassFilter4Action, &QAction::triggered, this, [this]{doHighPassFilter(4); });
    connect(unsharpMaskingAction, &QAction::triggered, this, &MainWindow::doUnsharpMasking);
    connect(highboostAction, &QAction::triggered, this, &MainWindow::doHighboost);

    connect(bitPlaneAction, &QAction::triggered, this, &MainWindow::showBitPlanes);
    connect(fourierAction, &QAction::triggered, this, &MainWindow::doFourierTransform);
    connect(viewFourierSpectrumAction, &QAction::triggered, this, &MainWindow::viewFourierSpectrum);
    connect(inverseFourierAction, &QAction::triggered, this, &MainWindow::doInverseFourier);

}

std::string MainWindow::getOpenFileUrl(std::string dialogTitle) {
    using namespace std;
    spdlog::debug("Showing up the open file dialog...");
    QUrl tempFileUrl = QFileDialog::getOpenFileUrl(this, dialogTitle.c_str(), *(new QUrl()), "Image Files (*.raw , *.pbm , *.pgm , *.ppm , *.bmp);; All Files (*)", 0, QFileDialog::DontUseNativeDialog);
    string fileUrl = tempFileUrl.toLocalFile().toUtf8().constData();
    return fileUrl;
}

std::string MainWindow::getSaveFileUrl(std::string dialogTitle) {
    using namespace std;
    spdlog::debug("Showing up the save file dialog...");
    QUrl tempFileUrl = QFileDialog::getSaveFileUrl(this, dialogTitle.c_str(), *(new QUrl()), "Raw Image File (*.raw);; PBM Image File (*.pbm);; PGM Image File (*.pgm);; PPM Image File (*.ppm);; Bitmap File (*.bmp)", 0, QFileDialog::DontUseNativeDialog);
    string fileUrl = tempFileUrl.toLocalFile().toUtf8().constData();
    return fileUrl;
}

int MainWindow::promptValue(std::string promptTitle, std::string promptText) {
    int value = QInputDialog::getInt(this, promptTitle.c_str(), promptText.c_str(), 0);
    return value;
}

bool MainWindow::askYesNoQuestion(std::string promptTitle, std::string promptText) {
    QMessageBox::StandardButton response = QMessageBox::question(this, promptTitle.c_str(), promptText.c_str());
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
