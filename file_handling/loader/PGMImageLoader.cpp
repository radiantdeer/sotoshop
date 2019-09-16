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
        fileread.close();
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
            grayLevel = std::stoi(level);
            Pixel **data = new Pixel *[height];
            for (int j = 0; j < height; j++) {
                data[j] = new Pixel[width];
            }
            char buff;
            int intBuff = 0;
            int i = 0, j = 0;
            int flag = 0;
            int value = 0;
            while (!fileread.eof() && i < height && j < width) {
                fileread >> std::noskipws >> buff;
                if (buff - '0' >= 0 && buff - '0' <= 9) {
                    intBuff = (buff - '0') + 10*intBuff;
                    flag = 1;
                } else if (buff = ' ' && flag) {
                    value = intBuff * MAX_COLOR / grayLevel;
                    data[i][j].setRed(value);
                    data[i][j].setGreen(value);
                    data[i][j].setBlue(value);
                    intBuff = 0;
                    flag = 0;
                    j++;
                    if (j == width) {
                        i++;
                        j = 0;
                    }
                }
            }
            return new Image(width, height, data, "pgm");
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
            fileread.read((char*) buff, sizeof(char)*imageSize);
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