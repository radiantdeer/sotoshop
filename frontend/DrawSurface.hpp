#include <QPaintEvent>
#include <QWidget>
#include "../datastruct/Image.hpp"

class DrawSurface : public QWidget {

    private:
        Image * activeImage;

    public:
        DrawSurface();
        DrawSurface(QWidget * parentWidget);
        DrawSurface(QWidget * parentWidget, Image * activeImage);

        Image * getActiveImage();

        void setActiveImage(Image * newImage);

        void paintEvent(QPaintEvent * event);

};