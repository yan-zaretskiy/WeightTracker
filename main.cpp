#include <QApplication>

#include "mainwindow.h"
#include "fancytabs.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    weighttracker::MainWindow window;
    //FancyTabs window;

    window.show();

    return a.exec();
}
