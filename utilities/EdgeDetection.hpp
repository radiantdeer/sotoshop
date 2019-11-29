#ifndef __EDGE_DETECTION_HPP
#define __EDGE_DETECTION_HPP

#include "../datastruct/Image.hpp"
#include "Convolution.hpp"

class EdgeDetection {
    public:
        static Image* gradient(Image *image);
        static Image* laplaceOfGaussian(Image* image);
};

#endif