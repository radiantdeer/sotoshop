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
                        Pixel *p = sobelInput->getPixelAt(x,y) & Pixel(255,255,255);
                        outputImage->setPixelAt(x, y, *p);
                    }
                }
            }
        }
    }
    spdlog::info("Done inversing...");
    return outputImage;
}

Image* HoughTransformation::HoughCircle(Image* input, int rStart, int rEnd) {
    Image* sobelInput = Convolution::sobelOperation(input, CommonConvolutions::SobelX, CommonConvolutions::SobelY);
    int thetaStart = 0; int thetaEnd = 360;
    int aStart = 0;     int aEnd = sobelInput->getWidth();
    int bStart = 0;     int bEnd = sobelInput->getHeight();
    // int rEnd = r;
    // int rStart = r;
    const int rRange = rEnd - rStart + 1;

    // a = x - rcos(theta)
    // b = y - rsin(theta)
    // x = a + rcos(theta)
    // y = b + rson(theta)

    // Initiate parameter values
    std::vector<std::vector<int>> hcParam;
    for (int k = 0; k < aEnd; k++) {
        std::vector<int> aParam;
        for (int l = 0; l < bEnd; l++) {
            aParam.push_back(0);
        }
        hcParam.push_back(aParam);
    }
    spdlog::info("Initiate values");

    // run through images
    for (int x = 0; x < sobelInput->getWidth(); x++) {
        for (int y = 0; y < sobelInput->getHeight(); y++) {
            if (sobelInput->getPixelAt(x,y).getRed() == 255) {
                for (int theta = thetaStart; theta <= thetaEnd; theta++) {
                    for (int r = rStart; r <= rEnd; r++) {
                        double aF = x - r*cos(radian(theta));
                        double bF = y - r*sin(radian(theta));
                        int a = floor(aF + 0.5);
                        int b = floor(bF + 0.5);
                        if (a >= 0 && a < aEnd && b >= 0 && b < bEnd) {
                            hcParam[a][b]++;
                        }
                    }
                }
            }
        }
    }
    spdlog::info("Running through images");
    /*** do Thresholding ***/
    const int T = 100;
    for (int a = aStart; a < aEnd; a++) {
        for (int b = bStart; b < bEnd; b++) {
            if (hcParam[a][b] < T) hcParam[a][b] = 0; else hcParam[a][b] = 1;
        }
    }
    spdlog::info("Done thresholding...");

    Image* outputImage = new Image(input->getWidth(), input->getHeight());
    // create lines from p vector
    for (int a = aStart; a < aEnd; a++ ) {
        for (int b = bStart; b < bEnd; b++) {
            double j = 0.0f;
            if (hcParam[a][b] == 1) {
                for (int theta = thetaStart; theta < thetaEnd; theta++) {
                    for (int r = rStart; r <= rEnd; r++) {
                        double xF = a + r*cos(radian(theta));
                        double yF = a + r*sin(radian(theta));
                        int x = floor(xF + 0.5);
                        int y = floor(yF + 0.5);
                        if (x >= 0 && x < outputImage->getWidth() && y >= 0 && y < outputImage->getHeight()) {
                            Pixel *p = sobelInput->getPixelAt(x, y) & Pixel(255, 255, 255);
                            outputImage->setPixelAt(x, y, *p);
                        }
                    }
                }
            }
        }
    }
    // create detected circles
    return outputImage;
}
