#include "Image.hpp"

#include <iostream>

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
    this->width = width;
    this->height = height;
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
Image * Image::add(Image B, int width, int height) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Pixel a = this->getPixelAt(i, j);
            Pixel b = B.getPixelAt(i, j);
            Pixel *pixel = new Pixel();

            pixel->setRed(a.getRed() + b.getRed());
            pixel->setBlue(a.getBlue() + b.getBlue());
            pixel->setGreen(a.getGreen() + b.getGreen());

            this->setPixelAt(width, height, *pixel);
            delete pixel;
        }
    }

    return this;
}

// TODO
// DEBUG METHOD
Image * Image::substract(Image B, int width, int height) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Pixel a = this->getPixelAt(i, j);
            Pixel b = B.getPixelAt(i, j);
            Pixel *pixel = new Pixel();

            pixel->setRed(a.getRed() - b.getRed());
            pixel->setBlue(a.getBlue() - b.getBlue());
            pixel->setGreen(a.getGreen() - b.getGreen());

            this->setPixelAt(width, height, *pixel);
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
            Pixel *pixel = new Pixel();

            pixel->setRed(a.getRed() + delta);
            pixel->setBlue(a.getBlue() + delta);
            pixel->setGreen(a.getGreen() + delta);

            this->setPixelAt(i, j, *pixel);
            delete pixel;
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

Image * Image::and(Image B, int width, int height) {
    for (int i = 0; i < this->getWidth(); i++) {
        for (int j = 0; j < this->getHeight(); j++) {
            Pixel a = this->getPixelAt(i, j);
            Pixel b = B.getPixelAt(i, j);
            Pixel *pixel = new Pixel();

            pixel->setRed(a.getRed() & b.getRed());
            pixel->setGreen(a.getGreen() & b.getGreen());
            pixel->setBlue(a.getBlue() & b.getBlue());

            this->setPixelAt(i, j, *pixel);
            delete pixel;
        }
    }
    return this;
}

Image * Image::or(Image B, int width, int height) {
    for (int i = 0; i < this->getWidth(); i++) {
        for (int j = 0; j < this->getHeight(); j++) {
            Pixel a = this->getPixelAt(i, j);
            Pixel b = B.getPixelAt(i, j);
            Pixel *pixel = new Pixel();

            pixel->setRed(a.getRed() | b.getRed());
            pixel->setGreen(a.getGreen() | b.getGreen());
            pixel->setBlue(a.getBlue() | b.getBlue());

            this->setPixelAt(i, j, *pixel);
            delete pixel;
        }
    }
    return this;
}

Image * Image::not() {
    for (int i = 0; i < this->getWidth(); i++) {
        for (int j = 0; j < this->getHeight(); j++) {
            Pixel a = this->getPixelAt(i, j);
            Pixel *pixel = new Pixel();

            pixel->setRed(~a.getRed());
            pixel->setGreen(~a.getGreen());
            pixel->setBlue(~a.getBlue());

            this->setPixelAt(i, j, *pixel);
            delete pixel;
        }
    }
    return this;
}


Image * Image::operator+(Image B) {
    Image * C = new Image(*this);
    return C->add(B, B.getWidth(), B.getHeight());
}

Image * Image::operator+(unsigned char deltaBrightness) {
    Image * C = new Image(*this);
    return C->adjustBrightness(deltaBrightness);
}

Image * Image::operator-(Image B) {
    Image * C = new Image(*this);
    return C->substract(B, B.getWidth(), B.getHeight());
}

Image * Image::operator-(unsigned char deltaBrightness) {
    Image * C = new Image(*this);
    return C->adjustBrightness(0 - deltaBrightness);
}

Image * Image::operator&(Image B) {
    Image * C = new Image(*this);
    return C->and(B, B.getWidth(), B.getHeight());
}

Image * Image::operator|(Image B) {
    Image * C = new Image(*this);
    return C->or(B, B.getWidth(), B.getHeight());
}

Image * Image::operator~() {
    Image * C = new Image(*this);
    return C->not();
}