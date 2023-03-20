#include "database.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QCoreApplication>
#include <QSqlRecord>
#include <QSqlField>

PlotDataBase::PlotDataBase(QObject *parent) : QObject(parent)
{
    OpenDataBase();
}

PlotDataBase::~PlotDataBase()
{
    db.close();
}

bool PlotDataBase::OpenDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/" DATABASE_NAME);
    return db.open();
}

bool PlotDataBase::CreateTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " TABLE " ("
                        TABLE_ID            " INTEGER PRIMARY KEY,"
                        TABLE_DATA_SET_NAME " TEXT,"
                        TABLE_PLOT_COLOR    " BLOB NOT NULL,"
                        TABLE_COORDINATES_X " BLOB NOT NULL,"
                        TABLE_COORDINATES_Y " BLOB NOT NULL)"))
    {
        qDebug() << "Create: " << query.lastError().text();
        return false;
    }

    return true;
}

QVector<PlotData> PlotDataBase::GetData()
{
    QVector<PlotData> plotsData;
    QSqlQuery query;
    query.exec("SELECT * FROM " TABLE);
    QByteArray coordsByteArrayX;
    QByteArray coordsByteArrayY;
    QByteArray plotColorByteArray;
    while (query.next()) {
        plotColorByteArray = query.record().field(2).value().toByteArray();
        coordsByteArrayX = query.record().field(3).value().toByteArray();
        coordsByteArrayY = query.record().field(4).value().toByteArray();
        PlotData currentPlotData(
                    query.record().field(0).value().toInt(),
                    query.record().field(1).value().toString(),
                    Deserialize<QColor>(plotColorByteArray),
                    Deserialize<QVector<double>>(coordsByteArrayX),
                    Deserialize<QVector<double>>(coordsByteArrayY));
        plotsData.push_back(currentPlotData);
    }
    return plotsData;
}

bool PlotDataBase::Delete(QString name)
{
    QSqlQuery query;
    if(!query.exec( "DELETE FROM " TABLE " WHERE " TABLE_DATA_SET_NAME "=\"" + name + "\""))
    {
        qDebug() << "Delete: " << query.lastError().text();
        return false;
    }
    return true;
}


bool PlotDataBase::InsertOrReplace(const PlotData &data)
{
    QSqlQuery query;

    query.prepare("INSERT OR REPLACE INTO " TABLE "(" TABLE_ID "," TABLE_DATA_SET_NAME","
                                TABLE_PLOT_COLOR"," TABLE_COORDINATES_X"," TABLE_COORDINATES_Y")"
                                "VALUES (:id, :name, :color, :x, :y)");

    query.bindValue(":id", data.id);
    query.bindValue(":name", data.name);
    query.bindValue(":color", Serialize(data.color));
    query.bindValue(":x", Serialize(data.dataX));
    query.bindValue(":y", Serialize(data.dataY));

    if(!query.exec()){
        qDebug() << "Insert: " << query.lastError().text();
        return false;
    }
    return true;
}
