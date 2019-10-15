#include "CommonConvolutions.hpp"

int averageMatrix[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
const ConvolutionMatrix CommonConvolutions::Average = ConvolutionMatrix(3, 3, averageMatrix);
