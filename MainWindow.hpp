#ifndef __MAINWINDOW_HPP
#define __MAINWINDOW_HPP

#include <iostream>
#include <QAction>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenuBar>
#include <QObject>
#include <QPaintEvent>
#include <QRegion>
#include <QSignalMapper>
#include <QUrl>
#include <string>

#include "DrawSurface.hpp"

class MainWindow : public QMainWindow {
    
    public:
        QAction * loadAction;
        QAction * saveAction;
        DrawSurface * drawspace;

        MainWindow() : QMainWindow() {
            QMenu * fileMenu = this->menuBar()->addMenu("File");
            loadAction = fileMenu->addAction("Load");
            saveAction = fileMenu->addAction("Save");  
            drawspace = new DrawSurface(this);
            this->setCentralWidget(drawspace);
            QRegion tempRegion;
            drawspace->paintEvent(new QPaintEvent(tempRegion));

            connect(loadAction, &QAction::triggered, this, &MainWindow::showLoadFileDialog);
        }

    public slots:
        void showLoadFileDialog() {
            QUrl tempFileUrl = QFileDialog::getOpenFileUrl(this, "Load Image", *(new QUrl()), "Image Files (*.raw, *.pbm, *.pgm, *.ppm, *.bmp);; All Files (*)");
            std::string fileUrl = tempFileUrl.toLocalFile().toUtf8().constData();
            std::cout << fileUrl << std::endl;
        }
        
};

#endif