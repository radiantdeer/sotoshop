#include "DrawSurface.hpp"

#include <iostream>
#include <QColor>
#include <QPainter>

#include "../datastruct/Pixel.hpp"
#include "../spdlog/spdlog.h"

DrawSurface::DrawSurface() : QWidget() {
    imageLoaded = false;
    activeImageLock = new std::mutex();
}

DrawSurface::DrawSurface(QWidget * parentWidget) : QWidget(parentWidget) {
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
    this->acquireLockImage();
    activeImage = newImage;
    if (newImage != nullptr) {
        this->imageLoaded = true;
    } else {
        this->imageLoaded = false;
    }
    this->releaseLockImage();
}

void DrawSurface::setImageLoaded(bool imageLoaded) {
    this->imageLoaded = imageLoaded;
}

void DrawSurface::acquireLockImage() {
    spdlog::debug("A thread is trying to lock the active image");
    this->activeImageLock->lock();
    spdlog::debug("A thread is now granted the active image lock");
}

void DrawSurface::releaseLockImage() {
    spdlog::debug("A thread is releasing the active image lock");
    this->activeImageLock->unlock();
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
        spdlog::warn("No active image in DrawSurface!");
    }
}