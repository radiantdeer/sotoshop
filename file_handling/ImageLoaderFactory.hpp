#ifndef __IMAGELOADERFACTORY_HPP
#define __IMAGELOADERFACTORY_HPP

#include <string>
#include "ImageLoader.hpp"

class ImageLoaderFactory {
    
    public:
        static ImageLoader * getImageLoader(std::string filename);

};

#endif