#include "ConvolutionMatrix.hpp"

ConvolutionMatrix::ConvolutionMatrix() {
    width = 0;
    height = 0;
    matrixSum = 1;
}

ConvolutionMatrix::ConvolutionMatrix(int height, int width, unsigned char* opMatrix) {
    this->opMatrix = new unsigned char * [height];
    this->matrixSum = 0;
    for (int j = 0; j < height; j++) {
        this->opMatrix[j] = new unsigned char [width];
        for (int i = 0; i < width; i++) {
            this->opMatrix[j][i] = opMatrix[(j * width) + i];
            this->matrixSum += opMatrix[(j * width) + i];
        }
    }
}

ConvolutionMatrix::ConvolutionMatrix(int height, int width, unsigned char** opMatrix) {
    this->opMatrix = new unsigned char * [height];
    this->matrixSum = 0;
    for (int j = 0; j < height; j++) {
        this->opMatrix[j] = new unsigned char [width];
        for (int i = 0; i < width; i++) {
            this->opMatrix[j][i] = opMatrix[j][i];
            this->matrixSum += opMatrix[j][i];
        }
    }
}

ConvolutionMatrix::ConvolutionMatrix(const ConvolutionMatrix& other) {
    this->opMatrix = new unsigned char * [other.getHeight()];
    this->matrixSum = other.getMatrixSum();
    for (int j = 0; j < other.getHeight(); j++) {
        this->opMatrix[j] = new unsigned char [other.getWidth()];
        for (int i = 0; i < other.getWidth(); i++) {
            this->opMatrix[j][i] = other.getElementAt(i, j);
        }
    }
}

unsigned char** ConvolutionMatrix::getMatrix() {
    return opMatrix;
}

unsigned char ConvolutionMatrix::getElementAt(int x, int y) const {
    return opMatrix[y][x];
}

int ConvolutionMatrix::getWidth() const {
    return width;
}

int ConvolutionMatrix::getHeight() const {
    return height;
}

int ConvolutionMatrix::getMatrixSum() const {
    return matrixSum;
}

void ConvolutionMatrix::setElementAt(int x, int y, unsigned char element) {
    int previousElm = opMatrix[y][x];
    matrixSum -= previousElm;
    opMatrix[y][x] = element;
    matrixSum += element;
}

ConvolutionMatrix& ConvolutionMatrix::operator=(const ConvolutionMatrix& other) {
    this->opMatrix = new unsigned char * [other.getHeight()];
    this->matrixSum = other.getMatrixSum();
    for (int j = 0; j < other.getHeight(); j++) {
        this->opMatrix[j] = new unsigned char [other.getWidth()];
        for (int i = 0; i < other.getWidth(); i++) {
            this->opMatrix[j][i] = other.getElementAt(i, j);
        }
    }
}
