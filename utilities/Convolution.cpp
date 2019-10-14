#include "Convolution.hpp"

Image* Convolution::convolve(Image* image, ConvolutionMatrix& opMatrix, bool padImage) {

    int resultWidth, resultHeight;
    Image *sourceImage;
    if (padImage) {
        resultWidth = image->getWidth();
        resultHeight = image->getHeight();
        sourceImage = new Image(*image);
        // TODO: pad the image here
    } else {
        resultWidth = image->getWidth() - ((opMatrix.getWidth() / 2) * 2);
        resultHeight = image->getHeight() - ((opMatrix.getHeight() / 2) * 2);
        sourceImage = image;
    }
    Image * result = new Image(resultWidth, resultHeight);

    for (int j = 0; j < resultHeight; j++) {
        for (int i = 0; i < resultWidth; i++) {
            int redSum = 0;
            int greenSum = 0;
            int blueSum = 0;
            for (int y = 0; y < opMatrix.getHeight(); y++) {
                for (int x = 0; x < opMatrix.getWidth(); x++) {
                    int opConstant = opMatrix.getElementAt(x, y);
                    Pixel currentPixel = sourceImage->getPixelAt(i + x, j + y);
                    redSum += (currentPixel.getRed() * opConstant);
                    greenSum += (currentPixel.getGreen() * opConstant);
                    blueSum += (currentPixel.getBlue() * opConstant);
                }
            }
            redSum /= opMatrix.getMatrixSum();
            greenSum /= opMatrix.getMatrixSum();
            blueSum /= opMatrix.getMatrixSum();
            result->setPixelAt(i, j, Pixel((unsigned char) redSum, (unsigned char) greenSum, (unsigned char) blueSum));
        }
    }

    return result;
}