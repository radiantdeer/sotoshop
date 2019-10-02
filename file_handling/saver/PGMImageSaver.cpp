#include <iostream>
#include <fstream>
#include <string>
#include "PGMImageSaver.hpp"

#define MAGIC_NUMBER "P5";
#define EXT ".pgm";
#define MAX_COLOR "255";

PGMImageSaver::PGMImageSaver() {

}

int PGMImageSaver::save(const Image& image, std::string fileUrl) {
    std::string filename = fileUrl;
    std::ofstream outfile(filename, std::ios::binary);
    if (outfile.good()) {
        int height = image.getHeight();
        int width = image.getWidth();
        outfile << MAGIC_NUMBER; outfile << "\n";
        outfile << std::to_string(width) + " " + std::to_string(height) + '\n';
        outfile << MAX_COLOR; outfile << "\n";
        unsigned char *buff = new unsigned char[width*height*sizeof(unsigned char)];
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Pixel pixel = image.getPixelAt(j, i);
                buff[i*width + j] = pixel.getRed();
            }
        }
        outfile.write((char *)buff, width*height*sizeof(unsigned char));
        outfile.close();
        return 0;
    } else {
        std::runtime_error("Cannot write file");
        return -1;
    }
}