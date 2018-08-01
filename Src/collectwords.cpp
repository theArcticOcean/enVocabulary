/**********************************************************
*
* @brief    The details of class collectWords.
*
* @author   theArcticOcean
***********************************************************/

#include "collectwords.h"
#include "ui_collectwords.h"
#include "endata.h"
#include "log.h"
#include "controller.h"
#include <QMessageBox>

/*
*   Constructor of class collectWords, set UI style.
*/
collectWords::collectWords(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::collectWords)
{
    ui->setupUi(this);
    ui->backButton->setStyleSheet("QPushButton{ border-image: url(:image/goBack.png); } ");
    ui->previousButton->setStyleSheet("QPushButton{ border-image: url(:image/left.png); }");
    ui->nextButton->setStyleSheet("QPushButton{ border-image: url(:image/right.png); }");
    ui->clearButton->setStyleSheet("QPushButton{ border-image: url(:image/trash.png); }");
    this->setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint | \
                         Qt::WindowMinimizeButtonHint);
#ifdef Q_OS_MACX
    this->setWindowIcon(QIcon(QString(":/image/dict.icns")));
#endif
    pageIndex = 1;

    ui->label1->setWordWrap(true);
    ui->label2->setWordWrap(true);
    ui->label3->setWordWrap(true);
    ui->label4->setWordWrap(true);
    ui->label5->setWordWrap(true);
    ui->label6->setWordWrap(true);
}

/*
*   Destrctor of class collectWords
*/
collectWords::~collectWords()
{
    delete ui;
}

/*
*   Interface of adding words in page collectWords.
*/
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

/*
*   Rewrite function showEvent to refresh UI when go to new page.
*/
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
        fillWordInLabels(i-offset,str);
        it++;
    }

    QString page = QString::number(pageIndex);
    page = page+" (";
    page = page+QString::number(model->getColWordPageCount());
    page = page+")";
    ui->label->setText(page);
}

/*
*   Rewrite mousePressEvent to record the postion of left button click on the page.
*   This is important for move our software on screen.
*/
void collectWords::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        lastPos = event->globalPos();
    }
}

/*
*   Records new postion in mouseMoveEvent and notify UIMgr to move CollectWordUI.
*/
void collectWords::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons().testFlag(Qt::LeftButton)){
        LOGDBG("collectWords UI moved.");
        Controller::getInstance()->sendMoveViewSignal(CollectWordUI, \
                                                      this->pos() + (event->globalPos() - lastPos));
        lastPos = event->globalPos();
    }
}

/*
*   Deal with user's click on back button.
*/
void collectWords::on_backButton_clicked()
{
    Controller *control = Controller::getInstance();
    control->sendViewMsg(CollectWordWndToHome);
}

/*
*   Go to next page and refresh UI.
*/
void collectWords::on_nextButton_clicked()
{
    pageIndex++;
    if(pageIndex > enData::getInstance()->getColWordPageCount()){
        pageIndex --;
        return;
    }
    showEvent(NULL);
}

/*
*   Go to previous page and refresh UI.
*/
void collectWords::on_previousButton_clicked()
{
    pageIndex--;
    if(pageIndex < 1){
        pageIndex++;
        return;
    }
    showEvent(NULL);
}

/*
*   Delete all collected words from disc and memory.
*/
void collectWords::on_clearButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this,
                                 QString("Clear Vocabulary History In Disc"),
                                 QString("Do you want to clear all history data in disc?"),
                                 QMessageBox::Yes | QMessageBox::Cancel);
    if(QMessageBox::Yes == reply){
        enData::getInstance()->clearWordFromDisc();
        enData::getInstance()->clearWordInMem();
        pageIndex = 1;
        showEvent(NULL);
    }
}
