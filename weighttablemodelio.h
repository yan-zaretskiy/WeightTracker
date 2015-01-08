#ifndef WEIGHTTABLEMODELIO_H
#define WEIGHTTABLEMODELIO_H

#include <QString>

namespace weighttracker {

class WeightTableModel;

class WeightTableModelIO
{
public:
    WeightTableModelIO() = default;
    ~WeightTableModelIO() = default;

    static bool populateModelFromFile(WeightTableModel* model, const QString& fileName);
    static bool writeModelToFile(WeightTableModel* model, const QString& fileName);
};

}

#endif // WEIGHTTABLEMODELIO_H
