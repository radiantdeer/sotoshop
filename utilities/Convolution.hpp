#ifndef __CONVOLUTION_HPP
#define __CONVOLUTION_HPP

#include <vector>
#include "ConvolutionMatrix.hpp"
#include "../datastruct/Image.hpp"

class Convolution {

    public:
        static Image* convolve(Image* image, const ConvolutionMatrix& opMatrix, bool padImage);
        static Image* medianConvolve(Image* image, int filterWidth, int filterHeight, bool padImage);
        static Image* unsharpMasking(Image* image);
        static Image* highboost(Image* image, double alpha);
        static Image* laplaceOp(Image* image);

        static Image* sobelOperation(Image* image, const ConvolutionMatrix& opMatrixX, const ConvolutionMatrix& opMatrixY);

        static Image* sobelOperation(Image* image, const ConvolutionMatrix& opMatrixX, const ConvolutionMatrix& opMatrixY);
        static Image* cannyOperation(Image* image, const ConvolutionMatrix& gaussianMatrix, const ConvolutionMatrix& opMatrixX, const ConvolutionMatrix& opMatrixY, int threshold);

    private:
        static Image* highpass(Image *image);
        // padWidth & padHeight is the length of padding on just one side, not the sum of both side
        static Image* padImage(Image* image, int padWidth, int padHeight);
        static std::vector<std::vector<double>> imageThetaMatrix(Image *image, const ConvolutionMatrix& opMatrixX, const ConvolutionMatrix& opMatrixY); 

};

#endif
