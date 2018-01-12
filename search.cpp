#include "search.h"
#include "log.h"
#include "ui_search.h"
#include "httpmanager.h"
#include <errno.h>

void Search::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        LOGDBG("0x%x", event->key());  // Key_Return
        http = new HttpManager();
        QString str = ui->lineEdit->text();
        bool ret = http->sendEnWordSearch((char *)str.toStdString().c_str());
        if(false == ret){
            LOGDBG("strerror: %s", strerror(errno));
        }
    }
}

Search::Search(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Search)
{
    ui->setupUi(this);
    http = NULL;
    ui->lineEdit->setPlaceholderText("Enter English word");
}

Search::~Search()
{
    delete ui;
}
