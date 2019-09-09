#include <QPaintEvent>
#include <QWidget>
#include "../datastruct/Image.hpp"

class DrawSurface : public QWidget {

    private:
        Image * activeImage;
        bool imageLoaded;

    public:
        DrawSurface();
        DrawSurface(QWidget * parentWidget);
        DrawSurface(QWidget * parentWidget, Image * activeImage);

        Image * getActiveImage();
        bool isImageLoaded();
        void setActiveImage(Image * newImage);
        void setImageLoaded(bool imageLoaded);

        void paintEvent(QPaintEvent * event);

};