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
    this->width = other.getWidth();
    this->height = other.getHeight();
    for (int j = 0; j < height; j++) {
        std::vector<Pixel> tempData;
        for (int i = 0; i < width; i++) {
            tempData.push_back(data[j][i]);
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

Image * Image::grayscale() {
    for (int i = 0; i < this->getWidth(); i++) {
        for (int j = 0; j < this->getHeight(); j++) {
            Pixel newPixel = this->getPixelAt(i, j);
            newPixel.makeGrayscale();
            this->setPixelAt(i,j, newPixel);
        }
    }
    return this;
}

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

Image * Image::magnify2() {
    Image * result = new Image(this->getWidth() * 2, this->getHeight() * 2);
    for (int j = 0; j < this->getHeight(); j++) {
        for (int i = 0; i < this->getWidth(); i++) {
            Pixel currentPixel = this->getPixelAt(i, j);
            result->setPixelAt(i * 2, j * 2, currentPixel);
            result->setPixelAt((i * 2) + 1, j * 2, currentPixel);
            result->setPixelAt(i * 2, (j * 2) + 1, currentPixel);
            result->setPixelAt((i * 2) + 1, (j * 2) + 1, currentPixel);
        }
    }
    return result;
}

Image * Image::shrink2() {
    Image * result = new Image((this->getWidth() / 2), (this->getHeight() / 2));
    for (int j = 0; j < this->getHeight(); j+=2) {
        for (int i = 0; i < this->getWidth(); i+=2) {
            int numPixel = 1;
            int red = this->getPixelAt(i, j).getRed();
            int green = this->getPixelAt(i, j).getGreen();
            int blue = this->getPixelAt(i, j).getBlue();
            if ((i + 1) < this->getWidth()) {
                red += this->getPixelAt(i + 1, j).getRed();
                green += this->getPixelAt(i + 1, j).getGreen();
                blue += this->getPixelAt(i + 1, j).getBlue();
                numPixel++;
            }
            if ((j + 1) < this->getHeight()) {
                red += this->getPixelAt(i, j+1).getRed();
                green += this->getPixelAt(i, j+1).getGreen();
                blue += this->getPixelAt(i, j+1).getBlue();
                numPixel++;
            }
            if (numPixel >= 3) { // this means that (i+1, j+1) is not out of bounds
                red += this->getPixelAt(i+1, j+1).getRed();
                green += this->getPixelAt(i+1, j+1).getGreen();
                blue += this->getPixelAt(i+1, j+1).getBlue();
                numPixel++;
            }
            red /= numPixel;
            green /= numPixel;
            blue /= numPixel;
            result->setPixelAt(i / 2, j / 2, Pixel((unsigned char)red, (unsigned char)green, (unsigned char)blue));
        }
    }
    return result;
}

Image * Image::operator+(Image& B) {
    Image * C = new Image(*this);
    return C->add(B);
}

Image * Image::operator+(unsigned char deltaBrightness) {
    Image * C = new Image(*this);
    return C->adjustBrightness(deltaBrightness);
}

Image * Image::operator-(Image& B) {
    Image * C = new Image(*this);
    return C->substract(B);
}

Image * Image::operator-(unsigned char deltaBrightness) {
    Image * C = new Image(*this);
    return C->adjustBrightness(0 - deltaBrightness);
}

Image * Image::operator*(Image& B) {
    Image * C = new Image(*this);
    return C->multiply(B);
}

Image * Image::operator&(Image& B) {
    Image * C = new Image(*this);
    return C->and_op(B);
}

Image * Image::operator|(Image& B) {
    Image * C = new Image(*this);
    return C->or_op(B);
}

Image * Image::operator~() {
    Image * C = new Image(*this);
    return C->not_op();
}

// Returns array of color histogram
// Returns a matrix of 3 x 256 for colored images (PPM, BMP)
// Returns a matrix of 1 x 256 for grayscaled images (PBM, PGM, RAW)
std::vector<std::vector<int>> Image::histogram() {
    std::vector<std::vector<int>> hist;
    if (this->getOriginalFormat() == "bmp" || this->getOriginalFormat() == "ppm") {
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

Image * Image::grayLevelSlicing(int a, int b, int val) {
    if (this->getOriginalFormat() == "bmp" || this->getOriginalFormat() == "ppm") {
        return this;
    } else {
        if (val > COLOR_LEVEL) {
            val = 255;
        }
        if (b < a) {
            return this;
        }
        for (int i=0; i < this->getWidth(); i++) {
            for (int j=0; j < this->getHeight(); j++) {
                if (this->getPixelAt(i,j).getRed() >= a && this->getPixelAt(i,j).getRed() <= b) {
                    Pixel *p = new Pixel(val, val, val);
                    this->setPixelAt(i,j,*p);
                    delete p;
                }
            }
        }
        return this;
    }
}

Image * Image::contrastStretch() {
    if ((this->getWidth() > 0) && (this->getHeight())) {
        spdlog::debug("Image::contrastStretch: Determining rmin and rmax...");
        int rrmin = this->getPixelAt(0, 0).getRed();
        int rrmax = this->getPixelAt(0, 0).getRed();
        int rgmin = this->getPixelAt(0, 0).getGreen();
        int rgmax = this->getPixelAt(0, 0).getGreen();
        int rbmin = this->getPixelAt(0, 0).getBlue();
        int rbmax = this->getPixelAt(0, 0).getBlue();

        for (int j = 0; j < this->getHeight(); j++) {
            for (int i = 0; i < this->getWidth(); i++) {
                if (rrmin > this->getPixelAt(i, j).getRed()) {
                    rrmin = this->getPixelAt(i, j).getRed();
                }
                if (rrmax < this->getPixelAt(i, j).getRed()) {
                    rrmax = this->getPixelAt(i, j).getRed();
                }
                if (rgmin > this->getPixelAt(i, j).getGreen()) {
                    rgmin = this->getPixelAt(i, j).getGreen();
                }
                if (rgmax < this->getPixelAt(i, j).getGreen()) {
                    rgmax = this->getPixelAt(i, j).getGreen();
                }
                if (rbmin > this->getPixelAt(i, j).getBlue()) {
                    rbmin = this->getPixelAt(i, j).getBlue();
                }
                if (rbmax < this->getPixelAt(i, j).getBlue()) {
                    rbmax = this->getPixelAt(i, j).getBlue();
                }
            }
        }
        spdlog::debug("Image::contrastStretch: Determined these parameters : ");
        spdlog::debug("redmin : {} | redmax : {} | greenmin : {} | greenmax : {} | bluemin : {} | bluemax : {}", rrmin, rrmax, rgmin, rgmax, rbmin, rbmax);
        contrastStretch(rrmin, rrmax, rgmin, rgmax, rbmin, rbmax);
    }
    return this;
}

Image * Image::contrastStretch(int rrmin, int rrmax, int rgmin, int rgmax, int rbmin, int rbmax) {
    for (int j = 0; j < this->getHeight(); j++) {
        for (int i = 0; i < this->getWidth(); i++) {
            int redVal = this->getPixelAt(i, j).getRed();
            int greenVal = this->getPixelAt(i, j).getGreen();
            int blueVal = this->getPixelAt(i, j).getBlue();
            float adjustedRedVal = (float) (redVal - rrmin) / (float) (rrmax - rrmin);
            float adjustedGreenVal = (float) (greenVal - rgmin) / (float) (rgmax - rgmin);
            float adjustedBlueVal = (float) (blueVal - rbmin) / (float) (rbmax - rbmin);
            adjustedRedVal *= MAX_GRAY;
            adjustedGreenVal *= MAX_GRAY;
            adjustedBlueVal *= MAX_GRAY;
            redVal = (int) adjustedRedVal;
            greenVal = (int) adjustedGreenVal;
            blueVal = (int) adjustedBlueVal;
            this->setPixelAt(i, j, Pixel(redVal, greenVal, blueVal));
        }
    }
    return this;
}