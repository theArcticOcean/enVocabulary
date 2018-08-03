/**********************************************************
*
* @brief    The file has details of class mySentences that
*           is responsible for UI page collected sentences.
*
* @author   theArcticOcean
***********************************************************/

#include "mysentences.h"
#include "ui_mysentences.h"
#include "log.h"
#include "controller.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>

/*
*   Constructor of class mySentences. Initinate UI page and refresh.
*/
mySentences::mySentences(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mySentences)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint | \
                         Qt::WindowMinimizeButtonHint);
    this->setWindowIcon(QIcon(QString(":/image/dict.icns")));
    vLayout = new QVBoxLayout(this);
    ui->backButton->setStyleSheet("QPushButton{ border-image: url(:image/goBack.png); }");
    backLabel = new QLabel(this);
    vLayout->addSpacing(50);
}

/*
*   Destructor of class mySentences.
*/
mySentences::~mySentences()
{
    delete ui;

    for(auto it:v_sentenceLabel) {
        vLayout->removeWidget(it);
        deleteObject(it);
    }
    v_sentenceLabel.clear();
    for(auto it:heartButtons){
        vLayout->removeWidget(it);
        deleteObject(it);
    }
    heartButtons.clear();
    for(auto it:sentenceHLays){
        deleteObject(it);
    }
    sentenceHLays.clear();

    delete vLayout;
    delete backLabel;
}

/*
*   Heart button signal-slot mechanism building.
*/
void mySentences::builtHButtonSigAndSLot()
{
    for (auto it:heartButtons) {
        connect(it, SIGNAL(clicked()), this, SLOT(on_collectClick()));
    }
}

/*
*   Rewrite show event when go to new UI page.
*/
void mySentences::showEvent(QShowEvent *event)
{
    updateSentences();
    builtHButtonSigAndSLot();
    this->ui->backButton->setGeometry( this->width() - 41, 10, 81, 41 );
    this->ui->backButton->setWindowIcon( QIcon(":/image/goBack.png") );
    LOGDBG( "backButton x: %d, this width: %d", this->ui->backButton->x(), this->width() );
}

/*
*   Record left-button click position to move software on the screen.
*/
void mySentences::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        lastPos = event->globalPos();
    }
}

/*
*   Move software on the screen.
*/
void mySentences::mouseMoveEvent(QMouseEvent *event)
{
    if( event->buttons().testFlag(Qt::LeftButton)) {
        LOGDBG("mySentences UI moved.");
        Controller::getInstance()->sendMoveViewSignal(MySentenceUI, this->pos() + (event->globalPos() - lastPos));
        lastPos =  event->globalPos();
    }
}

/*
*   Update sentence lines on UI page in showEvent.
*/
void mySentences::updateSentences()
{
    LOGDBG("start");

    for(auto it:v_sentenceLabel) {
        vLayout->removeWidget(it);
        deleteObject(it);
    }
    v_sentenceLabel.clear();
    for(auto it:heartButtons){
        vLayout->removeWidget(it);
        deleteObject(it);
    }
    heartButtons.clear();
    for(auto it:sentenceHLays){
        deleteObject(it);
    }
    sentenceHLays.clear();

    enData *model = enData::getInstance();
    const int size = model->getSentenceCount();
    for (int i=0; i<size; i++) {
        QLabel *sentenceLabel = new QLabel(this);
        sentenceLabel->setWordWrap( true );
        sentenceLabel->setFixedWidth( static_cast<int> ( this->width() * 20.0 / 21 ) );
        v_sentenceLabel.push_back(sentenceLabel);
        sentenceLabel->setParent(this);
        QString tmp = model->v_sentences[i].sentence;
        tmp.replace(QRegularExpression("<vocab>"), "<b>");
        tmp.replace(QRegularExpression("</vocab>"),"</b>");
        sentenceLabel->setText(tmp);
        QHBoxLayout *hLayout = new QHBoxLayout();
        hLayout->addWidget(sentenceLabel, 20);
        QPushButton *button = new QPushButton(this);
        button->setFixedWidth( static_cast<int>( this->width() * 1.0 / 21 ) );
        button->setIcon(QIcon(":/image/whiteHeart.png"));
        heartButtons.push_back(button);
        hLayout->addWidget(button, 1);
        if( sentenceLabel->wordWrap() )
        {
            vLayout->addLayout(hLayout, 2);
        }
        else
        {
            vLayout->addLayout(hLayout, 1);
        }
        sentenceHLays.push_back(hLayout);

        QLabel *translationLabel = new QLabel(this);
        translationLabel->setWordWrap( true );
        translationLabel->setFixedWidth( this->width() );
        v_sentenceLabel.push_back(translationLabel);
        translationLabel->setParent(this);
        //translationLabel->setLayoutDirection(Qt::LeftToRight);
        translationLabel->setText(model->v_sentences[i].translation);

        if( translationLabel->wordWrap() )
        {
            vLayout->addWidget(translationLabel, 2);
        }
        else
        {
            vLayout->addWidget(translationLabel, 1);
        }
    }

    // set red heart.
    for (int i=0; i<size; i++) {
        if(model->checkSentenceInDB(i)){
            heartButtons[i]->setIcon(QIcon(":/image/redHeart.png"));
        }
    }
    this->show();
    LOGDBG("end!");
}

/*
*   Deal with backButton click event. Go to search-word home page.
*/
void mySentences::on_backButton_clicked()
{
    LOGDBG("start");
    Controller *control = Controller::getInstance();
    control->sendViewMsg(SentenceWinToHome);
    LOGDBG("end!");
}

/*
*   Deal with collect button click event, add or delete relevant sentence.
*/
void mySentences::on_collectClick()
{
    QPushButton *button = dynamic_cast<QPushButton*>(sender());
    if(NULL == button){
        LOGDBG("dynamic_cast failed.");
        return ;
    }
    bool existed = false;
    enData *model = enData::getInstance();
    int index;
    int size = static_cast<int>(heartButtons.size());
    for( index=0; index < size ; ++index ){
        if(heartButtons[index] == button){
            LOGDBG("index is %d",index);
            if(model->checkSentenceInDB(index)) {
                existed = true;
            }
            break;
        }
    }

    // check it in database.
    if(existed) {
        button->setIcon(QIcon(":/image/whiteHeart.png"));
        model->deleteSentenceFromDBOnMySentencesWnd(index);
    }
    else {
        button->setIcon(QIcon(":/image/redHeart.png"));
        model->addSentenceToDB(index);
    }
}
