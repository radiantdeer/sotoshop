#include "ImageLoaderFactory.hpp"
#include "PBMImageLoader.hpp"
#include "PPMImageLoader.hpp"
#include "PGMImageLoader.hpp"
#include "RawImageLoader.hpp"
#include <locale>
#include <iostream>
#include "../../utilities/Utilities.hpp"

ImageLoader * ImageLoaderFactory::getImageLoader(std::string filename) {
    using namespace std;
    string extension = Utilities::getFileExtension(filename);
    ImageLoader * imageLoader;

    if ((extension == "raw") || (extension == "RAW")) {
        cout << "RAW" << endl;
        imageLoader = new RawImageLoader();
    } else if ((extension == "pgm") || (extension == "PGM")) {
        cout << "PGM" << endl;
        imageLoader = new PGMImageLoader;
    } else if ((extension == "ppm") || (extension == "PPM")) {
        cout << "PPM" << endl;
        imageLoader = new PPMImageLoader();
    } else if ((extension == "pbm") || (extension == "PBM")) {
        cout << "PBM" << endl;
        imageLoader = new PBMImageLoader();
    } else if ((extension == "bmp") || (extension == "BMP")) {
        cout << "BMP" << endl;
        imageLoader = new RawImageLoader();
    } else {
        cout << "Unsupported" << endl;
        imageLoader = new RawImageLoader();
    }

    return imageLoader;
}