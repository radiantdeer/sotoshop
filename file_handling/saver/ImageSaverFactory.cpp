#include "ImageSaverFactory.hpp"
#include "PBMImageSaver.hpp"
#include "PGMImageSaver.hpp"
#include "PPMImageSaver.hpp"
#include "RawImageSaver.hpp"
#include <iostream>
#include "../../utilities/Utilities.hpp"

ImageSaver * ImageSaverFactory::getImageSaver(std::string fileUrl) {
    ImageSaver * imageSaver;

    std::string format = Utilities::getFileExtension(fileUrl);
    if ((format == "raw") || (format == "RAW")) {
        std::cout << "RAW" << std::endl;
        imageSaver = new RawImageSaver();
    } else if ((format == "pgm") || (format == "PGM")) {
        std::cout << "PGM" << std::endl;
        imageSaver = new PGMImageSaver();
    } else if ((format == "ppm") || (format == "PPM")) {
        std::cout << "PPM" << std::endl;
        imageSaver = new PPMImageSaver();
    } else if ((format == "pbm") || (format == "PBM")) {
        std::cout << "PBM" << std::endl;
        imageSaver = new PBMImageSaver();
    } else if ((format == "bmp") || (format == "BMP")) {
        std::cout << "BMP" << std::endl;
        imageSaver = new RawImageSaver();
    } else {
        std::cout << "Unsupported" << std::endl;
        imageSaver = new RawImageSaver();
    }

    return imageSaver;
}