#ifndef _PBMIMAGELOADER_HPP
#define _PBMIMAGELOADER_HPP

#include "ImageLoader.hpp"

class PBMImageLoader : public ImageLoader {
    public:
        PBMImageLoader();
        Image * load(std::string filename);

    private:
        Image * loadASCII(std::string filename);
        Image * loadBinary(std::string filename);
};

#endif