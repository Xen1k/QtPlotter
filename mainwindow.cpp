#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBoxLayout>
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Plotter");

    db = new PlotDataBase();
    db->CreateTable();
    dbData = db->GetData();
    UploadChartsFromDB();

    connect(ui->refreshButton, SIGNAL(clicked()), this, SLOT(RefreshCharts()));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(RefreshCharts()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(SaveChartToDB()));
    connect(ui->uploadButton, SIGNAL(clicked()), this, SLOT(UploadChartsFromDB()));
    connect(ui->addNewSetButton, SIGNAL(clicked()), this, SLOT(AddPlotDataTab()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(DeleteActiveDataSetFromDb()));
    connect(ui->renameButton, SIGNAL(clicked()), this, SLOT(RenameDataSet()));
    connect(ui->changeColorButton, SIGNAL(clicked()), this, SLOT(ChangePlotColor()));
}

void MainWindow::UploadChartsFromDB()
{
    dbData = db->GetData();
    ui->tabWidget->clear();
    dataTableModels.clear();
    for(auto plotData : dbData)
        AddPlotDataTab(&plotData);
    RefreshCharts();
}

void MainWindow::RenameDataSet()
{
    if(ui->tabWidget->count() == 0)
    {
        QMessageBox::warning(this, "Ошибка", "Нет открытых наборов");
        return;
    }
    QString newName = QInputDialog::getText(this, "Переименовать набор", "Введите новое имя набора");
    if(newName.length() == 0)
        return;
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), newName);
    auto currentModel = dataTableModels[ui->tabWidget->currentIndex()];
    currentModel->SetPlotName(newName);
    db->InsertOrReplace(currentModel->GetPlotData());
}

void MainWindow::ChangePlotColor()
{
    if(ui->tabWidget->count() == 0)
    {
        QMessageBox::warning(this, "Ошибка", "Нет открытых наборов");
        return;
    }
    QColorDialog* colorDialog = new QColorDialog(this);
    colorDialog->setCurrentColor(QColor(0, 0, 255));
    colorDialog->exec();
    QColor color = colorDialog->selectedColor();
    ui->plot->graph(ui->tabWidget->currentIndex())->setPen(QPen(color, 1));
    dataTableModels[ui->tabWidget->currentIndex()]->SetPlotColor(color);
    ui->plot->replot();
}


void MainWindow::DeleteActiveDataSetFromDb()
{
    db->Delete(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
    UploadChartsFromDB();
}

void MainWindow::SaveChartToDB()
{
    for(auto& model : dataTableModels)
        db->InsertOrReplace(model->GetPlotData());
    QMessageBox::information(this, "Успешно", "Наборы сохранены");
}

void MainWindow::RefreshCharts()
{
    if(dataTableModels.length() == 0 || ui->tabWidget->count() == 0)
    {
        for(int i = 0; i < ui->plot->graphCount(); i++)
            ui->plot->graph(i)->data().data()->clear();
        ui->plot->replot();
        return;
    }
    int graphIndex = 0;
    SetOpenedChartAxisLimits();
    for(auto& model : dataTableModels)
    {
        ui->plot->graph(graphIndex)->setData(model->GetPlotData().dataX, model->GetPlotData().dataY);
        ui->plot->graph(graphIndex)->setPen(QPen(model->GetPlotData().color, 1));
        graphIndex++;
    }
    ui->plot->replot();
}

void MainWindow::SetOpenedChartAxisLimits()
{
    QPair<double, double> xLimits = dataTableModels[ui->tabWidget->currentIndex()]->GetAxisLimits(0);
    QPair<double, double> yLimits = dataTableModels[ui->tabWidget->currentIndex()]->GetAxisLimits(1);
    ui->plot->xAxis->setRange(xLimits.first - 1, xLimits.second + 1);
    ui->plot->yAxis->setRange(yLimits.first - 1, yLimits.second + 1);
    ui->plot->replot();
}

void MainWindow::AddPlotDataTab(PlotData* plotData)
{
    QWidget *tab = new QWidget();
    PlotDataTableModel *model = new PlotDataTableModel();
    QTableView *table = new QTableView();
    QString tabName;
    if(plotData)
    {
        model->SetPlotData(*plotData);
        tabName = plotData->name;
    }
    else
    {
        tabName = "Набор " + QString::number(dataTableModels.length());
        model->SetPlotName(tabName);
        model->SetRandomPlotDataId();
        model->SetPlotColor(PlotData::defaultColor);
    }
    ui->tabWidget->addTab(tab, tabName);
    tab->setLayout(new QVBoxLayout());
    table->setModel(model);
    tab->layout()->addWidget(table);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->plot->addGraph();
    connect(model, &PlotDataTableModel::ValueChanged, this, &MainWindow::RefreshOpenedChart);
    dataTableModels.push_back(model);
}

void MainWindow::RefreshOpenedChart()
{
    int graphIndex = ui->tabWidget->currentIndex();
    auto model = dataTableModels[graphIndex];
    ui->plot->graph(graphIndex)->setData(model->GetPlotData().dataX, model->GetPlotData().dataY);
    ui->plot->graph(graphIndex)->setPen(QPen(model->GetPlotData().color, 1));
    SetOpenedChartAxisLimits();
    ui->plot->replot();
}


MainWindow::~MainWindow()
{
    delete ui;
}

