#include "search.h"
#include "controller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Search sWin;
    sWin.show();
    Controller *control = Controller::getInstance();
    control->attachSearch(&sWin);
    control->signalAndSlotsConenct();
    return a.exec();
}
