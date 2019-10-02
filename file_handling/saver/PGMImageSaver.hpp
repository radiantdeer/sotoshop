#ifndef __PGMIMAGESAVER_HPP
#define __PGMIMAGESAVER_HPP

#include "ImageSaver.hpp"

class PGMImageSaver : public ImageSaver {

    public:
        PGMImageSaver();
        int save(const Image& image, std::string fileUrl);

};

#endif