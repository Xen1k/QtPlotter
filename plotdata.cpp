#include "plotdata.h"

const QColor PlotData::defaultColor = Qt::black;

PlotData::PlotData(int id, QString name, QColor color, QVector<double> dataX, QVector<double> dataY) :
    id(id),
    name(name),
    color(color),
    dataX(dataX),
    dataY(dataY)
    { }
