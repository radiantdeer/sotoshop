#ifndef __BITPLANESLICING_HPP
#define __BITPLANESLICING_HPP

#include <vector>
#include "../datastruct/Image.hpp"

class BitPlaneSlicing {

    public:
        static std::vector<Image> generate(Image * image);

};

#endif