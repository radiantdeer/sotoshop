#ifndef HISTOGRAMDIALOG_H
#define HISTOGRAMDIALOG_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

#include "../datastruct/ImageHistogram.hpp"

class HistogramDialog : public QWidget {
    public:
        HistogramDialog(ImageHistogram hist);
    private:
        QTabWidget *tabWidget;
};

#endif // HISTOGRAMDIALOG_H
