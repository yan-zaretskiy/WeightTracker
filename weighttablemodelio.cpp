#include <QXmlStreamReader>
#include <QDate>
#include <QLocale>

#include "weightdatamanager.h"
#include "weighttablemodelio.h"
#include "weighttablemodel.h"

namespace weighttracker {

bool WeightTableModelIO::populateModelFromFile(WeightTableModel& model, const QString &fileName)
{
    XMLReader reader(fileName);
    DataVector data;
    bool result = reader.read(data);
    model.setWeightData(std::move(data));

    return result;
}

bool WeightTableModelIO::writeModelToFile(WeightTableModel& model, const QString &fileName)
{
    XMLWriter writer(fileName);
    return writer.write(model.getWeightData());
}



XMLReader::XMLReader(QString filename) : file_(filename)
{ }

bool XMLReader::read(DataVector &data)
{
    bool open = file_.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!open)
        return false;
    else
    {
        data.clear();
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
                    data.emplace_back(DataPoint(date, value));
                else
                    return false;
            }
        }

        file_.close();
        return true;
    }
}



XMLWriter::XMLWriter(QString filename) : file_(filename)
{ }

bool XMLWriter::write(const DataVector &data)
{
    bool open = file_.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!open)
        return false;
    else
    {
        QXmlStreamWriter xml(&file_);
        xml.setAutoFormatting(true);
        xml.writeStartDocument();
        xml.writeStartElement("weight_points");
        for (auto& el : data)
        {
            xml.writeStartElement("weight");
            xml.writeAttribute("date", QLocale().toString(el.date, QLocale::ShortFormat));
            xml.writeCharacters(QString::number(el.value,'f', 1));
            xml.writeEndElement();
        }
        xml.writeEndElement(); // weights
        xml.writeEndDocument();
        file_.close();
        return !xml.hasError();
    }
}

}
