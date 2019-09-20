#ifndef _PPMIMAGELOADER_HPP
#define _PPMIMAGELOADER_HPP

#include "ImageLoader.hpp"

class PPMImageLoader : public ImageLoader {
    public:
        PPMImageLoader();
        Image * load(std::string filename);

    private:
        Image * loadASCII(std::string filename);
        Image * loadBinary(std::string filename);
};

#endif