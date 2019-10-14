#include "CommonConvolutions.hpp"

const int CommonConvolutions::averageMatrix[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
const ConvolutionMatrix CommonConvolutions::Average = ConvolutionMatrix(3, 3, (int **) averageMatrix);