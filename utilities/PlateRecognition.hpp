#ifndef __PLATE_RECOGNITION
#define __PLATE_RECOGNITION

#include <algorithm>
#include <vector>
#include "../datastruct/Image.hpp"
#include "../spdlog/spdlog.h"
#include <cmath>

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

class PlateRecognition {

    public:
        static double euclidean(Point& p1, Point& p2);
        static Image * findPlate(Image * image);

    private:
        static std::vector<numOfLabel> connectedComponentAnalysis(Image * image, int ** ccaMatrix, bool lineAnalysis);

};

#endif