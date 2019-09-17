#ifndef _PGMIMAGELOADER_HPP
#define _PGMIMAGELOADER_HPP

#include "ImageLoader.hpp"

class PGMImageLoader : public ImageLoader {
    public:
        PGMImageLoader();
        Image * load(std::string filename);

    private:
        Image * loadASCII(std::string filename);
        Image * loadBinary(std::string filename);
};

#endif