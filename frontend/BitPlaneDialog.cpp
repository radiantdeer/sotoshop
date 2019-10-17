#include <QVBoxLayout>

#include "BitPlaneDialog.hpp"
#include "DrawSurface.hpp"
#include <string>
#include <sstream>
#include "../spdlog/spdlog.h"

BitPlaneDialog::BitPlaneDialog(const std::vector<Image>& bitPlanes) : QWidget() {
    tabWidget = new QTabWidget();

    this->bitPlanes.resize(bitPlanes.size());
    for (int i = 0; i < bitPlanes.size(); i++) {
        this->bitPlanes[i] = bitPlanes[i];
    }

    for (int i = this->bitPlanes.size() - 1; i >= 0; i--) {
        std::ostringstream tabTitle;
        tabTitle << "Plane " << i;
        tabWidget->addTab(new DrawSurface(this, &(this->bitPlanes[i])), tabTitle.str().c_str());
    }

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
    setMinimumSize(800, 600);
    setWindowTitle(tr("Bit Planes"));
}