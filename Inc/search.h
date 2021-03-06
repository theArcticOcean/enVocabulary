/**********************************************************
*
* @brief    Class Search is home page of enVocabulary. The
*           file has declaration of it.
*
* @author   theArcticOcean
***********************************************************/

#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include "httpmanager.h"
#include <QMediaPlayer>
#include <QMouseEvent>
#include <QThread>
#include "qmlfunc.h"

namespace Ui {
class Search;
}

class Search : public QWidget
{
    Q_OBJECT

protected:
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void closeEvent(QCloseEvent *event);

public:
    explicit Search(QWidget *parent = 0);
    ~Search();
    void setKeyOnMessageBox( const bool & _keyOnMessageBox );
    HttpManager* getHttpManager();

public slots:
    void slotGetWord();
    void slotStateChanged(QMediaPlayer::State state);
    void slotPlayerError(QMediaPlayer::Error);

private slots:
    void on_uk_button_clicked();
    void on_us_button_clicked();

private:
    Ui::Search *ui;
    HttpManager *http;
    QMediaPlayer *player;
    enData *model;
    bool keyOnMessageBox;
    QPoint lastPos;
};

#endif // SEARCH_H
