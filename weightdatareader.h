#ifndef WEIGHTDATAREADER_H
#define WEIGHTDATAREADER_H

#include <QFile>
#include <QString>

namespace weighttracker {

class WeightDataManager;

class WeightDataReader
{
public:
    virtual bool read(WeightDataManager& wdm) = 0;
};


class XMLReader : public WeightDataReader
{
public:
    XMLReader(QString filename);
    bool read(WeightDataManager& wdm);

private:
    QFile file_;
};

}

#endif // WEIGHTDATAREADER_H
