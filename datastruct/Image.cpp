#include "Image.hpp"

#include <iostream>
#include <cmath>
#include <vector>
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
Image * Image::and_op(Image B, int width, int height) {
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

// TODO
// TESTING AND DEBUG
Image * Image::or_op(Image B, int width, int height) {
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

// TODO
// TESTING AND DEBUG
Image * Image::not_op() {
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

// TODO
// TESTING AND DEBUG
Image * Image::translate(int dx, int dy) {
    for (int i = 0; i < this->getWidth(); i++) {
        for (int j = 0; j < this->getHeight(); j++) {
            Pixel a = this->getPixelAt(i, j);
            if ((i + dx) < this->getWidth() && (j + dy) < this->getHeight()) {
                // Translated if image still in image size range
                Pixel *b = new Pixel();
                b->setRed(a.getRed());
                b->setGreen(a.getGreen());
                b->setBlue(a.getBlue());
                this->setPixelAt(i + dx, j + dy, *b);
                delete b;
            }
            Pixel *c = new Pixel(0,0,0);
            this->setPixelAt(i, j, *c);
            delete c;
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
Image * Image::multiply(Image B) {
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
    for (int i = 0; i < opWidth; i++) {
        for (int j = 0; j < opHeight; i++) {
            Pixel a = this->getPixelAt(i, j);
            Pixel b = B.getPixelAt(i,j);
            Pixel * c = a * b;
            this->setPixelAt(i,j, *c);
            delete c;
        }
    }
    return this;
}

// TODO
// TESTING AND DEBUG
Image * Image::flipH() {
    Image * tempImage = new Image(*this);
    for (int i = 0; i < tempImage->getWidth(); i++) {
        for (int j = 0; j < tempImage->getHeight(); j++) {
            Pixel a = tempImage->getPixelAt(i,j);
            Pixel *b = new Pixel(a);
            this->setPixelAt(tempImage->getWidth() - i, j, *b);
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
            this->setPixelAt(i, tempImage->getHeight() - j, *b);
            delete b;
        }
    }
    delete tempImage;
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

// TODO
// TESTING AND DEBUG
Image * Image::operator*(Image B) {
    Image * C = new Image(*this);
    return C->multiply(B);
}

Image * Image::operator&(Image B) {
    Image * C = new Image(*this);
    return C->and_op(B, B.getWidth(), B.getHeight());
}

Image * Image::operator|(Image B) {
    Image * C = new Image(*this);
    return C->or_op(B, B.getWidth(), B.getHeight());
}

Image * Image::operator~() {
    Image * C = new Image(*this);
    return C->not_op();
}

// Returns array of color histogram
// Returns a matrix of 3 x 256 for colored images (PPM, BMP)
// Returns a mateix of 1 x 256 for grayscaled images (PBM, PGM, RAW)
std::vector<std::vector<int>> Image::histogram() {
    std::vector<std::vector<int>> hist;
    if (this->getOriginalFormat() == "bmp" || this->getOriginalFormat() == "ppm" ) {
        // color
        std::vector<int> red (256, 0);
        std::vector<int> green (256, 0);
        std::vector<int> blue (256, 0);
        for (int i = 0; i < this->getWidth(); i++) {
            for (int j = 0; j < this->getHeight(); j++) {
                red[this->getPixelAt(i,j).getRed()]++;
                green[this->getPixelAt(i,j).getGreen()]++;
                blue[this->getPixelAt(i,j).getBlue()]++;
            }
        }
        hist.push_back(red);
        hist.push_back(green);
        hist.push_back(blue);
    } else {
        // grayscale
        std::vector<int> gray (256, 0);
        for (int i = 0; i < this->getWidth(); i++) {
            for (int j = 0; j < this->getHeight(); j++) {
                gray[this->getPixelAt(i, j).getRed()]++;
            }
        }
        hist.push_back(gray);
    }
    return hist;
}

std::vector<std::vector<int>> Image::equalizedHistogram() {
    std::vector<std::vector<int>> hist = this->histogram();
    std::vector<std::vector<int>> histEq;
    int totalPixel = this->getHeight() * this->getWidth();
    if (this->getOriginalFormat() == "bmp" || this->getOriginalFormat() == "ppm" ) {
        std::vector<int> redMap = hist.at(0);
        std::vector<int> greenMap = hist.at(1);
        std::vector<int> blueMap = hist.at(2);
        for (int i = 1; i < COLOR_LEVEL; i++) {
            redMap[i] += redMap[i-1];
            greenMap[i] += greenMap[i-1];
            blueMap[i] += blueMap[i-1];
        }
        for (int i = 0; i < COLOR_LEVEL; i++) {
            redMap[i] = redMap[i] * (COLOR_LEVEL-1) / totalPixel;
            greenMap[i] = greenMap[i] * (COLOR_LEVEL-1) / totalPixel;
            blueMap[i] = blueMap[i] * (COLOR_LEVEL-1) / totalPixel;
        }
        histEq.push_back(redMap);
        histEq.push_back(greenMap);
        histEq.push_back(blueMap);
    } else {
        std::vector<int> grayMap = hist.at(0);
        for (int i = 1; i < COLOR_LEVEL; i++) {
            grayMap[i] += grayMap[i-1];
        }

        for (int i = 0; i < COLOR_LEVEL; i++) {
            grayMap[i] = grayMap[i] * (COLOR_LEVEL-1) / totalPixel;
        }
        histEq.push_back(grayMap);
    }
    return histEq;
}

Image * Image::histogramEqualization() {
    std::vector<std::vector<int>> hist = this->histogram();
    int totalPixel = this->getHeight() * this->getWidth();
    if (this->getOriginalFormat() == "bmp" || this->getOriginalFormat() == "ppm" ) {
        std::vector<int> redMap = hist.at(0);
        std::vector<int> greenMap = hist.at(1);
        std::vector<int> blueMap = hist.at(2);
        for (int i = 1; i < COLOR_LEVEL; i++) {
            redMap[i] += redMap[i-1];
            greenMap[i] += greenMap[i-1];
            blueMap[i] += blueMap[i-1];
        }
        for (int i = 0; i < COLOR_LEVEL; i++) {
            redMap[i] = redMap[i] * (COLOR_LEVEL-1) / totalPixel;
            greenMap[i] = greenMap[i] * (COLOR_LEVEL-1) / totalPixel;
            blueMap[i] = blueMap[i] * (COLOR_LEVEL-1) / totalPixel;
        }
        for (int i = 0; i < this->getWidth(); i++) {
            for (int j = 0; j < this->getHeight(); j++) {
                Pixel a = this->getPixelAt(i,j);
                Pixel *p = new Pixel(redMap[a.getRed()], greenMap[a.getGreen()], blueMap[a.getBlue()]);
                this->setPixelAt(i,j,*p);
                delete p;
            }
        }
    } else {
        std::vector<int> grayMap = hist.at(0);
        for (int i = 1; i < COLOR_LEVEL; i++) {
            grayMap[i] += grayMap[i-1];
        }

        for (int i = 0; i < COLOR_LEVEL; i++) {
            grayMap[i] = grayMap[i] * (COLOR_LEVEL-1) / totalPixel;
        }

        for (int i = 0; i < this->getWidth(); i++) {
            for (int j = 0; j < this->getHeight(); j++) {
                Pixel a = this->getPixelAt(i,j);
                Pixel *p = new Pixel(grayMap[a.getRed()], grayMap[a.getGreen()], grayMap[a.getBlue()]);
                this->setPixelAt(i,j,*p);
                delete p;
            }
        }
    }
    return this;
}

std::vector<int> histSpecHelper(std::vector<int> &a, std::vector<int> &b) {
    for (int i = 0; i < COLOR_LEVEL; i++) {
        int minval = abs(a[i] - b[0]);
        int minj = 0;
        for (int j = 0; j < COLOR_LEVEL; j++) {
            if (abs(a[i] - b[j]) < minval) {
                minval = abs(a[i] - b[j]);
                minj = j;
            }
        }
        a[i] = minj;
    }
    return a;
}

// Match histogram of this image into something like B image.
Image * Image::histogramSpecification(Image& B) {
    // Equalize this image first.
    std::vector<std::vector<int>> histThis = this->equalizedHistogram();
    std::vector<std::vector<int>> histOp = B.equalizedHistogram();
    std::vector<int> redMapThis;
    std::vector<int> greenMapThis;
    std::vector<int> blueMapThis;
    std::vector<int> redMapOp;
    std::vector<int> greenMapOp;
    std::vector<int> blueMapOp;

    if (this->getOriginalFormat() == "bmp" || this->getOriginalFormat() == "ppm" ) {
        redMapThis = histThis.at(0);
        greenMapThis = histThis.at(1);
        blueMapThis = histThis.at(2);
    } else {
        redMapThis = histThis.at(0);
        greenMapThis = histThis.at(0);
        blueMapThis = histThis.at(0);
    }

    if (B.getOriginalFormat() == "bmp" || B.getOriginalFormat() == "ppm") {
        redMapOp = histOp.at(0);
        greenMapOp = histOp.at(1);
        blueMapOp = histOp.at(2);
    } else {
        redMapOp = histOp.at(0);
        greenMapOp = histOp.at(0);
        blueMapOp = histOp.at(0);
    }

    redMapThis = histSpecHelper(redMapThis, redMapOp);
    greenMapThis = histSpecHelper(greenMapThis, greenMapOp);
    blueMapThis = histSpecHelper(blueMapThis, blueMapOp);

    for (int i = 0; i < this->getWidth(); i++) {
        for (int j = 0; j < this->getHeight(); j++) {
            Pixel a = this->getPixelAt(i,j);
            Pixel *p = new Pixel(redMapThis[a.getRed()], greenMapThis[a.getGreen()], blueMapThis[a.getBlue()]);
            this->setPixelAt(i,j,*p);
            delete p;
        }
    }
    return this;
}