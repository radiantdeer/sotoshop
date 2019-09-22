#include "BMPImageLoader.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define BMP_HEADER_WINDOWS "BM"

int buffertoInteger(char* buffer, int start, int size);
void PrintPixel(Pixel pixel);

BMPImageLoader::BMPImageLoader() {

}

Image * BMPImageLoader::load(std::string fileUrl) {

  char *fileheader = new char[14];
  std::ifstream fileread(fileUrl.c_str());

  if (fileread.good()) {
    /* READ FILE HEADER */
    fileread.read(fileheader, 14);

    /* GET FILE FORMAT */
    std::string fileformat(fileheader, 2);
    std::cout << "Bitmap format: " << fileformat << std::endl;

    if (fileformat == BMP_HEADER_WINDOWS) {
      return loadBM(fileUrl);
    }
  }

  // fileread.close();

  delete [] fileheader;
  return new Image();
}

Image * BMPImageLoader::loadBM(std::string fileUrl) {
  std::string line;

  char *fileheader = new char[14];

  char *BitmapHeaderSize = new char[4];

  char* buff1;
  unsigned char* buff;
  std::ifstream fileread(fileUrl.c_str());
  if (fileread.good()) {
    std::cout << "Opened" << std::endl;

    /**********************************************/
    /* THIS SECTION READS */
    /* FILE HEADER */
    /* FILE HEADER SIZE IS 14 BYTES */
    /**********************************************/
    fileread.read(fileheader, 14);

    /* READ SIZE */
    // fileread.read(sizebuffer, 4);
    int filesize = buffertoInteger(fileheader, 2, 4);
    std::cout << "File size: " << filesize << std::endl;

    /* READ OFFSET */
    int offset = buffertoInteger(fileheader, 10, 4);
    std::cout << "Offset: " << offset << std::endl;

    /**********************************************/
    /* THIS SECTION READS */
    /* BITMAP HEADER */
    /* BITMAP HEADER SIZE IS 40 BYTES */
    /**********************************************/
    fileread.read(BitmapHeaderSize, 4);
    int headersize = buffertoInteger(BitmapHeaderSize, 0, 4);
    std::cout << "Header size: " << headersize << std::endl;
    fileread.seekg(14);
    char *bitmapheader = new char[headersize];

    /* READ BITMAP HEADER DETAIL */
    fileread.read(bitmapheader, headersize);

    // picture width
    int width = buffertoInteger(bitmapheader, 4, 4);
    // picture height
    int height = buffertoInteger(bitmapheader, 8, 4);
    std::cout << "Dimension: " << width << " x " << height << std::endl;

    // picture plane
    int plane = buffertoInteger(bitmapheader, 12, 2);
    std::cout << "Plane: " << plane << std::endl;

    // picture pixel count
    int pixelcount = buffertoInteger(bitmapheader, 14, 2);
    std::cout << "Pixel count in bits: " << pixelcount << std::endl;

    // picture compression
    int compression = buffertoInteger(bitmapheader, 16, 4);
    if (compression) {
      std::cout << "Compressed" << std::endl;
    } else {
      std::cout << "Uncompressed" << std::endl;
    }

    // picture image size
    int imgsize = buffertoInteger(bitmapheader, 20, 4);
    std::cout << "Bitmap size in byte: " << imgsize << std::endl;

    // picture horizontal resolution
    int resolutionh = buffertoInteger(bitmapheader, 24, 4);
    std::cout << "Resolution (horizontal): " << resolutionh << std::endl;

    // picture vertical resolution
    int resolutionv = buffertoInteger(bitmapheader, 28, 4);
    std::cout << "Resolution (vertical): " << resolutionv << std::endl;

    // picture color count
    int colorcount = buffertoInteger(bitmapheader, 32, 4);
    std::cout << "Color count: " << colorcount << std::endl;

    // picture important color count
    int importantcolorcount = buffertoInteger(bitmapheader, 36, 4);
    std::cout << "Color count (I): " << importantcolorcount << std::endl;

    /**********************************************/
    /* THIS SECTION READS */
    /* BITMAP COLOR TABLE */
    /* BITMAP COLOR TABLE IS UNDEFINED */
    /**********************************************/
    Pixel **data = new Pixel*[height];
    for (int i = 0; i < height; i++) {
      data[i] = new Pixel[width];
    }

    if (offset > filesize + headersize) {

      // int bitsize = pixelcount /  8;

      // int colortablesize = offset - (filesize + headersize);
      Pixel *color = new Pixel[colorcount];


      char *colorbuff = new char[colorcount*4];

      // READ COLOR TABLE
      fileread.seekg(54);
      fileread.read(colorbuff, colorcount*4);

      // INSERT INTO COLOR TABLE
      // NOTE THAT COLOR TABLE EXIST ONLY ON GRAYSCALE FORMAT
      for (int i = 0; i < colorcount; i++) {
        color[i].setBlue((unsigned char) colorbuff[i*4]);
        color[i].setGreen((unsigned char) colorbuff[i*4 + 1]);
        color[i].setRed((unsigned char) colorbuff[i*4 + 2]);

        // PrintPixel(color[i]);
      }

      // READ INDEX
      buff = new unsigned char[width];
      int coloridx;

      for (int i = 0; i < height; i++) {
        fileread.read((char *) buff, width);
        for (int j = 0; j < width; j ++) {
          coloridx = buffertoInteger((char *) buff + j, 0, 1);
          data[i][j].setBlue(colorbuff[coloridx]);
          data[i][j].setGreen(colorbuff[coloridx]);
          data[i][j].setRed(colorbuff[coloridx]);
        }
      }

    }

    /**********************************************/
    /* THIS SECTION READS */
    /* BITMAP COLOR */
    /* BITMAP COLOR STORED WITH PIXEL IF COLOR TABLE DOES NOT EXIST */
    /**********************************************/

    else {

      int bitsize = pixelcount /  8;
      buff = new unsigned char[bitsize * height * width];
      fileread.seekg(offset);

      fileread.read((char *) buff, bitsize * height * width);
      for (int i = 0; i < height; i++) {
        for (int j = 0; j < width ; j++) {
          data[i][j].setBlue((unsigned char) buff[(height - 1 - i) * width * bitsize + j*bitsize]);
          data[i][j].setGreen((unsigned char) buff[(height - 1 - i) * width * bitsize + j*bitsize + 1]);
          data[i][j].setRed((unsigned char) buff[(height - 1 - i) * width * bitsize + j*bitsize + 2]);
        }
      }

    }

    fileread.close();
    return new Image(width, height, data, "bmp");

  } else {
    // std::cout << "Not Opened" << std::endl;
    throw std::runtime_error("BMPIMageLoader::loadBM: " + fileUrl + " is unreadable.");
  }
}

int buffertoInteger(char* buffer, int start, int size) {
  int total = 0;
  for (int i = 0; i < size; i++) {
    total += int((unsigned char)(buffer[start + i]) << (8 * i));
  }

  int z = total;
  return z;
}

void PrintPixel(Pixel pixel) {
    unsigned char color = pixel.getRed();
    std::cout << buffertoInteger((char *) &color, 0, 1) << " ";
    color = pixel.getGreen();
    std::cout << buffertoInteger((char *) &color, 0, 1) << " ";
    color = pixel.getBlue();
    std::cout << buffertoInteger((char *) &color, 0, 1) << std::endl;;
}
