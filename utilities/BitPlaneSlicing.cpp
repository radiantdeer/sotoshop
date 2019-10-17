#include "BitPlaneSlicing.hpp"
#include "../spdlog/spdlog.h"

// This will assume image is grayscale.
// This algorithm will only read data from red values only.
std::vector<Image> BitPlaneSlicing::generate(Image * image) {
    std::vector<Image> result (8);

    spdlog::debug("BitPlaneSlicing::generate: Generating more images...");
    Image current (image->getWidth(), image->getHeight());
    current.setOriginalFormat("pbm");
    for (int i = 0; i < 8; i++) {
        result[i] = current;
    }

    spdlog::debug("BitPlaneSlicing::generate: Result size : {}", result.size());

    for (int j = 0; j < image->getHeight(); j++) {
        for (int i = 0; i < image->getWidth(); i++) {
            unsigned char value = image->getPixelAt(i, j).getRed();
            unsigned char mask = 0x1;
            for (int k = 0; k < 8; k++) {
                unsigned char thisValue = (value & mask) >> k;
                if (thisValue == 0x1) {
                    thisValue = 255;
                } else {
                    thisValue = 0;
                }
                result.at(k).setPixelAt(i, j, Pixel(thisValue, thisValue, thisValue));
                mask <<= 1;
            }
        }
    }

    return result;
}