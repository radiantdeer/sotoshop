#include "Fourier.hpp"
#include <cmath>
#include "../spdlog/spdlog.h"

// This Fourier transform will only support grayscale images
// Any non-grayscale images will be converted to grayscale
std::vector<std::vector<std::complex<double>> *> * Fourier::forward(Image * image) {
    Image * sourceImage;
    if ((image->getOriginalFormat() == "bmp") || (image->getOriginalFormat() == "ppm")) {
        spdlog::debug("Fourier::forward: Converting image to grayscale first...");
        sourceImage = new Image(*image);
        sourceImage->grayscale();
    } else {
        sourceImage = image;
    }
    spdlog::debug("Fourier::forward: Allocating vector for result...");
    std::vector<std::vector<std::complex<double>> *> * result = new std::vector<std::vector<std::complex<double>> *>(image->getHeight());
    for (int j = 0; j < image->getHeight(); j++) {
        result->at(j) = new std::vector<std::complex<double>>(image->getWidth());
        for (int i = 0; i < image->getWidth(); i++) {
            result->at(j)->at(i) = 0;
        }
    }
    spdlog::info("Fourier::forward: Starting Fourier transform...");
    int numOfFrequencies = sourceImage->getHeight() * sourceImage->getWidth();
    int done = 0;
    double prevPercentage = 0;
    double percentage = 0;
    for (int v = 0; v < result->size(); v++) {
        for (int u = 0; u < result->at(v)->size(); u++) {
            for (int j = 0; j < sourceImage->getHeight(); j++) {
                for (int i = 0; i < sourceImage->getWidth(); i++) {
                    result->at(v)->at(u) += std::polar((double) sourceImage->getPixelAt(i, j).getRed(), (double) -2 * M_PI * (((double) u * i / sourceImage->getWidth()) + ((double) v * j / sourceImage->getHeight())));
                }
            }
            result->at(v)->at(u) /= (sourceImage->getHeight() * sourceImage->getWidth());
            // Below is just for progress monitoring on logs
            done++;
            percentage = (double) done * 100 / (double) numOfFrequencies;
            if ((percentage - prevPercentage) > 1) {
                spdlog::info("Fourier::forward: {}%", percentage);
                prevPercentage = percentage;
            }
        }
    }
    spdlog::info("Fourier::forward: Done.");
    return result;
}

Image * Fourier::visualizeFrequencies(std::vector<std::vector<std::complex<double>> *> * frequencies) {
    Image * result = new Image(frequencies->at(0)->size(), frequencies->size());
    for (int j = 0; j < result->getHeight(); j++) {
        for (int i = 0; i < result->getWidth(); i++) {
            double thisValue = std::norm(frequencies->at(j)->at(i));
            thisValue = 128 * log10(thisValue + 1);
            Pixel thisPixel ((unsigned char) thisValue, (unsigned char) thisValue, (unsigned char) thisValue);
            result->setPixelAt(i, j, thisPixel);
        }
    }
    return result;
}

Image * Fourier::inverse(std::vector<std::vector<std::complex<double>> *> * frequencies) {
    spdlog::debug("Fourier::inverse: Allocating image result...");
    Image * result = new Image(frequencies->at(0)->size(), frequencies->size());

    spdlog::info("Fourier::inverse: Starting Inverse Fourier transform...");
    int numOfFrequencies = result->getHeight() * result->getWidth();
    int done = 0;
    double prevPercentage = 0;
    double percentage = 0;
    for (int v = 0; v < result->getHeight(); v++) {
        for (int u = 0; u < result->getWidth(); u++) {
            double thisValue = 0;
            for (int j = 0; j < frequencies->size(); j++) {
                for (int i = 0; i < frequencies->at(j)->size(); i++) {
                    std::complex<double> expValue = std::polar((double) 1, 2 * M_PI * (((double) u * i / result->getWidth()) + ((double) v * j / result->getHeight())));
                    std::complex<double> temp = frequencies->at(j)->at(i) * expValue;
                    thisValue += std::real(temp);
                }
            }
            spdlog::debug("{}", thisValue);
            result->setPixelAt(u, v, Pixel(thisValue, thisValue, thisValue));
            // Below is just for progress monitoring on logs
            done++;
            percentage = (double) done * 100 / (double) numOfFrequencies;
            if ((percentage - prevPercentage) > 1) {
                spdlog::info("Fourier::inverse: {}%", percentage);
                prevPercentage = percentage;
            }
        }
    }
    spdlog::info("Fourier::inverse: Done.");
    return result;
}

void Fourier::shift(std::vector<std::vector<std::complex<double>> *> * frequencies) {
    // Shift rows
    int centerPoint = frequencies->size() / 2;
    std::vector<std::complex<double>> * centerElement = nullptr;
    if ((frequencies->size() % 2) == 1) {
        centerElement = frequencies->at(centerPoint);
        frequencies->erase(frequencies->begin() + centerPoint);
    }
    for (int i = 0; i < centerPoint; i++) {
        std::vector<std::complex<double>> * temp = frequencies->at(i);
        frequencies->at(i) = frequencies->at(centerPoint + i);
        frequencies->at(centerPoint + i) = temp;
    }
    if (centerElement != nullptr) {
        frequencies->push_back(centerElement);
    }

    // Shift columns
    for (int j = 0; j < frequencies->size(); j++) {
        int centerPoint = frequencies->at(j)->size() / 2;
        std::complex<double> centerElement;
        if ((frequencies->size() % 2) == 1) {
            centerElement = frequencies->at(j)->at(centerPoint);
            frequencies->erase(frequencies->begin() + centerPoint);
        }
        for (int i = 0; i < centerPoint; i++) {
            std::complex<double> temp = frequencies->at(j)->at(i);
            frequencies->at(j)->at(i) = frequencies->at(j)->at(centerPoint + i);
            frequencies->at(j)->at(centerPoint + i) = temp;
        }
        if ((frequencies->size() % 2) == 1) {
            frequencies->at(j)->push_back(centerElement);
        }
    }
}
