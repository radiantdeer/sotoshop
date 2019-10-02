#ifndef __PPMIMAGESAVER_HPP
#define __PPMIMAGESAVER_HPP

#include "ImageSaver.hpp"

class PPMImageSaver : public ImageSaver {

    public:
        PPMImageSaver();
        int save(const Image& image, std::string);
};

#endif