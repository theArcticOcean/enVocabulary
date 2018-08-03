/**********************************************************
*
* @brief    The file contains details of class collectSentences
*           That manage page of collected sentences.
*
* @author   theArcticOcean
***********************************************************/

#include "collectsentences.h"
#include "ui_collectsentences.h"
#include "endata.h"
#include "controller.h"
#include "log.h"
#include <QMessageBox>

/*
*   Initiate UI page of collected sentences.
*   Connect checkBox click signal and slot function that shows delete button.
*/
collectSentences::collectSentences(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::collectSentences),
    sentenceCountPerPage( 6 )
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

/*
*   Destructor of class collectSentences
*/
collectSentences::~collectSentences()
{
    delete ui;
}

/*
*   Interface for add sentence in checkBox on page in every line.
*/
void collectSentences::fillSentenceInCheckBox(const int index, const QString &_str)
{
    QString str = enData::getInstance()->simpleSentence(_str);

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
    default:
        break;
    }
}

/*
*   Connect checkbox's click signal and slot function showDeleteButton.
*   The function is used in constructor function.
*/
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

/*
*   Rewrite showEvent function to receive widget show event before it shows.
*   We can custom page initiation.
*/
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
    while( pageIndex > model->getColSentencePageCount() )
    {
        --pageIndex;
    }
    if( 0 == pageIndex )
    {
        pageIndex = 1;
    }
    model->getCollectSentencePage(pageIndex);
    int size = model->v_collectSentences.size();
    int i;
    for(i=0; i<size; i++){
        sentenceUnit tmp = model->v_collectSentences[i];
        QString str = tmp.sentence+"\n\n"+tmp.translation;
        fillSentenceInCheckBox(i, str);
    }

    QString page = QString::number(pageIndex);
    page = page+" (";
    page = page+QString::number(model->getColSentencePageCount());
    page = page+")";
    ui->label->setText(page);
}

/*
*   Rewrite mousePressEvent to record the postion of left button click on the page.
*   This is important for move our software on screen.
*/
void collectSentences::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        lastPos = event->globalPos();
    }
}

/*
*   Records new postion in mouseMoveEvent and notify UIMgr to move CollectSentenceUI.
*/
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

/*
*   Interface of Checkbox's check action. It is useful for all checked and all not checked scene.
*/
void collectSentences::setCheckBoxChecked(QCheckBox *box, bool checked)
{
    if(!box->isHidden())
    {
        box->setChecked(checked);
    }
}

/*
*   Interface of show delete button.
*/
void collectSentences::showDeleteButton()
{
    ui->deleteButton->show();
}

/*
*   Deal with user's click on back button.
*/
void collectSentences::on_backButton_clicked()
{
    Controller *control = Controller::getInstance();
    control->sendViewMsg(CollectSenWndToHome);
}

/*
*   Delete sentences from current UI page and database.
*/
void collectSentences::on_deleteButton_clicked()
{
    LOGDBG("start");
    QString text;
    enData *model = enData::getInstance();
    if(ui->checkBox->isChecked()){
        text = ui->checkBox->text();
        model->deleteSentenceFromDBOnCollectedSentencesWnd( 0 );
//        ui->checkBox->setText("");
//        ui->checkBox->hide();
    }
    if(ui->checkBox_2->isChecked()){
        text = ui->checkBox_2->text();
        model->deleteSentenceFromDBOnCollectedSentencesWnd( 1 );
    }
    if(ui->checkBox_3->isChecked()){
        text = ui->checkBox_3->text();
        model->deleteSentenceFromDBOnCollectedSentencesWnd( 2 );
    }
    if(ui->checkBox_4->isChecked()){
        text = ui->checkBox_4->text();
        model->deleteSentenceFromDBOnCollectedSentencesWnd( 3 );
    }
    if(ui->checkBox_5->isChecked()){
        text = ui->checkBox_5->text();
        model->deleteSentenceFromDBOnCollectedSentencesWnd( 4 );
    }
    if(ui->checkBox_6->isChecked()){
        text = ui->checkBox_6->text();
        model->deleteSentenceFromDBOnCollectedSentencesWnd( 5 );
    }
    showEvent(NULL);
    LOGDBG("end!");
}

/*
*   Go to next page and refresh UI.
*/
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

/*
*   Go to previous page and refresh UI.
*/
void collectSentences::on_previousButton_clicked()
{
    pageIndex--;
    if(pageIndex == 0){
        pageIndex++;
        return ;
    }
    showEvent(NULL);
}

/*
*   Choose all sentences or choose none sentence.
*/
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

/*
*   Delete all collected sentences and refresh UI page.
*/
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
