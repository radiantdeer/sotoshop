#include "ImageSaverFactory.hpp"
#include "RawImageSaver.hpp"

ImageSaver * ImageSaverFactory::getImageSaver(std::string format) {
    ImageSaver * imageSaver;

    if ((format == "raw") || (format == "RAW")) {
        imageSaver = new RawImageSaver();
    } else if ((format == "pgm") || (format == "PGM")) {
        imageSaver = new RawImageSaver();
    } else if ((format == "ppm") || (format == "PPM")) {
        imageSaver = new RawImageSaver();
    } else if ((format == "pbm") || (format == "PBM")) {
        imageSaver = new RawImageSaver();
    } else if ((format == "bmp") || (format == "BMP")) {
        imageSaver = new RawImageSaver();
    } else {
        imageSaver = new RawImageSaver();
    }
    
    return imageSaver;
}