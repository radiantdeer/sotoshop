#include "CommonConvolutions.hpp"
#include "Convolution.hpp"
#include "EdgeDetection.hpp"
#include <cmath>

Image* EdgeDetection::gradient(Image *image) {
    Image *result = new Image(image->getWidth(), image->getHeight());
    for (int i=0; i < image->getWidth(); i++) {
        for (int j=0; j < image->getHeight(); j++) {
            Pixel *gx, *gy;
            if (i + 1 < image->getWidth()) {
                Pixel p1 = image->getPixelAt(i + 1 ,j);
                Pixel p2 = image->getPixelAt(i, j);
                gx = p1 - p2;
            } else {
                Pixel p2 = image->getPixelAt(i,j);
                gx = Pixel() - p2;
            }
            if (j + 1 < image->getHeight()) {
                Pixel p1 = image->getPixelAt(i ,j + 1);
                Pixel p2 = image->getPixelAt(i, j);
                gy = p1 - p2;
            } else {
                Pixel p2 = image->getPixelAt(i,j);
                gy = Pixel() - p2;
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

Image* EdgeDetection::laplaceOfGaussian(Image* image) {
    Image* log = Convolution::convolve(image, CommonConvolutions::LaplaceOfGaussian, true);
    return log;
}
