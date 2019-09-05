#ifndef __RAWIMAGELOADER_HPP
#define __RAWIMAGELOADER_HPP

#include "ImageSaver.hpp"

class RawImageSaver : public ImageSaver {
    
    public:
        RawImageSaver();
        int save(const Image& image, std::string fileUrl);

};

#endif