#include "Pixel.hpp"
#include <sstream>
#include "../spdlog/spdlog.h"

int thresholding(int value);

Pixel::Pixel() {
    red = 0;
    green = 0;
    blue = 0;
}

Pixel::Pixel(unsigned char red, unsigned char green, unsigned char blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;
}

Pixel::Pixel(const Pixel& pixel) {
    this->red = pixel.getRed();
    this->green = pixel.getGreen();
    this->blue = pixel.getBlue();
}


unsigned char Pixel::getRed() const {
    return red;
}

unsigned char Pixel::getGreen() const {
    return green;
}

unsigned char Pixel::getBlue() const {
    return blue;
}


void Pixel::setRed(unsigned char red) {
    unsigned char val = (unsigned char) thresholding(red);
    if (val != red) {
        spdlog::warn("Pixel::setRed: Something is trying to set red value outside valid range. It has been readjusted.");
    }
    this->red = val;
}

void Pixel::setGreen(unsigned char green) {
    unsigned char val = (unsigned char) thresholding(green);
    if (val != green) {
        spdlog::warn("Pixel::setGreen: Something is trying to set green value outside valid range. It has been readjusted.");
    }
    this->green = val;
}

void Pixel::setBlue(unsigned char blue) {
    unsigned char val = (unsigned char) thresholding(blue);
    if (val != blue) {
        spdlog::warn("Pixel::setBlue: Something is trying to set blue value outside valid range. It has been readjusted.");
    }
    this->blue = val;
}

std::string Pixel::toString() const {
    std::ostringstream stream;
    stream << "(" << (int) red << "," << (int) green << "," << (int) blue << ")";
    return stream.str();
}

Pixel * Pixel::operator*(Pixel B) {
    int valueRed = thresholding(this->getRed() * B.getRed());
    int valueGreen = thresholding(this->getGreen() * B.getGreen());
    int valueBlue = thresholding(this->getBlue() * B.getBlue());
    Pixel * C = new Pixel(valueRed, valueGreen, valueBlue);
    return C;
}

int thresholding(int value) {
    if (value > 255) {
        return 255;
    } else if (value < 0) {
        return 0;
    } else {
        return value;
    }
}