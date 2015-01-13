#ifndef WEIGHTTABLEMODELIO_H
#define WEIGHTTABLEMODELIO_H

#include <QString>
#include <QFile>
#include <vector>
#include "datapoint.h"


namespace weighttracker {

class WeightTableModel;
class WeightDataManager;

class WeightTableModelIO
{
public:
    WeightTableModelIO() = default;
    ~WeightTableModelIO() = default;

    static bool populateModelFromFile(WeightTableModel& model, const QString& fileName);
    static bool writeModelToFile(WeightTableModel& model, const QString& fileName);
};



class WeightDataReader
{
public:
    virtual bool read(DataVector& data) = 0;
};

class XMLReader : public WeightDataReader
{
public:
    XMLReader(QString filename);
    bool read(DataVector& data) override;

private:
    QFile file_;
};



class WeightDataWriter
{
public:
    virtual bool write(const DataVector& data) = 0;
};

class XMLWriter : public WeightDataWriter
{
public:
    XMLWriter(QString filename);
    bool write(const DataVector& data) override;

private:
    QFile file_;
};

}

#endif // WEIGHTTABLEMODELIO_H
