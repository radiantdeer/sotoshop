#include <mutex>
#include <QPaintEvent>
#include <QWidget>
#include "../datastruct/Image.hpp"

class DrawSurface : public QWidget {

    private:
        Image * activeImage;
        bool imageLoaded;
        std::mutex * activeImageLock;

    public:
        DrawSurface();
        DrawSurface(QWidget * parentWidget);
        DrawSurface(QWidget * parentWidget, Image * activeImage);

        Image * getActiveImage();
        bool isImageLoaded();
        // Changing active image is not thread-safe, please call acquireLockImage before changing &
        // releaseLockImage after changing
        void setActiveImage(Image * newImage);
        void setActiveImage(Image& newImage);
        void setImageLoaded(bool imageLoaded);
        void acquireLockImage();
        void releaseLockImage();
        void purgeImage();

        void paintEvent(QPaintEvent * event);

};