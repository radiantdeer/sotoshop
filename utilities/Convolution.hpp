#ifndef __CONVOLUTION_HPP
#define __CONVOLUTION_HPP

#include <vector>
#include "../datastruct/Image.hpp"

class Convolution {

    public:
        static Image* convolve(Image* image, unsigned char* opMatrix, int operatorWidth, int operatorHeight, bool padImage);
        static Image* convolve(Image* image, unsigned char** opMatrix, int operatorWidth, int operatorHeight, bool padImage);
        static Image* convolve(Image* image, std::vector<std::vector<Pixel>>* opMatrix);

};

#endif