#include <iostream>
#include "PBMImageLoader.hpp"

using namespace std;

int main() {
    string filename;
    cin >> filename;
    PBMImageLoader loader = PBMImageLoader();
    Image testImage = loader.load(filename);
    cout << "Image Dimension: " << testImage.getWidth() << "x" << testImage.getHeight() << "\n";
    for (int i = 0; i < testImage.getHeight(); i++) {
        for(int j = 0; j < testImage.getWidth(); j++) {
            printf("(%3d, %3d, %3d) ", testImage.getPixelAt(j, i).getRed(), testImage.getPixelAt(j, i).getGreen(), testImage.getPixelAt(j, i).getBlue());
        }
        cout << "\n";
    }
}