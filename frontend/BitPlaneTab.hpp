#ifndef __BITPLANETAB_H
#define __BITPLANETAB_H

#include <QWidget>
#include "BitPlaneTab.hpp"
#include "../datastruct/Image.hpp"

class BitPlaneTab : public QWidget {

    public:
        BitPlaneTab(Image * image, QWidget *parent = 0);
};

#endif