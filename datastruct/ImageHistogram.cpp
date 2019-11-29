#include "ImageHistogram.hpp"
#include "Image.hpp"

ImageHistogram::ImageHistogram() {
    redData.resize(Image::MAX_GRAY + 1, 0);
    greenData.resize(Image::MAX_GRAY + 1, 0);
    blueData.resize(Image::MAX_GRAY + 1, 0);
    grayscale = true;
    recalculateMean = true;
    recalculateMedian = true;
    recalculateMode = true;
}

ImageHistogram::ImageHistogram(std::vector<std::vector<int>> data, bool grayscale) {
    this->grayscale = grayscale;
    redData.resize(Image::MAX_GRAY + 1);
    if (grayscale) {
        for (int i = 0; i < data[0].size(); i++) {
            redData[i] = data[0][i];
        }
    } else {
        greenData.resize(Image::MAX_GRAY + 1);
        blueData.resize(Image::MAX_GRAY + 1);
        for (int i = 0; i < (Image::MAX_GRAY + 1); i++) {
            redData[i] = data[0][i];
            greenData[i] = data[1][i];
            blueData[i] = data[2][i];
        }
    }
    recalculateMean = true;
    recalculateMedian = true;
    recalculateMode = true;
}

std::vector<int> ImageHistogram::getRed() const {
    return redData;
}

std::vector<int> ImageHistogram::getGreen() const {
    if (grayscale) {
        return redData;
    } else {
        return greenData;
    }
}

std::vector<int> ImageHistogram::getBlue() const {
    if (grayscale) {
        return redData;
    } else {
        return blueData;
    }
}

std::vector<int> ImageHistogram::getGray() const {
    return redData;
}


int ImageHistogram::getValueAt(int graylevel, char channel) const {
    if (grayscale) {
        return redData[graylevel];
    } else {
        if (channel == 'R') {
            return redData[graylevel];   
        } else if (channel == 'G') {
            return greenData[graylevel];
        } else if (channel == 'B') {
            return blueData[graylevel];
        } else {
            return redData[graylevel];
        }
    }
}

void ImageHistogram::setValueAt(int graylevel, int value, char channel) {
    if (grayscale) {
        redData[graylevel] = value;
    } else {
        if (channel == 'R') {
            redData[graylevel] = value;   
        } else if (channel == 'G') {
            greenData[graylevel] = value;
        } else if (channel == 'B') {
            blueData[graylevel] = value;
        } else { // channel == 'Y' or something else 
            redData[graylevel] = value;
            greenData[graylevel] = value;
            blueData[graylevel] = value;
        }
    }
    recalculateMean = true;
    recalculateMedian = true;
    recalculateMode = true;
}

void ImageHistogram::incrementValueAt(int graylevel, int incrementBy, char channel) {
    if (grayscale) {
        redData[graylevel] += incrementBy;
    } else {
        if (channel == 'R') {
            redData[graylevel] += incrementBy;   
        } else if (channel == 'G') {
            greenData[graylevel] += incrementBy;
        } else if (channel == 'B') {
            blueData[graylevel] += incrementBy;
        } else { // channel == 'Y' or something else
            redData[graylevel] += incrementBy;
            greenData[graylevel] += incrementBy;
            blueData[graylevel] += incrementBy;
        }
    }
    recalculateMean = true;
    recalculateMedian = true;
    recalculateMode = true;
}

bool ImageHistogram::isGrayscale() const {
    return grayscale;
}

void ImageHistogram::setGrayscale(bool grayscale) {
    this->grayscale = grayscale;
    if (!grayscale) {
        greenData.resize(Image::MAX_GRAY + 1);
        blueData.resize(Image::MAX_GRAY + 1);
    }
}

double ImageHistogram::mean(char channel) {
    if (recalculateMean) {
        calculateMean();
    }

    if (grayscale) {
        return meanRed;
    } else {
        if  (channel == 'G') {
            return meanGreen;
        } else if (channel == 'B') {
            return meanBlue;
        } else {
            return meanRed;
        }
    }
}

double ImageHistogram::median(char channel) {
    if (recalculateMedian) {
        calculateMedian();
    }

    if (grayscale) {
        return medianRed;
    } else {
        if (channel == 'G') {
            return medianGreen;
        } else if (channel == 'B') {
            return medianBlue;
        } else {
            return medianRed;
        }
    }
}

int ImageHistogram::mode(char channel) {
    if (recalculateMode) {
        calculateMode();
    }

    if (grayscale) {
        return modeRed;
    } else {
        if (channel == 'G') {
            return modeGreen;
        } else if (channel == 'B') {
            return modeBlue;
        } else {
            return modeRed;
        }
    }
}

void ImageHistogram::calculateMean() {
    meanRed = 0;
    meanGreen = 0;
    meanBlue = 0;
    for (int i = 0; i < redData.size(); i++) {
        meanRed += redData[i];
        if (!grayscale) {
            meanGreen += greenData[i];
            meanBlue += blueData[i];
        }
    }
    meanRed = meanRed / redData.size();
    meanGreen = meanGreen / redData.size();
    meanBlue = meanBlue / redData.size();
    recalculateMean = false;
}

void ImageHistogram::calculateMedian() {
    
    recalculateMedian = false;
}

void ImageHistogram::calculateMode() {

    recalculateMode = false;
}

