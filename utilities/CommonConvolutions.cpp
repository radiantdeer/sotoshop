#include "CommonConvolutions.hpp"

int averageMatrix[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
const ConvolutionMatrix CommonConvolutions::Average = ConvolutionMatrix(3, 3, averageMatrix);
int averageMatrix5[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
const ConvolutionMatrix CommonConvolutions::Average5 = ConvolutionMatrix(5, 5, averageMatrix5);
int highFilter1[] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
const ConvolutionMatrix CommonConvolutions::HighFilter1 = ConvolutionMatrix(3, 3, highFilter1);
int highFilter2[] = {-1, -1, -1, -1, 9, -1, -1, -1, -1};
const ConvolutionMatrix CommonConvolutions::HighFilter2 = ConvolutionMatrix(3, 3, highFilter2);
int highFilter3[] = {0, 1, 0, 1, -4, 1, 0, 1, 0};
const ConvolutionMatrix CommonConvolutions::HighFilter3 = ConvolutionMatrix(3, 3, highFilter3);
int highFilter4[] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
const ConvolutionMatrix CommonConvolutions::HighFilter4 = ConvolutionMatrix(3, 3, highFilter4);
int gaussianMatrix[] = {1, 1, 2, 2, 2, 1, 1,
                        1, 2, 2, 4, 2, 2, 1,
                        2, 2, 4, 8, 4, 2, 2,
                        2, 4, 8, 16, 8, 4, 2,
                        2, 2, 4, 8, 4, 2, 2,
                        1, 2, 2, 4, 2, 2, 1,
                        1, 1, 2, 2, 2, 1, 1};
const ConvolutionMatrix CommonConvolutions::Gaussian = ConvolutionMatrix(7, 7, gaussianMatrix);
int sobelXMatrix[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
const ConvolutionMatrix CommonConvolutions::SobelX = ConvolutionMatrix(3, 3, sobelXMatrix);
int sobelYMatrix[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
const ConvolutionMatrix CommonConvolutions::SobelY = ConvolutionMatrix(3, 3, sobelYMatrix);
int prewittXMatrix[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
const ConvolutionMatrix CommonConvolutions::PrewittX = ConvolutionMatrix(3, 3, prewittXMatrix);
int prewittYMatrix[] = {1, 1, 1, 0, 0, 0, -1, -1, -1};
const ConvolutionMatrix CommonConvolutions::PrewittY = ConvolutionMatrix(3, 3, prewittYMatrix);
int robertXMatrix[] = {1, 0, 0, -1};
const ConvolutionMatrix CommonConvolutions::RobertX = ConvolutionMatrix(2, 2, robertXMatrix);
int robertYMatrix[] = {0, 1, -1, 0};
const ConvolutionMatrix CommonConvolutions::RobertY = ConvolutionMatrix(2, 2, robertYMatrix);

int laplace[] = {0, 1, 0, 1, -4, 1, 0, 1, 0};
const ConvolutionMatrix CommonConvolutions::Laplace = ConvolutionMatrix(3, 3, laplace);
int laplaceOfGaussian[] = {
     0,  0, -1,  0,  0,
     0, -1, -2, -1,  0,
    -1, -2, 16, -2, -1,
     0, -1, -2, -1,  0,
     0,  0, -1,  0, 0 };
const ConvolutionMatrix CommonConvolutions::LaplaceOfGaussian = ConvolutionMatrix(5, 5, laplaceOfGaussian);
