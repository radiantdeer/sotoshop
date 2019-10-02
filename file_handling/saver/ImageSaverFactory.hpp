#ifndef __IMAGESAVERFACTORY_HPP
#define __IMAGESAVERFACTORY_HPP

#include <string>
#include "ImageSaver.hpp"

class ImageSaverFactory {

    public:
        static ImageSaver * getImageSaver(std::string fileUrl);

};

#endif