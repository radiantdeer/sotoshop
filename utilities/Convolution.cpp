#include "Convolution.hpp"
#include <vector>
#include "../spdlog/spdlog.h"

Image* Convolution::convolve(Image* image, const ConvolutionMatrix& opMatrix, bool padImage) {

    int resultWidth, resultHeight;
    Image *sourceImage;

    spdlog::debug("Convolution::convolve: Preparing...");
    if (padImage) {
        spdlog::debug("Convolution::convolve: Adding padding to source image...");
        resultWidth = image->getWidth();
        resultHeight = image->getHeight();
        int padWidth = opMatrix.getWidth() / 2;
        int padHeight = opMatrix.getHeight() / 2;
        int sourceWidth = image->getWidth() + (padWidth * 2);
        int sourceHeight = image->getHeight() + (padHeight * 2);
        sourceImage = new Image(sourceWidth, sourceHeight);
        spdlog::debug("New source image dimension : {}x{}", sourceImage->getWidth(), sourceImage->getHeight());
        for (int j = 0; j < padHeight; j++) {
            for (int i = 0; i < sourceWidth; i++) {
                sourceImage->setPixelAt(i, j, Pixel(0, 0, 0));
            }
        }
        for (int j = padHeight; j < (resultHeight + padHeight); j++) {
            for (int i = 0; i < padWidth; i++) {
                sourceImage->setPixelAt(i, j, Pixel(0, 0, 0));
            }
            for (int i = padWidth; i < (resultWidth + padWidth); i++) {
                sourceImage->setPixelAt(i, j, image->getPixelAt(i - padWidth, j - padHeight));
            }
            for (int i = (resultWidth + padWidth); i < sourceWidth; i++) {
                sourceImage->setPixelAt(i, j, Pixel(0, 0, 0));
            }
        }
        for (int j = (resultHeight + padHeight); j < (resultHeight + (padHeight * 2)); j++) {
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
    spdlog::debug("Convolution::convolve: Starting convolution...");
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
            Pixel thisPixel (redSum, greenSum, blueSum);
            result->setPixelAt(i, j, thisPixel);
        }
    }
    spdlog::debug("Convolution::convolve: Convolution done.");

    return result;
}