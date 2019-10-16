#include "Image.hpp"

#include <iostream>
#include <cmath>
#include "../spdlog/spdlog.h"

Image::Image() {
    width = 0;
    height = 0;
    originalFormat = "raw";
}

Image::Image(int width, int height) {
    Pixel pixel(0, 0, 0);
    this->width = width;
    this->height = height;
    for (int j = 0; j < height; j++) {
        std::vector<Pixel> tempData;
        for (int i = 0; i < width; i++) {
            tempData.push_back(pixel);
        }
        this->data.push_back(tempData);
    }
    originalFormat = "raw";
}

Image::Image(int width, int height, std::string originalFormat) {
    Pixel pixel(0, 0, 0);
    this->width = width;
    this->height = height;
    for (int j = 0; j < height; j++) {
        std::vector<Pixel> tempData;
        for (int i = 0; i < width; i++) {
            tempData.push_back(pixel);
        }
        this->data.push_back(tempData);
    }
    this->originalFormat = originalFormat;
}

Image::Image(int width, int height, Pixel * data, std::string originalFormat) {
    this->width = width;
    this->height = height;
    for (int j = 0; j < height; j++) {
        std::vector<Pixel> tempData;
        for (int i = 0; i < width; i++) {
            tempData.push_back(data[(j * width) + i]);
        }
        this->data.push_back(tempData);
    }
    this->originalFormat = originalFormat;
}

Image::Image(int width, int height, Pixel ** data, std::string originalFormat) {
    this->width = width;
    this->height = height;
    for (int j = 0; j < height; j++) {
        std::vector<Pixel> tempData;
        for (int i = 0; i < width; i++) {
            tempData.push_back(data[j][i]);
        }
        this->data.push_back(tempData);
    }
    this->originalFormat = originalFormat;
}

Image::Image(const Image& other) {
    this->width = other.getWidth();
    this->height = other.getHeight();
    for (int j = 0; j < height; j++) {
        std::vector<Pixel> tempData;
        for (int i = 0; i < width; i++) {
            tempData.push_back(other.getPixelAt(i,j));
        }
        this->data.push_back(tempData);
    }
    this->originalFormat = originalFormat;
}


Pixel Image::getPixelAt(int x, int y) const {
    return data[y][x];
}

int Image::getWidth() const {
    return width;
}

int Image::getHeight() const {
    return height;
}

std::vector<std::vector<Pixel>> Image::getPixelData() const {
    return data;
}

std::string Image::getOriginalFormat() const {
    return originalFormat;
}

std::string Image::getFileUrl() const {
    return fileUrl;
}

void Image::setPixelAt(int x, int y, Pixel pixel) {
    data[y][x] = pixel;
}

void Image::setPixelAt(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    data[y][x].setRed(r);
    data[y][x].setGreen(g);
    data[y][x].setBlue(b);
}

void Image::setWidth(int width) {
    this->width = width;
}

void Image::setHeight(int height) {
    this->height = height;
}

void Image::setOriginalFormat(std::string originalFormat) {
    this->originalFormat = originalFormat;
}

void Image::setFileUrl(std::string fileUrl) {
    this->fileUrl = fileUrl;
}

// TODO
// DEBUG METHOD
Image * Image::add(Image& B) {
    int opWidth = this->getWidth() > B.getWidth() ? B.getWidth() : this->getWidth();
    int opHeight = this->getHeight() > B.getHeight() ? B.getHeight() : this->getHeight();
    for (int j = 0; j < opWidth; j++) {
        for (int i = 0; i < opHeight; i++) {
            Pixel a = this->getPixelAt(i, j);
            Pixel b = B.getPixelAt(i, j);
            Pixel *pixel = a + b;

            this->setPixelAt(i, j, *pixel);
            delete pixel;
        }
    }

    return this;
}

// TODO
// DEBUG METHOD
Image * Image::adjustBrightness(unsigned char delta) {
    for (int i = 0; i < this->getWidth(); i++) {
        for (int j = 0; j < this->getHeight(); j++) {
            Pixel a = this->getPixelAt(i, j);
            Pixel *pixel = a + delta;

            this->setPixelAt(i, j, *pixel);
            delete pixel;
        }
    }
    return this;
}

// TODO
// DEBUG METHOD
Image * Image::substract(Image& B) {
    int opWidth = this->getWidth() > B.getWidth() ? B.getWidth() : this->getWidth();
    int opHeight = this->getHeight() > B.getHeight() ? B.getHeight() : this->getHeight();
    for (int j = 0; j < opHeight; j++) {
        for (int i = 0; i < opWidth; i++) {
            Pixel a = this->getPixelAt(i, j);
            Pixel b = B.getPixelAt(i, j);
            Pixel *pixel = a - b;

            this->setPixelAt(i, j, *pixel);
            delete pixel;
        }
    }

    return this;
}

// TODO
// TESTING AND DEBUG
Image * Image::multiply(Image& B) {
    int opWidth, opHeight;
    if (B.getWidth() < this->getWidth()) {
        opWidth = B.getWidth();
    } else {
        opWidth = this->getWidth();
    }
    if (B.getHeight() < this->getHeight()) {
        opHeight = B.getHeight();
    } else {
        opHeight = this->getHeight();
    }
    for (int j = 0; j < opHeight; j++) {
        for (int i = 0; i < opWidth; i++) {
            Pixel a = this->getPixelAt(i, j);
            Pixel b = B.getPixelAt(i,j);
            Pixel * c = a * b;
            this->setPixelAt(i, j, *c);
            delete c;
        }
    }
    return this;
}

Image * Image::invert() {
    for (int i = 0; i < this->getWidth(); i++) {
        for (int j = 0; j < this->getHeight(); j++) {
            Pixel a = this->getPixelAt(i, j);
            Pixel * pixel = new Pixel();

            pixel->setRed(MAX_GRAY - a.getRed());
            pixel->setBlue(MAX_GRAY - a.getBlue());
            pixel->setGreen(MAX_GRAY - a.getGreen());

            this->setPixelAt(i, j, *pixel);
            delete pixel;
        }
    }
    return this;
}

// TODO
// TESTING AND DEBUG
Image * Image::grayscale() {
    for (int i = 0; i < this->getWidth(); i++) {
        for (int j = 0; j < this->getHeight(); j++) {
            Pixel oldPixel = this->getPixelAt(i, j);
            Pixel * newPixel = new Pixel();

            int grayValue = (0.299f * oldPixel.getRed()) + (0.587f * oldPixel.getGreen()) + (0.144f * oldPixel.getBlue());
            newPixel->setRed(grayValue);
            newPixel->setBlue(grayValue);
            newPixel->setGreen(grayValue);

            this->setPixelAt(i,j, *newPixel);
            delete newPixel;
        }
    }
    return this;
}

// TODO
// TESTING AND DEBUG
Image * Image::and_op(Image& B) {
    int opWidth = this->getWidth() > B.getWidth() ? B.getWidth() : this->getWidth();
    int opHeight = this->getHeight() > B.getHeight() ? B.getHeight() : this->getHeight();
    for (int j = 0; j < opHeight; j++) {
        for (int i = 0; i < opWidth; i++) {
            Pixel a = this->getPixelAt(i, j);
            Pixel b = B.getPixelAt(i, j);
            Pixel *pixel = a & b;
            this->setPixelAt(i, j, *pixel);
            delete pixel;
        }
    }
    return this;
}

// TODO
// TESTING AND DEBUG
Image * Image::or_op(Image& B) {
    int opWidth = this->getWidth() > B.getWidth() ? B.getWidth() : this->getWidth();
    int opHeight = this->getHeight() > B.getHeight() ? B.getHeight() : this->getHeight();
    for (int j = 0; j < opHeight; j++) {
        for (int i = 0; i < opWidth; i++) {
            Pixel a = this->getPixelAt(i, j);
            Pixel b = B.getPixelAt(i, j);
            Pixel *pixel = a | b;

            this->setPixelAt(i, j, *pixel);
            delete pixel;
        }
    }
    return this;
}

// TODO
// TESTING AND DEBUG
Image * Image::not_op() {
    for (int i = 0; i < this->getWidth(); i++) {
        for (int j = 0; j < this->getHeight(); j++) {
            Pixel a = this->getPixelAt(i, j);
            Pixel *pixel = ~a;
            this->setPixelAt(i, j, *pixel);
            delete pixel;
        }
    }
    return this;
}

// TODO
// TESTING AND DEBUG
Image * Image::translate(int dx, int dy) {
    for (int i = this->getWidth() - 1; i >= 0; i--) {
        for (int j = this->getHeight() - 1; j >= 0; j--) {
            if (((i - dx) >= 0) && ((j - dy) >= 0)) {
                Pixel a = this->getPixelAt(i - dx, j - dy);
                Pixel *b = new Pixel(a);
                this->setPixelAt(i, j, *b);
                delete b;
            } else {
                this->setPixelAt(i, j, Pixel(0, 0, 0));
            }
        }
    }
    return this;
}

// TODO
// TESTING AND DEBUG
Image * Image::rotate90CW(){
    Image *newImage = new Image(this->getHeight(), this->getWidth());
    // Insert new Image
    for (int i = 0; i < this->getWidth(); i++) {
        int k = this->getHeight() - 1;
        for (int j = 0; j < this->getHeight(); j++) {
            Pixel a = this->getPixelAt(i,j);
            Pixel *b = new Pixel();
            b->setRed(a.getRed());
            b->setGreen(a.getGreen());
            b->setBlue(a.getBlue());
            newImage->setPixelAt(k, i, *b);
            k--;
            delete b;
        }
    }
    return newImage;
}

// TODO
// TESTING AND DEBUG
Image * Image::rotate90CCW(){
    Image *newImage = new Image(this->getHeight(), this->getWidth());
    int k = this->getWidth() - 1;
    for (int i = 0; i < this->getWidth(); i++) {
        for (int j = 0; j < this->getHeight(); j++) {
            Pixel a = this->getPixelAt(i,j);
            Pixel *b = new Pixel();
            b->setRed(a.getRed());
            b->setGreen(a.getGreen());
            b->setBlue(a.getBlue());
            newImage->setPixelAt(j, k, *b);
            delete b;
        }
        k--;
    }
    return newImage;
}

// TODO
// TESTING AND DEBUG
Image * Image::flipH() {
    Image * tempImage = new Image(*this);
    for (int i = 0; i < tempImage->getWidth(); i++) {
        for (int j = 0; j < tempImage->getHeight(); j++) {
            Pixel a = tempImage->getPixelAt(i,j);
            Pixel *b = new Pixel(a);
            this->setPixelAt(tempImage->getWidth() - i - 1, j, *b);
            delete b;
        }
    }
    delete tempImage;
    return this;
}

// TODO
// TESTING AND DEBUG
Image * Image::flipV() {
    Image * tempImage = new Image(*this);
    for (int i = 0; i < tempImage->getWidth(); i++) {
        for (int j = 0; j < tempImage-> getHeight(); j++) {
            Pixel a = tempImage->getPixelAt(i, j);
            Pixel *b = new Pixel(a);
            this->setPixelAt(i, tempImage->getHeight() - j - 1, *b);
            delete b;
        }
    }
    delete tempImage;
    return this;
}

Image * Image::operator+(Image B) {
    Image * C = new Image(*this);
    return C->add(B);
}

Image * Image::operator+(unsigned char deltaBrightness) {
    Image * C = new Image(*this);
    return C->adjustBrightness(deltaBrightness);
}

Image * Image::operator-(Image B) {
    Image * C = new Image(*this);
    return C->substract(B);
}

Image * Image::operator-(unsigned char deltaBrightness) {
    Image * C = new Image(*this);
    return C->adjustBrightness(0 - deltaBrightness);
}

// TODO
// TESTING AND DEBUG
Image * Image::operator*(Image B) {
    Image * C = new Image(*this);
    return C->multiply(B);
}

Image * Image::operator&(Image B) {
    Image * C = new Image(*this);
    return C->and_op(B);
}

Image * Image::operator|(Image B) {
    Image * C = new Image(*this);
    return C->or_op(B);
}

Image * Image::operator~() {
    Image * C = new Image(*this);
    return C->not_op();
}