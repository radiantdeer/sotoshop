#ifndef __PGMIMAGELOADER_HPP
#define __PGMIMAGELOADER_HPP

#include "ImageLoader.hpp"

class PGMImageLoader : ImageLoader {
  public:
    PGMImageLoader();
    Image load(std::string fileUrl);
};

#endif