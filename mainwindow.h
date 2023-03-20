#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "plotdatatablemodel.h"
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QVector<PlotData> dbData;
    QVector<PlotDataTableModel*> dataTableModels;
    Ui::MainWindow *ui;
    PlotDataBase *db;
    void SetOpenedChartAxisLimits();
private slots:
    void RefreshCharts();
    void UploadChartsFromDB();
    void SaveChartToDB();
    void AddPlotDataTab(PlotData* plotData = nullptr);
    void DeleteActiveDataSetFromDb();
    void RenameDataSet();
    void ChangePlotColor();
    void RefreshOpenedChart();
};
#endif // MAINWINDOW_H
