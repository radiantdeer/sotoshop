#include "RawImageLoader.hpp"
#include <fstream>
#include <iostream>
#include <string>

RawImageLoader::RawImageLoader() { }

Image * RawImageLoader::load(std::string fileUrl) {
    std::ifstream fileIn(fileUrl);
    if (fileIn.good()) {
        std::string firstLine;
        std::getline(fileIn, firstLine);
        int width = std::stoi(firstLine.substr(0, firstLine.find(" ")));
        int height = std::stoi(firstLine.substr(firstLine.find(" ") + 1, firstLine.length()));

        Pixel * data = new Pixel[width * height];
        std::string tempBuffer;
        for (int i = 0; i < height; i++) {
            std::getline(fileIn, tempBuffer);
            for (int j = 0; j < width - 1; j++) {
                std::string currentPixelStr = tempBuffer.substr(0, tempBuffer.find(" "));
                tempBuffer = tempBuffer.substr(tempBuffer.find(" ") + 1, tempBuffer.length());
                unsigned char currentPixel = (unsigned char) std::stoi(currentPixelStr);
                data[i * width + j].setRed(currentPixel);
                data[i * width + j].setGreen(currentPixel);
                data[i * width + j].setBlue(currentPixel);
            }
            unsigned char currentPixel = (unsigned char) std::stoi(tempBuffer);
            data[i * width - 1].setRed(currentPixel);
            data[i * width - 1].setGreen(currentPixel);
            data[i * width - 1].setBlue(currentPixel);
        }
        return new Image(width, height, data, "raw");

    } else {
        std::cout << "RawImageLoader::load: Cannot open file!" << std::endl;
        return new Image();
    }
}