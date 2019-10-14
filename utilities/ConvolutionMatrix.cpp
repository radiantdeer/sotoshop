#include "ConvolutionMatrix.hpp"

ConvolutionMatrix::ConvolutionMatrix() {
    width = 0;
    height = 0;
    matrixSum = 1;
}

ConvolutionMatrix::ConvolutionMatrix(int height, int width, int* opMatrix) {
    this->opMatrix = new int * [height];
    this->matrixSum = 0;
    for (int j = 0; j < height; j++) {
        this->opMatrix[j] = new int [width];
        for (int i = 0; i < width; i++) {
            this->opMatrix[j][i] = opMatrix[(j * width) + i];
            this->matrixSum += opMatrix[(j * width) + i];
        }
    }
}

ConvolutionMatrix::ConvolutionMatrix(int height, int width, int** opMatrix) {
    this->opMatrix = new int * [height];
    this->matrixSum = 0;
    for (int j = 0; j < height; j++) {
        this->opMatrix[j] = new int [width];
        for (int i = 0; i < width; i++) {
            this->opMatrix[j][i] = opMatrix[j][i];
            this->matrixSum += opMatrix[j][i];
        }
    }
}

ConvolutionMatrix::ConvolutionMatrix(const ConvolutionMatrix& other) {
    this->opMatrix = new int * [other.getHeight()];
    this->matrixSum = other.getMatrixSum();
    for (int j = 0; j < other.getHeight(); j++) {
        this->opMatrix[j] = new int [other.getWidth()];
        for (int i = 0; i < other.getWidth(); i++) {
            this->opMatrix[j][i] = other.getElementAt(i, j);
        }
    }
}

int** ConvolutionMatrix::getMatrix() {
    return opMatrix;
}

int ConvolutionMatrix::getElementAt(int x, int y) const {
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

void ConvolutionMatrix::setElementAt(int x, int y, int element) {
    int previousElm = opMatrix[y][x];
    matrixSum -= previousElm;
    opMatrix[y][x] = element;
    matrixSum += element;
}

ConvolutionMatrix& ConvolutionMatrix::operator=(const ConvolutionMatrix& other) {
    this->opMatrix = new int * [other.getHeight()];
    this->matrixSum = other.getMatrixSum();
    for (int j = 0; j < other.getHeight(); j++) {
        this->opMatrix[j] = new int [other.getWidth()];
        for (int i = 0; i < other.getWidth(); i++) {
            this->opMatrix[j][i] = other.getElementAt(i, j);
        }
    }
    return *this;
}
