#include "search.h"
#include "log.h"
#include "ui_search.h"
#include "httpmanager.h"
#include <errno.h>
#include "endata.h"

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
    ui->uk_button->setText("");
    ui->uk_button->setStyleSheet("QPushButton{ border-image: url(:/image/stop_circle.png); }");
    ui->us_button->setText("");
    ui->us_button->setStyleSheet("QPushButton{ border-image: url(:/image/stop_circle.png); }");
}

Search::~Search()
{
    delete ui;
}

void Search::slotGetWord()
{
    LOGDBG("start");
    enData *model = enData::getInstance();
    ui->uk_label->setText(model->wordInf.uk_pronunciation.c_str());
    ui->us_label->setText(model->wordInf.us_pronunciation.c_str());
    LOGDBG("end!");
}
