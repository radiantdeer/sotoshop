#ifndef HISTOGRAMDIALOG_H
#define HISTOGRAMDIALOG_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QDialog>

class HistogramDialog : public QDialog {
    public:
        HistogramDialog(const std::vector<std::vector<int>> hist, QWidget *parent);
    private:
        QTabWidget *tabWidget;
};

#endif // HISTOGRAMDIALOG_H
