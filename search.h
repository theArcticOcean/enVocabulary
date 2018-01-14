#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include "httpmanager.h"

namespace Ui {
class Search;
}

class Search : public QWidget
{
    Q_OBJECT
protected:
    virtual void keyReleaseEvent(QKeyEvent *event);

public:
    explicit Search(QWidget *parent = 0);
    ~Search();

private:
    Ui::Search *ui;
    HttpManager *http;

public slots:
    void slotGetWord();
};

#endif // SEARCH_H
