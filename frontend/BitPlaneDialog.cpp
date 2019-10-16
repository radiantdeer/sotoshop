#include <QVBoxLayout>

#include "BitPlaneDialog.hpp"

BitPlaneDialog::BitPlaneDialog(const std::vector<Image> bitPlanes) : QWidget() {
    tabWidget = new QTabWidget();
    tabWidget->addTab(new QWidget(), tr("Plane 0"));
    tabWidget->addTab(new QWidget(), tr("Plane 1"));
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
    setMinimumSize(800, 600);
    setWindowTitle(tr("Bit Planes"));
}