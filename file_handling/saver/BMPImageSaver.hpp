#ifndef BMPIMAGESAVER_HPP
#define BMPIMAGESAVER_HPP

#include "ImageSaver.hpp"

class BMPImageSaver : public ImageSaver {
    public:
        BMPImageSaver();
        int save(const Image& image, std::string fileUrl);
};

#endif // BMPIMAGESAVER_HPP
