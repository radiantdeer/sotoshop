#include "Convolution.hpp"
#include <vector>

Image* Convolution::convolve(Image* image, ConvolutionMatrix& opMatrix, bool padImage) {

    int resultWidth, resultHeight;
    Image *sourceImage;

    if (padImage) {
        resultWidth = image->getWidth();
        resultHeight = image->getHeight();
        int padWidth = (opMatrix.getWidth() / 2) * 2;
        int padHeight = (opMatrix.getHeight() / 2) * 2;
        int sourceWidth = image->getWidth() + padWidth;
        int sourceHeight = image->getHeight() + padHeight;
        sourceImage = new Image(sourceWidth, sourceHeight);
        for (int j = 0; j < padHeight; j++) {
            for (int i = 0; i < sourceWidth; i++) {
                sourceImage->setPixelAt(i, j, Pixel(0, 0, 0));
            }
        }
        for (int j = padHeight; j < sourceHeight + padHeight; j++) {
            for (int i = 0; i < padWidth; i++) {
                sourceImage->setPixelAt(i, j, Pixel(0, 0, 0));
            }
            for (int i = padWidth; i < padWidth + sourceWidth; i++) {
                sourceImage->setPixelAt(i, j, image->getPixelAt(i, j));
            }
            for (int i = padWidth + sourceWidth; i < padWidth + (sourceWidth * 2); i++) {
                sourceImage->setPixelAt(i, j, Pixel(0, 0, 0));
            }
        }
        for (int j = sourceHeight + padHeight; j < sourceHeight + (padHeight * 2); j++) {
            for (int i = 0; i < sourceWidth; i++) {
                sourceImage->setPixelAt(i, j, Pixel(0, 0, 0));
            }
        }
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