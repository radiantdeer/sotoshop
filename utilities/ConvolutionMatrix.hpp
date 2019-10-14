#ifndef __CONVOLUTION_MATRIX_HPP
#define __CONVOLUTION_MATRIX_HPP

#include <vector>

class ConvolutionMatrix {

    public:
        ConvolutionMatrix();
        ConvolutionMatrix(int height, int width, int* opMatrix);
        ConvolutionMatrix(int height, int width, int** opMatrix);
        ConvolutionMatrix(const ConvolutionMatrix& other);

        int** getMatrix();
        int getElementAt(int x, int y) const;
        int getWidth() const;
        int getHeight() const;
        int getMatrixSum() const;
        void setElementAt(int x, int y, int element);

        ConvolutionMatrix& operator=(const ConvolutionMatrix& other);

    private:
        int width;
        int height;
        int matrixSum;
        int** opMatrix;

};

#endif