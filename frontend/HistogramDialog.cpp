#include <QVBoxLayout>

#include "HistogramDialog.hpp"
#include "HistogramTab.hpp"
#include "../spdlog/spdlog.h"

HistogramDialog::HistogramDialog(ImageHistogram hist) : QWidget () {
    tabWidget = new QTabWidget();
    if (hist.isGrayscale()) {
        tabWidget->addTab(new HistogramTab(hist.getGray(), "Gray"), tr("Gray"));
    } else {
        tabWidget->addTab(new HistogramTab(hist.getRed(), "Red"), tr("Red"));
        tabWidget->addTab(new HistogramTab(hist.getGreen(), "Green"), tr("Green"));
        tabWidget->addTab(new HistogramTab(hist.getBlue(), "Blue"), tr("Blue"));
    }

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
    setMinimumSize(1000, 600);
    setWindowTitle(tr("Image Histogram"));
}
