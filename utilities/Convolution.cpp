#include "Convolution.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>
#include "CommonConvolutions.hpp"
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
                    redSum += (opConstant * (int) currentPixel.getRed());
                    greenSum += (opConstant * (int) currentPixel.getGreen());
                    blueSum += (opConstant * (int) currentPixel.getBlue());
                }
            }
            redSum /= opMatrix.getMatrixSum();
            greenSum /= opMatrix.getMatrixSum();
            blueSum /= opMatrix.getMatrixSum();
            Pixel thisPixel (Pixel::thresholding(redSum), Pixel::thresholding(greenSum), Pixel::thresholding(blueSum));
            result->setPixelAt(i, j, thisPixel);
        }
    }
    spdlog::debug("Convolution::convolve: Convolution done.");
    return result;
}

Image* Convolution::medianConvolve(Image* image, int filterWidth, int filterHeight, bool padImage) {

    int resultWidth, resultHeight;
    Image * sourceImage;

    spdlog::debug("Convolution::medianConvolve: Preparing...");
    if (padImage) {
        spdlog::debug("Convolution::medianConvolve: Adding padding to source image...");
        resultWidth = image->getWidth();
        resultHeight = image->getHeight();
        int padWidth = filterWidth / 2;
        int padHeight = filterHeight / 2;
        sourceImage = Convolution::padImage(image, padWidth, padHeight);
    } else {
        resultWidth = image->getWidth() - ((filterWidth / 2) * 2);
        resultHeight = image->getHeight() - ((filterHeight / 2) * 2);
        sourceImage = image;
    }

    Image * result = new Image(resultWidth, resultHeight);
    result->setOriginalFormat(sourceImage->getOriginalFormat());
    spdlog::debug("Convolution::medianConvolve: Starting convolution...");
    for (int j = 0; j < resultHeight; j++) {
        for (int i = 0; i < resultWidth; i++) {
            std::vector<Pixel> filter;
            for (int y = 0; y < filterHeight; y++) {
                for (int x = 0; x < filterWidth; x++) {
                    filter.push_back(sourceImage->getPixelAt(i + x, j + y));
                }
            }
            std::sort(filter.begin(), filter.end());
            Pixel * thisPixel;
            if ((filter.size() % 2) == 1) {
                thisPixel = new Pixel(filter[filter.size() / 2]);
            } else {
                Pixel firstPixel = filter[filter.size() / 2];
                Pixel secondPixel = filter[(filter.size() / 2) + 1];
                int red = ((int) firstPixel.getRed() + (int) secondPixel.getRed()) / 2;
                int green = ((int) firstPixel.getGreen() + (int) secondPixel.getGreen()) / 2;
                int blue = ((int) firstPixel.getBlue() + (int) secondPixel.getBlue()) / 2;
                thisPixel = new Pixel(red, green, blue);
            }
            result->setPixelAt(i, j, *thisPixel);
            delete thisPixel;
        }
    }
    spdlog::debug("Convolution::medianConvolve: Convolution done.");
    return result;
}

Image * Convolution::unsharpMasking(Image* image) {
    Image * highpass = Convolution::highpass(image);
    spdlog::debug("Convolution::unsharpMasking: Calculating unsharp masking...");
    Image * result = new Image(*image);
    result->add(*highpass);
    delete highpass;
    return result;
}

Image* Convolution::highboost(Image* image, double alpha) {
    Image * highpass = Convolution::highpass(image);
    spdlog::debug("Convolution::highboost: Calculating highboost...");
    Image * result = new Image(*image);
    if (alpha < 1) {
        alpha = 1;
    }
    result->multiply(alpha - 1);
    result->add(*highpass);
    delete highpass;
    return result;
}

Image* Convolution::sobelOperation(Image* image, const ConvolutionMatrix& opMatrixX, const ConvolutionMatrix& opMatrixY) {
    int resultWidth, resultHeight;
    Image *sourceImage;

    // IMAGE CONVERTED TO GRAYSCALE FIRST
    Image * grayImage = image->grayscale();

    spdlog::debug("Convolution::sobelOperation: Preparing...");
    sourceImage = grayImage;

    // OPERATE WITH UNPADDED IMAGE
    resultWidth = image->getWidth() - ((opMatrixX.getWidth() / 2) * 2);
    resultHeight = image->getHeight() - ((opMatrixX.getHeight() / 2) * 2);

    // ADD FORMAT TO RESULT
    Image * result = new Image(resultWidth, resultHeight);
    result->setOriginalFormat(sourceImage->getOriginalFormat());

    for (int j = 0; j < resultHeight; j++) {
        for (int i = 0; i < resultWidth; i++) {
            // SOBEL VARIABLE
            int xSum = 0;
            int ySum = 0;
            int magnitude = 0;

            // COUNT X SUM
            for (int y = 0; y < opMatrixX.getHeight(); y++) {
                for (int x = 0; x < opMatrixX.getWidth(); x++) {
                    int opConstant = opMatrixX.getElementAt(x, y);
                    Pixel currentPixel = sourceImage->getPixelAt(i + x, j + y);
                    xSum += (opConstant * (int) currentPixel.getRed());
                }
            }

            // COUNT Y SUM
            for (int y = 0; y < opMatrixY.getHeight(); y++) {
                for (int x = 0; x < opMatrixY.getWidth(); x++) {
                    int opConstant = opMatrixY.getElementAt(x, y);
                    Pixel currentPixel = sourceImage->getPixelAt(i + x, j + y);
                    ySum += (opConstant * (int) currentPixel.getRed());
                }
            }

            // SET MAGNITUDE
            magnitude = abs(xSum) + abs(ySum);

            // SET NEW PIXEL VALUE
            Pixel thisPixel (Pixel::thresholding(magnitude), Pixel::thresholding(magnitude), Pixel::thresholding(magnitude));
            result->setPixelAt(i, j, thisPixel);
        }
    }

    Image * paddedResult;

    // PADD IMAGE
    spdlog::debug("Convolution::sobelOperation: Adding padding to result image...");
    int padWidth = opMatrixX.getWidth() / 2;
    int padHeight = opMatrixX.getHeight() / 2;
    paddedResult = Convolution::padImage(result, padWidth, padHeight);

    return paddedResult;
}

Image* Convolution::cannyOperation(Image* image, const ConvolutionMatrix& gaussianMatrix, const ConvolutionMatrix& opMatrixX, const ConvolutionMatrix& opMatrixY, int threshold) {

    /*********************************************************/
    /* GAUSSIAN BLURRING IMAGE                               */
    /*********************************************************/
    Image* gaussianImage = convolve(image, gaussianMatrix, true);

    /*********************************************************/
    /* MAKE GRADIENT X AND Y FROM IMAGE                      */
    /*********************************************************/
    // IMAGE CONVERTED TO GRAYSCALE FIRST
    Image * grayImage = gaussianImage->grayscale();
    Image * sourceImage;
    int resultWidth, resultHeight;
    std::vector<std::vector<int>> magnitudeX;
    std::vector<std::vector<int>> magnitudeY;

    int padWidth = opMatrixX.getWidth() / 2;
    int padHeight = opMatrixX.getHeight() / 2;

    spdlog::debug("Convolution::cannyOperation: Padding image...");
    sourceImage = Convolution::padImage(grayImage, padWidth, padHeight);

    // OPERATE WITH PADDED IMAGE
    resultWidth = image->getWidth();
    resultHeight = image->getHeight();

    // SOBEL OPERATION
    spdlog::debug("Convolution::cannyOperation: Starting Sobel Operation...");
    Image * sobel = sobelOperation(sourceImage, CommonConvolutions::SobelX, CommonConvolutions::SobelY);    
    std::vector<std::vector<double>> theta = imageThetaMatrix(sourceImage, CommonConvolutions::SobelX, CommonConvolutions::SobelY);
    Image * result = new Image(resultWidth, resultHeight);

    // "Thinning"
    spdlog::debug("Convolution::cannyOperation: Running non-max suppression...");
    for (int j = 1; j < theta.size() - 1; j++) {
        for (int i = 1; i < theta.at(j).size() - 1; i++) {
            int q = 255;
            int r = 255;

            if (((theta.at(j).at(i) >= 0) && (theta.at(j).at(i) < 22.5)) || ((theta.at(j).at(i) >= 157.5) && (theta.at(j).at(i) <= 180))) {
                q = sobel->getPixelAt(i, j + 1).getRed();
                r = sobel->getPixelAt(i, j - 1).getRed();
            } else if ((theta.at(j).at(i) >= 22.5) && (theta.at(j).at(i) < 67.5)) {
                q = sobel->getPixelAt(i + 1, j - 1).getRed();
                r = sobel->getPixelAt(i - 1, j + 1).getRed();
            } else if ((theta.at(j).at(i) >= 67.5) && (theta.at(j).at(i) < 112.5)) {
                q = sobel->getPixelAt(i + 1, j).getRed();
                r = sobel->getPixelAt(i - 1, j).getRed();
            } else if ((theta.at(j).at(i) >= 112.5) && (theta.at(j).at(i) < 157.5)) {
                q = sobel->getPixelAt(i - 1, j - 1).getRed();
                r = sobel->getPixelAt(i + 1, j + 1).getRed();
            }
            
            int thisVal = sobel->getPixelAt(i, j).getRed();
            if ((thisVal > q) && (thisVal > r)) {
                result->setPixelAt(i, j, Pixel(thisVal, thisVal, thisVal));
            }
        }
    }

    // Thresholding
    for (int j = 0; j < result->getHeight(); j++) {
        for (int i = 0; i < result->getWidth(); i++) {
            if (result->getPixelAt(i, j).getRed() >= threshold) {
                result->setPixelAt(i, j, Pixel(255, 255, 255));
            } else {
                result->setPixelAt(i, j, Pixel(0, 0, 0));
            }
        }
    }
    return result;
}

Image* Convolution::padImage(Image* image, int padWidth, int padHeight) {
    int originalWidth = image->getWidth();
    int originalHeight = image->getHeight();
    int paddedWidth = originalWidth + (padWidth * 2);
    int paddedHeight = originalHeight + (padHeight * 2);
    Image * paddedImage = new Image(paddedWidth, paddedHeight);
    paddedImage->setOriginalFormat(image->getOriginalFormat());

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

Image* Convolution::highpass(Image* image) {
    spdlog::debug("Convolution::highpass: Calculating lowpass...");
    Image * lowpass = convolve(image, CommonConvolutions::Average5, true);
    spdlog::debug("Convolution::highpass: Calculating highpass...");
    Image * highpass = new Image(*image);
    highpass->substract(*lowpass);
    delete lowpass;
    return highpass;
}

std::vector<std::vector<double>> Convolution::imageThetaMatrix(Image *image, const ConvolutionMatrix& opMatrixX, const ConvolutionMatrix& opMatrixY) {
    // IMAGE CONVERTED TO GRAYSCALE FIRST
    Image * sourceImage = image->grayscale();

    spdlog::debug("Convolution::sobelOperation: Preparing...");

    // OPERATE WITH UNPADDED IMAGE
    int resultWidth = image->getWidth() - ((opMatrixX.getWidth() / 2) * 2);
    int resultHeight = image->getHeight() - ((opMatrixX.getHeight() / 2) * 2);

    // ADD FORMAT TO RESULT

    std::vector<std::vector<double>> result;
    for (int j = 0; j < resultHeight; j++) {
        result.push_back(std::vector<double>());
        for (int i = 0; i < resultWidth; i++) {
            // SOBEL VARIABLE
            int xSum = 0;
            int ySum = 0;

            // COUNT X SUM
            for (int y = 0; y < opMatrixX.getHeight(); y++) {
                for (int x = 0; x < opMatrixX.getWidth(); x++) {
                    int opConstant = opMatrixX.getElementAt(x, y);
                    Pixel currentPixel = sourceImage->getPixelAt(i + x, j + y);
                    xSum += (opConstant * (int) currentPixel.getRed());
                }
            }

            // COUNT Y SUM
            for (int y = 0; y < opMatrixY.getHeight(); y++) {
                for (int x = 0; x < opMatrixY.getWidth(); x++) {
                    int opConstant = opMatrixY.getElementAt(x, y);
                    Pixel currentPixel = sourceImage->getPixelAt(i + x, j + y);
                    ySum += (opConstant * (int) currentPixel.getRed());
                }
            }

            double theta = atan2(ySum, xSum) * 180 / M_PI;
            if (theta < 0) {
                theta += 180;
            }

            result.at(j).push_back(theta);        
        }
    }

    return result;
}
