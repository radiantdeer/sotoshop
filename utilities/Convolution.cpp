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
        sourceImage = Convolution::padImage(image, padWidth, padHeight);
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

Image* Convolution::medianConvolve(Image* image, bool padImage) {
    Image * result;

    return result;
}


Image* Convolution::padImage(Image* image, int padWidth, int padHeight) {
    int originalWidth = image->getWidth();
    int originalHeight = image->getHeight();
    int paddedWidth = originalWidth + (padWidth * 2);
    int paddedHeight = originalHeight + (padHeight * 2);
    Image * paddedImage = new Image(paddedWidth, paddedHeight);

    spdlog::debug("Convolution::padImage: Padded image dimension : {}x{}", paddedImage->getWidth(), paddedImage->getHeight());
    for (int j = 0; j < padHeight; j++) {
        for (int i = 0; i < paddedWidth; i++) {
            paddedImage->setPixelAt(i, j, Pixel(0, 0, 0));
        }
    }
    for (int j = padHeight; j < (originalHeight + padHeight); j++) {
        for (int i = 0; i < padWidth; i++) {
            paddedImage->setPixelAt(i, j, Pixel(0, 0, 0));
        }
        for (int i = padWidth; i < (originalWidth + padWidth); i++) {
            paddedImage->setPixelAt(i, j, image->getPixelAt(i - padWidth, j - padHeight));
        }
        for (int i = (originalWidth + padWidth); i < paddedWidth; i++) {
            paddedImage->setPixelAt(i, j, Pixel(0, 0, 0));
        }
    }
    for (int j = (originalHeight + padHeight); j < (originalHeight + (padHeight * 2)); j++) {
        for (int i = 0; i < paddedWidth; i++) {
            paddedImage->setPixelAt(i, j, Pixel(0, 0, 0));
        }
    }

    return paddedImage;
}