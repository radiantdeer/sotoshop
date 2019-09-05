#ifndef __RAWIMAGELOADER_HPP
#define __RAWIMAGELOADER_HPP

#include "ImageLoader.hpp"

class RawImageLoader : public ImageLoader {
    
    public:
        RawImageLoader();
        Image load(std::string fileUrl);

};

#endif