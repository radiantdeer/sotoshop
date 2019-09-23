#include <iostream>
#include <fstream>
#include <string>
#include "PBMImageLoader.hpp"

#define ASCII_PBM "P1"
#define BINARY_PBM "P4"

Image * loadASCII(std::string filename);
Image * loadBinary(std::string filename);

PBMImageLoader::PBMImageLoader() {

}

Image * PBMImageLoader::load(std::string filename) {
    std::string line;
    std::ifstream fileread(filename);
    if (fileread.good()) {
        std::getline(fileread, line);
        if (line == ASCII_PBM) {
            return loadASCII(filename);
        } else if (line == BINARY_PBM) {
            return loadBinary(filename);
        } else {
            throw std::runtime_error("PBMImageLoader::load: " + filename + " is not a pbm file.");
        }
    }
    throw std::runtime_error("PBMImageLoader::load: " + filename + " is unreadable.");
}

Image * PBMImageLoader::loadASCII(std::string filename) {
    int height = 0, width = 0;
    std::string line;
    std::ifstream fileread(filename);
    if (fileread.good()) {
        std::getline(fileread, line);
        if (line != ASCII_PBM) {
            throw std::runtime_error("PBMImageLoader::loadASCII: " + filename + " is not a pbm file.");
        }
        std::getline(fileread, line);
        while (line[0] == '#') {
            std::getline(fileread, line);
        }

        width = std::stoi(line.substr(0, line.find(" ")));
        height = std::stoi(line.substr(line.find(" ") + 1, line.length()));
        Pixel **data = new Pixel*[height];
        for (int j = 0; j < height; j++) {
            data[j] = new Pixel[width];
        }

        for (int i = 0; i < height; i++) {
            std::getline(fileread, line);
            for (int j = 0; j < width; j++) {
                if (line[2*j] == '0') {
                    data[i][j].setRed(0);
                    data[i][j].setGreen(0);
                    data[i][j].setBlue(0);
                } else if (line[2*j] == '1') {
                    data[i][j].setRed(255);
                    data[i][j].setGreen(255);
                    data[i][j].setBlue(255);
                } else {
                    throw std::runtime_error("PBMImageLoader::loadASCII: there are invalid token at " + filename);
                }
            }
        }
        fileread.close();
        return new Image(width, height, data, "pbm");
    }
    throw std::runtime_error("PBMImageLoader::loadASCII: " + filename + " is unreadable.");
}

Image * PBMImageLoader::loadBinary(std::string filename) {
    int height = 0, width = 0;
    std::ifstream fileread(filename);
    std::string line;
    std::streampos begin, end;
    unsigned char* buff;
    if (fileread.good()) {
        std::getline(fileread, line);
        if (line != BINARY_PBM) {
            throw std::runtime_error("PBMImageLoader::loadASCII: " + filename + " is not a pbm file.");
        }
        std::getline(fileread, line);
        while (line[0] == '#') {
            std::getline(fileread, line);
        }
        width = std::stoi(line.substr(0, line.find(" ")));
        height = std::stoi(line.substr(line.find(" ") + 1, line.length()));
        Pixel **data = new Pixel*[height];
        for (int j = 0; j < height; j++) {
            data[j] = new Pixel[width];
        }
        int imageSize = height*width;
        buff = new unsigned char[height*width];
        fileread.read((char*) buff, sizeof(char)*height*width);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                data[i][j].setRed((unsigned char) buff[i*width + j]);
                data[i][j].setGreen((unsigned char) buff[i*width + j]);
                data[i][j].setBlue((unsigned char) buff[i*width + j]);
            }
        }
        fileread.close();
        return new Image(width, height, data, "pbm");
    } else {
        throw std::runtime_error("PBMImageLoader::loadBinary: " + filename + " is unreadable.");
    }
}