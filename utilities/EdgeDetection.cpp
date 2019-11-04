#include "EdgeDetection.hpp"

Image* EdgeDetection::gradient(Image *image) {
    Image *result = new Image(image->getWidth(), image->getHeight());
    for (int i=0; i < image->getWidth(); i++) {
        for (int j=0; j < image->getHeight(); j++) {
            Pixel *gx, *gy;
            if (i + 1 < image->getWidth()) {
                gx = image->getPixelAt(i + 1 ,j) - image->getPixelAt(i, j);
            } else {
                gx = Pixel() -image->getPixelAt(i,j);
            }
            if (j + 1 < image->getHeight()) {
                gy = image->getPixelAt(i ,j + 1) - image->getPixelAt(i, j);
            } else {
                gy = Pixel() - image->getPixelAt(i,j);
            }
            gx = gx->operator*(*gx);
            gy = gy->operator*(*gy);
            Pixel *a = new Pixel(int(sqrt(gx->getRed() + gy->getRed())), int(sqrt(gx->getGreen() + gy->getGreen())), int(sqrt(gx->getBlue() + gy->getBlue())));
            result->setPixelAt(i,j,*a);
            delete a;
            delete gx;
            delete gy;
        }
    }
    return result;
}
