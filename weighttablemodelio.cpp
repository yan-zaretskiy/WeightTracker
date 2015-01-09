#include "weighttablemodelio.h"
#include "weighttablemodel.h"
#include "weightdatareader.h"
#include "weightdatawriter.h"

namespace weighttracker {

bool WeightTableModelIO::populateModelFromFile(WeightTableModel& model, const QString &fileName)
{
    XMLReader reader(fileName);
    std::vector<DataPoint> data;
    bool result = reader.read(data);
    model.setData(std::move(data));

    return result;
}


bool WeightTableModelIO::writeModelToFile(WeightTableModel& model, const QString &fileName)
{
    XMLWriter writer(fileName);
    return writer.write(model.getData());
}

}
