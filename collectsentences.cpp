#include "collectsentences.h"
#include "ui_collectsentences.h"
#include "endata.h"
#include "controller.h"

collectSentences::collectSentences(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::collectSentences)
{
    ui->setupUi(this);
    ui->backButton->setStyleSheet("QPushButton{ border-image: url(:image/goBack.png); }");
    pageIndex = 0;
    pageCount = 0;
}

collectSentences::~collectSentences()
{
    delete ui;
}

void collectSentences::fillSentenceInCheckBox(const int index,QString str)
{
    switch(index){
    case 0:
        ui->checkBox->setText(str);
        break;
    case 1:
        ui->checkBox_2->setText(str);
    break;
    case 2:
        ui->checkBox_3->setText(str);
        break;
    case 3:
        ui->checkBox_4->setText(str);
        break;
    case 4:
        ui->checkBox_5->setText(str);
        break;
    case 5:
        ui->checkBox_6->setText(str);
        break;
    defalut:
        break;
    }
}

void collectSentences::showEvent(QShowEvent *event)
{
    enData *model = enData::getInstance();
    model->getCollectSentencePage(0);
    int size = model->v_collectSentences.size();
    int i;
    for(i=0; i<size; i++){
        sentenceUnit tmp = model->v_collectSentences[i];
        QString str = tmp.sentence+"\n\n"+tmp.translation;
        fillSentenceInCheckBox(i,str);
    }

    QString page = "1 (";
    page = page+QString::number(model->getColSentencePageCount());
    page = page+")";
    ui->label->setText(page);
}

void collectSentences::on_backButton_clicked()
{
    Controller *control = Controller::getInstance();
    control->sendViewMsg(CollectSenWndToHome);
}
