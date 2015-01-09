#ifndef WEIGHTDATAWRITER_H
#define WEIGHTDATAWRITER_H

#include <QFile>
#include <QString>
#include <vector>
#include "datapoint.h"

namespace weighttracker {

class WeightDataManager;

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

#endif // WEIGHTDATAWRITER_H
