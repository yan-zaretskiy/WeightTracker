#include <QXmlStreamReader>
#include <QDate>
#include <QLocale>

#include "weightdatareader.h"
#include "weightdatamanager.h"


namespace weighttracker {

XMLReader::XMLReader(QString filename) : file_(filename)
{ }

bool XMLReader::read(WeightDataManager &wdm)
{
    bool open = file_.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!open)
        return false;
    else
    {
        std::vector<DataPoint> temp;
        QXmlStreamReader xml(&file_);
        while (!xml.atEnd())
        {
            xml.readNext();
            if(xml.name() == "weight")
            {
                QString dateString = xml.attributes().value("date").trimmed().toString();
                QDate date = QDate::fromString(dateString, QLocale().dateFormat(QLocale::ShortFormat));
                bool correctDouble = true;
                double value = xml.readElementText().toDouble(&correctDouble);

                if (date.isValid() && correctDouble)
                    temp.emplace_back(DataPoint(date, value));
                else
                    return false;
            }
        }

        wdm.clear();
        for (auto& dp: temp)
            wdm.addDataPoint(dp.date, dp.value);

        file_.close();
        return true;
    }
}

}
