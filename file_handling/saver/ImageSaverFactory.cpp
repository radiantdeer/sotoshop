#include "ImageSaverFactory.hpp"
#include "PBMImageSaver.hpp"
#include "PGMImageSaver.hpp"
#include "PPMImageSaver.hpp"
#include "BMPImageSaver.hpp"
#include "RawImageSaver.hpp"
#include <iostream>
#include "../../spdlog/spdlog.h"
#include "../../utilities/Utilities.hpp"

ImageSaver * ImageSaverFactory::getImageSaver(std::string fileUrl) {
    ImageSaver * imageSaver;

    std::string format = Utilities::getFileExtension(fileUrl);
    if ((format == "raw") || (format == "RAW")) {
        spdlog::debug("ImageSaverFactory detects raw image");
        imageSaver = new RawImageSaver();
    } else if ((format == "pgm") || (format == "PGM")) {
        spdlog::debug("ImageSaverFactory detects PGM image");
        imageSaver = new PGMImageSaver();
    } else if ((format == "ppm") || (format == "PPM")) {
        spdlog::debug("ImageSaverFactory detects PPM image");
        imageSaver = new PPMImageSaver();
    } else if ((format == "pbm") || (format == "PBM")) {
        spdlog::debug("ImageSaverFactory detects PBM image");
        imageSaver = new PBMImageSaver();
    } else if ((format == "bmp") || (format == "BMP")) {
        spdlog::debug("ImageSaverFactory detects BMP image");
        imageSaver = new BMPImageSaver();
    } else {
        spdlog::warn("ImageSaverFactory receives an unsupported image!");
        imageSaver = new RawImageSaver();
    }

    return imageSaver;
}
