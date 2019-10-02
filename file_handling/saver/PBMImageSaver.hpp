#ifndef __PBMIMAGESAVER_HPP
#define __PBMIMAGESAVER_HPP

#include "ImageSaver.hpp"

class PBMImageSaver : public ImageSaver {

    public:
        PBMImageSaver();
        int save(const Image &image, std::string fileUrl);
};

#endif