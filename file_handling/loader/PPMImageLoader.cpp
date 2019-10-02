#include <fstream>
#include <iostream>
#include <string>
#include "PPMImageLoader.hpp"
#include "../../spdlog/spdlog.h"

#define MAX_COLOR 255
#define MAX_CHANNEL 3
#define PPM_ASCII "P3"
#define PPM_BINARY "P6"
#define RED_FLAG 0
#define GREEN_FLAG 1
#define BLUE_FLAG 2

PPMImageLoader::PPMImageLoader() {

}

Image * PPMImageLoader::load(std::string filename) {
    std::ifstream fileread(filename);
    std::string filetype;
    if (fileread.good()) {
        std::getline(fileread, filetype);
        while (filetype[0] == '#') {
            std::getline(fileread, filetype);
        }
        if (filetype == PPM_ASCII) {
            return this->loadASCII(filename);
        } else if (filetype == PPM_BINARY) {
            return this->loadBinary(filename);
        } else {
            spdlog::error("PPMImageLoader::load: {} is not a PPM file!", filename);
            throw std::runtime_error("PPMImageLoader::load: " + filename + " is not PPM file.");
        }
    } else {
        spdlog::error("PPMImageLoader::load: {} is unreadable!", filename);
        throw std::runtime_error("PPMImageLoader::load: " + filename + " unreadable.");
    }
}

Image * PPMImageLoader::loadASCII(std::string filename) {
    std::ifstream fileread(filename);
    std::string filetype;
    std::string dimensions;
    std::string level;
    int height = 0, width = 0, colorLevel = 0;
    if (fileread.good()) {
        std::getline(fileread, filetype);
        while (filetype[0] == '#') {
            std::getline(fileread, filetype);
        }
        if (filetype != PPM_ASCII) {
            spdlog::error("PPMImageLoader::loadASCII: {} is not a PPM file!", filename);
            throw std::runtime_error("PPMImageLoader::loadASCII: " + filename + " is not PPM file.");
        }
        std::getline(fileread, dimensions);
        while(dimensions[0] == '#') {
            std::getline(fileread, dimensions);
        }
        std::getline(fileread, level);
        while(level[0] == '#') {
            std::getline(fileread, level);
        }
        width = std::stoi(dimensions.substr(0, dimensions.find(' ')));
        height = std::stoi(dimensions.substr(dimensions.find(' ') + 1, dimensions.length()));
        colorLevel = std::stoi(level);
        Pixel **data = new Pixel *[height];
        for (int j = 0; j < height; j++) {
            data[j] = new Pixel[height];
        }
        unsigned char buff;
        int intBuff = 0;
        int i = 0, j = 0;
        int value = 0;
        int flag = 0;
        int colorFlag = 0;
        while(!fileread.eof() && i < height && j < width) {
            fileread >> std::noskipws >> buff;
            if (buff - '0' >= 0 && buff - '0' <= 9) {
                intBuff = (buff - '0') + 10*intBuff;
                flag = 1;
            } else if (buff = ' ' && flag) {
                value = intBuff * MAX_COLOR / colorLevel;
                if (colorFlag == RED_FLAG) {
                    data[i][j].setRed(value);
                } else if (colorFlag == GREEN_FLAG) {
                    data[i][j].setGreen(value);
                } else if (colorFlag == BLUE_FLAG) {
                    data[i][j].setBlue(value);
                }
                intBuff = 0;
                flag = 0;
                colorFlag++;
                if (colorFlag == MAX_CHANNEL) {
                    colorFlag = 0;
                    j++;
                }
                if (j == width) {
                    i++;
                    j = 0;
                }
            }
        }
        return new Image(width, height, data, "ppm");
    } else {
        spdlog::error("PPMImageLoader::loadASCII: {} is unreadable!", filename);
        throw std::runtime_error("PPMImageLoader::loadASCII: " + filename + " unreadable.");
    }
}

Image * PPMImageLoader::loadBinary(std::string filename) {
    std::ifstream fileread(filename, std::ios::binary);
    std::string filetype;
    std::string dimensions;
    std::string level;
    int height = 0, width = 0, colorLevel = 0;
    if (fileread.good()) {
        std::getline(fileread, filetype);
        while (filetype[0] == '#') {
            std::getline(fileread, filetype);
        }
        if (filetype != PPM_BINARY) {
            spdlog::error("PPMImageLoader::loadBinary: {} is not a PPM file!", filename);
            throw std::runtime_error("PPMImageLoader::loadBinary: " + filename + " is not PPM file.");
        }
        std::getline(fileread, dimensions);
        while(dimensions[0] == '#') {
            std::getline(fileread, dimensions);
        }
        std::getline(fileread, level);
        while(level[0] == '#') {
            std::getline(fileread, level);
        }
        width = std::stoi(dimensions.substr(0, dimensions.find(' ')));
        height = std::stoi(dimensions.substr(dimensions.find(' ') + 1, dimensions.length()));
        colorLevel = std::stoi(level);

        Pixel **data = new Pixel *[height];
        for (int j = 0; j < height; j++) {
            data[j] = new Pixel[height];
        }
        unsigned char *buff = new unsigned char[width * height * MAX_CHANNEL];
        fileread.read((char*) buff, sizeof(unsigned char) * width * height * MAX_CHANNEL);
        for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    data[i][j].setRed((unsigned char) buff[MAX_CHANNEL*(i*width + j)] * MAX_COLOR / colorLevel);
                    data[i][j].setGreen((unsigned char) buff[MAX_CHANNEL*(i*width + j) + 1] * MAX_COLOR / colorLevel);
                    data[i][j].setBlue((unsigned char) buff[MAX_CHANNEL*(i*width + j) + 2] * MAX_COLOR / colorLevel);
                }
            }
            fileread.close();
        return new Image(width, height, data, "ppm");
    } else {
        spdlog::error("PPMImageLoader::loadBinary: {} is unreadable!", filename);
        throw std::runtime_error("PPMImageLoader::loadBinary: " + filename + " unreadable.");
    }
}