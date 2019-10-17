#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "../../spdlog/spdlog.h"
#include "BMPImageSaver.hpp"

#define BMP_MAGICNUMBER "BM"
#define EXT ".bmp"

// Reverse Byte, array size is either 2 or four
unsigned char* ReverseByte(int value, int arraysize);

BMPImageSaver::BMPImageSaver()
{

}

int BMPImageSaver::save(const Image &image, std::string fileUrl) {
    std::string filename = fileUrl;
    std::ofstream outfile(filename, std::ios_base::out | std::ios_base::binary);

    unsigned char *fourbytecontainer;
    unsigned char *twobytecontainer;

    if (outfile.good()) {
        int height = image.getHeight();
        int width = image.getWidth();

        /**********************************************/
        /* THIS SECTION CALCULATES */
        /* COLOR TABLE */
        /* CHECK IF GRAYSCALE OR NOT */
        /**********************************************/

        std::cout << "check grayscale" << std::endl;

        int byteperpixel = 1;           // BYTE PER PIXEL
        int bitperpixel = 1;            // BIT PER PIXEL
        int bitmapcolortablesize = 1;   // BITMAP COLOR TABLE SIZE

        bool isGrayscale = true;
        std::vector<Pixel> colortable;
        std::vector<Pixel>::iterator it;
        std::vector<int> colorindex;

        int a = 0; int b = 0;

        while (a < height && isGrayscale) {
            while (b < width && isGrayscale) {
                Pixel p = image.getPixelAt(b, a);

                // CHECK IF THERE IS A NOT GRAYSCALE PIXEL
                if (p.getRed() != p.getGreen() || p.getGreen() != p.getBlue() || p.getRed() != p.getBlue()) {
                    isGrayscale = false;
                }

                b++;
            }
            a++;
        }

        std::cout << "check grayscale end" << std::endl;

        if (isGrayscale) {
            std::cout << "is grayscale" << std::endl;

            // PICTURE IS IN GRAYSCALE
            // FILL COLOR TABLE WITH GRAYSCALE VALUE
            // MAX GRAYSCALE VALUE IS 255
            unsigned char value = 0;
            do {
                Pixel p = Pixel(value, value, value);
                colortable.push_back(p);
            } while (++value);

            bitmapcolortablesize = UCHAR_MAX + 1;
            byteperpixel = 1;           // GRAYSCALE
        } else {
            std::cout << "not grayscale" << std::endl;
            // PICTURE IN COLOR
            // NO COLOR TABLE AND INDEX
            bitmapcolortablesize = 0;
            byteperpixel = 3;           // RGB
        }

        bitperpixel = byteperpixel * 8;

        /**********************************************/
        /* THIS SECTION CALCULATES */
        /* BMP FILE SIZE */
        /* BMP FILE SIZE IS BITMAP BYTES + HEADERS + COLOR TABLES */
        /**********************************************/

        int bmpfilesize = 14 + 40 + bitmapcolortablesize*4 + width*height*byteperpixel;

        std::cout << "BMP size " << bmpfilesize << std::endl;

        /**********************************************/
        /* THIS SECTION WRITES */
        /* FILE HEADER */
        /* FILE HEADER SIZE IS 14 BYTES */
        /**********************************************/

        // WRITES MAGIC NUMBER
        outfile << BMP_MAGICNUMBER;

        // WRITES SIZES OF BMP IN BYTES
        fourbytecontainer = ReverseByte(bmpfilesize, 4);
        for (int i = 0; i < 4; i++) {
            outfile << fourbytecontainer[i];
        }

        // WRITES 4 BYTES RESERVED
        char reserved[] = {0x00, 0x00, 0x00, 0x00};
        for (int i = 0; i < 4; i++) {
            outfile << reserved[i];
        }


        // WRITES OFFSET OF PIXEL ARRAY
        fourbytecontainer = ReverseByte(14 + 40 + bitmapcolortablesize*4, 4);
        for (int i = 0; i < 4; i++) {
            outfile << fourbytecontainer[i];
        }

        /**********************************************/
        /* THIS SECTION WRITESS */
        /* BITMAP HEADER */
        /* BITMAP HEADER SIZE IS 40 BYTES */
        /**********************************************/

        // WRITE HEADER SIZE = 40
        fourbytecontainer = ReverseByte(40, 4);
        for (int i = 0; i < 4; i++) {
            outfile << fourbytecontainer[i];
        }

        // WRITE BITMAP WIDTH
        fourbytecontainer = ReverseByte(width, 4);
        for (int i = 0; i < 4; i++) {
            outfile << fourbytecontainer[i];
        }

        //WRITE BITMAP HEIGHT
        fourbytecontainer = ReverseByte(height, 4);
        for (int i = 0; i < 4; i++) {
            outfile << fourbytecontainer[i];
        }

        // WRITE COLOR PLANES
        twobytecontainer = ReverseByte(1, 2);
        for (int i = 0; i < 2; i++) {
            outfile << twobytecontainer[i];
        }

        // WRITE BITS PER PIXEL
        twobytecontainer = ReverseByte(bitperpixel, 2);
        for (int i = 0; i < 2; i++) {
            outfile << twobytecontainer[i];
        }

        // WRITE COMPRESSION METHOD = 0
        fourbytecontainer = ReverseByte(0, 4);
        for (int i = 0; i < 4; i++) {
            outfile << fourbytecontainer[i];
        }

        // WRITE IMAGE SIZE
        // DUMMY IMAGE SIZE = 0
        fourbytecontainer = ReverseByte(0, 4);
        for (int i = 0; i < 4; i++) {
            outfile << fourbytecontainer[i];
        }

        // WRITE HORIZONTAL RESOLUTION
        // DEFAULT VALUE 2835
        fourbytecontainer = ReverseByte(2835, 4);
        for (int i = 0; i < 4; i++) {
            outfile << fourbytecontainer[i];
        }

        // WRITE VERTICAL RESOLUTION
        // DEFAULT VALUE 2835
        fourbytecontainer = ReverseByte(2835, 4);
        for (int i = 0; i < 4; i++) {
            outfile << fourbytecontainer[i];
        }

        // WRITE NUMBER OF COLOR AND IMPORTANT COLOR IN COLOR PALLETE
        // WRITE TWICE SINCE COLOR AND IMPORTANT COLOR ARE SAME AMOUNT
        fourbytecontainer = ReverseByte(bitmapcolortablesize, 4);
        for (int i = 0; i < 4; i++) {
            outfile << fourbytecontainer[i];
        }
        for (int i = 0; i < 4; i++) {
            outfile << fourbytecontainer[i];
        }

        /**********************************************/
        /* THIS SECTION WRITES */
        /* BITMAP COLOR TABLE */
        /* EACH BITMAP COLOR COMPRISES OF B, G, R, A */
        /**********************************************/
        unsigned char val3[4];
        for(unsigned long i = 0; i < colortable.size(); i++) {
            Pixel &p = colortable.at(i);
            val3[0] = p.getBlue();
            val3[1] = p.getGreen();
            val3[2] = p.getRed();
            val3[3] = 0x00;
            for (int j = 0; j < 4; j++) {
                outfile << val3[j];
            }
        }

        /**********************************************/
        /* THIS SECTION WRITES */
        /* BITMAP COLOR INDEX */
        /* BITMAP COLOR INDEX REFER TO COLOR TABLE */
        /**********************************************/

        if (isGrayscale){

            // CHECK COLOR INDEX

            for (int i = height - 1; i >= 0; i--) {
                for (int j = 0; j < width; j++) {
                    Pixel p = image.getPixelAt(j, i);
                    it = std::find(colortable.begin(), colortable.end(), p);
                    colorindex.push_back((int) std::distance(colortable.begin(), it));
                }
            }

            // OUTPUT CONTENT
            unsigned char *bitmapcoloridx;
            for (unsigned long i = 0; i < colorindex.size(); i++) {
                bitmapcoloridx = ReverseByte(colorindex[i], 1);
                outfile << bitmapcoloridx[0];
            }
        } else {
            unsigned char pixelcontainer[3];
            for (int i = height - 1; i >= 0; i--) {
                for (int j = 0; j < width; j++) {
                    Pixel p = image.getPixelAt(j, i);
                    pixelcontainer[0] = p.getBlue();
                    pixelcontainer[1] = p.getGreen();
                    pixelcontainer[2] = p.getRed();

                    for (int k = 0; k < 3; k++) {
                        outfile << pixelcontainer[k];
                    }
                }
            }
        }

        outfile.close();
        return 0;
    } else {

        // THIS PART THROW ERROR
        spdlog::error("BMPImageSaver::save: Cannot write file!");
        throw std::runtime_error("BMPImageSaver: Cannot write file");
        return -1;
    }
}

unsigned char* ReverseByte(int value, int arraysize) {
    unsigned char *inversedbyte = (unsigned char *) malloc(sizeof (unsigned char) * arraysize);
    for (int i = 0; i < arraysize; i++) {
        inversedbyte[i] = (value >> (i * 8)) & 0xff;
    }
    return inversedbyte;
}
