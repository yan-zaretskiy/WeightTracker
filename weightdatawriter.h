#ifndef WEIGHTDATAWRITER_H
#define WEIGHTDATAWRITER_H

#include <QFile>
#include <QString>

namespace weighttracker {

class WeightDataManager;

class WeightDataWriter
{
public:
    virtual bool write(WeightDataManager& wdm) = 0;
};


class XMLWriter : public WeightDataWriter
{
public:
    XMLWriter(QString filename);
    bool write(WeightDataManager& wdm);

private:
    QFile file_;
};

}

#endif // WEIGHTDATAWRITER_H
