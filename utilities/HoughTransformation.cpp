#include "HoughTransformation.hpp";
#include "Convolution.hpp";
#include "CommonConvolutions.hpp";
#include "../spdlog/spdlog.h"


#include <cmath>

#ifndef M_PI
#define M_PI 3.1415927
#endif

double radian(int deg) {
    return (deg*M_PI)/180;
}

Image* HoughTransformation::HoughLine(Image* input) {
    Image* sobelInput = Convolution::sobelOperation(input, CommonConvolutions::SobelX, CommonConvolutions::SobelY);

    /*** Convert edges into Hough space ***/
    std::vector<std::vector<int>> htParam; // Menyimpan parameter nilai tepi
    float sqrtD = sqrt((input->getHeight() * input->getHeight()) + (input->getWidth() * input->getWidth()));
    int lenhtR = ((int) sqrtD) * 2; // range dari -sqrtD sampai sqrtD
    int lenhtTheta = 180;           // range dari -90 sampe 90. Ubah jadi radian dengan param*PI/180
    const int rStart = (int) -sqrtD;
    const int rEnd = (int) sqrtD;
    const int thetaStart = 0;
    const int thetaEnd = 180;

    // Initialize parameter values
    for (int i = thetaStart; i <= thetaEnd; i++ ) {
        std::vector<int> thetaComponent;
        for (int j = rStart; j < rEnd; j++ ) {
            thetaComponent.push_back(0);
        }
        htParam.push_back(thetaComponent);
    }
    spdlog::info("Created param variable...");
    // create sin and cos lookup table
    double sinTable[181];
    double cosTable[181];
    for (int i = thetaStart; i <= thetaEnd; i++) {
        double thetaRadian = radian(i);
        sinTable[i] = sin(thetaRadian);
        cosTable[i] = cos(thetaRadian);
    }
    spdlog::info("Created lookup table...");
    // run through image
    for (int x = 0; x < sobelInput->getWidth(); x++) {
        for (int y = 0; y < sobelInput->getHeight(); y++) {
            if (sobelInput->getPixelAt(x,y).getRed() == 255) {
                for (int theta = thetaStart; theta <= thetaEnd; theta++) {
                    float j = cosTable[theta]*x + sinTable[theta]*y;
                    j += 0.5;
                    int r = floor(j);
                    htParam[theta][r + ((int)sqrtD)]++;
                }
            }
        }
    }
    spdlog::info("Created param...");
    // htParam is Hough function parameter

    /*** do Thresholding ***/
    const int T = 100;
    int m = htParam.size();
    int n = htParam.at(0).size();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (htParam[i][j] < T) htParam[i][j] = 0; else htParam[i][j] = 1;
        }
    }
    spdlog::info("Done thresholding...");
    /*** Inverse into image ***/
    // Membuat citra baru
    Image* outputImage = new Image(input->getWidth(), input->getHeight());
    // create lines from p vector
    for (int theta = thetaStart; theta <= thetaEnd; theta++ ) {
        for (int r = rStart; r < rEnd; r++) {
            double j = 0.0f;
            if (htParam[theta][r + ((int)sqrtD)] == 1) {
                for (int x = 0; x < outputImage->getWidth(); x++) {
                    if (sinTable[theta] == (double)0.0f) {
                        j++;
                    } else {
                        j = (r - (double)x*cosTable[theta]) / sinTable[theta];
                    }
                    j += 0.5;
                    int y = floor(j);
                    if (y >= 0 && y < outputImage->getHeight()) {
                        outputImage->setPixelAt(x, y, Pixel(255,255,255));
                    }
                }
            }
        }
    }
    spdlog::info("Done inversing...");
    return outputImage;
}
