#include "weighttablemodelio.h"
#include "weighttablemodel.h"
#include "weightdatareader.h"
#include "weightdatawriter.h"

namespace weighttracker {

bool WeightTableModelIO::populateModelFromFile(WeightTableModel& model, const QString &fileName)
{
    typedef WeightTableModel::WtModelAttorney Attorney;
    XMLReader reader(fileName);
    Attorney::beginResetModel(model);
    Attorney::beginResetModel(model);
    bool result = reader.read(Attorney::getWdManager(model));
    Attorney::refreshTrendsStartingAtRow(model, 0);
    Attorney::endResetModel(model);

    return result;
}


bool WeightTableModelIO::writeModelToFile(WeightTableModel& model, const QString &fileName)
{
    XMLReader writer(fileName);
    return writer.write(Attorney::getWdManager(model));
}

}
