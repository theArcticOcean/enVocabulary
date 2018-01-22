#include "viewmanager.h"
#include "controller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    viewManager *UIMgr = new viewManager();
    Controller *control = Controller::getInstance();
    control->attachViewManager(UIMgr);
    control->signalAndSlotsConenct();
    return a.exec();
}
