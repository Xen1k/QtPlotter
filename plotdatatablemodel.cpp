#include "plotdatatablemodel.h"
#include <QDebug>

PlotDataTableModel::PlotDataTableModel(QObject *parent) : QAbstractTableModel(parent) { }

int PlotDataTableModel::rowCount(const QModelIndex &) const { return 2; }

int PlotDataTableModel::columnCount(const QModelIndex &) const { return 10; }

bool PlotDataTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;
    auto& currentDataVector = index.row() == 0 ? plotData.dataX : plotData.dataY;
    if(index.column() < currentDataVector.length())
        currentDataVector[index.column()] = value.toDouble();
    else
        currentDataVector.push_back(value.toDouble());
    emit ValueChanged();
    return true;
}

QVariant PlotDataTableModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if(role != Qt::DisplayRole)
        return QVariant();
    auto& currentDataVector = index.row() == 0 ? plotData.dataX : plotData.dataY;
    if(index.column() < currentDataVector.length())
        value = currentDataVector[index.column()];
    return value;
}

const PlotData& PlotDataTableModel::GetPlotData() const { return plotData; }

void PlotDataTableModel::SetPlotName(QString plotName)
{
    plotData.name = plotName;
}

void PlotDataTableModel::SetPlotColor(QColor plotColor)
{
    plotData.color = plotColor;
}

void PlotDataTableModel::SetRandomPlotDataId() { plotData.id = rand(); }

void PlotDataTableModel::SetPlotData(PlotData plotData)
{
    this->plotData = plotData;
    layoutChanged();
}

QPair<double, double> PlotDataTableModel::GetAxisLimits(int axis)
{
    double min = __DBL_MAX__, max = __DBL_MIN__;
    QVector<double>& coordinates = axis == 0 ? plotData.dataX : plotData.dataY;
    for(auto coordinate : coordinates)
    {
        if(min > coordinate)
            min = coordinate;
        if(max < coordinate)
            max = coordinate;
    }
    return QPair<double, double>(min, max);
}

Qt::ItemFlags PlotDataTableModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

