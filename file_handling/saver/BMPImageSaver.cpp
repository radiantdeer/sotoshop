#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "BMPImageSaver.hpp"

#define BMP_MAGICNUMBER "BM"
#define EXT ".bmp"

// Reverse Byte, array size is either 2 or four
unsigned char* ReverseByte(int value, int arraysize);

BMPImageSaver::BMPImageSaver()
{

}

int BMPImageSaver::save(const Image &image, std::string fileUrl) {
    std::string filename = fileUrl + EXT;
    std::ofstream outfile(filename);

    if (outfile.good()) {
        int height = image.getHeight();
        int width = image.getWidth();

        /**********************************************/
        /* THIS SECTION CALCULATES */
        /* COLOR TABLE */
        /* INDEXING COLOR TABLE */
        /**********************************************/

        int colorcount;
        std::vector<Pixel> colortable;
        std::vector<Pixel>::iterator it;
        std::vector<int> colorindex;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Pixel p = image.getPixelAt(i, j);

                it = std::find(colortable.begin(), colortable.end(), p);

                if (it != colortable.end()) {

                    colorindex.push_back((int) std::distance(colortable.begin(), it));
                } else {
                    // ADD PIXEL TO COLOR TABLE
                    colortable.push_back(p);
                    it = std::find(colortable.begin(), colortable.end(), p);
                    colorindex.push_back((int) std::distance(colortable.begin(), it));
                }
            }
        }

        /**********************************************/
        /* THIS SECTION CALCULATES */
        /* BMP FILE SIZE */
        /* BMP FILE SIZE IS BITMAP BYTES + HEADERS + COLOR TABLES */
        /**********************************************/

        int bmpfilesize = 14 + 40 + width*height + colortable.size()*4;

        /**********************************************/
        /* THIS SECTION WRITES */
        /* FILE HEADER */
        /* FILE HEADER SIZE IS 14 BYTES */
        /**********************************************/

        // WRITES MAGIC NUMBER
        outfile << BMP_MAGICNUMBER;

        // WRITES SIZES OF BMP IN BYTES
        unsigned char* bmpfilesizechar = ReverseByte(bmpfilesize, 4);
        outfile << bmpfilesizechar;

        // WRITES 4 BYTES RESERVED
        unsigned char reserved[] = {0x00, 0x00, 0x00, 0x00};
        outfile << reserved;

        // WRITES OFFSET OF PIXEL ARRAY

        /**********************************************/
        /* THIS SECTION WRITESS */
        /* BITMAP HEADER */
        /* BITMAP HEADER SIZE IS 40 BYTES */
        /**********************************************/



        /**********************************************/
        /* THIS SECTION WRITES */
        /* BITMAP COLOR TABLE */
        /* BITMAP COLOR TABLE IS UNDEFINED */
        /**********************************************/

        /**********************************************/
        /* THIS SECTION WRITES */
        /* BITMAP COLOR */
        /* BITMAP COLOR STORED WITH PIXEL IF COLOR TABLE DOES NOT EXIST */
        /**********************************************/

        // OUTPUT CONTENT

        outfile.close();
        return 0;
    } else {

        // THIS PART THROW ERROR
        throw std::runtime_error("BMPImageSaver: Cannot write file");
        return -1;
    }
}

unsigned char* ReverseByte(int value, int arraysize) {
    int i;
    unsigned char inversedbyte[arraysize];

    inversedbyte[0] = value << 24;
    inversedbyte[1] = value << 8 & 0x00ff0000;

    if (arraysize > 2) {
        inversedbyte[1] = value >> 8 & 0x0000ff00;
        inversedbyte[0] = value >> 24;
    }
    return inversedbyte;
}
