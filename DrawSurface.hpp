#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include "datastruct/Image.hpp"

class DrawSurface : public QWidget {

    private:
        Image * activeImage;

    public:
        DrawSurface() : QWidget() {}
        DrawSurface(QWidget * parentWidget) : QWidget(parentWidget) {}
        DrawSurface(QWidget * parentWidget, Image * activeImage) : QWidget(parentWidget) {
            this->activeImage = activeImage;
        }

        Image * getActiveImage() {
            return activeImage;
        }

        void setActiveImage(Image * newImage) {
            activeImage = newImage;
        }

        // Implementing protected virtual method from QWidget
        // Currently set to draw a small red line
        // This function will be used to draw/re-draw the image
        void paintEvent(QPaintEvent * event) {
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

};