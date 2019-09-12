#ifndef __PGMIMAGELOADER_HPP
#define __PGMIMAGELOADER_HPP

#include "ImageLoader.hpp"

class BMPImageLoader : ImageLoader {
  public:
    BMPImageLoader();
    Image load(std::string fileUrl);
};

#endif