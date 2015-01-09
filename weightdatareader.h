#ifndef WEIGHTDATAREADER_H
#define WEIGHTDATAREADER_H

#include <QFile>
#include <QString>
#include <vector>
#include "datapoint.h"

namespace weighttracker {

class WeightDataManager;

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

}

#endif // WEIGHTDATAREADER_H
