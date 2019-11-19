#include <dirent.h>
#include "PlateRecognition.hpp"
#include "Convolution.hpp"
#include "CommonConvolutions.hpp"
#include "../file_handling/loader/ImageLoaderFactory.hpp"
#include "../frontend/BitPlaneDialog.hpp"

const std::string PlateRecognition::BIN_TEMPLATE_PATH = "/media/antonio/Data/Documents/UNIV/Sem VII/Pengcit/Templates/binary";
std::vector<CharacterTemplate> PlateRecognition::templates = std::vector<CharacterTemplate>(0);

int median(int l, int r) { 
    int n = r - l + 1; 
    n = (n + 1) / 2 - 1; 
    return n + l; 
} 

double PlateRecognition::euclidean(Point& p1, Point& p2) {
    return sqrt((p1.getX() - p2.getX()) * (p1.getX() - p2.getX()) + (p1.getY() - p2.getY()) * (p1.getY() - p2.getY()));
}

Image * PlateRecognition::findPlate(Image * image) {
    spdlog::debug("PlateRecognition::findPlate: Running...");

    spdlog::debug("PlateRecognition::findPlate: Allocating memory for image result...");
    Image * mask = new Image(*image);

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
    int ** ccaMatrix = new int * [mask->getHeight()];
    for (int j = 0; j < mask->getHeight(); j++) {
        ccaMatrix[j] = new int[mask->getWidth()];
        for (int i = 0; i < mask->getWidth(); i++) {
            ccaMatrix[j][i] = 0;
        }
    }

    std::vector<numOfLabel> labelSum = connectedComponentAnalysis(mask, ccaMatrix, true);

    spdlog::debug("PlateRecognition::findPlate: Sorting CCA result...");
    std::sort(labelSum.begin(), labelSum.end(), std::greater<>());

    spdlog::debug("PlateRecognition::findPlate: Showing only top 25 result...");
    for (int j = 0; j < mask->getHeight(); j++) {
        for (int i = 0; i < mask->getWidth(); i++) {
            bool found = false;
            for (int k = 0; (k < 25) && (k < labelSum.size()); k++) {
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
        for (int i = 0; i < mask->getWidth(); i++) {
            if (mask->getPixelAt(i, j).getRed() == 255) {
                if (firstEdge == -1) {
                    firstEdge = i;
                } else {
                    if (ccaMatrix[j][firstEdge] != ccaMatrix[j][i]) {
                        lastEdge = i;
                    } else {
                        firstEdge = i;
                    }
                }
            }
        }
        if (lastEdge != -1) {
            // color prevEdge -> lastEdge with white
            for (int i = firstEdge; i < lastEdge; i++) {
                mask->setPixelAt(i, j, Pixel(255, 255, 255));
            }

            // compensating for crooked rectangles
            int y = j - 1;
            if (y >= 0) {
                bool stop = false;
                int x = lastEdge;
                while ((x >= 0) && !stop) {
                    if (mask->getPixelAt(x, y).getRed() == 255) {
                        stop = true;
                    } else {
                        x--;
                    }
                }
                if (stop) {
                    int prevFirstEdge = x;
                    stop = false;
                    while(!stop) {
                        for (int i = prevFirstEdge; i < lastEdge; i++) {
                            mask->setPixelAt(i, y, Pixel(255, 255, 255));
                        }
                        y--;
                        if (y >= 0) {
                            if (mask->getPixelAt(prevFirstEdge, y).getRed() == 255) {
                                prevFirstEdge = prevFirstEdge;
                            } else if (mask->getPixelAt(prevFirstEdge - 1, y).getRed() == 255) {
                                prevFirstEdge--;
                            } else if (mask->getPixelAt(prevFirstEdge + 1, y).getRed() == 255) {
                                prevFirstEdge++;
                            } else {
                                stop = true;
                            }
                        } else {
                            stop = true;
                        }
                    }
                }
            }
        }
    }
    
    // Second CCA - to determine largest region : where a plate number might be there
    spdlog::debug("PlateRecognition::findPlate: Running second CCA from first run...");
    for (int j = 0; j < mask->getHeight(); j++) {
        for (int i = 0; i < mask->getWidth(); i++) {
            ccaMatrix[j][i] = 0;
        }
    }

    labelSum = connectedComponentAnalysis(mask, ccaMatrix, false);
    std::sort(labelSum.begin(), labelSum.end(), std::greater<>());
    int largestLabel = labelSum[0].label;
    for (int i = 0; i < labelSum.size(); i++) {
        spdlog::debug("#{} : {}", labelSum[i].label, labelSum[i].num);
    }

    // Make image only shows this largest blob
    int left = 0;
    int top = 0;
    int right = image->getWidth() - 1;
    int bottom = image->getHeight() - 1;

    // Left
    bool stop = false;
    int i, j;
    for (i = 0; (i < mask->getWidth()) && !stop; i++) {
        for (j = 0; (j < mask->getHeight()) && !stop; j++) {
            if (ccaMatrix[j][i] == largestLabel) {
                stop = true;
            }
        }
    }
    left = i;

    // Top
    stop = false;
    for (j = 0; (j < mask->getHeight()) && !stop; j++) {
        for (i = 0; (i < mask->getWidth()) && !stop; i++) {
            if (ccaMatrix[j][i] == largestLabel) {
                stop = true;
            }
        }
    }
    top = j;

    // Right
    stop = false;
    for (i = mask->getWidth() - 1; (i >= 0) && !stop; i--) {
        for (j = 0; (j < mask->getHeight()) && !stop; j++) {
            if (ccaMatrix[j][i] == largestLabel) {
                stop = true;
            }
        }
    }
    right = i;

    // Bottom
    stop = false;
    for (j = mask->getHeight() - 1; (j >= 0) && !stop; j--) {
        for (i = 0; (i < mask->getWidth()) && !stop; i++) {
            if (ccaMatrix[j][i] == largestLabel) {
                stop = true;
            }
        }
    }
    bottom = j;
    
    spdlog::debug("({},{}) ==> ({},{})", left, top, right, bottom);
    int resultWidth = right - left;
    int resultHeight = bottom - top;
    Image * result = new Image(resultWidth + 1, resultHeight + 1);
    for (int j = top; j <= bottom; j++) {
        for (int i = left; i <= right; i++) {
            result->setPixelAt(i - left, j - top, image->getPixelAt(i, j));
        }
    }

    delete ccaMatrix;
    delete mask;

    return result;
}

std::vector<numOfLabel> PlateRecognition::connectedComponentAnalysis(Image * image, int ** ccaMatrix, bool lineAnalysis) {
    std::vector<Point> queue;
    std::vector<numOfLabel> labelSum (1, numOfLabel(0, 0));
    int label = 1;
    for (int j = 0; j < image->getHeight(); j++) {
        for (int i = 0; i < image->getWidth(); i++) {
            if ((image->getPixelAt(i, j).getRed() == 255) && (ccaMatrix[j][i] == 0)) {
                labelSum.push_back(numOfLabel(label, 0));
                ccaMatrix[j][i] = label;
                labelSum[label].num++;
                queue.insert(queue.begin(), Point(i, j));
                while(queue.size() > 0) {
                    Point current = queue.front();
                    int x = current.getX();
                    int y = current.getY();
                    queue.erase(queue.begin());
                    if (lineAnalysis) {
                        if ((x - 1) >= 0) {
                            if ((image->getPixelAt(x-1,j).getRed() == 255) && (ccaMatrix[j][x-1] == 0)) {
                                ccaMatrix[j][x-1] = label;
                                labelSum[label].num++;
                                queue.push_back(Point(x-1, j));
                            }
                        }
                        if ((x + 1) < image->getWidth()) {
                            if ((image->getPixelAt(x+1,j).getRed() == 255) && (ccaMatrix[j][x+1] == 0)) {
                                ccaMatrix[j][x+1] = label;
                                labelSum[label].num++;
                                queue.push_back(Point(x+1, j));
                            }
                        }
                        if ((y - 1) >= 0) {
                            if ((image->getPixelAt(i, y-1).getRed() == 255) && (ccaMatrix[y-1][i] == 0)) {
                                ccaMatrix[y-1][i] = label;
                                labelSum[label].num++;
                                queue.push_back(Point(i, y-1));
                            }
                        }
                        if ((y + 1) < image->getHeight()) {
                            if ((image->getPixelAt(i, y+1).getRed() == 255) && (ccaMatrix[y+1][i] == 0)) {
                                ccaMatrix[y+1][i] = label;
                                labelSum[label].num++;
                                queue.push_back(Point(i, y+1));
                            }
                        }
                    } else {
                        if ((x - 1) >= 0) {
                            if ((image->getPixelAt(x-1,y).getRed() == 255) && (ccaMatrix[y][x-1] == 0)) {
                                ccaMatrix[y][x-1] = label;
                                labelSum[label].num++;
                                queue.push_back(Point(x-1, y));
                            }
                        }
                        if ((x + 1) < image->getWidth()) {
                            if ((image->getPixelAt(x+1,y).getRed() == 255) && (ccaMatrix[y][x+1] == 0)) {
                                ccaMatrix[y][x+1] = label;
                                labelSum[label].num++;
                                queue.push_back(Point(x+1, y));
                            }
                        }
                        if ((y - 1) >= 0) {
                            if ((image->getPixelAt(x, y-1).getRed() == 255) && (ccaMatrix[y-1][x] == 0)) {
                                ccaMatrix[y-1][x] = label;
                                labelSum[label].num++;
                                queue.push_back(Point(x, y-1));
                            }
                        }
                        if ((y + 1) < image->getHeight()) {
                            if ((image->getPixelAt(x, y+1).getRed() == 255) && (ccaMatrix[y+1][x] == 0)) {
                                ccaMatrix[y+1][x] = label;
                                labelSum[label].num++;
                                queue.push_back(Point(x, y+1));
                            }
                        }
                    }
                }
                label++;
            }
        }
    }

    return labelSum;
}

std::string PlateRecognition::recognizeCharacters(Image * image) {
    std::string characters;

    // Binarization
    image->binarySegmentation();

    // Preprocess #1 - if the amount of white (255) is much larger, it may be that the object is black.
    // Let's make a negative image out of it!
    ImageHistogram hist = image->histogram();
    int num0 = hist.getValueAt(0);
    int num255 = hist.getValueAt(255);
    if ((num255 - num0) > 20) {
        image->invert();
    }

    // CCA to detect objects and sort by largest object, which hopefully is the plate characters
    spdlog::debug("PlateRecognition::recognizeCharacter: CCA first run...");
    int ** ccaMatrix = new int * [image->getHeight()];
    for (int j = 0; j < image->getHeight(); j++) {
        ccaMatrix[j] = new int[image->getWidth()];
        for (int i = 0; i < image->getWidth(); i++) {
            ccaMatrix[j][i] = 0;
        }
    }

    std::vector<numOfLabel> labelSum = connectedComponentAnalysis(image, ccaMatrix, false);
    std::sort(labelSum.begin(), labelSum.end(), std::greater<>());
    std::vector<Image> majorObjects;
    for (int n = 0; n < labelSum.size(); n++) {
        if (labelSum[n].num >= 20) {
            spdlog::debug("#{} : {}", labelSum[n].label, labelSum[n].num);
            
            int left = 0;
            int top = 0;
            int right = image->getWidth() - 1;
            int bottom = image->getHeight() - 1;

            // Left
            bool stop = false;
            int i, j;
            for (i = 0; (i < image->getWidth()) && !stop; i++) {
                for (j = 0; (j < image->getHeight()) && !stop; j++) {
                    if (ccaMatrix[j][i] == labelSum[n].label) {
                        stop = true;
                    }
                }
            }
            left = i;

            // Top
            stop = false;
            for (j = 0; (j < image->getHeight()) && !stop; j++) {
                for (i = 0; (i < image->getWidth()) && !stop; i++) {
                    if (ccaMatrix[j][i] == labelSum[n].label) {
                        stop = true;
                    }
                }
            }
            top = j;

            // Right
            stop = false;
            for (i = image->getWidth() - 1; (i >= 0) && !stop; i--) {
                for (j = 0; (j < image->getHeight()) && !stop; j++) {
                    if (ccaMatrix[j][i] == labelSum[n].label) {
                        stop = true;
                    }
                }
            }
            right = i;

            // Bottom
            stop = false;
            for (j = image->getHeight() - 1; (j >= 0) && !stop; j--) {
                for (i = 0; (i < image->getWidth()) && !stop; i++) {
                    if (ccaMatrix[j][i] == labelSum[n].label) {
                        stop = true;
                    }
                }
            }
            bottom = j;

            int resultWidth = right - left;
            int resultHeight = bottom - top;
            Image * thisObject = new Image(resultWidth + 1, resultHeight + 1);

            for (int j = top; j <= bottom; j++) {
                for (int i = left; i <= right; i++) {
                    thisObject->setPixelAt(i - left, j - top, image->getPixelAt(i, j));
                }
            }

            majorObjects.push_back(*thisObject);
            thisObject = thisObject->magnify2();
            HuMoments hm = PlateRecognition::calculateHuMoments(thisObject);
            
            double closest = hm.distance(templates[0].constants);
            char closestChar = templates[0].character;
            for (int i = 1; i < templates.size(); i++) {
                double distance = hm.distance(templates[i].constants);
                if (closest > distance) {
                    closest = distance;
                    closestChar = templates[i].character;
                }
            }
            spdlog::debug("{}", closestChar);
            
            delete thisObject;
        } else {
            break;
        }
    }

    BitPlaneDialog * bitPlaneDialog = new BitPlaneDialog(majorObjects);
    bitPlaneDialog->show();

    delete ccaMatrix;
    return characters;
}


double PlateRecognition::moments(Image * image, int i, int j) {
    double result = 0;

    for (int y = 0; y < image->getHeight(); y++) {
        for (int x = 0; x < image->getWidth(); x++) {
            if (image->getPixelAt(x, y).getRed() == 255) {
                result += (pow(x, i) * pow(y, j));
            }
        }
    }

    return result;
}

double PlateRecognition::centralMoments(Image * image, int i, int j) {
    double xbar = moments(image, 1, 0) / moments(image, 0, 0);
    double ybar = moments(image, 0, 1) / moments(image, 0, 0);
    double result = 0;

    for (int y = 0; y < image->getHeight(); y++) {
        for (int x = 0; x < image->getWidth(); x++) {
            if (image->getPixelAt(x, y).getRed() == 255) {
                result += (pow(x - xbar, i) * pow(y - ybar, j));
            }
        }
    }

    return result;
}

double PlateRecognition::normalizedCentralMoments(Image * image, int i, int j) {
    return (centralMoments(image, i, j) / pow(centralMoments(image, 0, 0), ((i + j) / (3))));
}

HuMoments PlateRecognition::calculateHuMoments(Image * image) {
    HuMoments result;
    
    double n20 = normalizedCentralMoments(image, 2, 0);
    double n02 = normalizedCentralMoments(image, 0, 2);
    result.h0 = n20 + n02;
    result.h1 = pow(n20 - n02, 2) + (4 * pow(normalizedCentralMoments(image, 1, 1), 2));

    double n30 = normalizedCentralMoments(image, 3, 0);
    double n03 = normalizedCentralMoments(image, 0, 3);
    double n11 = normalizedCentralMoments(image, 1, 1);
    double n12 = normalizedCentralMoments(image, 1, 2);
    double n21 = normalizedCentralMoments(image, 2, 1);
    result.h2 = pow(n30 - (3 * n12), 2) + pow((3 * n21) - n03, 2);
    result.h3 = pow(n30 + n12, 2) + pow(n21 + n30, 2);
    result.h4 = (n30 - (3 * n12)) * (n30 + n12) * (pow(n30 + n12, 2) - (3 * pow(n21 + n03, 2)));
    result.h4 += (((3 * n21) - n03) * ((3 * pow(n30 + n12, 2)) - pow(n21 + n03, 2)));
    result.h5 = (n20 - n02) * (pow(n30 + n12, 2) - pow(n21 + n03, 2) + (4 * n11 * (n30 + n12) * (n21 + n03)));

    return result;
}

void PlateRecognition::loadCharacterTemplate() {

    DIR * dir;
    struct dirent *ent;
    spdlog::info("PlateRecognition::loadCharacterTemplate: Loading templates, this may take a while...");
    if ((dir = opendir (BIN_TEMPLATE_PATH.c_str())) != NULL) {
        ent = readdir (dir);
        ent = readdir (dir);
        while ((ent = readdir (dir)) != NULL) {
            ImageLoader * loader = ImageLoaderFactory::getImageLoader(ent->d_name);
            std::string sourceFileName = BIN_TEMPLATE_PATH + "/" + ent->d_name;
            Image * image = loader->load(sourceFileName);
            image = image->magnify2();
            HuMoments hm = PlateRecognition::calculateHuMoments(image);
            std::string pureFileName = std::string(ent->d_name);
            pureFileName = pureFileName.substr(0, pureFileName.find("."));
            
            CharacterTemplate thisTemplate;
            thisTemplate.character = pureFileName[0];
            thisTemplate.templateWidth = image->getWidth();
            thisTemplate.templateHeight = image->getHeight();
            thisTemplate.constants = hm;
            templates.push_back(thisTemplate);

            delete image;
            delete loader;
        }
        closedir (dir);
    }
    spdlog::info("PlateRecognition::loadCharacterTemplate: Template loaded. {} templates present", templates.size());
}