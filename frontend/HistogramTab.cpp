#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QLegend>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QVBoxLayout>
#include "HistogramTab.hpp"
#include "../spdlog/spdlog.h"

QT_CHARTS_USE_NAMESPACE

HistogramTab::HistogramTab(const std::vector<int> hist, std::string color, QWidget *parent) : QWidget (parent)
{
    QBarSet *colorBarSet = new QBarSet("Color");
    if (color == "Gray") {
        colorBarSet->setColor(Qt::black);
    } else if (color == "Red") {
        colorBarSet->setColor(Qt::red);
    } else if (color == "Green") {
        colorBarSet->setColor(Qt::green);
    } else if (color == "Blue") {
        colorBarSet->setColor(Qt::blue);
    }
    for (int i = 0; i < 255; i++) {
        *colorBarSet << hist.at(i);
    }
    QBarSeries *series = new QBarSeries();
    series->append(colorBarSet);

    QChart *chart = new QChart();
    chart->addSeries(series);

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 255);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(false);

    QChartView * chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(chartView);
    this->setLayout(layout);
}
