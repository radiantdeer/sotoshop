#ifndef __PGMIMAGELOADER_HPP
#define __PGMIMAGELOADER_HPP

#include "ImageLoader.hpp"

class BMPImageLoader : public ImageLoader {
  public:
    BMPImageLoader();
    Image * load(std::string fileUrl);
    Image * loadBM(std::string fileUrl);

    static int buffertoInteger(char* buffer, int start, int size);

};

#endif