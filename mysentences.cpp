#include "mysentences.h"
#include "ui_mysentences.h"
#include "log.h"
#include "controller.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>

mySentences::mySentences(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mySentences)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint | \
                         Qt::WindowMinimizeButtonHint);
    vLayout = new QVBoxLayout(this);
    ui->backButton->setStyleSheet("QPushButton{ border-image: url(:image/goBack.png); }");
    backLabel = new QLabel(this);
    vLayout->addSpacing(50);
}

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

void mySentences::builtHButtonSigAndSLot()
{
    for (auto it:heartButtons) {
        connect(it, SIGNAL(clicked()), this, SLOT(on_collectClick()));
    }
}

void mySentences::showEvent(QShowEvent *event)
{
    updateSentences();
    builtHButtonSigAndSLot();
}

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
    int size = model->getSentenceCount();
    for (int i=0; i<size; i++) {
        QLabel *sentenceLabel = new QLabel(this);
        v_sentenceLabel.push_back(sentenceLabel);
        sentenceLabel->setParent(this);
        QString tmp = model->v_sentences[i].sentence;
        tmp.replace(QRegularExpression("<vocab>"), "<b>");
        tmp.replace(QRegularExpression("</vocab>"),"</b>");
        sentenceLabel->setText(tmp);
        QHBoxLayout *hLayout = new QHBoxLayout();
        hLayout->addWidget(sentenceLabel, 20);
        QPushButton *button = new QPushButton(this);
        button->setIcon(QIcon(":/image/whiteHeart.png"));
        heartButtons.push_back(button);
        hLayout->addWidget(button, 1);//Qt::AlignAbsolute);
        vLayout->addLayout(hLayout,1);

        QLabel *translationLabel = new QLabel(this);
        v_sentenceLabel.push_back(translationLabel);
        translationLabel->setParent(this);
        //translationLabel->setLayoutDirection(Qt::LeftToRight);
        translationLabel->setText(model->v_sentences[i].translation);
        vLayout->addWidget(translationLabel,1);
    }
    this->show();
    LOGDBG("end!");
}

void mySentences::on_backButton_clicked()
{
    LOGDBG("start");
    Controller *control = Controller::getInstance();
    control->sendViewMsg(SentenceWinToHome);
    LOGDBG("end!");
}

void mySentences::on_collectClick()
{
    QPushButton *button = dynamic_cast<QPushButton*>(sender());
    if(NULL == button){
        LOGDBG("dynamic_cast failed.");
        return ;
    }
    // check it in database.
    bool existed = false;
    if(existed){
        button->setIcon(QIcon(":/image/whiteHeart.png"));
        // delete it from SQLite.
    }
    else {
        button->setIcon(QIcon(":/image/redHeart.png"));
        // insert it into SQLite.
    }
}
