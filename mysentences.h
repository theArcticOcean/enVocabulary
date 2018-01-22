#ifndef MYSENTENCES_H
#define MYSENTENCES_H

#include <QWidget>
#include <QShowEvent>

namespace Ui {
class mySentences;
}

class mySentences : public QWidget
{
    Q_OBJECT

public:
    explicit mySentences(QWidget *parent = 0);
    ~mySentences();
protected:
    virtual void showEvent(QShowEvent *event);
private:
    Ui::mySentences *ui;
};

#endif // MYSENTENCES_H
