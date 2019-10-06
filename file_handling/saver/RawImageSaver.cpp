#include <fstream>
#include <iostream>
#include "RawImageSaver.hpp"
#include "../../spdlog/spdlog.h"

RawImageSaver::RawImageSaver() {

}

int RawImageSaver::save(const Image& image, std::string fileUrl) {
    std::ofstream fileOut (fileUrl);
    if (fileOut.good()) {
        int width = image.getWidth();
        int height = image.getHeight();
        fileOut << width << " " << height << std::endl;
        for (int i = 0; i < height; i++) {
            Pixel temp = image.getPixelAt(0, i);
            fileOut << (unsigned int) temp.getRed();
            for (int j = 1; j < width; j++) {
                Pixel temp = image.getPixelAt(j, i);
                fileOut << " " << (unsigned int) temp.getRed();
            }
            fileOut << std::endl;
        }
        fileOut.close();
        return 0;
    } else {
        spdlog::error("RawImageSaver::save: Failed saving raw image to file!");
        return 1;
    }
}
