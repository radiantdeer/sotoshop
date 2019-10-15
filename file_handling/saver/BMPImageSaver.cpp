#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "BMPImageSaver.hpp"

#define BMP_MAGICNUMBER "BM"
#define EXT ".bmp"

// Reverse Byte, array size is either 2 or four
char* ReverseByte(int value, int arraysize);

BMPImageSaver::BMPImageSaver()
{

}

int BMPImageSaver::save(const Image &image, std::string fileUrl) {
    std::string filename = fileUrl;
    std::ofstream outfile(filename);

    char *fourbytecontainer;
    char *twobytecontainer;

    if (outfile.good()) {
        int height = image.getHeight();
        int width = image.getWidth();

        /**********************************************/
        /* THIS SECTION CALCULATES */
        /* COLOR TABLE */
        /* INDEXING COLOR TABLE */
        /**********************************************/

        int colorcount = 0;
        std::vector<Pixel> colortable;
        std::vector<Pixel>::iterator it;
        std::vector<int> colorindex;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Pixel p = image.getPixelAt(i, j);

                it = std::find(colortable.begin(), colortable.end(), p);

                if (it != colortable.end()) {
                    colorindex.push_back((int) std::distance(colortable.begin(), it));
                    colorcount++;
                } else {
                    // ADD PIXEL TO COLOR TABLE
                    colortable.push_back(p);
                    it = std::find(colortable.begin(), colortable.end(), p);
                    colorindex.push_back((int) std::distance(colortable.begin(), it));
                }
            }
        }

        // BIT PER PIXEL
        int bitperpixel = 1;

        // BITMAP COLOR TABLE SIZE
        // ROUND COLORTABLE SIZE TO NEAREST 2*2*2*2*2.......
        int bitmapcolortablesize = 1;
        while (bitmapcolortablesize < colortable.size()) {
            bitmapcolortablesize *= 2;
            bitperpixel++;
        }

        // ROUND BIT PER PIXEL SIZE TO NEAREST BYTE SIZE
        bitperpixel = bitperpixel + (bitperpixel) % 4;

        /**********************************************/
        /* THIS SECTION CALCULATES */
        /* BMP FILE SIZE */
        /* BMP FILE SIZE IS BITMAP BYTES + HEADERS + COLOR TABLES */
        /**********************************************/

        int bmpfilesize = 14 + 40 + width*height + bitmapcolortablesize*4;

        /**********************************************/
        /* THIS SECTION WRITES */
        /* FILE HEADER */
        /* FILE HEADER SIZE IS 14 BYTES */
        /**********************************************/

        // WRITES MAGIC NUMBER
        outfile << BMP_MAGICNUMBER;

        // WRITES SIZES OF BMP IN BYTES
        fourbytecontainer = ReverseByte(bmpfilesize, 4);
        outfile << fourbytecontainer;

        // WRITES 4 BYTES RESERVED
        char reserved[] = {0x00, 0x00, 0x00, 0x00};
        outfile << reserved;

        // WRITES OFFSET OF PIXEL ARRAY

        /**********************************************/
        /* THIS SECTION WRITESS */
        /* BITMAP HEADER */
        /* BITMAP HEADER SIZE IS 40 BYTES */
        /**********************************************/

        // WRITE HEADER SIZE = 40
        fourbytecontainer = ReverseByte(40, 4);
        outfile << fourbytecontainer;

        // WRITE BITMAP WIDTH
        fourbytecontainer = ReverseByte(width, 4);
        outfile << fourbytecontainer;

        //WRITE BITMAP HEIGHT
        fourbytecontainer = ReverseByte(height, 4);
        outfile << fourbytecontainer;

        // WRITE COLOR PLANES
        twobytecontainer = ReverseByte(1, 2);
        outfile << twobytecontainer;

        // WRITE BITS PER PIXEL
        fourbytecontainer = ReverseByte(bitperpixel, 4);
        outfile << fourbytecontainer;

        // WRITE COMPRESSION METHOD = 0
        fourbytecontainer = ReverseByte(0, 4);
        outfile << fourbytecontainer;

        // WRITE IMAGE SIZE
        // DUMMY IMAGE SIZE = 0
        fourbytecontainer = ReverseByte(0, 4);
        outfile << fourbytecontainer;

        // WRITE HORIZONTAL RESOLUTION
        // DEFAULT VALUE 2835
        fourbytecontainer = ReverseByte(2835, 4);
        outfile << fourbytecontainer;

        // WRITE VERTICAL RESOLUTION
        // DEFAULT VALUE 2835
        fourbytecontainer = ReverseByte(2835, 4);
        outfile << fourbytecontainer;

        // WRITE NUMBER OF COLOR AND IMPORTANT COLOR IN COLOR PALLETE
        // WRITE TWICE SINCE COLOR AND IMPORTANT COLOR ARE SAME AMOUNT
        fourbytecontainer = ReverseByte(bitmapcolortablesize, 4);
        outfile << fourbytecontainer;
        outfile << fourbytecontainer;


        /**********************************************/
        /* THIS SECTION WRITES */
        /* BITMAP COLOR TABLE */
        /* EACH BITMAP COLOR COMPRISES OF B, G, R, A */
        /**********************************************/
        char colorcontainer[4];
        int bitmapcolortablebuffsize = 0;
        for(int i = 0; i < colortable.size(); i++) {
            colorcontainer[3] = 0x00;                       // ALPHA
            colorcontainer[2] = colortable[i].getRed();     // RED
            colorcontainer[1] = colortable[i].getGreen();   // GREEN
            colorcontainer[0] = colortable[i].getBlue();    // BLUE
            outfile << colorcontainer;
            bitmapcolortablebuffsize++;
        }

        // PADDING COLOR TABLE
        if (bitmapcolortablebuffsize != bitmapcolortablesize - 1) {
            fourbytecontainer = ReverseByte(0, 4);
            for (int i = 0; i < bitmapcolortablesize - bitmapcolortablebuffsize; i++) {
                outfile << fourbytecontainer;
            }
        }

        /**********************************************/
        /* THIS SECTION WRITES */
        /* BITMAP COLOR INDEX */
        /* BITMAP COLOR INDEX REFER TO COLOR TABLE */
        /**********************************************/

        char bitmapcoloridx;
        // OUTPUT CONTENT
        for (int i = 0; i < colorindex.size(); i++) {
            bitmapcoloridx = (char) colorindex[i];
            outfile << bitmapcoloridx;
        }

        outfile.close();
        return 0;
    } else {

        // THIS PART THROW ERROR
        throw std::runtime_error("BMPImageSaver: Cannot write file");
        return -1;
    }
}

char* ReverseByte(int value, int arraysize) {
    char *inversedbyte = (char *) malloc(sizeof (char) * arraysize);

    inversedbyte[0] = value << 24;
    inversedbyte[1] = value << 8 & 0x00ff0000;

    if (arraysize > 2) {
        inversedbyte[1] = value >> 8 & 0x0000ff00;
        inversedbyte[0] = value >> 24;
    }
    return inversedbyte;
}
