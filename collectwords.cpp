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
    pageIndex = 1;

    ui->label1->setWordWrap(true);
    ui->label2->setWordWrap(true);
    ui->label3->setWordWrap(true);
    ui->label4->setWordWrap(true);
    ui->label5->setWordWrap(true);
    ui->label6->setWordWrap(true);
}

collectWords::~collectWords()
{
    delete ui;
}

void collectWords::fillWordInLabels(const int index, QString str)
{
    switch(index){
    case 0:
        ui->label1->setText(str);
        ui->label1->show();
        break;
    case 1:
        ui->label2->setText(str);
        ui->label2->show();
        break;
    case 2:
        ui->label3->setText(str);
        ui->label3->show();
        break;
    case 3:
        ui->label4->setText(str);
        ui->label4->show();
        break;
    case 4:
        ui->label5->setText(str);
        ui->label5->show();
        break;
    case 5:
        ui->label6->setText(str);
        ui->label6->show();
        break;
    defalut:
        break;
    }
}

void collectWords::showEvent(QShowEvent *event)
{
    ui->label1->hide();
    ui->label2->hide();
    ui->label3->hide();
    ui->label4->hide();
    ui->label5->hide();
    ui->label6->hide();

    enData *model = enData::getInstance();
    int offset = (pageIndex-1)*COLLECT_WORD_PAGESIZE;
    int size = model->m_collectWords.size();
    int max = size < (offset+COLLECT_WORD_PAGESIZE) ? size:(offset+COLLECT_WORD_PAGESIZE);
    int tmp;
    map<ulong, wordUnit>::iterator it = model->m_collectWords.begin();
    tmp = offset;
    while(tmp--) it++;
    for(int i=offset; i<max; i++){
        wordUnit tmp = it->second;
        QString str = QString(tmp.word.c_str());
        str = str+"\n\n";
        str = str+tmp.translation.c_str();
        fillWordInLabels(i,str);
    }

    QString page = QString::number(pageIndex);
    page = page+" (";
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

void collectWords::on_backButton_clicked()
{
    Controller *control = Controller::getInstance();
    control->sendViewMsg(CollectWordWndToHome);
}

void collectWords::on_nextButton_clicked()
{
    pageIndex++;
    if(pageIndex > enData::getInstance()->getColWordPageCount()){
        pageIndex --;
        return;
    }
    showEvent(NULL);
}

void collectWords::on_previousButton_clicked()
{
    pageIndex--;
    if(pageIndex < 1){
        pageIndex++;
        return;
    }
    showEvent(NULL);
}
