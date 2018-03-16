#include "collectwords.h"
#include "ui_collectwords.h"
#include "endata.h"
#include "log.h"
#include "controller.h"

collectWords::collectWords(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::collectWords)
{
    ui->setupUi(this);
    ui->backButton->setStyleSheet("QPushButton{ border-image: url(:image/goBack.png); } ");
    ui->previousButton->setStyleSheet("QPushButton{ border-image: url(:image/left.png); }");
    ui->nextButton->setStyleSheet("QPushButton{ border-image: url(:image/right.png); }");
    pageIndex = 0;
    pageCount = 0;

    connectCheckBoxClickAndShowDelBtn();
}

collectWords::~collectWords()
{
    delete ui;
}

void collectWords::connectCheckBoxClickAndShowDelBtn()
{
    connect(ui->checkBox,SIGNAL(clicked(bool)),this,SLOT(showDeleteButton()));
    connect(ui->checkBox_2,SIGNAL(clicked(bool)),this,SLOT(showDeleteButton()));
    connect(ui->checkBox_3,SIGNAL(clicked(bool)),this,SLOT(showDeleteButton()));
    connect(ui->checkBox_4,SIGNAL(clicked(bool)),this,SLOT(showDeleteButton()));
    connect(ui->checkBox_5,SIGNAL(clicked(bool)),this,SLOT(showDeleteButton()));
    connect(ui->checkBox_6,SIGNAL(clicked(bool)),this,SLOT(showDeleteButton()));
}

void collectWords::fillWordInCheckBox(const int index, QString str)
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

void collectWords::showEvent(QShowEvent *event)
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
    model->getCollectWordPage(0);
    int size = model->v_collectWords.size();
    int i;
    for(i=0; i<size; i++){
        wordUnit tmp = model->v_collectWords[i];
        QString str = tmp.word+"\n\n"+tmp.translation;
        fillWordInCheckBox(i,str);
    }

    QString page = "1 (";
    page = page+QString::number(model->getColWordPageCount());
    page = page+")";
    ui->label->setText(page);
}

void collectWords::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        lastPos = event->globalPos();
    }
}

void collectWords::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons().testFlag(Qt::LeftButton)){
        LOGDBG("collectWords UI moved.");
        Controller::getInstance()->sendMoveViewSignal(CollectWordUI, \
                                                      this->pos() + (event->globalPos() - lastPos));
        lastPos = event->globalPos();
    }
}

void collectWords::showDeleteButton()
{
    ui->deleteButton->show();
}

void collectWords::on_deleteButton_clicked()
{
    LOGDBG("start");
    QString text;
    enData *model = enData::getInstance();
    if(ui->checkBox->isChecked()){
        text = ui->checkBox->text();
        model->deleteWordFromDB(text);
        ui->checkBox->setText("");
        ui->checkBox->hide();
    }
    if(ui->checkBox_2->isChecked()){
        text = ui->checkBox_2->text();
        model->deleteWordFromDB(text);
        ui->checkBox_2->setText("");
        ui->checkBox_2->hide();
    }
    if(ui->checkBox_3->isChecked()){
        text = ui->checkBox_3->text();
        model->deleteWordFromDB(text);
        ui->checkBox_3->setText("");
        ui->checkBox_3->hide();
    }
    if(ui->checkBox_4->isChecked()){
        text = ui->checkBox_4->text();
        model->deleteWordFromDB(text);
        ui->checkBox_4->setText("");
        ui->checkBox_4->hide();
    }
    if(ui->checkBox_5->isChecked()){
        text = ui->checkBox_5->text();
        model->deleteWordFromDB(text);
        ui->checkBox_5->setText("");
        ui->checkBox_5->hide();
    }
    if(ui->checkBox_6->isChecked()){
        text = ui->checkBox_6->text();
        model->deleteWordFromDB(text);
        ui->checkBox_6->setText("");
        ui->checkBox_6->hide();
    }
    LOGDBG("end!");
}

void collectWords::on_backButton_clicked()
{
    Controller *control = Controller::getInstance();
    control->sendViewMsg(CollectWordWndToHome);
}
