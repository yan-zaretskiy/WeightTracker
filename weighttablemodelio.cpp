#include <memory>

#include "weighttablemodelio.h"
#include "weighttablemodel.h"
#include "weightdatareader.h"
#include "weightdatawriter.h"

namespace weighttracker {

bool WeightTableModelIO::populateModelFromFile(WeightTableModel *model, const QString &fileName)
{
    std::unique_ptr<WeightDataReader> reader(new XMLReader(fileName));
    model->beginResetModel();
    bool result = reader->read(model->wdm_);
    model->refreshTrendsStartingAtRow(0);
    model->endResetModel();

    return result;
}


bool WeightTableModelIO::writeModelToFile(WeightTableModel *model, const QString &fileName)
{
    std::unique_ptr<WeightDataWriter> writer(new XMLWriter(fileName));

    return writer->write(model->wdm_);
}

}
