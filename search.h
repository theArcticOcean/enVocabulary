#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include "httpmanager.h"
#include <QMediaPlayer>
#include <QMouseEvent>
#include <QThread>

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

    QPoint lastPos;
private:
    Ui::Search *ui;
    HttpManager *http;
    QMediaPlayer *player;
    enData *model;

public slots:
    void slotGetWord();
    void slotStateChanged(QMediaPlayer::State state);
    void slotPlayerError(QMediaPlayer::Error);
private slots:
    void on_uk_button_clicked();
    void on_us_button_clicked();
};

#endif // SEARCH_H
