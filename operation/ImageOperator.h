#ifndef IMAGEOPERATOR_H
#define IMAGEOPERATOR_H

#include "../datastruct/Image.hpp"

class ImageOperator
{
public:
    ImageOperator();
    Image AddTwoImages(Image A, Image B, int width, int height);
    Image SubstractTwoImages(Image A, Image B, int width, int height);
    void BrightenImage(Image A, unsigned char brightness);

};

#endif // IMAGEOPERATOR_H
