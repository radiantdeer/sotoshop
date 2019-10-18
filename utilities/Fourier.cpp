#include "Fourier.hpp"
#include <atomic>
#include <cmath>
#include "../spdlog/spdlog.h"
#include <thread>

const int MAX_THREAD = 4;
std::atomic<int> fourierMaxThread (MAX_THREAD);
std::atomic<int> done(0);
std::atomic<double> prevPercentage;
std::atomic<double> percentage;

// This Fourier transform will only support grayscale images
// Any non-grayscale images will be converted to grayscale
std::vector<std::vector<std::vector<std::complex<double>>>> * Fourier::forward(Image * image) {
    Image * sourceImage = image;

    spdlog::debug("Fourier::forward: Allocating vector for result...");
    std::vector<std::vector<std::vector<std::complex<double>>>> * result = new std::vector<std::vector<std::vector<std::complex<double>>>>();
    for (int j = 0; j < image->getHeight(); j++) {
        result->push_back(std::vector<std::vector<std::complex<double>>>());
        for (int i = 0; i < image->getWidth(); i++) {
            result->at(j).push_back(std::vector<std::complex<double>>());
            result->at(j).at(i).push_back(std::complex<double>(0, 0));
            result->at(j).at(i).push_back(std::complex<double>(0, 0));
            result->at(j).at(i).push_back(std::complex<double>(0, 0));
        }
    }
    spdlog::info("Fourier::forward: Starting Fourier transform...");
    int numOfFrequencies = sourceImage->getHeight() * sourceImage->getWidth();
    int v = 0;
    done.store(0);
    percentage.store(0);
    prevPercentage.store(0);
    while (v < result->size()) {
        fourierMaxThread -= 1;
        std::thread newThread (singleRowForward, image, result, v);
        newThread.detach();
        v++;
        while (fourierMaxThread <= 0) {}
    }

    while (fourierMaxThread < MAX_THREAD) {}

    spdlog::info("Fourier::forward: Done.");
    return result;
}

Image * Fourier::visualizeFrequencies(std::vector<std::vector<std::vector<std::complex<double>>>> * frequencies) {
    Image * result = new Image(frequencies->at(0).size(), frequencies->size());
    for (int j = 0; j < result->getHeight(); j++) {
        for (int i = 0; i < result->getWidth(); i++) {
            double redValue = std::norm(frequencies->at(j).at(i).at(0));
            redValue = 128 * log10(redValue + 1);
            double greenValue = std::norm(frequencies->at(j).at(i).at(1));
            greenValue = 128 * log10(greenValue + 1);
            double blueValue = std::norm(frequencies->at(j).at(i).at(2));
            blueValue = 128 * log10(blueValue + 1);
            Pixel thisPixel ((unsigned char) redValue, (unsigned char) greenValue, (unsigned char) blueValue);
            result->setPixelAt(i, j, thisPixel);
        }
    }
    return result;
}

Image * Fourier::inverse(std::vector<std::vector<std::vector<std::complex<double>>>> * frequencies) {
    spdlog::debug("Fourier::inverse: Allocating image result...");
    Image * result = new Image(frequencies->at(0).size(), frequencies->size());

    spdlog::info("Fourier::inverse: Starting Inverse Fourier transform...");
    int numOfFrequencies = result->getHeight() * result->getWidth();
    int v = 0;
    done.store(0);
    prevPercentage.store(0);
    percentage.store(0);

    while (v < result->getHeight()) {
        fourierMaxThread -= 1;
        std::thread newThread (singleRowInverse, result, frequencies, v);
        newThread.detach();
        v++;
        while (fourierMaxThread <= 0) {}
    }

    while (fourierMaxThread < MAX_THREAD) {}

    spdlog::info("Fourier::inverse: Done.");
    return result;
}

void Fourier::shift(std::vector<std::vector<std::vector<std::complex<double>>>> * frequencies) {
    // Shift rows
    int centerPoint = frequencies->size() / 2;
    std::vector<std::vector<std::complex<double>>> centerElement;
    if ((frequencies->size() % 2) == 1) {
        centerElement = frequencies->at(centerPoint);
        frequencies->erase(frequencies->begin() + centerPoint);
    }
    for (int i = 0; i < centerPoint; i++) {
        std::vector<std::vector<std::complex<double>>> temp = frequencies->at(i);
        frequencies->at(i) = frequencies->at(centerPoint + i);
        frequencies->at(centerPoint + i) = temp;
    }
    if ((frequencies->size() % 2) == 1) {
        frequencies->push_back(centerElement);
    }

    // Shift columns
    for (int j = 0; j < frequencies->size(); j++) {
        int centerPoint = frequencies->at(j).size() / 2;
        std::vector<std::complex<double>> centerElement;
        if ((frequencies->size() % 2) == 1) {
            centerElement = frequencies->at(j).at(centerPoint);
            frequencies->erase(frequencies->begin() + centerPoint);
        }
        for (int i = 0; i < centerPoint; i++) {
            std::vector<std::complex<double>> temp = frequencies->at(j).at(i);
            frequencies->at(j).at(i) = frequencies->at(j).at(centerPoint + i);
            frequencies->at(j).at(centerPoint + i) = temp;
        }
        if ((frequencies->size() % 2) == 1) {
            frequencies->at(j).push_back(centerElement);
        }
    }
}

void Fourier::singleRowForward(Image * image, std::vector<std::vector<std::vector<std::complex<double>>>> * resultArray, int row) {
    for (int u = 0; u < resultArray->at(row).size(); u++) {
        for (int j = 0; j < image->getHeight(); j++) {
            for (int i = 0; i < image->getWidth(); i++) {
                resultArray->at(row).at(u).at(0) += std::polar((double) image->getPixelAt(i, j).getRed(), (double) -2 * M_PI * (((double) u * i / image->getWidth()) + ((double) row * j / image->getHeight())));
                resultArray->at(row).at(u).at(1) += std::polar((double) image->getPixelAt(i, j).getGreen(), (double) -2 * M_PI * (((double) u * i / image->getWidth()) + ((double) row * j / image->getHeight())));
                resultArray->at(row).at(u).at(2) += std::polar((double) image->getPixelAt(i, j).getBlue(), (double) -2 * M_PI * (((double) u * i / image->getWidth()) + ((double) row * j / image->getHeight())));
            }
        }
        resultArray->at(row).at(u).at(0) /= (image->getHeight() * image->getWidth());
        resultArray->at(row).at(u).at(1) /= (image->getHeight() * image->getWidth());
        resultArray->at(row).at(u).at(2) /= (image->getHeight() * image->getWidth());
    }
    fourierMaxThread += 1;
    done += 1;
    percentage = ((double) done * 100 / image->getHeight());
    if ((percentage - prevPercentage) > 1) {
        spdlog::info("Fourier::forward: {}%", percentage);
        prevPercentage.store(percentage);
    }
}

void Fourier::singleRowInverse(Image * image, std::vector<std::vector<std::vector<std::complex<double>>>> * array, int row) {
    for (int u = 0; u < image->getWidth(); u++) {
        double redValue = 0;
        double greenValue = 0;
        double blueValue = 0;
        for (int j = 0; j < array->size(); j++) {
            for (int i = 0; i < array->at(j).size(); i++) {
                std::complex<double> expRedValue = std::polar((double) 1, 2 * M_PI * (((double) u * i / image->getWidth()) + ((double) row * j / image->getHeight())));
                std::complex<double> tempRed = array->at(j).at(i).at(0) * expRedValue;
                redValue += std::real(tempRed);
                std::complex<double> expGreenValue = std::polar((double) 1, 2 * M_PI * (((double) u * i / image->getWidth()) + ((double) row * j / image->getHeight())));
                std::complex<double> tempGreen = array->at(j).at(i).at(1) * expGreenValue;
                greenValue += std::real(tempGreen);
                std::complex<double> expBlueValue = std::polar((double) 1, 2 * M_PI * (((double) u * i / image->getWidth()) + ((double) row * j / image->getHeight())));
                std::complex<double> tempBlue = array->at(j).at(i).at(2) * expBlueValue;
                blueValue += std::real(tempBlue);
            }
        }
        image->setPixelAt(u, row, Pixel(redValue, greenValue, blueValue));
    }
    fourierMaxThread++;
    done++;
    percentage = (double) done * 100 / (double) image->getHeight();
    if ((percentage - prevPercentage) > 1) {
        spdlog::info("Fourier::inverse: {}%", percentage);
        prevPercentage.store(percentage);
    }
}
