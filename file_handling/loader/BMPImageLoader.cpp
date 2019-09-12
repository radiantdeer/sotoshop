#include "BMPImageLoader.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define BMP_HEADER "BM"

int buffertoInteger(char* buffer, int start, int size);

BMPImageLoader::BMPImageLoader() {

}

Image BMPImageLoader::load(std::string fileUrl) {
  std::string line;

  char *fileheader = new char[14];

  char *fileformat = new char[2];
  char *BitmapHeaderSize = new char[4];
  
  unsigned char z;
  std::ifstream fileread(fileUrl.c_str());
  if (fileread.good()) {
    std::cout << "Opened" << std::endl;
    
    /* READ FILE HEADER */
    fileread.read(fileheader, 14);

    /* GET FILE FORMAT */
    fileformat[0] = fileheader[0];
    fileformat[1] = fileheader[1];
    std::cout << fileformat[0] << fileformat[1] << std::endl;

    /* READ SIZE */
    // fileread.read(sizebuffer, 4);
    int filesize = buffertoInteger(fileheader, 2, 4);
    std::cout << filesize << std::endl;

    /* READ OFFSET */
    int offset = buffertoInteger(fileheader, 10, 4);
    std::cout << offset << std::endl;

    /* READ BITMAP HEADER */
    fileread.read(BitmapHeaderSize, 4);
    int headersize = buffertoInteger(BitmapHeaderSize, 0, 4);
    std::cout << headersize << std::endl;

    fileread.seekg(14);
    char *bitmapheader = new char[headersize];
    fileread.read(bitmapheader, headersize);
    int width = buffertoInteger(bitmapheader, 4, 4);
    int height = buffertoInteger(bitmapheader, 8, 4);
    std::cout << width << " " << height << std::endl;

  } else {
    std::cout << "Not Opened" << std::endl;
  }

  fileread.close();

  delete [] fileheader;
  return Image();
}

int buffertoInteger(char* buffer, int start, int size) {
  int total = 0;
  for (int i = 0; i < size; i++) {
    total += int((unsigned char)(buffer[start + i]) << (8 * i));
  }

  int z = total;
  return z;
}