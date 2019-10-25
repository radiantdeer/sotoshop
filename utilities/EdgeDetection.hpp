#ifndef EDGEDETECTION_HPP
#define EDGEDETECTION_HPP

#include <vector>
#include "../datastruct/Image.hpp"
#include "Convolution.hpp"
#include "CommonConvolutions.hpp"

class EdgeDetection
{
    public:
        static Image* SobelOperation(Image* image, const int constant, bool padImage);
};

#endif // EDGEDETECTION_HPP
