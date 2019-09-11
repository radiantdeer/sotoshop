#include "Pixel.hpp"
#include <sstream>

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
    this->red = red;
}

void Pixel::setGreen(unsigned char green) {
    this->green = green;
}

void Pixel::setBlue(unsigned char blue) {
    this->blue = blue;
}

std::string Pixel::toString() const {
    std::ostringstream stream;
    stream << "(" << (int) red << "," << (int) green << "," << (int) blue << ")";
    return stream.str(); 
}