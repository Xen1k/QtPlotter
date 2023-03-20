#ifndef PLOTDATA_H
#define PLOTDATA_H

#include <QVector>
#include <QColor>

class PlotData
{
public:
    const static QColor defaultColor;
    int id;
    QString name;
    QColor color;
    QVector<double> dataX;
    QVector<double> dataY;
    PlotData(int id, QString name, QColor color, QVector<double> dataX, QVector<double> dataY);
    PlotData() = default;
};

#endif // PLOTDATA_H
