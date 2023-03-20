#ifndef PLOTDATATABLEMODEL_H
#define PLOTDATATABLEMODEL_H

#include <QAbstractTableModel>
#include "plotdata.h"

class PlotDataTableModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    PlotData plotData;
public:
    PlotDataTableModel(QObject *parent=nullptr);
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPair<double, double> GetAxisLimits(int axis);
    const PlotData& GetPlotData() const;
    void SetPlotData(PlotData plotData);
    void SetPlotName(QString plotName);
    void SetPlotColor(QColor plotColor);
    void SetRandomPlotDataId();
signals:
    void ValueChanged();
};

#endif // PLOTDATATABLEMODEL_H
