#ifndef __CONVOLUTION_MATRIX_HPP
#define __CONVOLUTION_MATRIX_HPP

#include <vector>

class ConvolutionMatrix {

    public:
        ConvolutionMatrix();
        ConvolutionMatrix(int height, int width, unsigned char* opMatrix);
        ConvolutionMatrix(int height, int width, unsigned char** opMatrix);
        ConvolutionMatrix(const ConvolutionMatrix& other);

        unsigned char** getMatrix();
        unsigned char getElementAt(int x, int y) const;
        int getWidth() const;
        int getHeight() const;
        int getMatrixSum() const;
        void setElementAt(int x, int y, unsigned char element);

        ConvolutionMatrix& operator=(const ConvolutionMatrix& other);

    private:
        int width;
        int height;
        int matrixSum;
        unsigned char** opMatrix;

};

#endif