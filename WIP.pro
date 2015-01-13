QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


TARGET = WeightTracker
TEMPLATE = app

SOURCES += \
    main.cpp \
    weightdatamanager.cpp \
    wtwidget.cpp \
    weighttablemodel.cpp \
    weightdataanalyzer.cpp \
    adddatadialog.cpp \
    mainwindow.cpp \
    weighttablemodelio.cpp \
    undocommands.cpp \
    qcustomplot.cpp \
    weightdataprovider.cpp \
    weightplotmanager.cpp

HEADERS += \
    weightdatamanager.h \
    datapoint.h \
    wtwidget.h \
    weighttablemodel.h \
    weightdataanalyzer.h \
    adddatadialog.h \
    mainwindow.h \
    settings.h \
    weighttablemodelio.h \
    undocommands.h \
    qcustomplot.h \
    weightdataprovider.h \
    weightplotmanager.h \
    common.h

CONFIG += c++11

FORMS += \
    wtwidget.ui \
    adddatadialog.ui

RESOURCES = weighttracker.qrc
