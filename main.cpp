#include "search.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Search sWin;
    sWin.show();
    return a.exec();
}
