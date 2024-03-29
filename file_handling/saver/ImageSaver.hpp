#ifndef __IMAGESAVER_HPP
#define __IMAGESAVER_HPP

#include <string>
#include "../../datastruct/Image.hpp"

class ImageSaver {

    public:
        virtual int save(const Image& image, std::string fileUrl) = 0;

};

#endif