#include "search.h"
#include "log.h"
#include "ui_search.h"
#include "httpmanager.h"
#include <errno.h>
#include "endata.h"
#include <QQuickItem>
#include "controller.h"
#include <QQmlContext>
#include <QMessageBox>

void Search::keyReleaseEvent(QKeyEvent *event)
{
    LOGDBG("event->key() is 0x%x",event->key());
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        http = HttpManager::getInstance();
        QString str = ui->lineEdit->text();
        bool ret = http->sendEnWordSearchRequest((char *)str.toStdString().c_str());
        if(false == ret){
            LOGDBG("strerror: %s", strerror(errno));
        }
    }
}

void Search::mousePressEvent(QMouseEvent *event)
{
    LOGDBG("start");
    if(event->button() == Qt::LeftButton){
        lastPos = event->globalPos();
    }

//!   we do these thing by QML
//    int rx = event->x() - ui->quickWidget->x();
//    int ry = event->y() - ui->quickWidget->y();

//    if(rx >=0 && rx <150 && ry >= 0 && ry < 50){
//        http = HttpManager::getInstance();
//        http->sendEnWordSentenceRequest();
//    }
//    else if(rx >= 75 && rx < 150 && ry >= 50 && ry < 200){
//        Controller *control = Controller::getInstance();
//        control->sendViewMsg(GotoCollectWord);
//    }
//    else if(rx >= 75 && rx < 150 && ry >= 200 && ry < 400){
//        Controller *control = Controller::getInstance();
//        control->sendViewMsg(GotoCollectSen);
//    }
    LOGDBG("end!");
}


void Search::mouseMoveEvent(QMouseEvent *event)
{
    if( event->buttons().testFlag(Qt::LeftButton)) {
        LOGDBG("search UI moved.");
        Controller::getInstance()->sendMoveViewSignal(SearchUI, this->pos() + (event->globalPos() - lastPos));
        lastPos =  event->globalPos();
    }
}

void Search::closeEvent(QCloseEvent *event)
{
    if (QEvent::Close == event->type()) {
        Controller *control = Controller::getInstance();
        control->sendViewMsg(CloseAllUI);
    }
}

Search::Search(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Search)
{
    ui->setupUi(this);
    ui->lineEdit->setPlaceholderText("Enter English word");
    ui->uk_button->setText("");
    ui->uk_button->setStyleSheet("QPushButton{ border-image: url(:/image/stop_circle.png); }");
    ui->us_button->setText("");
    ui->us_button->setStyleSheet("QPushButton{ border-image: url(:/image/stop_circle.png); }");
    ui->textBrowser->setReadOnly(true);
    ui->quickWidget->rootContext()->setContextProperty("qmlFunc",new QmlFunc());
    ui->quickWidget->setSource(QUrl("qrc:/Res/rect.qml"));
    this->setWindowIcon(QIcon(QString(":/image/dict.icns")));

    http = NULL;
    model = enData::getInstance();
    player = new QMediaPlayer();
    player->setVolume(100);

    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), \
            this, SLOT(slotStateChanged(QMediaPlayer::State)));
    connect(player, SIGNAL(error(QMediaPlayer::Error)), this, \
            SLOT(slotPlayerError(QMediaPlayer::Error)));
}

Search::~Search()
{
    delete ui;
}

void Search::slotGetWord()
{
    LOGDBG("start");
    QString word = ui->lineEdit->text();
    ui->uk_label->setText(model->wordInf.uk_pronunciation);
    ui->us_label->setText(model->wordInf.us_pronunciation);
    QString definition = QObject::tr("中文解释:\n");
    definition = definition+model->wordInf.cn_definition;
    definition = definition+"\n\n";
    definition = definition+QObject::tr("English definitions:\n");
    QString tmp = model->wordInf.en_definition;
    QStringList  strArray = tmp.split(";");
    foreach (QString str, strArray) {
        //! find the definition which has not original word.
        if(!str.contains(word,Qt::CaseInsensitive)){
            definition = definition+str;
            definition = definition+";\n";
        }
    }
    ui->textBrowser->setText(definition);
    enData *model = enData::getInstance();
    model->addWordToDB(word);
#ifdef DEBUG
    model->showTableVocabulary();
#endif
    LOGDBG("end!");
}

void Search::slotStateChanged(QMediaPlayer::State state)
{
    if(state != QMediaPlayer::PlayingState){
        ui->uk_button->setStyleSheet("QPushButton{border-image:url(:/image/stop_circle.png);}");
        ui->us_button->setStyleSheet("QPushButton{border-image:url(:/image/stop_circle.png);}");
    }
}

void Search::slotPlayerError(QMediaPlayer::Error err)
{
    QMessageBox::warning(this,"Play Error",player->errorString());
}

void Search::on_uk_button_clicked()
{
    QString url = model->wordInf.uk_audio_addresses;
    if(url.isEmpty()){
        LOGDBG("url is empty: %s",url.toStdString().c_str());
        return ;
    }
    player->setMedia(QUrl(url));
    player->play();
    ui->uk_button->setStyleSheet("QPushButton{border-image:url(:/image/play_circle.png);}");
    LOGDBG("played. url is %s", url.toStdString().c_str());
}

void Search::on_us_button_clicked()
{
    QString url = model->wordInf.us_audio_addresses;
    if(url.isEmpty()) {
        LOGDBG("url is empty: %s",url.toStdString().c_str());
        return ;
    }
    player->setMedia(QUrl(url));
    player->play();
    ui->us_button->setStyleSheet("QPushButton{border-image:url(:/image/play_circle.png);}");
    LOGDBG("played. url is %s", url.toStdString().c_str());
}
