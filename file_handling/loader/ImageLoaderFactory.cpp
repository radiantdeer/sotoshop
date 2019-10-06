#include "ImageLoaderFactory.hpp"
#include "PBMImageLoader.hpp"
#include "PPMImageLoader.hpp"
#include "PGMImageLoader.hpp"
#include "BMPImageLoader.hpp"
#include "RawImageLoader.hpp"
#include <locale>
#include "../../spdlog/spdlog.h"
#include "../../utilities/Utilities.hpp"

ImageLoader * ImageLoaderFactory::getImageLoader(std::string filename) {
    using namespace std;
    string extension = Utilities::getFileExtension(filename);
    ImageLoader * imageLoader;

    if ((extension == "raw") || (extension == "RAW")) {
        spdlog::debug("ImageLoaderFactory detects raw image");
        imageLoader = new RawImageLoader();
    } else if ((extension == "pgm") || (extension == "PGM")) {
        spdlog::debug("ImageLoaderFactory detects PGM image");
        imageLoader = new PGMImageLoader();
    } else if ((extension == "ppm") || (extension == "PPM")) {
        spdlog::debug("ImageLoaderFactory detects PPM image");
        imageLoader = new PPMImageLoader();
    } else if ((extension == "pbm") || (extension == "PBM")) {
        spdlog::debug("ImageLoaderFactory detects PBM image");
        imageLoader = new PBMImageLoader();
    } else if ((extension == "bmp") || (extension == "BMP")) {
        spdlog::debug("ImageLoaderFactory detects BMP image");
        imageLoader = new BMPImageLoader();
    } else {
        spdlog::warn("ImageLoaderFactory receives an unsupported image format!");
        imageLoader = new RawImageLoader();
    }

    return imageLoader;
}
