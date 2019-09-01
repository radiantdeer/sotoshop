#include "ImageLoaderFactory.hpp"
#include "RawImageLoader.hpp"
#include <locale>
#include <iostream>

ImageLoaderFactory::ImageLoaderFactory() {

}

ImageLoader * ImageLoaderFactory::getInstance(std::string filename) {
    using namespace std;
    string extension = filename.substr(filename.find(".") + 1, filename.length());
    ImageLoader * imageLoader;
    cout << extension << endl;

    if ((extension == "raw") || (extension == "RAW")) {
        cout << "RAW" << endl;
        imageLoader = new RawImageLoader();
    } else if ((extension == "pgm") || (extension == "PGM")) {
        cout << "PGM" << endl;
        imageLoader = new RawImageLoader();
    } else if ((extension == "ppm") || (extension == "PPM")) {
        cout << "PPM" << endl;
        imageLoader = new RawImageLoader();
    } else if ((extension == "pbm") || (extension == "PBM")) {
        cout << "PBM" << endl;
        imageLoader = new RawImageLoader();
    } else if ((extension == "bmp") || (extension == "BMP")) {
        cout << "BMP" << endl;
        imageLoader = new RawImageLoader();
    } else {
        cout << "Unsupported" << endl;
        imageLoader = new RawImageLoader();
    }
    
    return imageLoader;
}