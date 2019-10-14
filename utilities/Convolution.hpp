#ifndef __CONVOLUTION_HPP
#define __CONVOLUTION_HPP

#include <vector>
#include "ConvolutionMatrix.hpp"
#include "../datastruct/Image.hpp"

class Convolution {

    public:
        static Image* convolve(Image* image, ConvolutionMatrix& opMatrix, bool padImage);

};

#endif