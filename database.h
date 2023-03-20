#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QDataStream>
#include "plotdata.h"

#define DATABASE_NAME       "DataBase.db"
#define TABLE               "Coordinates"
#define TABLE_COORDINATES_X "coordinates_x"
#define TABLE_COORDINATES_Y "coordinates_y"
#define TABLE_DATA_SET_NAME "name"
#define TABLE_PLOT_COLOR    "color"
#define TABLE_ID            "id"

class PlotDataBase : public QObject
{
    Q_OBJECT
public:
    explicit PlotDataBase(QObject *parent = 0);
    ~PlotDataBase();
    bool InsertOrReplace(const PlotData &data);
    bool CreateTable();
    bool Delete(QString name);
    QVector<PlotData> GetData();

    template<typename T>
    static QByteArray Serialize(const T& x)
    {
        QByteArray ba;
        QDataStream s(&ba, QIODevice::WriteOnly);
        s << x;
        return ba;
    }

    template<typename T>
    static T Deserialize(QByteArray& ba)
    {
        QDataStream s(&ba, QIODevice::ReadOnly);
        T res;
        s >> res;
        return res;
    }

private:
    bool OpenDataBase();
    QSqlDatabase db;
};

#endif // DATABASE_H
