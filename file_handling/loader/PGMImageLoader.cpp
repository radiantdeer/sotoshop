#include <iostream>
#include <fstream>
#include <string>
#include "PGMImageLoader.hpp"

#define ASCII_PGM "P2"
#define BINARY_PGM "P5"

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
    int height, width, grayLevel;
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
            level = std::stoi(level);
            std::cout << "Image dimensions: ", width, "x", height, "\n";
            std::cout << "Gray level      : ", level;
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
            height = std::stoi(dimensions.substr(level.find(' ') + 1, dimensions.length()));
            level = std::stoi(level);
            std::cout << "Image dimensions: ", width, "x", height, "\n";
            std::cout << "Gray level      : ", level;
        } else {
            throw std::runtime_error("Not PGM File");
        }
    } else {
        throw std::runtime_error("Unreadable");
    }
}