#ifndef __MAINWINDOW_HPP
#define __MAINWINDOW_HPP

#include <QAction>
#include <QMainWindow>
#include <QObject>
#include <QSignalMapper>
#include <string>

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
        void refresh();
        void doMeanFilter();
        void doMedianFilter();
        void doHighPassFilter(int filterVariation);
        void grayLevelSlicing();

        void equalizeImageHist();
        void showHistogram();
        void specifyHist();

        void showBitPlanes();

    private:
        QAction * loadAction;
        QAction * saveAction;

        QAction * negativeImageAction;
        QAction * convertToGrayscaleAction;
        QAction * moveAction;
        QAction * rotateAction;
        QAction * flipAction;
        QAction * zoomAction;

        QAction * meanFilterAction;
        QAction * medianFilterAction;
        QAction * highPassFilter1Action;
        QAction * highPassFilter2Action;
        QAction * highPassFilter3Action;
        QAction * highPassFilter4Action;

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

        QAction * equalizeAction;
        QAction * histogramAction;
        QAction * specifyHistAction;

        QAction * bitPlaneAction;

        DrawSurface * drawSurface;
        HistogramDialog * histDialog;
        BitPlaneDialog * bitPlaneDialog;

        void connectActionsToControllers();
        std::string getOpenFileUrl(std::string dialogTitle);
        std::string getSaveFileUrl(std::string dialogTitle);
        bool askForPadding();
        static std::string getFileExtension(std::string fileUrl);
        int promptValue(std::string promptTitle, std::string promptText);

};

#endif
