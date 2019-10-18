#ifndef __FOURIER_HPP
#define __FOURIER_HPP

#include <complex>
#include <vector>
#include "../datastruct/Image.hpp"

class Fourier {

    public:
        static std::vector<std::vector<std::vector<std::complex<double>>>> * forward(Image * image);
        static Image * visualizeFrequencies(std::vector<std::vector<std::vector<std::complex<double>>>> * frequencies);
        static Image * inverse(std::vector<std::vector<std::vector<std::complex<double>>>> * frequencies);
        static void shift(std::vector<std::vector<std::vector<std::complex<double>>>> * frequencies);

};

#endif