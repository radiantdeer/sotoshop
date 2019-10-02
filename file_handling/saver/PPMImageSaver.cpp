#include <iostream>
#include <fstream>
#include <string>
#include "PPMImageSaver.hpp"

#define MAGIC_NUMBER "P6";
#define EXT ".ppm";
#define MAX_COLOR "255";
#define MAX_CHANNEL 3

PPMImageSaver::PPMImageSaver() {

}

int PPMImageSaver::save(const Image& image, std::string fileUrl) {
    std::string filename = fileUrl;
    std::ofstream outfile(filename, std::ios::binary);
    if (outfile.good()) {
        int height = image.getHeight();
        int width = image.getWidth();
        outfile << MAGIC_NUMBER; outfile << "\n";
        outfile << std::to_string(width) + " " + std::to_string(height) + '\n';
        outfile << MAX_COLOR; outfile << "\n";
        unsigned char *buff = new unsigned char[width*height*MAX_CHANNEL*sizeof(unsigned char)];
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Pixel pixel = image.getPixelAt(j, i);
                buff[MAX_CHANNEL*(i*width + j)] = pixel.getRed();
                buff[MAX_CHANNEL*(i*width + j) + 1] = pixel.getGreen();
                buff[MAX_CHANNEL*(i*width + j) + 2] = pixel.getBlue();
            }
        }
        outfile.write((char *)buff, width*height*MAX_CHANNEL*sizeof(unsigned char));
        outfile.close();
        return 0;
    } else {
        std::runtime_error("Cannot write file");
        return -1;
    }
}