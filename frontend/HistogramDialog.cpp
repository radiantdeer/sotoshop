#include <QVBoxLayout>

#include "HistogramDialog.hpp"
#include "HistogramTab.hpp"
#include "../spdlog/spdlog.h"

HistogramDialog::HistogramDialog(const std::vector<std::vector<int>> hist) : QWidget () {
    tabWidget = new QTabWidget();
    if (hist.size() == 1) {
        tabWidget->addTab(new HistogramTab(hist.at(0), "Gray"), tr("Gray"));
    } else if (hist.size() == 3) {
        tabWidget->addTab(new HistogramTab(hist.at(0), "Red"), tr("Red"));
        tabWidget->addTab(new HistogramTab(hist.at(1), "Green"), tr("Green"));
        tabWidget->addTab(new HistogramTab(hist.at(2), "Blue"), tr("Blue"));
    }

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
    setMinimumSize(1000, 600);
    setWindowTitle(tr("Image Histogram"));
}
