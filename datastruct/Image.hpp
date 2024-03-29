#ifndef __IMAGE_HPP
#define __IMAGE_HPP

#include <string>
#include <vector>
#include "ImageHistogram.hpp"
#include "Pixel.hpp"

class Image {

    public:
        static const int MAX_GRAY = 255;
        static const int COLOR_LEVEL = 256;

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
        void setPixelAt(int x, int y, unsigned char r, unsigned char g, unsigned char b);
        void setWidth(int width);
        void setHeight(int height);
        void setOriginalFormat(std::string originalFormat);
        void setFileUrl(std::string fileUrl);

        Image * add(Image& B);
        Image * adjustBrightness(unsigned char delta);
        Image * substract(Image& B);
        Image * multiply(Image& b);
        Image * multiply(double alpha);
        Image * invert();
        Image * grayscale();
        Image * and_op(Image& B);
        Image * or_op(Image& B);
        Image * not_op();
        Image * translate(int dx, int dy);
        Image * rotate90CW();
        Image * rotate90CCW();
        Image * flipH();
        Image * flipV();
        Image * magnify2();
        Image * shrink2();
        Image * nthPower(double n, double c);

        Image * operator+(Image& B);
        Image * operator+(unsigned char deltaBrightness);
        Image * operator-(Image& B);
        Image * operator-(unsigned char deltaBrightness);
        Image * operator*(Image& B);
        Image * operator*(double alpha);
        Image * operator&(Image& B);
        Image * operator|(Image& B);
        Image * operator~();

        Image * logTrans();
        Image * invLogTrans();

        ImageHistogram histogram();
        ImageHistogram equalizedHistogram();
        Image * histogramEqualization();
        Image * histogramSpecification(Image& B);

        Image * grayLevelSlicing(int a, int b, int val);
        Image * contrastStretch();
        Image * contrastStretch(int rrmin, int rrmax, int rgmin, int rgmax, int rbmin, int rbmax);

        // Binary Image
        Image * binarySegmentation();
        Image * binaryThinning();
    private:
        int width;
        int height;
        std::vector<std::vector<Pixel>> data;
        std::string originalFormat;
        std::string fileUrl;
};

#endif