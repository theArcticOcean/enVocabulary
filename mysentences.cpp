#include "mysentences.h"
#include "ui_mysentences.h"
#include "log.h"

mySentences::mySentences(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mySentences)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
}

mySentences::~mySentences()
{
    delete ui;
}

void mySentences::showEvent(QShowEvent *event)
{
    if(QEvent::Show == event->type()){
        LOGDBG("show.");
    }
}
