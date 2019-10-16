#include "CommonConvolutions.hpp"

int averageMatrix[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
const ConvolutionMatrix CommonConvolutions::Average = ConvolutionMatrix(3, 3, averageMatrix);
int highFilter1[] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
const ConvolutionMatrix CommonConvolutions::HighFilter1 = ConvolutionMatrix(3, 3, highFilter1);
int highFilter2[] = {-1, -1, -1, -1, 9, -1, -1, -1, -1};
const ConvolutionMatrix CommonConvolutions::HighFilter2 = ConvolutionMatrix(3, 3, highFilter2);
int highFilter3[] = {0, 1, 0, 1, -4, 1, 0, 1, 0};
const ConvolutionMatrix CommonConvolutions::HighFilter3 = ConvolutionMatrix(3, 3, highFilter3);
int highFilter4[] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
const ConvolutionMatrix CommonConvolutions::HighFilter4 = ConvolutionMatrix(3, 3, highFilter4);