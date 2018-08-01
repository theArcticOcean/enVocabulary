/**********************************************************
*
* @brief    The file defines main entry of software enVocabulary.
*
* @author   theArcticOcean
***********************************************************/

#include "viewmanager.h"
#include "controller.h"
#include <QApplication>
#include <QQmlEngine>
#include <QQmlComponent>
#include "qmlfunc.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qmlRegisterType<QmlFunc>("weiy.QmlFunc", 1, 0, "QmlFunc");

    viewManager UIMgr;
    Controller *control = Controller::getInstance();
    control->attachViewManager(&UIMgr);
    control->signalAndSlotsConenct();

    qDebug()<<"SSL version use for build: "<<QSslSocket::sslLibraryBuildVersionString();
    qDebug()<<"SSL version use for run-time: "<<QSslSocket::sslLibraryVersionString();
    qDebug()<<QSslSocket::supportsSsl()<<endl;
    qDebug()<<QCoreApplication::libraryPaths();
    return a.exec();
}
