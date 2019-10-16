#ifndef __PIXEL_HPP
#define __PIXEL_HPP

#include <string>

class Pixel {

    private:
        unsigned char red;
        unsigned char green;
        unsigned char blue;

    public:
        Pixel();
        Pixel(unsigned char, unsigned char, unsigned char);
        Pixel(const Pixel&);

        unsigned char getRed() const;
        unsigned char getGreen() const;
        unsigned char getBlue() const;

        void setRed(unsigned char);
        void setGreen(unsigned char);
        void setBlue(unsigned char);

        std::string toString() const;

        Pixel * operator+(Pixel B);
        Pixel * operator*(Pixel B);
};

#endif