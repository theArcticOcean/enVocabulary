#ifndef MYSENTENCES_H
#define MYSENTENCES_H

#include <QWidget>
#include <QShowEvent>
#include <QThread>

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
private slots:
    void on_pushButton_clicked();

private:
    Ui::mySentences *ui;
};

#endif // MYSENTENCES_H
