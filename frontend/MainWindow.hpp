#ifndef __MAINWINDOW_HPP
#define __MAINWINDOW_HPP

#include <QAction>
#include <QMainWindow>
#include <QObject>
#include <QSignalMapper>
#include <string>

#include "DrawSurface.hpp"
#include "../file_handling/loader/ImageLoaderFactory.hpp"
#include "../file_handling/saver/ImageSaverFactory.hpp"

class MainWindow : public QMainWindow {
    
    public:
        MainWindow();
        QAction * getLoadAction();
        QAction * getSaveAction();
        DrawSurface * getDrawSurface();

    public slots:
        void loadFile();

    private:
        QAction * loadAction;
        QAction * saveAction;
        DrawSurface * drawSurface;
        
        std::string getFileUrl(std::string dialogTitle);
        
};

#endif