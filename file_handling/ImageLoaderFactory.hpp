#ifndef __IMAGELOADERFACTORY_HPP
#define __IMAGELOADERFACTORY_HPP

#include <string>
#include "ImageLoader.hpp"

class ImageLoaderFactory {
    
    public:
        ImageLoaderFactory();
        ImageLoader * getInstance(std::string filename);

};

#endif