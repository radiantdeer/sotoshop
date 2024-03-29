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
            tempData.push_back(other.getPixelAt(i, j));
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
    for (int j = 0; j < opHeight; j++) {
        for (int i = 0; i < opWidth; i++) {
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

Image * Image::multiply(double alpha) {
    for (int j = 0; j < this->getHeight(); j++) {
        for (int i = 0; i < this->getWidth(); i++) {
            Pixel a = this->getPixelAt(i, j);
            Pixel * c = a * alpha;
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
    Image *newImage = new Image(this->getWidth(), this->getHeight());
    for (int i = 0; i < this->getWidth(); i++) {
        for (int j = 0; j < this->getHeight(); j++) {
            if (((i + dx) >= 0) && ((i + dx) < this->getWidth()) && ((j + dy) >= 0) && ((j + dy) < this->getHeight())) {
                Pixel *a = new Pixel(this->getPixelAt(i, j));
                newImage->setPixelAt(i + dx, j + dy, *a);
                delete a;
            }
        }
    }
    return newImage;
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

Image * Image::nthPower(double n, double c) {
    for (int j = 0; j < this->getHeight(); j++) {
        for (int i = 0; i < this->getWidth(); i++) {
            // s = cr^n
            Pixel a = this->getPixelAt(i,j);
            int redVal   = 255*c*pow(double(a.getRed())  /(COLOR_LEVEL-1), n);
            int greenVal = 255*c*pow(double(a.getGreen())/(COLOR_LEVEL-1), n);
            int blueVal  = 255*c*pow(double(a.getBlue()) /(COLOR_LEVEL-1), n);
            Pixel *p = new Pixel(redVal, greenVal, blueVal);
            this->setPixelAt(i, j, *p);
            delete p;
        }
    }
    return this;
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

Image * Image::operator*(double alpha) {
    Image * C = new Image(*this);
    return C->multiply(alpha);
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
ImageHistogram Image::histogram() {
    ImageHistogram hist;
    if (this->getOriginalFormat() == "bmp" || this->getOriginalFormat() == "ppm") {
        // color
        hist.setGrayscale(false);
        for (int i = 0; i < this->getWidth(); i++) {
            for (int j = 0; j < this->getHeight(); j++) {
                hist.incrementValueAt(this->getPixelAt(i,j).getRed(), 1, 'R');
                hist.incrementValueAt(this->getPixelAt(i,j).getGreen(), 1, 'G');
                hist.incrementValueAt(this->getPixelAt(i,j).getBlue(), 1, 'B');
            }
        }
        spdlog::debug("Means : ");
        spdlog::debug("  Red : {}", hist.mean('R'));
        spdlog::debug("  Green : {}", hist.mean('G'));
        spdlog::debug("  Blue : {}", hist.mean('B'));
    } else {
        // grayscale
        hist.setGrayscale(true);
        for (int i = 0; i < this->getWidth(); i++) {
            for (int j = 0; j < this->getHeight(); j++) {
                hist.incrementValueAt(this->getPixelAt(i, j).getRed());
            }
        }
        spdlog::debug("Mean (Gray) : {}", hist.mean('R'));
    }
    return hist;
}

ImageHistogram Image::equalizedHistogram() {
    ImageHistogram hist = this->histogram();
    std::vector<std::vector<int>> temp;
    ImageHistogram *histEq;
    int totalPixel = this->getHeight() * this->getWidth();
    if (this->getOriginalFormat() == "bmp" || this->getOriginalFormat() == "ppm") {
        temp.push_back(hist.getRed());
        temp.push_back(hist.getGreen());
        temp.push_back(hist.getBlue());
        histEq = new ImageHistogram(temp, false);
        for (int i = 1; i < COLOR_LEVEL; i++) {
            histEq->incrementValueAt(i, histEq->getValueAt(i-1, 'R'), 'R');
            histEq->incrementValueAt(i, histEq->getValueAt(i-1, 'G'), 'G');
            histEq->incrementValueAt(i, histEq->getValueAt(i-1, 'B'), 'B');
        }
        for (int i = 0; i < COLOR_LEVEL; i++) {
            histEq->setValueAt(i, histEq->getValueAt(i, 'R') * (COLOR_LEVEL-1) / totalPixel, 'R');
            histEq->setValueAt(i, histEq->getValueAt(i, 'G') * (COLOR_LEVEL-1) / totalPixel, 'G');
            histEq->setValueAt(i, histEq->getValueAt(i, 'R') * (COLOR_LEVEL-1) / totalPixel, 'B');
        }
    } else {
        temp.push_back(hist.getRed());
        histEq = new ImageHistogram(temp, true);
        for (int i = 1; i < COLOR_LEVEL; i++) {
            histEq->incrementValueAt(i, histEq->getValueAt(i-1));
        }

        for (int i = 0; i < COLOR_LEVEL; i++) {
            histEq->setValueAt(i, histEq->getValueAt(i) * (COLOR_LEVEL-1) / totalPixel);
        }
    }

    return *histEq;
}

Image * Image::histogramEqualization() {
    ImageHistogram hist = this->histogram();
    int totalPixel = this->getHeight() * this->getWidth();
    if (this->getOriginalFormat() == "bmp" || this->getOriginalFormat() == "ppm" ) {
        std::vector<int> redMap = hist.getRed();
        std::vector<int> greenMap = hist.getGreen();
        std::vector<int> blueMap = hist.getBlue();
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
        std::vector<int> grayMap = hist.getGray();
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
    for (int i = 0; i < Image::COLOR_LEVEL; i++) {
        int minval = abs(a[i] - b[0]);
        int minj = 0;
        for (int j = 0; j < Image::COLOR_LEVEL; j++) {
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
    ImageHistogram histThis = this->equalizedHistogram();
    ImageHistogram histOp = B.equalizedHistogram();
    std::vector<int> redMapThis;
    std::vector<int> greenMapThis;
    std::vector<int> blueMapThis;
    std::vector<int> redMapOp;
    std::vector<int> greenMapOp;
    std::vector<int> blueMapOp;

    if (this->getOriginalFormat() == "bmp" || this->getOriginalFormat() == "ppm" ) {
        redMapThis = histThis.getRed();
        greenMapThis = histThis.getGreen();
        blueMapThis = histThis.getBlue();
    } else {
        redMapThis = histThis.getGray();
        greenMapThis = histThis.getGray();
        blueMapThis = histThis.getGray();
    }

    if (B.getOriginalFormat() == "bmp" || B.getOriginalFormat() == "ppm") {
        redMapOp = histOp.getRed();
        greenMapOp = histOp.getGreen();
        blueMapOp = histOp.getBlue();
    } else {
        redMapOp = histOp.getGray();
        greenMapOp = histOp.getGray();
        blueMapOp = histOp.getGray();
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

Image * Image::logTrans() {
    const double c = double(COLOR_LEVEL - 1) / log10(COLOR_LEVEL);
    for(int j = 0; j < this->getHeight(); j++) {
        for(int i = 0; i < this->getWidth(); i++) {
            // s = c * log(1 + r);
            Pixel a = this->getPixelAt(i,j);
            int redVal = c*log10(a.getRed() + 1);
            int greenVal = c*log10(a.getGreen() + 1);
            int blueVal = c*log10(a.getBlue() + 1);
            Pixel * p = new Pixel(redVal,greenVal,blueVal);
            this->setPixelAt(i,j,*p);
        }
    }
    return this;
}

Image * Image::invLogTrans() {
    const double BASE = 1.02;
    const double c = double(COLOR_LEVEL - 1) / (pow(BASE, COLOR_LEVEL) - 1);
    for(int j = 0; j < this->getHeight(); j++) {
        for(int i = 0; i < this->getWidth(); i++) {
            // s = c * (e^r -1)
            Pixel a = this->getPixelAt(i,j);
            int redVal = c*(pow(BASE, a.getRed()) - 1);
            int greenVal = c*(pow(BASE, a.getGreen()) - 1);
            int blueVal = c*(pow(BASE, a.getBlue()) - 1);
            Pixel * p = new Pixel(redVal,greenVal,blueVal);
            this->setPixelAt(i,j,*p);
        }
    }
    return this;
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
            redVal = Pixel::thresholding((int) adjustedRedVal);
            greenVal = Pixel::thresholding((int) adjustedGreenVal);
            blueVal = Pixel::thresholding((int) adjustedBlueVal);
            this->setPixelAt(i, j, Pixel(redVal, greenVal, blueVal));
        }
    }
    return this;
}

Image * Image::binarySegmentation() {
    this->grayscale();
    int threshold = this->MAX_GRAY / 2;
    int newThreshold = threshold;
    std::vector<int> gBright;
    std::vector<int> gDarker;
    while (abs(newThreshold - threshold) > 20) {
        threshold = newThreshold;
        for (int i = 0; i < this->getWidth(); i++) {
            for (int j = 0; j < this->getHeight(); j++) {
                Pixel p = this->getPixelAt(i, j);
                if (p.getRed() > threshold) {
                    gBright.push_back(p.getRed());
                } else {
                    gDarker.push_back(p.getRed());
                }
            }
        }
        int avgDarker = 0;
        int avgBright = 0;
        int tD = 0;
        int tB = 0;
        if (gDarker.size() > 0) {
            for (int i = 0; i < gDarker.size(); i++) {
                tD += gDarker.at(i);
            }
            avgDarker = tD / gDarker.size();
        }
        if (gBright.size() > 0) {
            for (int i = 0; i < gBright.size(); i++) {
                tB += gBright.at(i);
            }
            avgBright = tB / gBright.size();
        }
        newThreshold = (int)((avgBright + avgDarker)/2);
    }
    threshold = newThreshold;
    for (int i = 0; i < this->getWidth(); i++) {
        for (int j = 0; j < this->getHeight(); j++) {
            Pixel p = this->getPixelAt(i, j);
            if (p.getRed() > threshold) {
                this->setPixelAt(i, j, Pixel(255, 255, 255));
            } else {
                this->setPixelAt(i, j, Pixel(0, 0, 0));
            }
        }
    }
    return this;
}

Image * Image::binaryThinning() {
    this->binarySegmentation();
    int y[9] = {0};
    int ok;
    do {
        ok = 1;
        for (int i = 1; i < this->getWidth()-1; i++) {
            for (int j = 1; j < this->getHeight()-1; j++) {
                if (this->getPixelAt(i,j).getRed() == 255) {
                    int count = 0;
                    for (int k = -1; k <= 1; k++) {
                        for (int l = -1; l <= 1; l++) {
                            if (this->getPixelAt(i + k, j + l).getRed() == 255) count++;
                        }
                    }
                    if ((count > 2) && (count < 8)) {
                        y[0] = this->getPixelAt(i-1, j-1).getRed();
                        y[1] = this->getPixelAt(i  , j-1).getRed();
                        y[2] = this->getPixelAt(i+1, j-1).getRed();
                        y[3] = this->getPixelAt(i+1, j  ).getRed();
                        y[4] = this->getPixelAt(i+1, j+1).getRed();
                        y[5] = this->getPixelAt(i  , j+1).getRed();
                        y[6] = this->getPixelAt(i-1, j+1).getRed();
                        y[7] = this->getPixelAt(i-1, j  ).getRed();
                        y[8] = this->getPixelAt(i-1, j-1).getRed();
                        int trans = 0;
                        for (int m = 0; m <= 7; m++) {
                            if (y[m] == 0 && y[m+1] == 255) trans++;
                            if (trans == 1) {
                                this->setPixelAt(i, j, Pixel(0,0,0));
                            }
                        }
                    }
                }
            }
        }
    } while (ok == 0);
    return this;
}