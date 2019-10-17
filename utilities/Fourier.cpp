#include "Fourier.hpp"
#include <cmath>
#include "../spdlog/spdlog.h"

// This Fourier transform will only support grayscale images
// Any non-grayscale images will be converted to grayscale
std::vector<std::vector<std::complex<double>> *> * Fourier::forward(Image * image) {
    Image * sourceImage;
    if ((image->getOriginalFormat() == "bmp") || (image->getOriginalFormat() == "pgm")) {
        spdlog::info("Fourier::forward: Converting image to grayscale first...");
        sourceImage = new Image(*image);
        sourceImage->grayscale();
    } else {
        sourceImage = image;
    }
    spdlog::info("Fourier::forward: Allocating vector for result...");
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
            done++;
            percentage = (double) done * 100 / (double) numOfFrequencies;
            if ((percentage - prevPercentage) > 1) {
                spdlog::info("{}%", percentage);
                prevPercentage = percentage;
            }
        }
    }
    spdlog::info("Fourier::forward: Done.");
    return result;
}

Image * Fourier::inverse(std::vector<std::vector<std::complex<double>> *> * frequencies) {
    Image * result = nullptr;

    return result;
}
