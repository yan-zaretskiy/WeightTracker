#include <QXmlStreamWriter>
#include <QDate>
#include <QLocale>

#include "WeightDataWriter.h"
#include "WeightDataManager.h"


namespace weighttracker {

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

