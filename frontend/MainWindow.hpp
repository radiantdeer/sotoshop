#ifndef __MAINWINDOW_HPP
#define __MAINWINDOW_HPP

#include <QAction>
#include <QMainWindow>
#include <QObject>
#include <QSignalMapper>
#include <complex>
#include <string>
#include <vector>

#include "BitPlaneDialog.hpp"
#include "DrawSurface.hpp"
#include "HistogramDialog.hpp"
#include "../datastruct/Image.hpp"
#include "../file_handling/loader/ImageLoaderFactory.hpp"
#include "../file_handling/saver/ImageSaverFactory.hpp"

class MainWindow : public QMainWindow {

    public:
        MainWindow();
        QAction * getLoadAction();
        QAction * getSaveAction();
        DrawSurface * getDrawSurface();
        void setActiveImage(Image * image);

    public slots:
        void loadFile();
        void saveFile();
        void makeNegativeImage();
        void convertToGrayscaleImage();
        void brightenImage();
        void moveImage();
        void rotateImageCW();
        void rotateImageCCW();
        void flipImageHorizontal();
        void flipImageVertical();
        void zoomIn();
        void zoomOut();
        void addImage();
        void substractImage();
        void multiplyImage();
        void operateAndImage();
        void operateOrImage();
        void operateNotImage();
        void nthPower();
        void refresh();
        void doMeanFilter();
        void doMedianFilter();
        void doHighPassFilter(int filterVariation);
        void doUnsharpMasking();
        void doHighboost();
        void doGradient();
        void doLaplaceOfGaussian();

        void grayLevelSlicing();
        void contrastStretching(bool automatic);

        void equalizeImageHist();
        void showHistogram();
        void specifyHist();

        void logOperation();
        void invLogOperation();

        void showBitPlanes();
        void doFourierTransform();
        void viewFourierSpectrum();
        void doInverseFourier();

        void sobelOperation();
        void prewittOperation();
        void robertOperation();
        void cannyOperation();

        void doLineHough();
        void doCircleHough();

        void doBinarySegmentation();
        void doBinaryThinning();
        void doPlateRecognition();

    private:
        QAction * loadAction;
        QAction * saveAction;

        QAction * negativeImageAction;
        QAction * convertToGrayscaleAction;
        QAction * moveAction;
        QAction * rotateAction;
        QAction * flipAction;
        QAction * zoomAction;

        QAction * nthPowerAction;
        QAction * logAction;
        QAction * invLogAction;

        QAction * meanFilterAction;
        QAction * medianFilterAction;
        QAction * highPassFilter1Action;
        QAction * highPassFilter2Action;
        QAction * highPassFilter3Action;
        QAction * highPassFilter4Action;
        QAction * unsharpMaskingAction;
        QAction * highboostAction;

        QAction * rotateCWAction;
        QAction * rotateCCWAction;
        QAction * flipHAction;
        QAction * flipVAction;
        QAction * zoomInAction;
        QAction * zoomOutAction;
        QAction * grayLevelSlicingAction;
        QAction * brightenAction;
        QAction * additionAction;
        QAction * substractAction;
        QAction * multiplyAction;
        QAction * andAction;
        QAction * orAction;
        QAction * notAction;
        QAction * contrastStretchingAutoAction;
        QAction * contrastStretchingManualAction;

        QAction * equalizeAction;
        QAction * histogramAction;
        QAction * specifyHistAction;

        QAction * bitPlaneAction;
        QAction * fourierAction;
        QAction * viewFourierSpectrumAction;
        QAction * inverseFourierAction;

        QAction * laplaceAction;
        QAction * gradientAction;
        QAction * laplaceOfGaussianAction;
        QAction * sobelOperationAction;
        QAction * prewittOperationAction;
        QAction * robertOperationAction;
        QAction * cannyOperationAction;

        QAction * lineHoughAction;
        QAction * circleHoughAction;

        QAction * binarySegmentationAction;
        QAction * binaryThinningAction;

        QAction * plateRecognitionAction;

        DrawSurface * drawSurface;
        HistogramDialog * histDialog;
        BitPlaneDialog * bitPlaneDialog;
        std::vector<std::vector<std::vector<std::complex<double>>>> * fourierFrequencies;
        DrawSurface * fourierDialog;
        Image * fourierVisual;

        void connectActionsToControllers();
        std::string getOpenFileUrl(std::string dialogTitle);
        std::string getSaveFileUrl(std::string dialogTitle);
        bool askYesNoQuestion(std::string promptTitle, std::string promptText);
        static std::string getFileExtension(std::string fileUrl);
        int promptValue(std::string promptTitle, std::string promptText);

};

#endif
