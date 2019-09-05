#include <iostream>
#include <fstream>
#include "PBMImageLoader.hpp"

#define PLAIN_PBM "P1"
#define BINARY_PBM "P4"

PBMImageLoader::PBMImageLoader() {

}

Image PBMImageLoader::load(std::string filename) {
    int height = 0, width = 0;
    std::string line;
    std::ifstream fileread(filename);
    if (!fileread.bad()) {
        std::getline(fileread, line);
        if (line == PLAIN_PBM) {
            std::cout << "Plain PBM\n";
        } else if (line == BINARY_PBM) {
            std::cout << "Binary PBM\n";
        } else {
            std::cout << "Not PBM\n";
        }
    }
    return Image();
}