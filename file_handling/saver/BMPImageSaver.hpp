#ifndef BMPIMAGESAVER_HPP
#define BMPIMAGESAVER_HPP

#include "ImageSaver.hpp"

class BMPImageSaver : public ImageSaver {
    public:
        BMPImageSaver();
        int save(const Image& image, std::string fileUrl);

    private:
        unsigned char * ReverseByte(int value, int arraysize);
};

#endif // BMPIMAGESAVER_HPP
