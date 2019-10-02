#include "ImageOperator.h"

ImageOperator::ImageOperator()
{

}

// TODO
// DEBUG METHOD
Image ImageOperator::AddTwoImages(Image A, Image B, int width, int height) {
    Image *C = new Image(A);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Pixel a = A.getPixelAt(i, j);
            Pixel b = B.getPixelAt(i, j);
            Pixel *pixel = new Pixel();

            pixel->setRed(a.getRed() + b.getRed());
            pixel->setBlue(a.getBlue() + b.getBlue());
            pixel->setGreen(a.getGreen() + b.getGreen());

            C->setPixelAt(width, height, *pixel);
        }
    }

    return *C;
}


// TODO
// DEBUG METHOD
Image ImageOperator::SubstractTwoImages(Image A, Image B, int width, int height) {
    Image *C = new Image(A);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Pixel a = A.getPixelAt(i, j);
            Pixel b = B.getPixelAt(i, j);
            Pixel *pixel = new Pixel();

            pixel->setRed(a.getRed() - b.getRed());
            pixel->setBlue(a.getBlue() - b.getBlue());
            pixel->setGreen(a.getGreen() - b.getGreen());

            C->setPixelAt(width, height, *pixel);
        }
    }

    return *C;
}

// TODO
// DEBUG METHOD
void ImageOperator::BrightenImage(Image A, unsigned char brightness) {
    for (int i = 0; i < A.getWidth(); i++) {
        for (int j = 0; j < A.getHeight(); j++) {
            Pixel a = A.getPixelAt(i, j);
            Pixel *pixel = new Pixel();

            pixel->setRed(a.getRed() + brightness);
            pixel->setBlue(a.getBlue() + brightness);
            pixel->setGreen(a.getGreen() + brightness);

            A.setPixelAt(i, j, *pixel);
        }
    }
}
