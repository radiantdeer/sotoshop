#ifndef __COMMON_CONVOLUTIONS_HPP
#define __COMMON_CONVOLUTIONS_HPP

#include "ConvolutionMatrix.hpp"

class CommonConvolutions {

    public:
        static const ConvolutionMatrix Average;
        static const ConvolutionMatrix Average5;
        static const ConvolutionMatrix HighFilter1;
        static const ConvolutionMatrix HighFilter2;
        static const ConvolutionMatrix HighFilter3;
        static const ConvolutionMatrix HighFilter4;
        static const ConvolutionMatrix SobelX;
        static const ConvolutionMatrix SobelY;
        static const ConvolutionMatrix PrewittX;
        static const ConvolutionMatrix PrewittY;

};

#endif
