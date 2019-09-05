#include "Image.hpp"

Image::Image() {
    width = 0;
    height = 0;
    originalFormat = "raw";
}

Image::Image(int width, int height, Pixel * data, std::string originalFormat) {
    this->width = width;
    this->height = height;
    this->data.reserve(height);
    for (int j = 0; j < height; j++) {
        this->data[j].reserve(width);
        for (int i = 0; i < width; i++) {
            this->data[j][i] = data[(j * width) + i];
        }
    }
    this->originalFormat = originalFormat;
}

Image::Image(int width, int height, Pixel ** data, std::string originalFormat) {
    this->width = width;
    this->height = height;
    this->data.reserve(height);
    for (int j = 0; j < height; j++) {
        this->data[j].reserve(width);
        for (int i = 0; i < width; i++) {
            this->data[j][i] = data[j][i];
        }
    }
    this->originalFormat = originalFormat;
}

Image::Image(const Image& other) {
    this->width = width;
    this->height = height;
    this->data.reserve(height);
    for (int j = 0; j < height; j++) {
        this->data[j].reserve(width);
        for (int i = 0; i < width; i++) {
            this->data[j][i] = data[j][i];
        }
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

