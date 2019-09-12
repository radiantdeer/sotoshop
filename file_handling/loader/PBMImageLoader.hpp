#ifndef _PBMIMAGELOADER_HPP
#define _PBMIMAGELOADER_HPP

#include "ImageLoader.hpp"

class PBMImageLoader : public ImageLoader {
    public:
        PBMImageLoader();
        Image load(std::string filename);
};

#endif