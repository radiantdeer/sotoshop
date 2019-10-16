#include "Pixel.hpp"
#include <sstream>

int Pixel::thresholding(int value) {
    if (value > 255) {
        return 255;
    } else if (value < 0) {
        return 0;
    } else {
        return value;
    }
}

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

unsigned char Pixel::grayscaleValue() const {
    return (0.299f * getRed()) + (0.587f * getGreen()) + (0.144f * getBlue());
}

unsigned char Pixel::meanGrayscaleValue() const {
    return ((int) getRed() + (int) getGreen() + (int) getBlue()) / 3;
}

void Pixel::makeGrayscale() {
    unsigned char grayValue = grayscaleValue();
    this->setRed(grayValue);
    this->setBlue(grayValue);
    this->setGreen(grayValue);
}

void Pixel::makeMeanGrayscale() {
    unsigned char grayValue = meanGrayscaleValue();
    this->setRed(grayValue);
    this->setBlue(grayValue);
    this->setGreen(grayValue);
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

bool Pixel::operator==(Pixel& B) {
    return (this->getRed() == B.getRed()) && (this->getGreen() == B.getGreen()) && (this->getBlue() == B.getBlue());
}

bool Pixel::operator>(Pixel& B) {
    return (this->meanGrayscaleValue() > B.meanGrayscaleValue());
}

bool Pixel::operator>=(Pixel& B) {
    return (this->meanGrayscaleValue() >= B.meanGrayscaleValue());
}

bool Pixel::operator<(Pixel& B) {
    return (this->meanGrayscaleValue() < B.meanGrayscaleValue());
}

bool Pixel::operator<=(Pixel& B) {
    return (this->meanGrayscaleValue() <= B.meanGrayscaleValue());
}

