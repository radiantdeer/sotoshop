#ifndef __IMAGE_HPP
#define __IMAGE_HPP

#include <string>
#include <vector>
#include "Pixel.hpp"

class Image {
    
    public:
        static const int MAX_GRAY = 255;
        Image();
        Image(int, int);
        Image(int, int, std::string);
        Image(int, int, Pixel *, std::string);
        Image(int, int, Pixel **, std::string);
        Image(const Image&);

        Pixel getPixelAt(int x, int y) const;
        int getWidth() const;
        int getHeight() const;
        std::vector<std::vector<Pixel>> getPixelData() const;
        std::string getOriginalFormat() const;
        std::string getFileUrl() const;

        void setPixelAt(int x, int y, Pixel pixel);
        void setWidth(int width);
        void setHeight(int height);
        void setOriginalFormat(std::string originalFormat);
        void setFileUrl(std::string fileUrl);

        Image * add(Image B, int width, int height);
        Image * adjustBrightness(unsigned char delta);
        Image * substract(Image B, int width, int height);
        Image * invert();
        Image * grayscale();
        Image * and(Image B, int width, int height);
        Image * or(Image B, int width, int height);
        Image * not();

        Image * operator+(Image B);
        Image * operator+(unsigned char deltaBrightness);
        Image * operator-(Image B);
        Image * operator-(unsigned char deltaBrightness);
        Image * operator&(Image B);
        Image * operator|(Image B);
        Image * operator~();

    private:
        int width;
        int height;
        std::vector<std::vector<Pixel>> data;
        std::string originalFormat;
        std::string fileUrl;
};

#endif