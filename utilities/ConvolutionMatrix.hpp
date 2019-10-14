#ifndef __CONVOLUTION_MATRIX_HPP
#define __CONVOLUTION_MATRIX_HPP

#include <vector>

class ConvolutionMatrix {

    public:
        ConvolutionMatrix();
        ConvolutionMatrix(int height, int width, unsigned char* opMatrix);
        ConvolutionMatrix(int height, int width, unsigned char** opMatrix);

        unsigned char** getMatrix();
        unsigned char getElementAt(int x, int y);
        void setElementAt(int x, int y, unsigned char element);
        void setMatrix(unsigned char* opMatrix);
        void setMatrix(unsigned char** opMatrix);

        ConvolutionMatrix& operator=(unsigned char* opMatrix);
        ConvolutionMatrix& operator=(unsigned char** opMatrix);

    private:
        int width;
        int height;
        unsigned char** opMatrix;

};

#endif