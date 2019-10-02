#include "DrawSurface.hpp"

#include <iostream>
#include <QColor>
#include <QPainter>

#include "../datastruct/Pixel.hpp"

DrawSurface::DrawSurface() : QWidget() {
    activeImage = nullptr;
    imageLoaded = false;
    activeImageLock = new std::mutex();
}

DrawSurface::DrawSurface(QWidget * parentWidget) : QWidget(parentWidget) {
    activeImage = nullptr;
    imageLoaded = false;
    activeImageLock = new std::mutex();
}

DrawSurface::DrawSurface(QWidget * parentWidget, Image * activeImage) : QWidget(parentWidget) {
    this->activeImage = activeImage;
    imageLoaded = true;
    activeImageLock = new std::mutex();
}

Image * DrawSurface::getActiveImage() {
    return activeImage;
}

bool DrawSurface::isImageLoaded() {
    return imageLoaded;
}

void DrawSurface::setActiveImage(Image * newImage) {
    acquireLockImage();
    activeImage = newImage;
    if (newImage != nullptr) {
        this->imageLoaded = true;
    } else {
        this->imageLoaded = false;
    }
    releaseLockImage();
}

void DrawSurface::setImageLoaded(bool imageLoaded) {
    this->imageLoaded = imageLoaded;
}

void DrawSurface::acquireLockImage() {
    this->activeImageLock->lock();
}

void DrawSurface::releaseLockImage() {
    this->activeImageLock->unlock();
}

void DrawSurface::purgeImage() {
    acquireLockImage();
    if (activeImage != nullptr) {
        delete activeImage;
        imageLoaded = false;
    }
    releaseLockImage();
}

// Implementing protected virtual method from QWidget
// Currently set to draw a small red line
// This function will be used to draw/re-draw the image
void DrawSurface::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    if (this->isImageLoaded()) {
        acquireLockImage();
        for (int i = 0; i < activeImage->getHeight(); i++) {
            for (int j = 0; j < activeImage->getWidth(); j++) {
                Pixel thisPixel = activeImage->getPixelAt(j, i);
                QColor * thisPixelColor = new QColor(thisPixel.getRed(), thisPixel.getGreen(), thisPixel.getBlue());
                painter.setPen(*thisPixelColor);
                painter.drawPoint(j, i);
            }
        }
        releaseLockImage();
    } else {
        std::cout << "[WARN] No active image in DrawSurface!" << std::endl;
    }
}