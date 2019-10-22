#ifndef __HOUGHTRANSFORMATION_H
#define __HOUGHTRANSFORMATION_H

#include "../datastruct/Image.hpp";

class HoughTransformation {
    public:
        static Image* HoughLine(Image* input);
        //static Image* HoughCircle(Image* input);
};

#endif