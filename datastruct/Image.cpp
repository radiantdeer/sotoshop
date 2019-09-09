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

