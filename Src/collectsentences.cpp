#include "collectsentences.h"
#include "ui_collectsentences.h"
#include "endata.h"
#include "controller.h"
#include "log.h"
#include <QMessageBox>

collectSentences::collectSentences(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::collectSentences)
{
    ui->setupUi(this);
    ui->backButton->setStyleSheet("QPushButton{ border-image: url(:image/goBack.png); } ");
    ui->previousButton->setStyleSheet("QPushButton{ border-image: url(:image/left.png); }");
    ui->nextButton->setStyleSheet("QPushButton{ border-image: url(:image/right.png); }");
    ui->clearButton->setStyleSheet("QPushButton{ border-image: url(:image/trash.png); }");
    this->setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint | \
                         Qt::WindowMinimizeButtonHint);
    this->setWindowIcon(QIcon(QString(":/image/dict.icns")));
    pageIndex = 1;

    connectCheckBoxClickAndShowDelBtn();
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
        ui->checkBox->show();
        break;
    case 1:
        ui->checkBox_2->setText(str);
        ui->checkBox_2->show();
        break;
    case 2:
        ui->checkBox_3->setText(str);
        ui->checkBox_3->show();
        break;
    case 3:
        ui->checkBox_4->setText(str);
        ui->checkBox_4->show();
        break;
    case 4:
        ui->checkBox_5->setText(str);
        ui->checkBox_5->show();
        break;
    case 5:
        ui->checkBox_6->setText(str);
        ui->checkBox_6->show();
        break;
    defalut:
        break;
    }
}

void collectSentences::connectCheckBoxClickAndShowDelBtn()
{
    connect(ui->checkBox,SIGNAL(clicked(bool)),this,SLOT(showDeleteButton()));
    connect(ui->checkBox_2,SIGNAL(clicked(bool)),this,SLOT(showDeleteButton()));
    connect(ui->checkBox_3,SIGNAL(clicked(bool)),this,SLOT(showDeleteButton()));
    connect(ui->checkBox_4,SIGNAL(clicked(bool)),this,SLOT(showDeleteButton()));
    connect(ui->checkBox_5,SIGNAL(clicked(bool)),this,SLOT(showDeleteButton()));
    connect(ui->checkBox_6,SIGNAL(clicked(bool)),this,SLOT(showDeleteButton()));
    connect(ui->checkBox_7,SIGNAL(clicked(bool)),this,SLOT(showDeleteButton()));
}

void collectSentences::showEvent(QShowEvent *event)
{
    ui->checkBox->hide();
    ui->checkBox_2->hide();
    ui->checkBox_3->hide();
    ui->checkBox_4->hide();
    ui->checkBox_5->hide();
    ui->checkBox_6->hide();
    ui->deleteButton->hide();
    ui->checkBox->setChecked(false);
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox_4->setChecked(false);
    ui->checkBox_5->setChecked(false);
    ui->checkBox_6->setChecked(false);
    ui->checkBox_7->setChecked(false);

    enData *model = enData::getInstance();
    model->getCollectSentencePage(pageIndex);
    int size = model->v_collectSentences.size();
    int i;
    for(i=0; i<size; i++){
        sentenceUnit tmp = model->v_collectSentences[i];
        QString str = tmp.sentence+"\n\n"+tmp.translation;
        fillSentenceInCheckBox(i,str);
    }

    QString page = QString::number(pageIndex);
    page = page+" (";
    page = page+QString::number(model->getColSentencePageCount());
    page = page+")";
    ui->label->setText(page);
}

void collectSentences::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        lastPos = event->globalPos();
    }
}

void collectSentences::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons().testFlag(Qt::LeftButton))
    {
        LOGDBG("collectSentences UI moved.");
        Controller::getInstance()->sendMoveViewSignal(CollectSentenceUI, \
                                                      this->pos() + (event->globalPos() - lastPos));
        lastPos = event->globalPos();
    }
}

void collectSentences::setCheckBoxChecked(QCheckBox *box, bool checked)
{
    if(!box->isHidden())
    {
        box->setChecked(checked);
    }
}

void collectSentences::showDeleteButton()
{
    ui->deleteButton->show();
}

void collectSentences::on_backButton_clicked()
{
    Controller *control = Controller::getInstance();
    control->sendViewMsg(CollectSenWndToHome);
}

void collectSentences::on_deleteButton_clicked()
{
    LOGDBG("start");
    QString text;
    enData *model = enData::getInstance();
    if(ui->checkBox->isChecked()){
        text = ui->checkBox->text();
        model->deleteSentenceFromDB(text);
        ui->checkBox->setText("");
        ui->checkBox->hide();
    }
    if(ui->checkBox_2->isChecked()){
        text = ui->checkBox_2->text();
        model->deleteSentenceFromDB(text);
        ui->checkBox_2->setText("");
        ui->checkBox_2->hide();
    }
    if(ui->checkBox_3->isChecked()){
        text = ui->checkBox_3->text();
        model->deleteSentenceFromDB(text);
        ui->checkBox_3->setText("");
        ui->checkBox_3->hide();
    }
    if(ui->checkBox_4->isChecked()){
        text = ui->checkBox_4->text();
        model->deleteSentenceFromDB(text);
        ui->checkBox_4->setText("");
        ui->checkBox_4->hide();
    }
    if(ui->checkBox_5->isChecked()){
        text = ui->checkBox_5->text();
        model->deleteSentenceFromDB(text);
        ui->checkBox_5->setText("");
        ui->checkBox_5->hide();
    }
    if(ui->checkBox_6->isChecked()){
        text = ui->checkBox_6->text();
        model->deleteSentenceFromDB(text);
        ui->checkBox_6->setText("");
        ui->checkBox_6->hide();
    }
    LOGDBG("end!");
}

void collectSentences::on_nextButton_clicked()
{
    pageIndex++;
    int pageCount = enData::getInstance()->getColSentencePageCount();
    if(pageIndex > pageCount){
        pageIndex--;
        return ;
    }
    showEvent(NULL);
}

void collectSentences::on_previousButton_clicked()
{
    pageIndex--;
    if(pageIndex == 0){
        pageIndex++;
        return ;
    }
    showEvent(NULL);
}

void collectSentences::on_checkBox_7_clicked()
{
    if(ui->checkBox_7->isChecked())
    {
        setCheckBoxChecked(ui->checkBox,true);
        setCheckBoxChecked(ui->checkBox_2,true);
        setCheckBoxChecked(ui->checkBox_3,true);
        setCheckBoxChecked(ui->checkBox_4,true);
        setCheckBoxChecked(ui->checkBox_5,true);
        setCheckBoxChecked(ui->checkBox_6,true);
        setCheckBoxChecked(ui->checkBox_7,true);
    }
    else {
        setCheckBoxChecked(ui->checkBox,false);
        setCheckBoxChecked(ui->checkBox_2,false);
        setCheckBoxChecked(ui->checkBox_3,false);
        setCheckBoxChecked(ui->checkBox_4,false);
        setCheckBoxChecked(ui->checkBox_5,false);
        setCheckBoxChecked(ui->checkBox_6,false);
        setCheckBoxChecked(ui->checkBox_7,false);
    }
}

void collectSentences::on_clearButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this,
                                 QString("Clear Sentences In Disc"),
                                 QString("Do you want to clear all sentence data in disc?"),
                                 QMessageBox::Yes | QMessageBox::Cancel);

    if(QMessageBox::Yes == reply){
        enData::getInstance()->clearSentenceFromDB();
        enData::getInstance()->clearSentenceInMem();
        pageIndex = 1;
        showEvent(NULL);
    }
}
