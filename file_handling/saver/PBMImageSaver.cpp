#include <iostream>
#include <fstream>
#include <string>
#include "PBMImageSaver.hpp"

#define MAGIC_NUMBER "P4";
#define EXT ".pbm";


PBMImageSaver::PBMImageSaver() {

}

int PBMImageSaver::save(const Image &image, std::string fileUrl) {
    std::string filename = fileUrl;
    std::ofstream outfile(filename);
    if (outfile.good()) {
        int height = image.getHeight();
        int width = image.getWidth();
        outfile << MAGIC_NUMBER; outfile << "\n";
        outfile << std::to_string(width) + " " + std::to_string(height) + '\n';
        int data = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int value = (image.getPixelAt(j,i).getRed() + image.getPixelAt(j,i).getGreen() + image.getPixelAt(j,i).getBlue()) / 3;
                if (value < 127) {
                    data = 0;
                } else {
                    data = 255;
                }
                char buff = (char) data;
                outfile.write(&buff, sizeof(char));
            }
        }
        outfile.close();
        return 0;
    } else {
        throw std::runtime_error("Cannot write file");
    }
}