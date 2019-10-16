#ifndef __PIXEL_HPP
#define __PIXEL_HPP

#include <string>

class Pixel {

    private:
        unsigned char red;
        unsigned char green;
        unsigned char blue;

    public:
        static int thresholding(int value);
        Pixel();
        Pixel(unsigned char, unsigned char, unsigned char);
        Pixel(const Pixel&);

        unsigned char getRed() const;
        unsigned char getGreen() const;
        unsigned char getBlue() const;

        void setRed(unsigned char);
        void setGreen(unsigned char);
        void setBlue(unsigned char);

        unsigned char grayscaleValue() const;
        unsigned char meanGrayscaleValue() const;
        void makeGrayscale();
        void makeMeanGrayscale();

        std::string toString() const;

        bool operator==(Pixel& B);
        bool operator>(Pixel& B);
        bool operator>=(Pixel& B);
        bool operator<(Pixel& B);
        bool operator<=(Pixel& B);
  
        Pixel * operator+(Pixel& B);
        Pixel * operator+(unsigned char delta);
        Pixel * operator-(Pixel& B);
        Pixel * operator*(Pixel& B);
        Pixel * operator&(Pixel& B);
        Pixel * operator|(Pixel& B);
        Pixel * operator~();

};

#endif