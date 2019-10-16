#ifndef HISTOGRAMTAB_H
#define HISTOGRAMTAB_H

#include <QWidget>

class HistogramTab : public QWidget
{
public:
    HistogramTab(const std::vector<int> hist, std::string color, QWidget *parent = 0);
};

#endif // HISTOGRAMTAB_H
