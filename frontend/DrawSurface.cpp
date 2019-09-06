#include "DrawSurface.hpp"

#include <QPainter>

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

// Implementing protected virtual method from QWidget
// Currently set to draw a small red line
// This function will be used to draw/re-draw the image
void DrawSurface::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.setPen(QColor(Qt::red));
    for (int i = 50; i < 100; i++) {
        painter.drawPoint(i, 20);
    }
    for (int i = 0; i < 50; i++) {
        painter.drawPoint(100, 20 + i);
    }
    for (int i = 0; i < 50; i++) {
        painter.drawPoint(50, 20 + i);
    }
    for (int i = 50; i < 100; i++) {
        painter.drawPoint(i, 70);
    }
}