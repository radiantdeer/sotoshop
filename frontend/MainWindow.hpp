#ifndef __MAINWINDOW_HPP
#define __MAINWINDOW_HPP

#include <QAction>
#include <QMainWindow>
#include <QObject>
#include <QSignalMapper>
#include <string>

#include "DrawSurface.hpp"
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

    private:
        QAction * loadAction;
        QAction * saveAction;
        DrawSurface * drawSurface;
        
        void connectActionsToControllers();
        std::string getFileUrl(std::string dialogTitle);
        
};

#endif