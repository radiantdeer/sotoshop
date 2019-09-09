#include "DrawSurface.hpp"

#include <iostream>
#include <QColor>
#include <QPainter>

#include "../datastruct/Pixel.hpp"

DrawSurface::DrawSurface() : QWidget() {
    imageLoaded = false;
}

DrawSurface::DrawSurface(QWidget * parentWidget) : QWidget(parentWidget) {
    imageLoaded = false;
}

DrawSurface::DrawSurface(QWidget * parentWidget, Image * activeImage) : QWidget(parentWidget) {
    this->activeImage = activeImage;
    imageLoaded = true;
}

Image * DrawSurface::getActiveImage() {
    return activeImage;
}

bool DrawSurface::isImageLoaded() {
    return imageLoaded;
}

void DrawSurface::setActiveImage(Image * newImage) {
    activeImage = newImage;
}

void DrawSurface::setImageLoaded(bool imageLoaded) {
    this->imageLoaded = imageLoaded;
}

// Implementing protected virtual method from QWidget
// Currently set to draw a small red line
// This function will be used to draw/re-draw the image
void DrawSurface::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    if (this->isImageLoaded()) {
        for (int i = 0; i < activeImage->getHeight(); i++) {
            for (int j = 0; j < activeImage->getWidth(); j++) {
                Pixel thisPixel = activeImage->getPixelAt(j, i);
                QColor * thisPixelColor = new QColor(thisPixel.getRed(), thisPixel.getGreen(), thisPixel.getBlue());
                painter.setPen(*thisPixelColor);
                painter.drawPoint(j, i);
            }
        }
    } else {
        std::cout << "[WARN] No active image in DrawSurface!" << std::endl;
    }
}