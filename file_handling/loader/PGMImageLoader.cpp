#include <iostream>
#include <fstream>
#include <string>
#include "PGMImageLoader.hpp"

#define ASCII_PGM "P2"
#define BINARY_PGM "P5"
#define MAX_COLOR 255

Image * loadASCII(std::string filename);
Image * loadBinary(std::string filename);

PGMImageLoader::PGMImageLoader() {

}

Image * PGMImageLoader::load(std::string filename) {
    std::string fileType;
    std::ifstream fileread(filename);
    if (fileread.good()) {
        std::getline(fileread, fileType);
        while (fileType[0] == '#') {
            std::getline(fileread, fileType);
        }
        if (fileType == ASCII_PGM) {
            return loadASCII(filename);
        } else if (fileType == BINARY_PGM) {
            return loadBinary(filename);
        } else {
            throw std::runtime_error("Not PGM");
        }
    } else {
        throw std::runtime_error("Unreadable");
    }
}

Image * loadASCII(std::string filename) {
    std::string fileType;
    std::string dimensions;
    std::string level;
    int height = 0, width = 0, grayLevel = 0;
    std::ifstream fileread(filename);
    if (fileread.good()) {
        std::getline(fileread, fileType);
        while (fileType[0] == '#') {
            std::getline(fileread, fileType);
        }
        fileread.close();
        if (fileType == ASCII_PGM) {
            std::getline(fileread, dimensions);
            while (dimensions[0] == '#') {
                std::getline(fileread, dimensions);
            }
            std::getline(fileread, level);
            while (level[0] == '#') {
                std::getline(fileread, level);
            }
            width = std::stoi(dimensions.substr(0, dimensions.find(' ')));
            height = std::stoi(dimensions.substr(level.find(' ') + 1, dimensions.length()));
            level = std::stoi(level);
            return new Image(width, height);
        } else {
            throw std::runtime_error("Not PGM File");
        }
    } else {
        throw std::runtime_error("Unreadable");
    }
}

Image * loadBinary(std::string filename) {
    std::string fileType;
    std::string dimensions;
    std::string level;
    int height, width, grayLevel;
    unsigned char *buff;
    std::ifstream fileread(filename);
    if (fileread.good()) {
        std::getline(fileread, fileType);
        while (fileType[0] == '#') {
            std::getline(fileread, fileType);
        }
        if (fileType == BINARY_PGM) {
            std::getline(fileread, dimensions);
            while (dimensions[0] == '#') {
                std::getline(fileread, dimensions);
            }
            std::getline(fileread, level);
            while (level[0] == '#') {
                std::getline(fileread, level);
            }
            width = std::stoi(dimensions.substr(0, dimensions.find(' ')));
            height = std::stoi(dimensions.substr(dimensions.find(' ') + 1, dimensions.length()));
            grayLevel = std::stoi(level);
            Pixel **data = new Pixel*[height];
            for (int j = 0; j < height; j++) {
                data[j] = new Pixel[width];
            }
            int imageSize = height*width;
            buff = new unsigned char [imageSize];
            fileread.read((char*) buff, sizeof(char)*height*width);
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    int value = ((int) buff[i*width + j] * MAX_COLOR / grayLevel);
                    data[i][j].setRed(value);
                    data[i][j].setGreen(value);
                    data[i][j].setBlue(value);
                }
            }
            fileread.close();
            return new Image(width, height, data, "pgm");
        } else {
            throw std::runtime_error("Not PGM File");
        }
    } else {
        throw std::runtime_error("Unreadable");
    }
}