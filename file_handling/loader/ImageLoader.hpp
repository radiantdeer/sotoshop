#ifndef __IMAGELOADER_HPP
#define __IMAGELOADER_HPP

#include <string>
#include "../../datastruct/Image.hpp"

class ImageLoader {

    public:
        virtual Image * load(std::string fileUrl) = 0;

};

#endif