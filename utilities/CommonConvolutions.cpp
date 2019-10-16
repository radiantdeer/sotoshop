#include "CommonConvolutions.hpp"

int averageMatrix[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
const ConvolutionMatrix CommonConvolutions::Average = ConvolutionMatrix(3, 3, averageMatrix);
int highFilter1[] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
const ConvolutionMatrix CommonConvolutions::HighFilter1 = ConvolutionMatrix(3, 3, highFilter1);
int highFilter2[] = {-1, -1, -1, -1, 9, -1, -1, -1, -1};
const ConvolutionMatrix CommonConvolutions::HighFilter2 = ConvolutionMatrix(3, 3, highFilter2);