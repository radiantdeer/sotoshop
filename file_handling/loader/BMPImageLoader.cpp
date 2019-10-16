#include "BMPImageLoader.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../../spdlog/spdlog.h"

#define BMP_HEADER_WINDOWS "BM"

int buffertoInteger(char* buffer, int start, int size);

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
    spdlog::debug("BMPImageLoader::load: Bitmap format: {}", fileformat);

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
    spdlog::debug("BMPImageLoader::loadBM: Opened");

    /**********************************************/
    /* THIS SECTION READS */
    /* FILE HEADER */
    /* FILE HEADER SIZE IS 14 BYTES */
    /**********************************************/
    fileread.read(fileheader, 14);

    /* READ SIZE */
    // fileread.read(sizebuffer, 4);
    int filesize = buffertoInteger(fileheader, 2, 4);
    spdlog::debug("BMPImageLoader::loadBM: File size: {}", filesize);

    /* READ OFFSET */
    int offset = buffertoInteger(fileheader, 10, 4);
    spdlog::debug("BMPImageLoader::loadBM: Offset: {}", offset);

    /**********************************************/
    /* THIS SECTION READS */
    /* BITMAP HEADER */
    /* BITMAP HEADER SIZE IS 40 BYTES */
    /**********************************************/
    fileread.read(BitmapHeaderSize, 4);
    int headersize = buffertoInteger(BitmapHeaderSize, 0, 4);
    spdlog::debug("BMPImageLoader::loadBM: Header size: {}", headersize);
    fileread.seekg(14);
    char *bitmapheader = new char[headersize];

    /* READ BITMAP HEADER DETAIL */
    fileread.read(bitmapheader, headersize);

    // picture width
    int width = buffertoInteger(bitmapheader, 4, 4);
    // picture height
    int height = buffertoInteger(bitmapheader, 8, 4);
    spdlog::debug("BMPImageLoader::loadBM: Dimension: {}x{}", width, height);

    // picture plane
    int plane = buffertoInteger(bitmapheader, 12, 2);
    spdlog::debug("BMPImageLoader::loadBM: Plane: {}", plane);

    // picture pixel count
    int pixelcount = buffertoInteger(bitmapheader, 14, 2);
    spdlog::debug("BMPImageLoader::loadBM: Pixel count in bits: {}", pixelcount);

    // picture compression
    int compression = buffertoInteger(bitmapheader, 16, 4);
    if (compression) {
      spdlog::debug("BMPImageLoader::loadBM: Compressed");
    } else {
      spdlog::debug("BMPImageLoader::loadBM: Uncompressed");
    }

    // picture image size
    int imgsize = buffertoInteger(bitmapheader, 20, 4);
    spdlog::debug("BMPImageLoader::loadBM: Bitmap size in byte: {}", imgsize);

    // picture horizontal resolution
    int resolutionh = buffertoInteger(bitmapheader, 24, 4);
    spdlog::debug("BMPImageLoader::loadBM: Resolution (horizontal): {}", resolutionh);

    // picture vertical resolution
    int resolutionv = buffertoInteger(bitmapheader, 28, 4);
    spdlog::debug("BMPImageLoader::loadBM: Resolution (vertical): {}", resolutionv);

    // picture color count
    int colorcount = buffertoInteger(bitmapheader, 32, 4);
    spdlog::debug("BMPImageLoader::loadBM: Color count: {}", colorcount);

    // picture important color count
    int importantcolorcount = buffertoInteger(bitmapheader, 36, 4);
    spdlog::debug("BMPImageLoader::loadBM: Color count (I): {}", importantcolorcount);

    /**********************************************/
    /* THIS SECTION READS */
    /* BITMAP COLOR TABLE */
    /* BITMAP COLOR TABLE IS UNDEFINED */
    /**********************************************/
    Pixel **data = new Pixel*[height];
    for (int i = 0; i < height; i++) {
      data[i] = new Pixel[width];
    }

    if (colorcount > 0) {

      spdlog::debug("BMPImageLoader::loadBM: With color table format");

      int bitsize = pixelcount /  8;
      buff = new unsigned char[width * height * bitsize];

      // READ COLOR INDEX
      fileread.seekg(offset);
      fileread.read((char *) buff, width * height * bitsize);

      // READ COLOR TABLE
      // File might be too large to load, so close and reopen file
      fileread.close();
      fileread.clear();
      fileread.open(fileUrl.c_str());

      Pixel *color = new Pixel[colorcount];
      char *colorbuff = new char[colorcount*4];

      fileread.seekg(14 + headersize);
      fileread.read(colorbuff, colorcount*4);

      // INSERT INTO COLOR TABLE
      // NOTE THAT COLOR TABLE EXIST ONLY ON GRAYSCALE FORMAT
      for (int i = 0; i < colorcount; i++) {
        color[i].setBlue((unsigned char) colorbuff[i*4]);
        color[i].setGreen((unsigned char) colorbuff[i*4 + 1]);
        color[i].setRed((unsigned char) colorbuff[i*4 + 2]);

        spdlog::debug("{}", color[i].toString());
      }

      // READ INDEX
      for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j ++) {
          int k = buffertoInteger((char *) buff + (height - 1 - i) * width + j, 0, 1);
          data[i][j].setBlue(color[k].getBlue());
          data[i][j].setGreen(color[k].getGreen());
          data[i][j].setRed(color[k].getRed());
        }
      }

      delete [] buff;
    }

    /**********************************************/
    /* THIS SECTION READS */
    /* BITMAP COLOR */
    /* BITMAP COLOR STORED WITH PIXEL IF COLOR TABLE DOES NOT EXIST */
    /**********************************************/

    else {
      spdlog::debug("BMPImageLoader::loadBM: Color format");
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

      delete [] buff;

    }

    fileread.close();
    return new Image(width, height, data, "bmp");

  } else {
    throw std::runtime_error("BMPImageLoader::loadBM: " + fileUrl + " is unreadable.");
  }
}

int BMPImageLoader::buffertoInteger(char* buffer, int start, int size) {
  int total = 0;
  for (int i = 0; i < size; i++) {
    total += int((unsigned char)(buffer[start + i]) << (8 * i));
  }

  int z = total;
  return z;
}
