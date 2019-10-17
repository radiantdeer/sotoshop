#ifndef BITPLANEDIALOG_H
#define BITPLANEDIALOG_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <vector>
#include "../datastruct/Image.hpp"

class BitPlaneDialog : public QWidget {
    public:
        BitPlaneDialog(const std::vector<Image>& bitPlanes);

    private:
        std::vector<Image> bitPlanes;
        QTabWidget *tabWidget;
};

#endif // HISTOGRAMDIALOG_H