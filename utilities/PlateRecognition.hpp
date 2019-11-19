#ifndef __PLATE_RECOGNITION
#define __PLATE_RECOGNITION

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include "../datastruct/Image.hpp"
#include "../spdlog/spdlog.h"

class Point {

    public:
        Point(int x = 0, int y = 0) {
            this->x = x;
            this->y = y;
        }

        int getX() const {
            return x;
        }
        
        int getY() const {
            return y;
        }

        void setX(int x) {
            this->x = x;
        }

        void setY(int y) {
            this->y = y;
        }

    private:
        int x;
        int y;

};

class PixelPoint {

    public:
        int intensity;
        Point position;
        PixelPoint * rootPos;

        PixelPoint(Point pos, int intensity) {
            this->position = pos;
            this->intensity = intensity;
            this->rootPos = nullptr;
        }

        bool operator< (PixelPoint& pp) {
            return (this->intensity < pp.intensity);
        }

        bool operator<= (PixelPoint& pp) {
            return (this->intensity <= pp.intensity);
        }

        bool operator> (PixelPoint& pp) {
            return (this->intensity > pp.intensity);
        }

        bool operator>= (PixelPoint& pp) {
            return (this->intensity >= pp.intensity);
        }
};

struct numOfLabel
{
    int label;
    int num;

    numOfLabel(int l, int n) : label(l), num(n) {}

    bool operator > (numOfLabel& a2)
    {
        return (num > a2.num);
    }
};

struct HuMoments {

    double h0;
    double h1;
    double h2;
    double h3;
    double h4;
    double h5;
    double h6;

    double distance(HuMoments& other) {
        return sqrt(pow(h0 - other.h0, 2) + pow(h1 - other.h1, 2) + pow(h2 - other.h2, 2) + pow(h3 - other.h3, 2) + pow(h4 - other.h4, 2) + pow(h5 - other.h5, 2));
    }
 
};

struct CharacterTemplate {

    char character;
    int templateWidth;
    int templateHeight;
    HuMoments constants;

};

class PlateRecognition {

    public:
        static const std::string BIN_TEMPLATE_PATH;

        static double euclidean(Point& p1, Point& p2);
        static Image * findPlate(Image * image);
        static std::string recognizeCharacters(Image * image);
        static double moments(Image * image, int i, int j);
        static double centralMoments(Image * image, int i, int j);
        static double normalizedCentralMoments(Image * image, int i, int j);
        static HuMoments calculateHuMoments(Image * image);
        static void loadCharacterTemplate();

    private:
        static std::vector<CharacterTemplate> templates;
        static std::vector<numOfLabel> connectedComponentAnalysis(Image * image, int ** ccaMatrix, bool lineAnalysis);
};

#endif