#ifndef __IMGHISTOGRAM_HPP
#define __IMGHISTOGRAM_HPP

#include <vector>

class ImageHistogram {

    public:
        ImageHistogram();
        ImageHistogram(std::vector<std::vector<int>> data, bool grayscale);
        std::vector<int> getRed() const;
        std::vector<int> getGreen() const;
        std::vector<int> getBlue() const;
        std::vector<int> getGray() const;

        // channel = 'R', 'G', 'B' (you don't need to set this if image is grayscale)
        // If the image/histogram is in RGB and channel is set to 'Y', it will update all channels
        int getValueAt(int graylevel, char channel = 'R') const;
        void setValueAt(int graylevel, int value, char channel = 'R');
        void incrementValueAt(int graylevel, int incrementBy = 1, char channel = 'R');

        bool isGrayscale() const;
        void setGrayscale(bool grayscale);

        double mean(char channel = 'R');
        double median(char channel = 'R');
        int mode(char channel = 'R');

    private:
        std::vector<int> redData;
        std::vector<int> greenData;
        std::vector<int> blueData;
        double meanRed;
        double medianRed;
        int modeRed;
        double meanGreen;
        double medianGreen;
        int modeGreen;
        double meanBlue;
        double medianBlue;
        int modeBlue;
        bool grayscale;
        bool recalculateMean;
        bool recalculateMedian;
        bool recalculateMode;
        void calculateMean();
        void calculateMedian();
        void calculateMode();

};

#endif