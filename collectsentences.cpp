#include "collectsentences.h"
#include "ui_collectsentences.h"

collectSentences::collectSentences(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::collectSentences)
{
    ui->setupUi(this);
}

collectSentences::~collectSentences()
{
    delete ui;
}
