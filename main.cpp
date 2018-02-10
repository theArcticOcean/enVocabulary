#include "viewmanager.h"
#include "controller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    viewManager UIMgr;
    Controller *control = Controller::getInstance();
    control->attachViewManager(&UIMgr);
    control->signalAndSlotsConenct();

    qDebug()<<"SSL version use for build: "<<QSslSocket::sslLibraryBuildVersionString();
    qDebug()<<"SSL version use for run-time: "<<QSslSocket::sslLibraryVersionNumber();
    qDebug()<<QCoreApplication::libraryPaths();
    return a.exec();
}
