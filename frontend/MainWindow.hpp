#ifndef __MAINWINDOW_HPP
#define __MAINWINDOW_HPP

#include <QAction>
#include <QMainWindow>
#include <QObject>
#include <QSignalMapper>
#include <string>

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
        void moveImage();
        void rotateImage();
        void flipImage();
        void zoomImage();
        void refresh();
        void doMeanFilter();
        void doMedianFilter();
        void doHighPassFilter(int filterVariation);
        void showHistogram();

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
        QAction * histogramAction;
        DrawSurface * drawSurface;
        HistogramDialog * histDialog;

        void connectActionsToControllers();
        std::string getOpenFileUrl(std::string dialogTitle);
        std::string getSaveFileUrl(std::string dialogTitle);
        bool askForPadding();
        static std::string getFileExtension(std::string fileUrl);

};

#endif
