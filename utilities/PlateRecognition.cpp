#include "PlateRecognition.hpp"
#include "Convolution.hpp"
#include "CommonConvolutions.hpp"

int median(int l, int r) { 
    int n = r - l + 1; 
    n = (n + 1) / 2 - 1; 
    return n + l; 
} 

double PlateRecognition::euclidean(Point& p1, Point& p2) {
    return sqrt((p1.getX() - p2.getX()) * (p1.getX() - p2.getX()) + (p1.getY() - p2.getY()) * (p1.getY() - p2.getY()));
}

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

Image * PlateRecognition::findPlate(Image * image) {
    spdlog::debug("PlateRecognition::findPlate: Running...");

    spdlog::debug("PlateRecognition::findPlate: Allocating memory for image result...");
    Image * mask = new Image(*image);
    Image * result = new Image(*image);

    mask = Convolution::sobelOperation(mask, CommonConvolutions::SobelX, CommonConvolutions::SobelY);

    // Binarization
    spdlog::debug("PlateRecognition::findPlate: Binary thresholding...");
    for (int j = 0; j < (mask->getHeight()); j += 1) {
        for (int i = 0; i < (mask->getWidth()); i += 1) {
            int val = mask->getPixelAt(i, j).getRed();
            if (val < 128) {
                mask->setPixelAt(i, j, Pixel(0, 0, 0));
            } else {
                mask->setPixelAt(i, j, Pixel(255, 255, 255));
            }
        }
    }

    // CCA - Connected-Component Analysis
    spdlog::debug("PlateRecognition::findPlate: CCA first run...");
    int ccaMatrix[mask->getHeight()][mask->getWidth()];
    for (int j = 0; j < mask->getHeight(); j++) {
        for (int i = 0; i < mask->getWidth(); i++) {
            ccaMatrix[j][i] = 0;
        }
    }
    std::vector<Point> queue;
    std::vector<numOfLabel> labelSum (1, numOfLabel(0, 0));
    int label = 1;
    for (int j = 0; j < mask->getHeight(); j++) {
        for (int i = 0; i < mask->getWidth(); i++) {
            if ((mask->getPixelAt(i, j).getRed() == 255) && (ccaMatrix[j][i] == 0)) {
                labelSum.push_back(numOfLabel(label, 0));
                ccaMatrix[j][i] = label;
                labelSum[label].num++;
                queue.insert(queue.begin(), Point(i, j));
                while(queue.size() > 0) {
                    Point current = queue.front();
                    int x = current.getX();
                    int y = current.getY();
                    queue.erase(queue.begin());
                    if ((x - 1) >= 0) {
                        if ((mask->getPixelAt(x-1,j).getRed() == 255) && (ccaMatrix[j][x-1] == 0)) {
                            ccaMatrix[j][x-1] = label;
                            labelSum[label].num++;
                            queue.push_back(Point(x-1, j));
                        }
                    }
                    if ((x + 1) < mask->getWidth()) {
                        if ((mask->getPixelAt(x+1,j).getRed() == 255) && (ccaMatrix[j][x+1] == 0)) {
                            ccaMatrix[j][x+1] = label;
                            labelSum[label].num++;
                            queue.push_back(Point(x+1, j));
                        }
                    }
                    if ((y - 1) >= 0) {
                        if ((mask->getPixelAt(i, y-1).getRed() == 255) && (ccaMatrix[y-1][i] == 0)) {
                            ccaMatrix[y-1][i] = label;
                            labelSum[label].num++;
                            queue.push_back(Point(i, y-1));
                        }
                    }
                    if ((y + 1) < mask->getHeight()) {
                        if ((mask->getPixelAt(i, y+1).getRed() == 255) && (ccaMatrix[y+1][i] == 0)) {
                            ccaMatrix[y+1][i] = label;
                            labelSum[label].num++;
                            queue.push_back(Point(i, y+1));
                        }
                    }
                }
                label++;
            }
        }
    }

    spdlog::debug("PlateRecognition::findPlate: Sorting CCA result...");
    std::sort(labelSum.begin(), labelSum.end(), std::greater<>());

    spdlog::debug("PlateRecognition::findPlate: Showing only top 28 result...");
    for (int j = 0; j < mask->getHeight(); j++) {
        for (int i = 0; i < mask->getWidth(); i++) {
            bool found = false;
            for (int k = 0; (k < 28) && (k < labelSum.size()); k++) {
                if (ccaMatrix[j][i] == labelSum[k].label) {
                    found = true;
                    break;
                }
            }
            if (found) {
                mask->setPixelAt(i, j, Pixel(255, 255, 255));
            } else {
                mask->setPixelAt(i, j, Pixel(0, 0, 0));
            }
        }
    }

    spdlog::debug("PlateRecognition::findPlate: Running scanline...");
    for (int j = 0; j < mask->getHeight(); j++) {
        int firstEdge = -1;
        int lastEdge = -1;
        bool colorThis = false;
        for (int i = 0; i < mask->getWidth(); i++) {
            if (mask->getPixelAt(i, j).getRed() == 255) {
                if (firstEdge == -1) {
                    firstEdge = i;
                } else {
                    lastEdge = i;
                }
            }
        }
        for (int i = firstEdge; i < lastEdge; i++) {
            mask->setPixelAt(i, j, Pixel(255, 255, 255));
        }
    }

    result->and_op(*mask);

    return result;
}

/*
// Vertical check to find possible vertical edges of plate number
for (int i = 0; i < image->getWidth(); i++) {
    for (int j = 0; j < image->getHeight(); j++) {
        if (!visited[j][i]) {
            visited[j][i] = true;
            int val = image->getPixelAt(i, j).getRed();
            if (val == 255) {
                std::vector<Point> undoBuffer;
                undoBuffer.push_back(Point(i, j));
                image->setPixelAt(i, j, Pixel(255, 0, 0));
                int w = i;
                int k;

                // vertical detection
                for (k = j; k < image->getHeight(); k++) {
                    visited[k][w] = true;
                    int val2 = image->getPixelAt(w, k).getRed();
                    undoBuffer.push_back(Point(w, k));
                    image->setPixelAt(w, k, Pixel(255, 0, 0));
                    if (val2 != 255) {
                        int valLeft = (w - 1) >= 0 ? image->getPixelAt(w - 1, k).getRed() : 0;
                        int valRight = (w + 1) < image->getWidth() ? image->getPixelAt(w + 1, k).getRed() : 0;
                        if (valLeft == 255) {
                            w--;
                            visited[k][w] = true;
                            undoBuffer.push_back(Point(w, k));
                            image->setPixelAt(w, k, Pixel(255, 0, 0));
                        } else if (valRight == 255) {
                            w++;
                            visited[k][w] = true;
                            undoBuffer.push_back(Point(w, k));
                            image->setPixelAt(w, k, Pixel(255, 0, 0));
                        } else {
                            break;
                        }
                    }
                }

                if ((k - 1 - j) <= 5) {
                    // undo this 
                    for (int x = 0; x < undoBuffer.size(); x++) {
                        Point thisPoint = undoBuffer[x];
                        image->setPixelAt(thisPoint.getX(), thisPoint.getY(), Pixel(255, 255, 255));
                    }
                }

                undoBuffer.clear();

                // horizontal detection
                k = j;
                for (w = i; w < image->getWidth(); w++) {
                    visited[k][w] = true;
                    int val2 = image->getPixelAt(w, k).getRed();
                    if (val2 != 255) {
                        int valUp = (k - 1) >= 0 ? image->getPixelAt(w, k - 1).getRed() : 0;
                        int valDown = (k + 1) < image->getHeight() ? image->getPixelAt(w, k + 1).getRed() : 0;
                        if (valUp == 255) {
                            k--;
                            visited[k][w] = true;
                            undoBuffer.push_back(Point(w, k));
                            image->setPixelAt(w, k, Pixel(255, 0, 0));
                        } else if (valDown == 255) {
                            k++;
                            visited[k][w] = true;
                            undoBuffer.push_back(Point(w, k));
                            image->setPixelAt(w, k, Pixel(255, 0, 0));
                        } else {
                            break;
                        }
                    } else {
                        undoBuffer.push_back(Point(w, k));
                        image->setPixelAt(w, k, Pixel(255, 0, 0));
                    }
                }
                
                if ((w - 1 - i) <= 5) {
                    // undo this 
                    for (int x = 0; x < undoBuffer.size(); x++) {
                        Point thisPoint = undoBuffer[x];
                        image->setPixelAt(thisPoint.getX(), thisPoint.getY(), Pixel(255, 255, 255));
                    }
                }

                undoBuffer.clear();

            }
        }
    }
}
*/

/*
if (colorThis) {
    if (val < 128) {
        // still inside polygon
        if (previousEdge) {
            previousEdge = false;
        }
        image->setPixelAt(i, j, Pixel(255, 255, 255));
    } else {
        if (!previousEdge) { // found the end of this polygon
            colorThis = false;
            previousEdge = true;
        } // else, it's still the first edge of the polygon, continue
    }
} else {
    if (val >= 128) { // found a polygon side
        previousEdge = true;
        colorThis = true;
    }
}
*/

//double avg = 0;
//double variance = 0;
//int num = 0;
/*
std::vector<int> intensity;
for (int l = j; (l < j + WINDOW_HEIGHT); l++) {
    if (l > image->getHeight()) {
        break;
    }
    for (int k = i; (k < i + WINDOW_WIDTH); k++) {
        if (k > image->getWidth()) {
            break;
        }
        int val = image->getPixelAt(k, l).getRed();
        int x;
        for (x = 0; x < intensity.size(); x++) {
            if (intensity[x] > val) {
                break;
            }
        }
        intensity.insert(intensity.begin() + x, val);
        //avg += (int) image->getPixelAt(k, l).getRed();
        //num++;
    }
}
int medianPos = median(0, intensity.size());
int q1Pos = median(0, medianPos);
int q3Pos = median(medianPos, intensity.size());
double eiqr = 1.5 * (intensity[q3Pos] - intensity[q1Pos]);
int range = intensity[intensity.size() - 1] - intensity[0];
intensity.clear();
//avg = avg / num;
//spdlog::debug("avg : {}", avg);
*/

/*
for (int l = j; (l < j + WINDOW_HEIGHT); l++) {
    if (l > image->getHeight()) {
        break;
    }
    for (int k = i; (k < i + WINDOW_WIDTH); k++) {
        if (k > image->getWidth()) {
            break;
        }
        //variance += pow(avg - image->getPixelAt(k, l).getRed(), 2);
    }
}
*/

//variance = variance / num;
//spdlog::debug("variance : {}", variance);
//double stdev = 0;
//stdev = sqrt(variance);

/*
if ((range >= 100)) {
    spdlog::debug("({}, {}) => R : {} | IQR : {}", i, j, range, eiqr);
} else {
    for (int l = j; (l < j + WINDOW_HEIGHT); l++) {
        if (l > image->getHeight()) {
            break;
        }
        for (int k = i; (k < i + WINDOW_WIDTH); k++) {
            if (k > image->getWidth()) {
                break;
            }
            image->setPixelAt(k, l, Pixel(0, 0, 0));
        }
    }
}
*/