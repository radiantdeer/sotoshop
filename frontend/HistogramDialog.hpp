#ifndef HISTOGRAMDIALOG_H
#define HISTOGRAMDIALOG_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

class HistogramDialog : public QWidget {
    public:
        HistogramDialog(const std::vector<std::vector<int>> hist);
    private:
        QTabWidget *tabWidget;
};

#endif // HISTOGRAMDIALOG_H
